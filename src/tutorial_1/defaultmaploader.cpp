/***************************************************************************
*   Copyright (C) 2005-2009 by the FIFE team                              *
*   http://www.fifengine.de                                               *
*   This file is part of FIFE.                                            *
*                                                                         *
*   FIFE is free software; you can redistribute it and/or                 *
*   modify it under the terms of the GNU Lesser General Public            *
*   License as published by the Free Software Foundation; either          *
*   version 2.1 of the License, or (at your option) any later version.    *
*                                                                         *
*   This library is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
*   Lesser General Public License for more details.                       *
*                                                                         *
*   You should have received a copy of the GNU Lesser General Public      *
*   License along with this library; if not, write to the                 *
*   Free Software Foundation, Inc.,                                       *
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
***************************************************************************/

// Standard C++ library includes
#include <string>
#include <vector>

// 3rd party includes
#include "boost/filesystem.hpp"
#include "extern/tinyxml/tinyxml.h"

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "model/model.h"
#include "model/structures/layer.h"
#include "model/structures/instance.h"
#include "model/metamodel/grids/cellgrid.h"
#include "model/metamodel/modelcoords.h"
#include "model/metamodel/action.h"
#include "vfs/vfs.h"
#include "vfs/vfsdirectory.h"
#include "util/resource/resource_location.h"
#include "util/structures/rect.h"
#include "video/image_location.h"
#include "video/imagepool.h"
#include "video/animationpool.h"
#include "video/animation.h"
#include "video/renderbackend.h"
#include "view/visual.h"
#include "view/camera.h"

#include "defaultmaploader.h"

namespace fs = boost::filesystem;

namespace FIFE
{
	class AnimationLoader : public ResourceLoader {
	public:
		AnimationLoader(ImagePool* imagePool, const std::string& mapDirectory)
			: m_imagePool(imagePool), m_mapDirectory(mapDirectory) {

		}
		virtual IResource* loadResource(const ResourceLocation& location);
	private:
		ImagePool* m_imagePool;
		std::string m_mapDirectory;
	};

	IResource* AnimationLoader::loadResource(const ResourceLocation& location) {
		const std::string animFile = m_mapDirectory + "/" + location.getFilename();

		fs::path animPath(animFile);

		TiXmlDocument doc;
		bool isFileLoaded = doc.LoadFile(animPath.file_string());

		Animation* animation = NULL;

		if (isFileLoaded) {
			TiXmlElement* root = doc.RootElement();

			if (root) {
				animation = new Animation();

				int animDelay = 0;
				root->QueryValueAttribute("delay", &animDelay);

				int animXoffset = 0;
				int animYoffset = 0;
				int action = 0;
				root->QueryValueAttribute("x_offset", &animXoffset);
				root->QueryValueAttribute("y_offset", &animYoffset);
				root->QueryValueAttribute("action", &action);

				for (TiXmlElement* frameElement = root->FirstChildElement("frame"); frameElement; frameElement = frameElement->NextSiblingElement("frame")) {
					if (animation) {
						animation->setActionFrame(action);

						const std::string* sourceId = frameElement->Attribute(std::string("source"));

						if (sourceId) {
							std::string frameFile = *sourceId;

							fs::path framePath(location.getFilename());

							if (framePath.has_branch_path()) {
								frameFile = framePath.branch_path().directory_string() + "/" + *sourceId;
							}

							framePath = fs::path(frameFile);

							ImageLocation* imageLocation = new ImageLocation(framePath.file_string());

							if (imageLocation) {
								int frameXoffset = 0;
								int frameYoffset = 0;

								int success = root->QueryValueAttribute("x_offset", &frameXoffset);

								if (success == TIXML_SUCCESS) {
									imageLocation->setXShift(frameXoffset);
								}
								else {
									imageLocation->setXShift(animXoffset);
								}

								success = root->QueryValueAttribute("y_offset", &frameYoffset);

								if (success == TIXML_SUCCESS) {
									imageLocation->setYShift(frameYoffset);
								}
								else {
									imageLocation->setYShift(animYoffset);
								}

								int imageIndex = m_imagePool->addResourceFromLocation(imageLocation);

								int frameDelay = 0;
								success = root->QueryValueAttribute("delay", &frameDelay);

								if (success == TIXML_SUCCESS) {
									animation->addFrame(ResourcePtr(m_imagePool, imageIndex), frameDelay);
								}
								else {
									animation->addFrame(ResourcePtr(m_imagePool, imageIndex), animDelay);
								}
							}
						}
					}
				}
			}
		}

		return animation;
	}

