/*
-----------------------------------------------------------------------------
Copyright (c) 2006-2012 Catalin Alexandru Nastase

KG game engine (http://k-game.sourceforge.net) is made available under the LGPL License (http://www.gnu.org/copyleft/lgpl.html)

Under the LGPL you may use KG game engine for any purpose you wish, as long as you:
1. Release any modifications to the KG game engine source back to the community
2. Pass on the source to KG game engine with all the copyrights intact, or link back to a place where the source code can be obtained (http://k-game.sourceforge.net)
3. Make it clear where you have customised it.
The above is a precis, please do read the full license agreement.
-----------------------------------------------------------------------------
*/

#ifndef _KEY_EVENT_RECEIVER_H_
#define _KEY_EVENT_RECEIVER_H_

#include <core/Config.h>

namespace input
{

class KeyEvent;

//! Interface class for creating an event receiver and adding it to a renderwindow,
//! to set that renderwindow to receive keyboard events.
class ENGINE_PUBLIC_EXPORT KeyEventReceiver
{
public:

	virtual ~KeyEventReceiver();

	//! Called if a key is pressed.
	//! \param event: Reference to a KeyEvent object, to get information
	//! about the event.
	virtual void keyPressed(KeyEvent& event);

	//! Called if a key is released.
	//! \param event: Reference to a KeyEvent object, to get information
	//! about the event.
	virtual void keyReleased(KeyEvent& event);
};

} // end namespace input

#endif