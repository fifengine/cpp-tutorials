//*****************************************************************************
// FILE NAME:  Game.cpp
//
//*****************************************************************************
#include "Game.h"
#include "ViewController.h"
#include "MouseListener.h"
#include "KeyListener.h"

// fife includes
#include "controller/engine.h"
#include "controller/enginesettings.h"
#include "util/log/logger.h"
#include "loaders/native/map/maploader.h"
#include "model/structures/map.h"
#include "model/structures/layer.h"
#include "view/camera.h"
#include "eventchannel/eventmanager.h"
#include "gui/guimanager.h"
#include "gui/fifechan/fifechanmanager.h"
#include "gui/fifechan/console/console.h"
#include "util/time/timemanager.h"
#include "video/renderbackend.h"

// 3rd party includes
#include "boost/filesystem.hpp"
#include "SDL.h"

// standard includes
#include <cassert>

namespace fs = boost::filesystem;

//!***************************************************************
//! @details:
//! constructor
//!
//!***************************************************************
Game::Game()
: m_map(0), m_mainCamera(0), m_mouseListener(0), m_keyListener(0), m_player(0), m_quit(false)
{
	// create the engine
	m_engine = new FIFE::Engine();

	// create view controller
	m_viewController = new ViewController();

	// apply game settings
	InitSettings();

	// initialize the engine
	m_engine->init();

	// create default gui
	FIFE::FifechanManager* guiManager = new FIFE::FifechanManager();

	// setup the gui
	guiManager->setDefaultFont(
		m_engine->getSettings().getDefaultFontPath(),
		m_engine->getSettings().getDefaultFontSize(),
		m_engine->getSettings().getDefaultFontGlyphs()
	);
		
	guiManager->init(
		m_engine->getRenderBackend()->getName(),
		m_engine->getRenderBackend()->getScreenWidth(),
		m_engine->getRenderBackend()->getScreenHeight()
	);

	m_engine->setGuiManager(guiManager);
	m_engine->getEventManager()->addSdlEventListener(guiManager);
}

//!***************************************************************
//! @details:
//! destructor
//!
//!***************************************************************
Game::~Game()
{
	// clean up our resources
	delete m_viewController;
	m_viewController = 0;

	delete m_mouseListener;
	m_mouseListener = 0;

	delete m_keyListener;
	m_keyListener = 0;

	// the engine will clean up its resources
	delete m_engine;
	m_engine = 0;
}

//!***************************************************************
//! @details:
//! initializes the game and subsystems
//!
//! @return: 
//! void
//! 
//!***************************************************************
void Game::Init()
{
	// load the game map
	CreateMap();

	// initialize the cameras and view
	InitView();

	// initialize the user input
	CreateInput();

	// prep the engine for running
	m_engine->initializePumping();
}

//!***************************************************************
//! @details:
//! this is the game loop
//! internally calls the engine tick to make sure everything
//! updates
//!
//! @return: 
//! bool
//! 
//!***************************************************************
void Game::Run()
{  
    int lastTime = 0;
    int currTime = 0;
	while (!m_quit)
	{
        // update fps reading approx. every second
        if ((currTime > 0) && (currTime - lastTime >= 1e3))
        {
            // create FPS string
            std::ostringstream oss;
            oss << " [FPS: " << static_cast<int>(1e3/m_engine->getTimeManager()->getAverageFrameTime()) << "]";

            // show fps in title, and keep it updated
		    FIFE::EngineSettings& settings = m_engine->getSettings();
		    std::string windowTitle = settings.getWindowTitle();
		    windowTitle += oss.str();

		    // this functionality should be part of the engine
		    // it will be migrated there soon, so the client
		    // does not have to call SDL directly
		    SDL_SetWindowTitle(m_engine->getRenderBackend()->getWindow(), windowTitle.c_str());

            // update the last time fps was calculated
            lastTime = m_engine->getTimeManager()->getTime();
        }

		// engine timer tick
		m_engine->pump();

        // update the current run time
        currTime = m_engine->getTimeManager()->getTime();
	}
}

//!***************************************************************
//! @details:
//! signal to stop the game loop
//!
//! @return: 
//! void
//! 
//!***************************************************************
void Game::Quit()
{
	m_quit = true;
}

//!***************************************************************
//! @details:
//! toggle the console during game time
//!
//! @return: 
//! void
//! 
//!***************************************************************
void Game::toggleConsole()
{
	// get the engine's GUI manager
	FIFE::FifechanManager* guiManager = static_cast<FIFE::FifechanManager*>(m_engine->getGuiManager());
	guiManager->getConsole()->toggleShowHide();
}

