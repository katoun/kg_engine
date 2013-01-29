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

#ifndef _INPUT_DRIVER_H_
#define _INPUT_DRIVER_H_

#include <EngineConfig.h>
#include <core/SystemDriver.h>
#include <core/Position2d.h>

namespace input
{

class Cursor;
class InputDevice;
enum InputType;

//! Defines the functionality of a input API
//! Author: Kat'Oun
//! version: 1.0
class ENGINE_PUBLIC_EXPORT InputDriver: public core::SystemDriver
{
public:

	//! Default Constructor
	InputDriver(const std::string& name);

	//! Destructor
	virtual ~InputDriver();

	//! Returns the number of joysticks found.
	virtual int numJoySticks() = 0;

	//!	Returns the number of mice found.
	virtual int numMice() = 0;

	//!	Returns the number of keyboards found.
	virtual int numKeyboards() = 0;

	//! Return an input device.
	virtual InputDevice* createInputDevice(InputType type, bool buffered) = 0;

	virtual void removeInputDevice(InputDevice* device) = 0;

	Cursor* getCursor();

	const core::position2d& getCursorCenter() const;

protected:

	core::position2d mCursorCenter;

	Cursor* mCursor;
};

} // end namespace input

#endif