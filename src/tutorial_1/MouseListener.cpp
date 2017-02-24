//*****************************************************************************
// FILE NAME:  MouseListener.cpp
//
//*****************************************************************************
// standard includes
#include <iostream>

// fife includes
#include "eventchannel/mouse/ec_mouseevent.h"
#include "model/metamodel/modelcoords.h"
#include "model/structures/location.h"
#include "util/structures/rect.h"
#include "model/structures/instance.h"
#include "view/camera.h"

#include "Game.h"
#include "ViewController.h"
#include "MouseListener.h"

//!***************************************************************
//! @details:
//! constructor
//!
//! @param[in]: parent
//! the game object that created this listener
//!
//! @param[in]: cam
//! the camera for manipulation
//!
//! @param[in]: eventManager
//! the engine event manager for registering for events
//! 
//!***************************************************************
MouseListener::MouseListener(Game* parent, FIFE::Camera *cam, FIFE::EventManager* eventManager, FIFE::TimeManager* timeManager)
: m_parent(parent), m_dragX(0), m_dragY(0), m_camera(cam), m_autoscreenscroller(cam, eventManager, timeManager),
  m_controller(0), m_prevEventType(FIFE::MouseEvent::UNKNOWN_EVENT)
{

}

//!***************************************************************
//! @details:
//! destructor
//!
//!***************************************************************
MouseListener::~MouseListener()
{

}

//!***************************************************************
//! @details:
//! overridden from base class
//!
//! @param[in]: evt
//! signaled event
//!
//! @return: 
//! void
//! 
//!***************************************************************
void MouseListener::mouseEntered(FIFE::MouseEvent& evt)
{
	SetPreviousMouseEvent(evt.getType());
}

//!***************************************************************
//! @details:
//! overridden from base class
//!
//! @param[in]: evt
//! signaled event
//!
//! @return: 
//! void
//! 
//!***************************************************************
void MouseListener::mouseExited(FIFE::MouseEvent& evt)
{
	SetPreviousMouseEvent(evt.getType());
}

//!***************************************************************
//! @details:
//! overridden from base class
//!
//! @param[in]: evt
//! signaled event
//!
//! @return: 
//! void
//! 
//!***************************************************************
void MouseListener::mousePressed(FIFE::MouseEvent& evt)
{
	if (evt.getButton() == FIFE::MouseEvent::LEFT)
	{
		// save mouse position
		m_dragX = evt.getX();
		m_dragY = evt.getY();
	}

	SetPreviousMouseEvent(evt.getType());
}

//!***************************************************************
//! @details:
//! overridden from base class
//!
//! @param[in]: evt
//! signaled event
//!
//! @return: 
//! void
//! 
//!***************************************************************
void MouseListener::mouseReleased(FIFE::MouseEvent& evt)
{
	// only activate the move action if the mouse was pressed and released without dragging
	if (m_controller && evt.getButton() == FIFE::MouseEvent::LEFT && m_prevEventType != FIFE::MouseEvent::DRAGGED)
	{
		// move controller to clicked spot
		FIFE::Location destination(m_controller->getLocation());
		FIFE::ScreenPoint screenPoint(evt.getX(), evt.getY());
		FIFE::ExactModelCoordinate mapCoords = m_camera->toMapCoordinates(screenPoint, false);
		mapCoords.z = 0.0;
		destination.setMapCoordinates(mapCoords);
		m_controller->move("walk", destination, m_controller->getTotalTimeMultiplier());
	}

	SetPreviousMouseEvent(evt.getType());
}

//!***************************************************************
//! @details:
//! overridden from base class
//!
//! @param[in]: evt
//! signaled event
//!
//! @return: 
//! void
//! 
//!***************************************************************
void MouseListener::mouseClicked(FIFE::MouseEvent& evt)
{
	// this event never seems to fire, not sure if it works properly
	// I suspect this would remove the need for my keeping track of the previous
	// event for dragging vs. releasing if this event signifies what I think

	SetPreviousMouseEvent(evt.getType());
}