	DefaultMapLoader::DefaultMapLoader(Model* model, VFS* vfs, ImagePool* imagePool, AnimationPool* animPool, RenderBackend* renderBackend)
	: m_model(model), m_vfs(vfs), m_imagePool(imagePool), m_animPool(animPool), m_renderBackend(renderBackend),
	  m_loaderName("fifedefault"), m_mapDirectory("") {
		
	}

	Map* DefaultMapLoader::loadMapFile(const ResourceLocation& location) {
		std::cout << "loading: " << location.getFilename() << " in c++ " << std::endl;
		Map* map = NULL;

		// reset percent done callback just in case
		// it has residual data from last load
		m_percentDoneCallback.reset();

		fs::path mapPath(location.getFilename());
		
		if (mapPath.has_branch_path()) {
			// save the directory where the map file is located
			m_mapDirectory = mapPath.branch_path().directory_string();

			m_vfs->addSource(new VFSDirectory(m_vfs, mapPath.branch_path().directory_string()));
			m_animPool->addResourceLoader(new AnimationLoader(m_imagePool, mapPath.branch_path().directory_string()));
		}

		TiXmlDocument mapFile;

		if (mapFile.LoadFile(mapPath.file_string())) {
			const TiXmlElement* root = mapFile.RootElement();

			if (root) {
				const std::string* loaderName = root->Attribute(std::string("loaderName"));
				
				if (loaderName) {
					m_loaderName = *loaderName;
				}

				int numElements = 0;
				int numElementsRetVal = root->QueryValueAttribute("elements", &numElements);
				m_percentDoneCallback.setTotalNumberOfElements(numElements);

				const std::string* mapName = root->Attribute(std::string("id"));

				if (mapName) {
					try {
						map = m_model->createMap(*mapName);
					}
					catch (NameClash&) {
						// TODO - handle exception
						assert(false); 
					}

					if (map) {
						std::string ns = "";
						for (const TiXmlElement *importElement = root->FirstChildElement("import"); importElement; importElement = importElement->NextSiblingElement("import")) {
							const std::string* importDir = importElement->Attribute(std::string("dir"));
							const std::string* importFile = importElement->Attribute(std::string("file"));
							loadImport(importDir, importFile);
						}

						// iterate over elements looking for layers
						for (const TiXmlElement* layerElement = root->FirstChildElement("layer"); layerElement; layerElement = layerElement->NextSiblingElement("layer")) {
							// defaults
							double xOffset = 0.0;
							double yOffset = 0.0;
							double xScale = 1.0;
							double yScale = 1.0;
							double rotation = 0.0;

 							int xOffsetRetVal = layerElement->QueryValueAttribute("x_offset", &xOffset);
 							int yOffsetRetVal = layerElement->QueryValueAttribute("y_offset", &yOffset);
 							int xScaleRetVal = layerElement->QueryValueAttribute("x_scale", &xScale);
 							int yScaleRetVal = layerElement->QueryValueAttribute("y_scale", &yScale);
 							int rotationRetVal = layerElement->QueryValueAttribute("rotation", &rotation);

							const std::string* layerName = layerElement->Attribute(std::string("id"));
							const std::string* pathing = layerElement->Attribute(std::string("pathing"));
							const std::string* gridType = layerElement->Attribute(std::string("grid_type"));

							if (xOffsetRetVal == TIXML_SUCCESS &&
								yOffsetRetVal == TIXML_SUCCESS &&
								xScaleRetVal == TIXML_SUCCESS && 
								yScaleRetVal == TIXML_SUCCESS &&
								rotationRetVal == TIXML_SUCCESS &&
								layerName && 
								pathing && 
								gridType) {
								PathingStrategy pathStrategy = CELL_EDGES_ONLY;

								if ("cell_edges_and_diagonals" == *pathing) {
									pathStrategy = CELL_EDGES_AND_DIAGONALS;
								}
								else if ("freeform" == *pathing) {
									pathStrategy = FREEFORM;
								}

								CellGrid* grid = NULL;
								if (gridType) {
									grid = m_model->getCellGrid(*gridType);
								}
								else {
									grid = m_model->getCellGrid("square");
								}

								if (grid) {
									grid->setXShift(xOffset);
									grid->setXScale(xScale);
									grid->setYShift(yOffset);
									grid->setYScale(yScale);
									grid->setRotation(rotation);

									Layer *layer = NULL;
									try {
										layer = map->createLayer(*layerName, grid);
									}
									catch (NameClash&) {
										// TODO - handle exception
										assert(false);
									}

									if (layer) {
										layer->setPathingStrategy(pathStrategy);

										double curr_x = 0;
										double curr_y = 0;
										
										for (const TiXmlElement* instances = layerElement->FirstChildElement("instances"); instances; instances = instances->NextSiblingElement("instances")) {
											for (const TiXmlElement* instance = instances->FirstChildElement("i"); instance; instance = instance->NextSiblingElement("i")) {
												double x = 0;
												double y = 0;
												double z = 0;
												int r = 0;
												int stackpos = 0;

												const std::string* instanceId = instance->Attribute(std::string("id"));
												const std::string* objectId = instance->Attribute(std::string("o"));

												if (!objectId) {
													objectId = instance->Attribute(std::string("object"));
												}

												if (!objectId) {
													objectId = instance->Attribute(std::string("obj"));
												}

												const std::string* namespaceId = instance->Attribute(std::string("ns"));

												if (!namespaceId) {
													namespaceId = instance->Attribute(std::string("namespace"));
												}

												int xRetVal = instance->QueryValueAttribute("x", &x);
												int yRetVal = instance->QueryValueAttribute("y", &y);
												int zRetVal = instance->QueryValueAttribute("z", &z);
												int rRetVal = instance->QueryValueAttribute("r", &r);

												if (xRetVal == TIXML_SUCCESS) {
													curr_x = x;
												}
												else {
													x = ++curr_x;		
												}

												if (yRetVal == TIXML_SUCCESS) {
													curr_y = y;
												}
												else {
													y = curr_y;
												}

												if (rRetVal != TIXML_SUCCESS) {
													rRetVal = instance->QueryValueAttribute("rotation", &r);
												}

												int stackRetVal = instance->QueryValueAttribute("stackpos", &stackpos);

												if (objectId) {
													if (namespaceId) {
														ns = *namespaceId;
													}

													Object* object = m_model->getObject(*objectId, ns);

													if (object) {
														Instance* inst = NULL;
														if (instanceId) {
															inst = layer->createInstance(object, ExactModelCoordinate(x,y,z), *instanceId);
														}
														else {
															inst = layer->createInstance(object, ExactModelCoordinate(x,y,z));
														}

														if (inst) {
															if (rRetVal != TIXML_SUCCESS) {
																ObjectVisual* objVisual = object->getVisual<ObjectVisual>();
																std::vector<int> angles;
																objVisual->getStaticImageAngles(angles);
																if (!angles.empty()) {
																	r = angles[0];
																}
															}

															inst->setRotation(r);

															InstanceVisual* instVisual = InstanceVisual::create(inst);
															
															if  (instVisual && (stackRetVal == TIXML_SUCCESS)) {
																instVisual->setStackPosition(stackpos);
															}

															if (object->getAction("default")) {
																Location target(layer);

																inst->act("default", target, true);
															}
														}
													}
												}

												// increment % done counter
												m_percentDoneCallback.incrementCount();
											}
										}
									}
								}
							}

							// increment % done counter
							m_percentDoneCallback.incrementCount();
						}

						for (const TiXmlElement* cameraElement = root->FirstChildElement("camera"); cameraElement; cameraElement = cameraElement->NextSiblingElement("camera")) {
							const std::string* cameraId = cameraElement->Attribute(std::string("id"));
							const std::string* refLayerId = cameraElement->Attribute(std::string("ref_layer_id"));
							
							int refCellWidth = 0;
							int refCellHeight = 0;
							int success = cameraElement->QueryIntAttribute("ref_cell_width", &refCellWidth);
							success &= cameraElement->QueryIntAttribute("ref_cell_height", &refCellHeight);

							if (cameraId && refLayerId && success == TIXML_SUCCESS) {
								double tilt = 0.0;
								double zoom = 1.0;
								double rotation = 0.0;
								cameraElement->QueryDoubleAttribute("tilt", &tilt);
								cameraElement->QueryDoubleAttribute("zoom", &zoom);
								cameraElement->QueryDoubleAttribute("rotation", &rotation);

								const std::string* viewport = cameraElement->Attribute(std::string("viewport"));

								Layer* layer = NULL;
								try {
									layer = map->getLayer(*refLayerId);
								}
								catch (NotFound&) {
									// TODO - handle exception
									assert(false);
								}

								Camera* cam = NULL;
								if (layer) {
									if (viewport) {
										// parse out the viewport parameters
										std::string::size_type lastpos = 0;
										std::string::size_type pos = 0;
										std::vector<int> viewportParameters;
										while ((pos = viewport->find(',', lastpos)) != std::string::npos) {
											std::string value = viewport->substr(lastpos, pos-lastpos);
											std::istringstream iss(value);
											
											int viewportParameter = 0;
											if (!(iss >> viewportParameter)) {
												// something bad happened
												// clear vector and exit loop
												viewportParameters.clear();
												break;
											}

											viewportParameters.push_back(viewportParameter);

											lastpos = pos+1;

											if (viewportParameters.size() == 3) {
												// go ahead and grab the last parameter
												// no more commas coming
												std::string value = viewport->substr(lastpos, viewport->size()-lastpos);
												std::istringstream iss(value);

												int viewportParameter = 0;
												if (!(iss >> viewportParameter)) {
													// something bad happened
													// clear vector and exit loop
													viewportParameters.clear();
													break;
												}

												viewportParameters.push_back(viewportParameter);
											}
										}

										// make sure the right number of viewport parameters were parsed
										if (viewportParameters.size() == 4) {
											Rect rect(viewportParameters[0], viewportParameters[1], 
														viewportParameters[2], viewportParameters[3]);

											try {
												cam = map->addCamera(*cameraId, layer, rect);
											}
											catch (NameClash&) {
												// TODO - handle exception
												assert(false);
											}
										}
									}
									else {
										Rect rect(0, 0, m_renderBackend->getScreenWidth(), m_renderBackend->getScreenHeight());

										try {
											cam = map->addCamera(*cameraId, layer, rect);
										}
										catch (NameClash&) {
											// TODO - handle exception
											assert(false);
										}
									}
								}

								if (cam) {
									cam->setCellImageDimensions(refCellWidth, refCellHeight);
									cam->setRotation(rotation);
									cam->setTilt(tilt);
									cam->setZoom(zoom);
								}
							}

							// increment % done counter
							m_percentDoneCallback.incrementCount();
						}
					}
				}
			}
		}

		return map;
	}

