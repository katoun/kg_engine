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

#include <input/MouseEvent.h>

namespace input
{

MouseEvent::MouseEvent()
{
	mButton = MOUSE_BUTTON_UNKNOWN;
	mAxisX = 0;
	mAxisY = 0;
	mAxisZ = 0;
}

MouseEvent::~MouseEvent() {}

void MouseEvent::set(MouseButton button, signed int axisX, signed short int axisY, signed int axisZ, InputDevice *source)
{
	InputEvent::set(source);
	mButton = button;
	mAxisX = axisX;
	mAxisY = axisY;
	mAxisZ = axisZ;
}

MouseButton MouseEvent::getButton() const
{
	return mButton;
}

signed int MouseEvent::getAxisX() const
{
	return mAxisX;
}

signed int MouseEvent::getAxisY() const
{
	return mAxisY;
}

signed int MouseEvent::getAxisZ() const
{
	return mAxisZ;
}

} // end namespace input
