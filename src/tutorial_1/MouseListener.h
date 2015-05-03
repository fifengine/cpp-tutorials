//*****************************************************************************
// FILE NAME:  MouseListener.h
//
//*****************************************************************************
#ifndef MOUSE_LISTENER_H_
#define MOUSE_LISTENER_H_

#include "eventchannel/mouse/ec_imouselistener.h"
#include "eventchannel/mouse/ec_mouseevent.h"

#include "ScreenScroller.h"

namespace FIFE
{
	class Command;
	class Camera;
	class EventManager;
	class TimeManager;
	class Instance;
}

class Game;

//! handles listening to mouse events
class MouseListener : public FIFE::IMouseListener
{
public:
	MouseListener(Game* parent, FIFE::Camera* cam, FIFE::EventManager* eventManager, FIFE::TimeManager* timeManager);
	~MouseListener();

	// overridden from base class
	virtual void mouseEntered(FIFE::MouseEvent& evt);
	virtual void mouseExited(FIFE::MouseEvent& evt);
	virtual void mousePressed(FIFE::MouseEvent& evt);
	virtual void mouseReleased(FIFE::MouseEvent& evt);
	virtual void mouseClicked(FIFE::MouseEvent& evt);
	virtual void mouseWheelMovedUp(FIFE::MouseEvent& evt);
	virtual void mouseWheelMovedDown(FIFE::MouseEvent& evt);
	virtual void mouseMoved(FIFE::MouseEvent& evt);
	virtual void mouseDragged(FIFE::MouseEvent& evt);

	void SetController(FIFE::Instance* controller);
private:
	void SetPreviousMouseEvent(FIFE::MouseEvent::MouseEventType type);
private:
	Game* m_parent;
	int m_dragX;
	int m_dragY;
	FIFE::Camera* m_camera;
	ScreenScroller m_autoscreenscroller;
	FIFE::Instance* m_controller;
	FIFE::MouseEvent::MouseEventType m_prevEventType;
};

#endif