	bool DefaultMapLoader::isLoadable(const ResourceLocation& Location) const {
		fs::path mapPath(Location.getFilename());

		TiXmlDocument mapFile;

		if (mapFile.LoadFile(mapPath.file_string())) {
			const TiXmlElement* root = mapFile.RootElement();

			if (root) {
				const std::string* loaderName = root->Attribute(std::string("loader"));

				if (loaderName && *loaderName == getLoaderName()) {
					return true;
				}
			}
		}

		return false;
	}

	const std::string& DefaultMapLoader::getLoaderName() const {
		return m_loaderName;
	}

	void DefaultMapLoader::loadImport(const std::string* dir, const std::string* file) {
		if (dir && file) {
			std::string importFile = *dir + *file;

			fs::path importPath(importFile);
			loadObjectFile(importPath.file_string());
		}
		else if (file) {
			std::string importFile = *file;
			
			fs::path importPath(importFile);
			loadObjectFile(importPath.file_string());
		}
		else if (dir) {
			std::string importFile = *dir;
			
			fs::path importPath(importFile);
			loadObjectDirectory(importPath.file_string());
			m_importDirectories.push_back(importPath.file_string());
		}
	}

	void DefaultMapLoader::loadObjectFile(const std::string& file) {
		const std::string importFile = m_mapDirectory + "/" + fs::path(file).file_string();

		fs::path objectPath(importFile);

		TiXmlDocument objectFile;

		bool isFileLoaded = objectFile.LoadFile(objectPath.file_string());

		if (isFileLoaded) {
			TiXmlElement* root = objectFile.RootElement();

			if (root && root->ValueStr() == "object") {
				const std::string* objectId = root->Attribute(std::string("id"));
				const std::string* namespaceId = root->Attribute(std::string("namespace"));

				Object* obj = NULL;
				if (objectId && namespaceId) {
					const std::string* parentId = root->Attribute(std::string("parent"));

					if (parentId) {
						Object* parent = m_model->getObject(*parentId, *namespaceId);
						if (parent) {
							try {
								obj = m_model->createObject(*objectId, *namespaceId, parent);
							}
							catch  (NameClash&) {
								// TODO - handle exception
								assert(false);
							}
						}
					}
					else {
						// this will make sure the object has not already been loaded
						if (m_model->getObject(*objectId, *namespaceId) == NULL) {
							try {
								obj = m_model->createObject(*objectId, *namespaceId);
							}
							catch (NameClash&) {
								// TODO - handle exception
								assert(false);
							}
						}
					}
				}

				if (obj) {
					obj->setResourceLocation(ResourceLocation(file));
					ObjectVisual::create(obj);

					int isBlocking = 0;
					root->QueryIntAttribute("blocking", &isBlocking);
					obj->setBlocking(isBlocking);

					int isStatic = 0;
					root->QueryIntAttribute("static", &isStatic);
					obj->setStatic(isStatic);

					const std::string* pather = root->Attribute(std::string("pather"));

					if (pather) {
						obj->setPather(m_model->getPather(*pather));
					}
					else {
						obj->setPather(m_model->getPather("RoutePather"));
					}

					// loop over all image tags
					for (TiXmlElement* imageElement = root->FirstChildElement("image"); imageElement; imageElement = imageElement->NextSiblingElement("image")) {
						const std::string* sourceId = imageElement->Attribute(std::string("source"));

						if (sourceId) {
							ImageLocation *imageLocation = NULL;
							
							std::string imageFile = *sourceId;

							fs::path imagePath(file);

							if (imagePath.has_branch_path()) {
								imageFile = imagePath.branch_path().string() + "/" + *sourceId;
							}

							imagePath = fs::path(imageFile);

							imageLocation = new ImageLocation(imagePath.file_string());

							if (imageLocation) {
								int xOffset = 0;
								int success = imageElement->QueryIntAttribute("x_offset", &xOffset);

								if (success == TIXML_SUCCESS) {
									imageLocation->setXShift(xOffset);
								}

								int yOffset = 0;
								success = imageElement->QueryIntAttribute("y_offset", &yOffset);

								if (success == TIXML_SUCCESS) {
									imageLocation->setYShift(yOffset);
								}

								int resourceId = m_imagePool->addResourceFromLocation(imageLocation);

								int direction = 0;
								success = imageElement->QueryIntAttribute("direction", &direction);

								ObjectVisual* objVisual = obj->getVisual<ObjectVisual>();

								if (objVisual) {
									objVisual->addStaticImage(direction, resourceId);
								}
							}
						}

						// increment % done counter
						m_percentDoneCallback.incrementCount();
					}

					for (TiXmlElement* actionElement = root->FirstChildElement("action"); actionElement; actionElement = actionElement->NextSiblingElement("action")) {
						const std::string* actionId = actionElement->Attribute(std::string("id"));

						if (actionId) {						
							Action* action = obj->createAction(*actionId);
							ActionVisual::create(action);

							for (TiXmlElement* animElement = actionElement->FirstChildElement("animation"); animElement; animElement = animElement->NextSiblingElement("animation")) {
								const std::string* sourceId = animElement->Attribute(std::string("source"));

								if (sourceId) {
									std::string animFile = *sourceId;

									fs::path animPath(file);

									if (animPath.has_branch_path()) {
										animFile = animPath.branch_path().string() + "/" + *sourceId;
									}

									animPath = fs::path(animFile);

									int animId = m_animPool->addResourceFromFile(animPath.file_string());

									Animation& animation = m_animPool->getAnimation(animId);

									int direction = 0;
									int success = animElement->QueryIntAttribute("direction", &direction);

									if (action) {
										ActionVisual* actionVisual = action->getVisual<ActionVisual>();

										if (actionVisual) {
											actionVisual->addAnimation(direction, animId);
										}

										action->setDuration(animation.getDuration());
									}
								}

								// increment % done counter
								m_percentDoneCallback.incrementCount();
							}
						}
					}
				}

				// increment % done counter
				m_percentDoneCallback.incrementCount();
			}
		}
	}

