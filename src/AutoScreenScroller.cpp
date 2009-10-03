
#include "AutoScreenScroller.h"

#include "util/time/timemanager.h"
#include "eventchannel/eventmanager.h"
#include "eventchannel/command/ec_command.h"

AutoScreenScroller::AutoScreenScroller(FIFE::Camera* camera, FIFE::EventManager* eventManager)
: m_camera(camera), m_eventManager(eventManager), ScrollAmount(20),
  ScrollActivationPercent(0.02f), m_eventRegistered(false)
{
	// set the period for timing event in ms
	setPeriod(20);

	// get the viewport for the camera
	const FIFE::Rect& viewport = m_camera->getViewPort();

	// calculate borders to activate automatic scrolling
	m_scrollAreaTop = viewport.h - (viewport.h*ScrollActivationPercent);
	m_scrollAreaBottom = viewport.y + (viewport.h*ScrollActivationPercent);
	m_scrollAreaRight = viewport.w - (viewport.w*ScrollActivationPercent);
	m_scrollAreaLeft = viewport.x + (viewport.w*ScrollActivationPercent);
}

AutoScreenScroller::~AutoScreenScroller()
{
	unregisterEvent();
}

void AutoScreenScroller::evaluateLocation()
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

void AutoScreenScroller::updateLocation(int x, int y)
{
	m_cursorX = x;
	m_cursorY = y;

	evaluateLocation();

	if (m_shouldScroll && !m_eventRegistered)
	{
		FIFE::TimeManager::instance()->registerEvent(this);
		m_eventManager->addSdlEventListener(this);
		m_eventRegistered = true;

		// go ahead and call it here once
		updateEvent(-1);
	}
}

void AutoScreenScroller::unregisterEvent()
{
	if (m_eventRegistered)
	{
		FIFE::TimeManager::instance()->unregisterEvent(this);
		m_eventManager->removeSdlEventListener(this);
		m_eventRegistered = false;
	}
}

void AutoScreenScroller::updateEvent(unsigned long time)
{
	// temporary for now, for getting latest camera location
	//evaluateLocation();

	if (m_shouldScroll)
	{
		FIFE::Location camLocation(m_camera->getLocationRef());
		camLocation.setMapCoordinates(m_camera->toMapCoordinates(m_scrollCoords));

		m_camera->setLocation(camLocation);
	}
	else
	{
		unregisterEvent();
	}
}

bool AutoScreenScroller::onSdlEvent(SDL_Event& evt)
{
	// if it is a mouse focus event and we have lost focus
	if (evt.type == SDL_ACTIVEEVENT 
		&& (evt.active.state & SDL_APPMOUSEFOCUS) 
		&& !evt.active.gain)
	{
		unregisterEvent();
		return true;
	}

	return false;
}