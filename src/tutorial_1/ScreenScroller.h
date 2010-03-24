//*****************************************************************************
// FILE NAME:  ScreenScroller.h
//
//*****************************************************************************
#ifndef SCREEN_SCROLLER_H_
#define SCREEN_SCROLLER_H_

#include "util/time/timeevent.h"
#include "eventchannel/command/ec_icommandlistener.h"
#include "eventchannel/sdl/ec_isdleventlistener.h"
#include "view/camera.h"

namespace FIFE
{
	class Command;
	class EventManager;
}

//! provides automatic scrolling when the cursor is near the edge of the screen
class ScreenScroller : public FIFE::TimeEvent, public FIFE::ISdlEventListener
{
public:
	ScreenScroller(FIFE::Camera* camera, FIFE::EventManager* eventManager);
	~ScreenScroller();

	void updateLocation(int x, int y);
	void unregisterEvent();
private:
	void evaluateLocation();
	void updateEvent(unsigned long time);
	bool onSdlEvent(SDL_Event& evt);
private:
	FIFE::Camera* m_camera;
	FIFE::EventManager* m_eventManager;
	const int ScrollAmount;
	const float ScrollActivationPercent; 
	int m_cursorX;
	int m_cursorY;
	bool m_shouldScroll;
	bool m_eventRegistered;
	FIFE::ScreenPoint m_scrollCoords;
	int m_scrollAreaTop;
	int m_scrollAreaBottom;
	int m_scrollAreaRight;
	int m_scrollAreaLeft;
};

#endif