	void DefaultMapLoader::loadObjectDirectory(const std::string& directory) {
		fs::path fullDirPath(directory + "/");

		std::set<std::string> files = m_vfs->listFiles(fullDirPath.directory_string());

		std::set<std::string>::iterator iter;
		for (iter = files.begin(); iter != files.end(); ++iter) {
			if (fs::extension(*iter) == ".xml") {
				loadObjectFile(fullDirPath.directory_string() + *iter);
			}	
		}

		std::set<std::string> setNestedDirectories = m_vfs->listDirectories(fullDirPath.directory_string());

		// had to copy std::set contents to a vector to get around
		// the problem of std::remove not compiling because of a
		// problem with how set defines its iterators on gcc
		std::vector<std::string> nestedDirectories(setNestedDirectories.begin(), setNestedDirectories.end());

		// don't like this, but need to make sure to not include the .svn directory
		nestedDirectories.erase(std::remove(nestedDirectories.begin(), nestedDirectories.end(), ".svn"), nestedDirectories.end());

		std::vector<std::string>::iterator vecIter;
		for (vecIter = nestedDirectories.begin(); vecIter != nestedDirectories.end(); ++vecIter) {
			loadObjectDirectory(fullDirPath.directory_string() + *vecIter);
		}
	}

	std::vector<std::string> DefaultMapLoader::getImportDirectories() const {
		return m_importDirectories;
	}

	void DefaultMapLoader::setPercentDoneInterval(unsigned int percent) {
		m_percentDoneCallback.setPercentDoneInterval(percent);
	}

	void DefaultMapLoader::addPercentDoneListener(PercentDoneListener* listener) {
		m_percentDoneCallback.addListener(listener);
	}

	DefaultMapLoader* createDefaultMapLoader(Model* model, VFS* vfs, ImagePool* imagePool, AnimationPool* animPool, RenderBackend* renderBackend) {
		return (new DefaultMapLoader(model, vfs, imagePool, animPool, renderBackend));
	}
}
