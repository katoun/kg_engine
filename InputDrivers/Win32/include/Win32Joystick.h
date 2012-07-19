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

#ifndef _WIN32_JOYSTICK_H_
#define _WIN32_JOYSTICK_H_

#include <Win32Config.h>
#include <input/Joystick.h>

namespace input
{

enum JoystickButton;
enum PovDirection;

class Win32Joystick : public Joystick
{
public:

	//! Constructor
	Win32Joystick(bool buffered);

	//! Destructor
	~Win32Joystick();

protected:

	void initializeImpl();
	void uninitializeImpl();

	void updateImpl(float elapsedTime);

	void enumerate();

	signed int axisX, axisY, axisZ, axisRX, axisRY, axisRZ;
	signed int axisVX, axisVY, axisVZ, axisVRX, axisVRY, axisVRZ;
	signed int axisAX, axisAY, axisAZ, axisARX, axisARY, axisARZ;
	signed int axisFX, axisFY, axisFZ, axisFRX, axisFRY, axisFRZ;

	signed int sliderX, sliderY;
	signed int vsliderX, vsliderY;
	signed int	asliderX, asliderY;
	signed int fsliderX, fsliderY;

	PovDirection povDirection0, povDirection1, povDirection2, povDirection3;

	signed int mAxisNumber;
};

} // end namespace input

#endif