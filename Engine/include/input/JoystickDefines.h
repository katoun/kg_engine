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

#ifndef _JOYSTICK_DEFINES_H_
#define _JOYSTICK_DEFINES_H_

namespace input
{

//! Joystick scan codes.
enum JoystickButton
{
	JOYSTICK_BUTTON_UNKNOWN,
	JOYSTICK_BUTTON1,
	JOYSTICK_BUTTON2,
	JOYSTICK_BUTTON3,
	JOYSTICK_BUTTON4,
	JOYSTICK_BUTTON5,
	JOYSTICK_BUTTON6,
	JOYSTICK_BUTTON7,
	JOYSTICK_BUTTON8,
	JOYSTICK_BUTTON9,
	JOYSTICK_BUTTON10,
	JOYSTICK_BUTTON11,
	JOYSTICK_BUTTON12,
	JOYSTICK_BUTTON13,
	JOYSTICK_BUTTON14,
	JOYSTICK_BUTTON15,
	JOYSTICK_BUTTON16,
	JOYSTICK_BUTTON17,
	JOYSTICK_BUTTON18,
	JOYSTICK_BUTTON19,
	JOYSTICK_BUTTON20,
	JOYSTICK_BUTTON21,
	JOYSTICK_BUTTON22,
	JOYSTICK_BUTTON23,
	JOYSTICK_BUTTON24,
	JOYSTICK_BUTTON25,
	JOYSTICK_BUTTON26,
	JOYSTICK_BUTTON27,
	JOYSTICK_BUTTON28,
	JOYSTICK_BUTTON29,
	JOYSTICK_BUTTON30,
	JOYSTICK_BUTTON31,
	JOYSTICK_BUTTON32
};

enum PovDirection
{
	POV_DIRECTION_CENTER,
	POV_DIRECTION_NORTH,
	POV_DIRECTION_SOUTH,
	POV_DIRECTION_EAST,
	POV_DIRECTION_WEST,
	POV_DIRECTION_NORTHEAST,
	POV_DIRECTION_SOUTHEAST,
	POV_DIRECTION_NORTHWEST,
	POV_DIRECTION_SOUTHWEST
};

} // end namespace input

#endif