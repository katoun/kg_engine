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

#include <input/Keyboard.h>
#include <input/KeyEvent.h>

namespace input
{

Keyboard::Keyboard()
{
	mInputType = INPUT_TYPE_KEYBOARD;
	
	mEvent = new KeyEvent();
}

Keyboard::~Keyboard()
{
	delete mEvent;
}

void Keyboard::addKeyEventReceiver(input::KeyEventReceiver* newEventReceiver)
{
	mKeyEventReceivers.push_back(newEventReceiver);
}

void Keyboard::removeKeyEventReceiver(input::KeyEventReceiver* oldEventReceiver)
{
	std::list<input::KeyEventReceiver*>::iterator i;
	for (i = mKeyEventReceivers.begin(); i != mKeyEventReceivers.end(); ++i)
	{
		if ((*i) == oldEventReceiver)
		{
			mKeyEventReceivers.erase(i);
			return;
		}
	}
}

} // end namespace input
