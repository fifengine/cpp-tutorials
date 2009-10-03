
#ifndef DEMO_MOUSE_LISTENER_H_
#define DEMO_MOUSE_LISTENER_H_

#include "eventchannel/mouse/ec_imouselistener.h"
#include "AutoScreenScroller.h"

namespace FIFE
{
	class MouseEvent;
	class Command;
	class Camera;
	class EventManager;
	class Instance;
}

class DemoMouseListener : public FIFE::IMouseListener
{
public:
	DemoMouseListener(FIFE::Camera* cam, FIFE::EventManager* eventManager);
	~DemoMouseListener();

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
	int m_dragX;
	int m_dragY;
	FIFE::Camera* m_camera;
	AutoScreenScroller m_autoscreenscroller;
	FIFE::Instance* m_controller;
};

#endif