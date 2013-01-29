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

#ifndef _MOUSE_EVENT_H_
#define _MOUSE_EVENT_H_

#include <EngineConfig.h>
#include <input/InputEvent.h>
#include <input/MouseDefines.h>

namespace input
{

//! MouseEvent class - DOCUMENTATION NEEDED
class ENGINE_PUBLIC_EXPORT MouseEvent: public InputEvent
{
public:

	//! Constructor
	MouseEvent();

	//! Destructor
	~MouseEvent();

	//! Set the mouse event
	//! \param button: ButtonCode enumeration type, specifying the button.
	//! \param axisX: The X axis of the mouse.
	//! \param axisY: The Y axis of the mouse.
	//! \param axisZ: The Z axis of the mouse.
	//! \param source: Which input device the mouse event is coming from.
	void set(MouseButton button, int axisX, signed short int axisY, int axisZ, InputDevice *source);

	//! getButton - DOCUMENTATION NEEDED
	MouseButton getButton() const;

	//! getAxisX - DOCUMENTATION NEEDED
	int getAxisX() const;

	//! getAxisY - DOCUMENTATION NEEDED
	int getAxisY() const;

	//! getAxisZ - DOCUMENTATION NEEDED
	int getAxisZ() const;

protected:

	MouseButton mButton;

	int mAxisX;
	int mAxisY;
	int mAxisZ;
};

} // end namespace input

#endif