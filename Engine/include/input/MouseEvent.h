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

#ifndef _MOUSE_EVENT_H_
#define _MOUSE_EVENT_H_

#include <core/Config.h>
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
	void set(MouseButton button, signed int axisX, signed short int axisY, signed int axisZ, InputDevice *source);

	//! getButton - DOCUMENTATION NEEDED
	MouseButton getButton() const;

	//! getAxisX - DOCUMENTATION NEEDED
	signed int getAxisX() const;

	//! getAxisY - DOCUMENTATION NEEDED
	signed int getAxisY() const;

	//! getAxisZ - DOCUMENTATION NEEDED
	signed int getAxisZ() const;

protected:

	MouseButton mButton;

	signed int mAxisX;
	signed int mAxisY;
	signed int mAxisZ;
};

} // end namespace input

#endif