//*****************************************************************************
// FILE NAME:  ScreenScroller.cpp
//
//*****************************************************************************
#include "ScreenScroller.h"

#include "util/time/timemanager.h"
#include "eventchannel/eventmanager.h"
#include "eventchannel/command/ec_command.h"

//!***************************************************************
//! @details:
//! constructor
//!
//! @param[in]: camera
//! the camera that will be manipulated
//!
//! @param[in]: eventManager
//! the engine's event manager that will send events
//!
//! @return: 
//! 
//! 
//!***************************************************************
ScreenScroller::ScreenScroller(FIFE::Camera* camera, FIFE::EventManager* eventManager, FIFE::TimeManager* timeManager)
: m_camera(camera), m_eventManager(eventManager), m_timeManager(timeManager), ScrollAmount(20),
  ScrollActivationPercent(0.02f), m_eventRegistered(false)
{
	// set the period for timing event in ms
	setPeriod(20);

	if (m_camera)
	{
		// get the viewport for the camera
		const FIFE::Rect& viewport = m_camera->getViewPort();

		// calculate borders to activate automatic scrolling
        m_scrollAreaTop = static_cast<int>(viewport.h - (viewport.h*ScrollActivationPercent));
		m_scrollAreaBottom = static_cast<int>(viewport.y + (viewport.h*ScrollActivationPercent));
		m_scrollAreaRight = static_cast<int>(viewport.w - (viewport.w*ScrollActivationPercent));
		m_scrollAreaLeft = static_cast<int>(viewport.x + (viewport.w*ScrollActivationPercent));
	}
}

//!***************************************************************
//! @details:
//! destructor
//! 
//!***************************************************************
ScreenScroller::~ScreenScroller()
{
	unregisterEvent();
}

//!***************************************************************
//! @details:
//! tests the current cursor location to see if it is in
//! the area that will cause the screen to scroll
//!
//! @return: 
//! void
//! 
//!***************************************************************
void ScreenScroller::evaluateLocation()
{
	m_scrollCoords = m_camera->toScreenCoordinates(m_camera->getLocationRef().getMapCoordinates());

	m_shouldScroll = false;

	// check left
	if (m_cursorX <= m_scrollAreaLeft)
	{
		// modify x value
		m_scrollCoords[0] -= ScrollAmount;

		m_shouldScroll = true;
	}
	// check right
	else if (m_cursorX >= m_scrollAreaRight)
	{
		// modify x value
		m_scrollCoords[0] += ScrollAmount;

		m_shouldScroll = true;
	}

	// check top
	if (m_cursorY >= m_scrollAreaTop)
	{
		// modify y value
		m_scrollCoords[1] += ScrollAmount;

		m_shouldScroll = true;
	}
	// check bottom
	else if (m_cursorY <= m_scrollAreaBottom)
	{
		// modify y value
		m_scrollCoords[1] -= ScrollAmount;

		m_shouldScroll = true;
	}	
}

//!***************************************************************
//! @details:
//! updates the cursor location
//!
//! @param[in]: x
//! cursor x position
//!
//! @param[in]: y
//! cursor y position
//!
//! @return: 
//! void
//! 
//!***************************************************************
void ScreenScroller::updateLocation(int x, int y)
{
	m_cursorX = x;
	m_cursorY = y;

	// test whether we shoul be scrolling
	evaluateLocation();

	if (m_shouldScroll && !m_eventRegistered)
	{
		// register ourself as an event handler to make
		// sure we can capture if our window looses focus
		// so we don't keep scrolling in that case
		m_timeManager->registerEvent(this);
		m_eventManager->addSdlEventListener(this);
		m_eventRegistered = true;

		// call our internal updater here to kick it off
		updateEvent(-1);
	}
}

//!***************************************************************
//! @details:
//! unregister ourselves from the event notifications
//! we no longer need them
//!
//! @return: 
//! void
//! 
//!***************************************************************
void ScreenScroller::unregisterEvent()
{
	if (m_eventRegistered)
	{
		m_timeManager->unregisterEvent(this);
		m_eventManager->removeSdlEventListener(this);
		m_eventRegistered = false;
	}
}

//!***************************************************************
//! @details:
//! this is called by the time manager on an interval for event
//! notification, we will use this to change our location if we
//! should be scrolling
//!
//! @param: time
//!
//! @return: 
//! void
//! 
//!***************************************************************
void ScreenScroller::updateEvent(uint32_t time)
{
	if (m_shouldScroll)
	{
		FIFE::Location camLocation(m_camera->getLocationRef());
		FIFE::ExactModelCoordinate mapCoords = m_camera->toMapCoordinates(m_scrollCoords, false);
		mapCoords.z = 0.0;
		camLocation.setMapCoordinates(mapCoords);

		m_camera->setLocation(camLocation);
	}
	else
	{
		unregisterEvent();
	}
}

//!***************************************************************
//! @details:
//! handles SDL events, we need this to make sure we capture
//! focus lost events on our window
//!
//! @param[in]: evt
//! SDL event
//!
//! @return: 
//! bool
//! 
//!***************************************************************

bool ScreenScroller::onSdlEvent(SDL_Event& evt)
{
	// if it is a mouse focus event and we have lost focus
	// then we need to unregister for events until
	// we have regained focus
	if (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_LEAVE)
	{
		unregisterEvent();
		return true;
	}

	return false;
}