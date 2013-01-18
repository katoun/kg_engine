/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2013 Catalin Alexandru Nastase

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
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
