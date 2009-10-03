
#include <iostream>

#include "modules.h"
#include "controller/engine.h"
#include "util/log/logger.h"
#include "model/structures/map.h"
#include "model/structures/layer.h"
#include "view/view.h"
#include "view/camera.h"
#include "eventchannel/eventmanager.h"

#include "map_loader.h"
#include "DemoMouseListener.h"

int main(int argc, char *argv[])
{
	FIFE::Engine engine;

	FIFE::EngineSettings& settings = engine.getSettings();

	FIFE::LogManager* logManager = engine.getLogManager();

	settings.setRenderBackend("OpenGL");
	settings.setScreenHeight(600);
	settings.setScreenWidth(800);
	settings.setBitsPerPixel(0);
	settings.setFullScreen(false);
	settings.setInitialVolume(5.0);
	settings.setWindowTitle("FIFE - Map Loader tech demo");
	settings.setDefaultFontGlyphs("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&amp;`'*#=[]\"");
	settings.setDefaultFontPath("C:\\Documents and Settings\\jesse\\My Documents\\Programming\\FIFE\\Trunk\\clients\\rio_de_hola\\fonts\\FreeSans.ttf");
	settings.setImageChunkingSize(256);

	if (logManager)
	{
		logManager->setLogToFile(true);
		logManager->setLogToPrompt(true);
		logManager->addVisibleModule(LM_CONTROLLER);
		logManager->addVisibleModule(LM_VFS);
		//logManager->addVisibleModule(LM_CAMERA);
		//logManager->addVisibleModule(LM_VIEW);
	}

	engine.init();

	if (engine.getModel() && engine.getView() && engine.getVFS() && engine.getImagePool() && engine.getAnimationPool() && engine.getRenderBackend() && engine.getMapLoader())
	{
		FIFE::MapLoader* map_loader = engine.getMapLoader();

		//FIFE::MapLoader map_loader(engine.getModel(), engine.getView(), engine.getVFS(), engine.getImagePool(), engine.getAnimationPool(), engine.getRenderBackend());
		
		FIFE::Map* map = map_loader->loadMapFile("C:\\Documents and Settings\\jesse\\My Documents\\Programming\\FIFE\\Trunk\\clients\\rio_de_hola\\maps\\shrine.xml");
		//FIFE::Map* map = map_loader->loadMapFile("C:\\Documents and Settings\\jesse\\My Documents\\Programming\\FIFE\\Trunk\\clients\\rio_de_hola\\maps\\tourist_beach.xml");

		//settings.setDefaultFontPath("C:\\Documents and Settings\\jesse\\My Documents\\Programming\\FIFE\\Trunk\\clients\\rio_de_hola\\fonts\\samanata.ttf");
		//FIFE::Map* map = map_loader->loadMapFile("C:\\Documents and Settings\\jesse\\My Documents\\Programming\\FIFE\\Trunk\\clients\\Zero-Projekt\\Trunk\\maps\\worldmap.xml");

		if (map)
		{
			if (engine.getView())
			{
				FIFE::Camera *mainCam = engine.getView()->getCamera("shrine_main");

				if (mainCam)
				{
					// enable the main camera
					mainCam->setEnabled(true);

					FIFE::RendererBase* renderer = mainCam->getRenderer("InstanceRenderer");

					// render all the layers on the main camera
					if (renderer)
					{
						renderer->activateAllLayers(map);
					}

					// add mouse listener to control main camera
					if (engine.getEventManager() && engine.getModel())
					{
						DemoMouseListener* mouseListener = new DemoMouseListener(mainCam, engine.getEventManager());
						engine.getEventManager()->addMouseListener(mouseListener);
						FIFE::Layer* layer = map->getLayer("TechdemoMapGroundObjectLayer");

						if (layer)
						{
							FIFE::Instance* mainCharacter = layer->getInstance("PC");

							if (mainCharacter)
							{
								mouseListener->SetController(mainCharacter);
							}
						}
					}
				}

				FIFE::Camera* smallCam = engine.getView()->getCamera("shrine_small");

				// for now turn off the small camera view if it exists
				if (smallCam)
				{
					smallCam->setEnabled(false);
				}
			}
		}
	}

	engine.initializePumping();

 	while (1)
 	{
		// handle user input


 		engine.pump();
 	}

	return 0;
}