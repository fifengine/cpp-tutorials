//*****************************************************************************
// FILE NAME:  KeyListener.h
//
//*****************************************************************************
#ifndef KEY_LISTENER_H_
#define KEY_LISTENER_H_

#include "eventchannel/key/ikeylistener.h"

namespace FIFE
{
	class KeyEvent;
}

class Game;

//! handler for listening to keyboard input
class KeyListener : public FIFE::IKeyListener
{
public:
	KeyListener(Game* parent);
	~KeyListener();

	// overridden from base class
	virtual void keyPressed(FIFE::KeyEvent& evt);
	virtual void keyReleased(FIFE::KeyEvent& evt);

private:
	Game* m_parent;
};

#endif
