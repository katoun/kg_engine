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

#ifndef _JOISTICK_EVENT_RECEIVER_H_
#define _JOISTICK_EVENT_RECEIVER_H_

#include <EngineConfig.h>

namespace input
{

class JoystickEvent;

//! Interface class for creating an event receiver and adding it to a renderwindow,
//! to set that renderwindow to receive keyboard events.
class ENGINE_PUBLIC_EXPORT JoystickEventReceiver
{
public:

	virtual ~JoystickEventReceiver();

	//! Called if a button is pressed.
	//! \param event: Reference to a JoystickEvent object, to get information
	//! about the event.
	virtual void buttonPressed(JoystickEvent& event);

	//! Called if a button is released.
	//! \param event: Reference to a JoystickEvent object, to get information
	//! about the event.
	virtual void buttonReleased(JoystickEvent& event);

	//! Called if an axis is moved.
	//! \param event: Reference to a JoystickEvent object, to get information
	//! about the event.
	virtual void axisMoved(JoystickEvent& event);

	//! Called if an slider is moved.
	//! \param event: Reference to a JoystickEvent object, to get information
	//! about the event.
	virtual void sliderMoved(JoystickEvent& event);

	//! Called if a pov is moved.
	//! \param event: Reference to a JoystickEvent object, to get information
	//! about the event.
	virtual void povMoved(JoystickEvent& event);
};

} // end namespace input

#endif