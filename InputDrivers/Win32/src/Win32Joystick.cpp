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