//!***************************************************************
//! @details:
//! accessor for the game's view controller
//!
//! @return: 
//! ViewController*
//! 
//!***************************************************************
ViewController* Game::GetViewController()
{
	return m_viewController;
}

//!***************************************************************
//! @details:
//! initialize the engine settings
//!
//! @return: 
//! void
//! 
//!***************************************************************
void Game::InitSettings()
{
	// grab the current settings from the engine
	FIFE::EngineSettings& settings = m_engine->getSettings();

	// get the path to the font we are going to use
	fs::path defaultFontPath("../../assets/fonts/FreeSans.ttf");

	// change the engine settings to suite our game
	settings.setRenderBackend("OpenGL");
	settings.setScreenHeight(600);
	settings.setScreenWidth(800);
	settings.setBitsPerPixel(0);
	settings.setFullScreen(false);
	settings.setInitialVolume(5.0);
	settings.setWindowTitle("FIFE - Tutorials");
	settings.setDefaultFontGlyphs("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&amp;`'*#=[]\"");
	settings.setDefaultFontPath(defaultFontPath.string());

	FIFE::LogManager* logManager = m_engine->getLogManager();
	if (logManager)
	{
		// disable logging
		logManager->setLogToFile(false);
		logManager->setLogToPrompt(false);
	}
}

//!***************************************************************
//! @details:
//! attaches the correct map loader to the engine and loads the
//! map file
//!
//! @return: 
//! void
//! 
//!***************************************************************
void Game::CreateMap()
{
	if (m_engine->getModel() && m_engine->getVFS() && m_engine->getImageManager() && 
		m_engine->getRenderBackend())
	{
		// create the default loader for the FIFE map format
		//FIFE::DefaultMapLoader* mapLoader = FIFE::createDefaultMapLoader(m_engine->getModel(), m_engine->getVFS(), 
		//	m_engine->getImagePool(), m_engine->getAnimationPool(), m_engine->getRenderBackend());
		FIFE::MapLoader* mapLoader = new FIFE::MapLoader(m_engine->getModel(), m_engine->getVFS(), 
			m_engine->getImageManager(), m_engine->getRenderBackend());

		fs::path mapPath("../../assets/maps/shrine.xml");

		if (mapLoader) {
			// load the map
			m_map = mapLoader->load(mapPath.string());
		}

		// done with map loader safe to delete
		delete mapLoader;
		mapLoader = 0;
	}
}

//!***************************************************************
//! @details:
//! create the user input devices and attach to engine
//!
//! @return: 
//! void
//! 
//!***************************************************************
void Game::CreateInput()
{
	if (m_engine->getEventManager() && m_engine->getModel())
	{
		// attach our key listener to the engine
		m_keyListener = new KeyListener(this);
		m_engine->getEventManager()->addKeyListener(m_keyListener);

		// attach our mouse listener to the engine
		m_mouseListener = new MouseListener(this, m_mainCamera, m_engine->getEventManager(), m_engine->getTimeManager());
		m_engine->getEventManager()->addMouseListener(m_mouseListener);

		// grab the layer that has our main character
		FIFE::Layer* layer = m_map->getLayer("TechdemoMapGroundObjectLayer");

		if (layer)
		{
			// query the layer for our main character
			m_player = layer->getInstance("PC");

			if (m_player)
			{
				// set the main characters default action to standing
				m_player->actRepeat("stand", m_player->getLocationRef());

				// attach the mouse controller to our main character
				// to control the player
				m_mouseListener->SetController(m_player);
			}

			// query the layer for the other character we are interested in
			FIFE::Instance* m_npc = layer->getInstance("NPC:girl");

			if (m_npc)
			{
				// set this character's action to standing as well
				m_npc->actRepeat("stand", m_npc->getLocationRef());
			}
		}
	}
}

//!***************************************************************
//! @details:
//! initialize the camera and view
//!
//! @return: 
//! void
//! 
//!***************************************************************
void Game::InitView()
{
	if (m_map)
	{
		// get the main camera for this map
		m_mainCamera = m_map->getCamera("main");

		if (m_mainCamera)
		{
			// attach the controller to the camera
			m_viewController->AttachCamera(m_mainCamera);
			m_viewController->EnableCamera(true);

			// get the renderer associated with viewing objects on the map
			FIFE::RendererBase* renderer = m_mainCamera->getRenderer("InstanceRenderer");

			if (renderer)
			{
				// activate all layers associated with the renderer
				// for this map, this must be done to see anything
				renderer->activateAllLayers(m_map);
			}

			// get the mini camera attached to the map
			FIFE::Camera* miniCamera = m_map->getCamera("small");

			// default the small camera to off, we will revisit the
			// mini camera in a later demo
			if (miniCamera)
			{
				miniCamera->setEnabled(false);
			}
		}
	}
}