//!***************************************************************
//! @details:
//! overridden from base class
//!
//! @param[in]: evt
//! signaled event
//!
//! @return: 
//! void
//! 
//!***************************************************************
void MouseListener::mouseWheelMovedUp(FIFE::MouseEvent& evt)
{
	// zoom in
	m_parent->GetViewController()->ZoomIn();

	// save mouse position
	m_dragX = evt.getX();
	m_dragY = evt.getY();

	SetPreviousMouseEvent(evt.getType());
}

//!***************************************************************
//! @details:
//! overridden from base class
//!
//! @param[in]: evt
//! signaled event
//!
//! @return: 
//! void
//! 
//!***************************************************************
void MouseListener::mouseWheelMovedDown(FIFE::MouseEvent& evt)
{
	// zoom out
	m_parent->GetViewController()->ZoomOut();

	SetPreviousMouseEvent(evt.getType());
}

//!***************************************************************
//! @details:
//! overridden from base class
//!
//! @param[in]: evt
//! signaled event
//!
//! @return: 
//! void
//! 
//!***************************************************************
void MouseListener::mouseWheelMovedRight(FIFE::MouseEvent& evt)
{
	SetPreviousMouseEvent(evt.getType());
}

//!***************************************************************
//! @details:
//! overridden from base class
//!
//! @param[in]: evt
//! signaled event
//!
//! @return: 
//! void
//! 
//!***************************************************************
void MouseListener::mouseWheelMovedLeft(FIFE::MouseEvent& evt)
{
	SetPreviousMouseEvent(evt.getType());
}

//!***************************************************************
//! @details:
//! overridden from base class
//!
//! @param[in]: evt
//! signaled event
//!
//! @return: 
//! void
//! 
//!***************************************************************
void MouseListener::mouseMoved(FIFE::MouseEvent& evt)
{
	m_autoscreenscroller.updateLocation(evt.getX(), evt.getY());

	SetPreviousMouseEvent(evt.getType());
}

//!***************************************************************
//! @details:
//! overridden from base class
//!
//! @param[in]: evt
//! signaled event
//!
//! @return: 
//! void
//! 
//!***************************************************************
void MouseListener::mouseDragged(FIFE::MouseEvent& evt)
{
	if (evt.getButton() == FIFE::MouseEvent::LEFT)
	{
		// unregister the auto-scrolling event
		// we are now scrolling manually and do not
		// want both trying to do it at the same time
		m_autoscreenscroller.unregisterEvent();

		// get the screen coordinates of the mouse
		int currX = evt.getX();
		int currY = evt.getY();

		// get the mouse delta for camera movement
		FIFE::ScreenPoint delta(m_dragX - currX, m_dragY - currY);

		// get the current camera location
		FIFE::ScreenPoint cameraScreenCoords = m_camera->toScreenCoordinates(m_camera->getLocationRef().getMapCoordinates());
		cameraScreenCoords += delta;

		// set the new coordinates
		FIFE::Location camLocation(m_camera->getLocationRef());
		FIFE::ExactModelCoordinate mapCoords = m_camera->toMapCoordinates(cameraScreenCoords, false);
		mapCoords.z = 0.0;
		camLocation.setMapCoordinates(mapCoords);
		m_camera->setLocation(camLocation);

		// update last saved x,y values for dragging
		m_dragX = currX;
		m_dragY = currY;
	}

	// make sure to save that the last event was drag
	// this is important to get around the drag & click problem
	SetPreviousMouseEvent(evt.getType());
}

//!***************************************************************
//! @details:
//! store the character we want to move with our mouse
//!
//! @param[in]: controller
//! the character to manipulate
//!
//! @return: 
//! void
//! 
//!***************************************************************
void MouseListener::SetController(FIFE::Instance* controller)
{
	m_controller = controller;
}

//!***************************************************************
//! @details:
//! saves the last action that was received
//! this is done to make sure we can distinguish between dragging
//! and clicking since they do 2 different things
//!
//! if we do not keep track we will drag and then when we release
//! it will perform the action for the mouse release as well as
//! the mouse dragged
//!
//! @param[in]: type
//! event type
//!
//! @return: 
//! void
//! 
//!***************************************************************
void MouseListener::SetPreviousMouseEvent(FIFE::MouseEvent::MouseEventType type)
{
	m_prevEventType = type;
}