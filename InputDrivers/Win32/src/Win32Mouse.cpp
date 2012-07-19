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

#include <input/MouseDefines.h>
#include <input/MouseEvent.h>
#include <input/MouseEventReceiver.h>
#include <Win32Mouse.h>
#include <Win32InputDriver.h>

namespace input
{

Win32Mouse::Win32Mouse(bool buffered): Mouse()
{
	mBuffered = buffered;
}

Win32Mouse::~Win32Mouse() {}

void Win32Mouse::sendButtonDown(oi_mouse button)
{
	mEvent->set(mButtonMap[button], 0, 0, 0, this);

	std::list<input::MouseEventReceiver*>::const_iterator i;
	for (i = mMouseEventReceivers.begin(); i != mMouseEventReceivers.end(); ++i)
	{
		(*i)->mousePressed(*mEvent);
	}
}

void Win32Mouse::sendButtonUp(oi_mouse button)
{
	mEvent->set(mButtonMap[button], 0, 0, 0, this);
	
	std::list<input::MouseEventReceiver*>::const_iterator i;
	for (i = mMouseEventReceivers.begin(); i != mMouseEventReceivers.end(); ++i)
	{
		(*i)->mouseReleased(*mEvent);
	}
}

void Win32Mouse::sendMove(signed int axisX, signed int axisY)
{
	mEvent->set(MOUSE_BUTTON_UNKNOWN, axisX, axisY, 0, this);

	std::list<input::MouseEventReceiver*>::const_iterator i;
	for (i = mMouseEventReceivers.begin(); i != mMouseEventReceivers.end(); ++i)
	{
		(*i)->mouseMoved(*mEvent);
	}
}

void Win32Mouse::setGrab(bool grabbed)
{
	oi_app_grab((oi_bool)grabbed);
}

void Win32Mouse::initializeImpl()
{
	mButtonMap[OIP_UNKNOWN] = MOUSE_BUTTON_UNKNOWN;
	mButtonMap[OIP_BUTTON_LEFT] = MOUSE_LEFT_BUTTON;
	mButtonMap[OIP_BUTTON_RIGHT] = MOUSE_RIGHT_BUTTON;
	mButtonMap[OIP_BUTTON_MIDDLE] = MOUSE_MIDDLE_BUTTON;

	setGrab(true);
}

void Win32Mouse::uninitializeImpl()
{
	setGrab(false);
}

void Win32Mouse::updateImpl(float elapsedTime) {}

} // end namespace input
