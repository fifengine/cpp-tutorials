//*****************************************************************************
// FILE NAME:  Game.h
//
//*****************************************************************************
#ifndef GAME_H_
#define GAME_H_

// forward declarations for fife classes
namespace FIFE
{
	class Engine;
	class EngineSettings;
	class Map;
	class Camera;
	class Instance;
}

class ViewController;
class MouseListener;
class KeyListener;

//! main interface to the demo
class Game
{
public:
	Game();
	~Game();

	void Init();
	bool Run();
	void Quit();

	void toggleConsole();
	ViewController* GetViewController();
private:
	void InitSettings();
	void CreateMap();
	void CreateInput();
	void InitView();

private:
	FIFE::Engine* m_engine;
	FIFE::Map* m_map;
	FIFE::Camera* m_mainCamera;
	ViewController* m_viewController;
	MouseListener* m_mouseListener;
	KeyListener* m_keyListener;
	FIFE::Instance* m_player;
	bool m_quit;
};

#endif

