/**************************************************************************
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

#ifndef FIFE_DEFAULT_MAPLOADER_H_
#define FIFE_DEFAULT_MAPLOADER_H_

// Standard C++ library includes
#include <string>
#include <vector>

// 3rd party library includes

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder

#include "imaploader.h"
#include "percentdonecallback.h"

namespace FIFE
{
	class Model;
	class Map;
	class VFS;
	class Layer;
	class ImagePool;
	class AnimationPool;
	class RenderBackend;
	class PercentDoneListener;

	class DefaultMapLoader : public IMapLoader
	{
	public:
		DefaultMapLoader(Model* model, VFS* vfs, ImagePool* imagePool, AnimationPool* animPool, RenderBackend* renderBackend);
		Map* loadMapFile(const ResourceLocation& location);
		bool isLoadable(const ResourceLocation& Location) const;
		
		std::vector<std::string> getImportDirectories() const;

		void setPercentDoneInterval(unsigned int percent);
		void addPercentDoneListener(PercentDoneListener* listener);
	private:
		const std::string& getLoaderName() const;
		void loadImport(const std::string* dir, const std::string* file);
		void loadObjectFile(const std::string& file);
		void loadObjectDirectory(const std::string& directory);
	private:
		Model* m_model;
		VFS* m_vfs;
		ImagePool* m_imagePool;
		AnimationPool* m_animPool;
		RenderBackend* m_renderBackend;
		PercentDoneCallback m_percentDoneCallback;

		std::string m_loaderName;
		std::string m_mapDirectory;
		std::vector<std::string> m_importDirectories;

	};

	DefaultMapLoader* createDefaultMapLoader(Model* model, VFS* vfs, ImagePool* imagePool, AnimationPool* animPool, RenderBackend* renderBackend);
}

#endif
