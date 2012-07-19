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

#ifndef _INPUT_DRIVER_H_
#define _INPUT_DRIVER_H_

#include <core/Config.h>
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
	virtual signed int numJoySticks() = 0;

	//!	Returns the number of mice found.
	virtual signed int numMice() = 0;

	//!	Returns the number of keyboards found.
	virtual signed int numKeyboards() = 0;

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