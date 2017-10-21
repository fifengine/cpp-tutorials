//*****************************************************************************
// FILE NAME:  KeyListener.cpp
//
//*****************************************************************************
#include <iostream>

#include "eventchannel/key/keyevent.h"
#include "eventchannel/key/key.h"

#include "Game.h"
#include "ViewController.h"
#include "KeyListener.h"

#include <cassert>

//!***************************************************************
//! @details:
//! constructor
//!
//! @param[in]: parent
//! the game object that created the key listener
//! 
//!***************************************************************
KeyListener::KeyListener(Game* parent)
: m_parent(parent)
{
	assert(m_parent);
}

//!***************************************************************
//! @details:
//! destructor
//!
//!***************************************************************
KeyListener::~KeyListener()
{

}

//!***************************************************************
//! @details:
//! overridden function from base class
//! this function is called when a key is pressed
//! and will be repeatedly called when the key is held down
//!
//! @param[in]: evt
//! the signaled event
//!
//! @return: 
//! void
//! 
//!***************************************************************
void KeyListener::keyPressed(FIFE::KeyEvent& evt)
{

}

//!***************************************************************
//! @details:
//! overridden function from base class
//! this function is called when a key is released
//!
//! @param[in]: evt
//! the signaled event
//!
//! @return: 
//! void
//! 
//!***************************************************************
void KeyListener::keyReleased(FIFE::KeyEvent& evt)
{
	// map event key value to a game action
	switch (evt.getKey().getValue())
	{
		case FIFE::Key::ESCAPE:
		{
			m_parent->Quit();
			break;
		}
		case FIFE::Key::LEFT:
		{
			m_parent->GetViewController()->RotateLeft();
			break;
		}
		case FIFE::Key::RIGHT:
		{
			m_parent->GetViewController()->RotateRight();
			break;
		}
		case FIFE::Key::UP:
		{
			m_parent->GetViewController()->ZoomIn();
			break;
		}
		case FIFE::Key::DOWN:
		{
			m_parent->GetViewController()->ZoomOut();
			break;
		}
		case FIFE::Key::BACKQUOTE:
		{
			m_parent->toggleConsole();
			break;
		}
		default:
		{
			break;
		}
	}

	// mark event as consumed
	evt.consume();
}