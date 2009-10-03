
#include <iostream>

#include "eventchannel/mouse/ec_mouseevent.h"
#include "model/metamodel/modelcoords.h"
#include "model/structures/location.h"
#include "util/structures/rect.h"
#include "model/structures/instance.h"

#include "view/camera.h"

#include "DemoMouseListener.h"

DemoMouseListener::DemoMouseListener(FIFE::Camera *cam, FIFE::EventManager* eventManager)
: m_dragX(0), m_dragY(0), m_camera(cam), m_autoscreenscroller(cam, eventManager),
  m_controller(0)
{

}

DemoMouseListener::~DemoMouseListener()
{

}

void DemoMouseListener::mouseEntered(FIFE::MouseEvent& evt)
{

}

void DemoMouseListener::mouseExited(FIFE::MouseEvent& evt)
{

}

void DemoMouseListener::mousePressed(FIFE::MouseEvent& evt)
{
	if (evt.getButton() == FIFE::MouseEvent::LEFT)
	{
		// save mouse position
		m_dragX = evt.getX();
		m_dragY = evt.getY();
	}
}

void DemoMouseListener::mouseReleased(FIFE::MouseEvent& evt)
{
	if (m_controller && evt.getButton() == FIFE::MouseEvent::LEFT)
	{
		// move controller to clicked spot
		FIFE::Location destination(m_controller->getLocation());
		FIFE::ScreenPoint screenPoint(evt.getX(), evt.getY());
		destination.setMapCoordinates(m_camera->toMapCoordinates(screenPoint));
		m_controller->move("walk", destination, 0.5);
	}	
}

void DemoMouseListener::mouseClicked(FIFE::MouseEvent& evt)
{
	
}

void DemoMouseListener::mouseWheelMovedUp(FIFE::MouseEvent& evt)
{

}

void DemoMouseListener::mouseWheelMovedDown(FIFE::MouseEvent& evt)
{

}

void DemoMouseListener::mouseMoved(FIFE::MouseEvent& evt)
{
	m_autoscreenscroller.updateLocation(evt.getX(), evt.getY());
}

void DemoMouseListener::mouseDragged(FIFE::MouseEvent& evt)
{
	if (evt.getButton() == FIFE::MouseEvent::LEFT)
	{
		// unregister the auto-scrolling event
		m_autoscreenscroller.unregisterEvent();

		// get the screen coordinates of the mouse
		int currX = evt.getX();
		int currY = evt.getY();

		// get the mouse delta for camera movement
		FIFE::ScreenPoint delta(m_dragX - currX, m_dragY - currY);

		// get the current camera location
		FIFE::ScreenPoint cameraScreenCoords = m_camera->toScreenCoordinates(m_camera->getLocationRef().getMapCoordinates());
		cameraScreenCoords += delta;

		FIFE::Location camLocation(m_camera->getLocationRef());
		//FIFE::Location& camLocation = m_camera->getLocationRef();
		camLocation.setMapCoordinates(m_camera->toMapCoordinates(cameraScreenCoords));
		
		m_camera->setLocation(camLocation);

		// alert the camera to update its internals
		//m_camera->refresh();

		// update last saved x,y values for dragging
		m_dragX = currX;
		m_dragY = currY;
	}
}

void DemoMouseListener::SetController(FIFE::Instance* controller)
{
	m_controller = controller;
}