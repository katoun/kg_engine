/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2012 Catalin Alexandru Nastase

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