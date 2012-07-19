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

#include <input/JoystickEvent.h>
#include <input/JoystickEventReceiver.h>
#include <Win32Joystick.h>

namespace input
{

Win32Joystick::Win32Joystick(bool buffered): Joystick()
{
	mBuffered = buffered;

	axisX = axisY = axisZ = axisRX = axisRY = axisRZ = 0;
	axisVX = axisVY = axisVZ = axisVRX = axisVRY = axisVRZ = 0;
	axisAX = axisAY = axisAZ = axisARX = axisARY = axisARZ = 0;
	axisFX = axisFY = axisFZ = axisFRX = axisFRY = axisFRZ = 0;

	sliderX = sliderY = 0;
	vsliderX = vsliderY = 0;
	asliderX = asliderY = 0;
	fsliderX = fsliderY = 0;

	povDirection0 = povDirection1 = povDirection2 = povDirection3 = POV_DIRECTION_CENTER;

	mAxisNumber = 0;
}

Win32Joystick::~Win32Joystick() {}

void Win32Joystick::initializeImpl() {}

void Win32Joystick::uninitializeImpl() {}

void Win32Joystick::updateImpl(float elapsedTime) {}

void Win32Joystick::enumerate() {}

} // end namespace input
