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

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <core/Config.h>
#include <input/InputDevice.h>

#include <list>

namespace input
{

class KeyEventReceiver;
class KeyEvent;
enum KeyCode;
enum KeyModifier;

//! A common base class for keyboard device.
class ENGINE_PUBLIC_EXPORT Keyboard : public InputDevice
{
public:

	//! Constructor
	Keyboard();

	//! Destructor
	virtual ~Keyboard();

	void addKeyEventReceiver(input::KeyEventReceiver* newEventReceiver);
	void removeKeyEventReceiver(input::KeyEventReceiver* oldEventReceiver);

protected:

	//// event-cascading
	std::list<input::KeyEventReceiver*> mKeyEventReceivers;

	KeyEvent* mEvent;
};

} // end namespace input

#endif