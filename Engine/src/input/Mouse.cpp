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

#include <input/Mouse.h>
#include <input/MouseEvent.h>

namespace input
{

Mouse::Mouse()
{
	mInputType = INPUT_TYPE_MOUSE;
	
	mEvent = new MouseEvent();
}

Mouse::~Mouse()
{
	delete mEvent;
}

void Mouse::addMouseEventReceiver(input::MouseEventReceiver* newEventReceiver)
{
	mMouseEventReceivers.push_back(newEventReceiver);
}

void Mouse::removeMouseEventReceiver(input::MouseEventReceiver* oldEventReceiver)
{
	std::list<input::MouseEventReceiver*>::iterator i;
	for (i = mMouseEventReceivers.begin(); i != mMouseEventReceivers.end(); ++i)
	{
		if ((*i) == oldEventReceiver)
		{
			mMouseEventReceivers.erase(i);
			return;
		}
	}
}

} // end namespace input
