/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

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

#ifndef _JOYSTICK_EVENT_H_
#define _JOYSTICK_EVENT_H_

#include <core/Config.h>
#include <input/InputEvent.h>
#include <input/JoystickDefines.h>

namespace input
{

//! JoystickEvent class - DOCUMENTATION NEEDED
class ENGINE_PUBLIC_EXPORT JoystickEvent: public InputEvent
{
public:

	//! Contructor
	JoystickEvent();

	//! Destructor
	~JoystickEvent();

	//! Set the joystick event
	//! \param button: ButtonCode enumeration type, specifying the button.
	//! \param source: The input device which this event originates from.
	//! \param axisX: x-axis position.
	//! \param axisY: y-axis position.
	//! \param axisZ: z-axis position.
	//! \param axisRX: x-axis rotation.
	//! \param axisRY: y-axis rotation.
	//! \param axisRZ: z-axis rotation.
	//! \param axisVX: x-axis velocity.
	//! \param axisVY: y-axis velocity.
	//! \param axisVZ: z-axis velocity.
	//! \param axisVRX: x-axis angular velocity.
	//! \param axisVRY: y-axis angular velocity.
	//! \param axisVRZ: z-axis angular velocity.
	//! \param axisAX: x-axis acceleration.
	//! \param axisAY: y-axis acceleration.
	//! \param axisAZ: z-axis acceleration.
	//! \param axisARX: x-axis angular acceleration.
	//! \param axisARY: y-axis angular acceleration.
	//! \param axisARZ: z-axis angular acceleration.
	//! \param axisFX: x-axis force.
	//! \param axisFY: y-axis force.
	//! \param axisFZ: z-axis force.
	//! \param axisFRX: x-axis torque.
	//! \param axisFRY: y-axis torque.
	//! \param axisFRZ: z-axis torque.
	//! \param sliderX: extra x-axis position.
	//! \param sliderY: extra y-axis position.
	//! \param vsliderX: extra x-axis rotation.
	//! \param vsliderY: extra y-axis rotation.
	//! \param asliderX: extra x-axis acceleration.
	//! \param asliderY: extra y-axis acceleration.
	//! \param fsliderX: extra x-axis force.
	//! \param fsliderY: extra y-axis force.
	//! \param povDirection0: pov0 direction.
	//! \param povDirection1: pov1 direction.
	//! \param povDirection2: pov2 direction.
	//! \param povDirection3: pov3 direction.
	//! \param source: The input device which this event originates from.
	void set(JoystickButton button,
			signed int axisX, signed int axisY, signed int axisZ, signed int axisRX, signed int axisRY, signed int axisRZ,
			signed int axisVX, signed int axisVY, signed int axisVZ, signed int axisVRX, signed int axisVRY, signed int axisVRZ,
			signed int axisAX, signed int axisAY, signed int axisAZ, signed int axisARX, signed int axisARY, signed int axisARZ,
			signed int axisFX, signed int axisFY, signed int axisFZ, signed int axisFRX, signed int axisFRY, signed int axisFRZ,
			signed int sliderX, signed int sliderY,
			signed int vsliderX, signed int vsliderY,
			signed int	asliderX, signed int asliderY,
			signed int fsliderX, signed int fsliderY,
			PovDirection povDirection0, PovDirection povDirection1, PovDirection povDirection2, PovDirection povDirection3,
			InputDevice *source);

	//! getButton - DOCUMENTATION NEEDED
	JoystickButton getButton() const;

	//! getAxisX - DOCUMENTATION NEEDED
	signed int getAxisX() const;

	//! getAxisY - DOCUMENTATION NEEDED
	signed int getAxisY() const;

	//! getAxisZ - DOCUMENTATION NEEDED
	signed int getAxisZ() const;

	//! getAxisRX - DOCUMENTATION NEEDED
	signed int getAxisRX() const;

	//! getAxisRY - DOCUMENTATION NEEDED
	signed int getAxisRY() const;

	//! getAxisRZ - DOCUMENTATION NEEDED
	signed int getAxisRZ() const;


	//! getAxisVX - DOCUMENTATION NEEDED
	signed int getAxisVX() const;

	//! getAxisVY - DOCUMENTATION NEEDED
	signed int getAxisVY() const;

	//! getAxisVZ - DOCUMENTATION NEEDED
	signed int getAxisVZ() const;

	//! getAxisVRX - DOCUMENTATION NEEDED
	signed int getAxisVRX() const;

	//! getAxisVRY - DOCUMENTATION NEEDED
	signed int getAxisVRY() const;

	//! getAxisVRZ - DOCUMENTATION NEEDED
	signed int getAxisVRZ() const;


	//! getAxisAX - DOCUMENTATION NEEDED
	signed int getAxisAX() const;

	//! getAxisAY - DOCUMENTATION NEEDED
	signed int getAxisAY() const;

	//! getAxisAZ - DOCUMENTATION NEEDED
	signed int getAxisAZ() const;

	//! getAxisARX - DOCUMENTATION NEEDED
	signed int getAxisARX() const;

	//! getAxisARY - DOCUMENTATION NEEDED
	signed int getAxisARY() const;

	//! getAxisARZ - DOCUMENTATION NEEDED
	signed int getAxisARZ() const;


	//! getAxisFX - DOCUMENTATION NEEDED
	signed int getAxisFX() const;

	//! getAxisFY - DOCUMENTATION NEEDED
	signed int getAxisFY() const;

	//! getAxisFZ - DOCUMENTATION NEEDED
	signed int getAxisFZ() const;

	//! getAxisFRX - DOCUMENTATION NEEDED
	signed int getAxisFRX() const;

	//! getAxisFRY - DOCUMENTATION NEEDED
	signed int getAxisFRY() const;

	//! getAxisFRZ - DOCUMENTATION NEEDED
	signed int getAxisFRZ() const;


	//! getSliderX - DOCUMENTATION NEEDED
	signed int getSliderX() const;

	//! getSliderY - DOCUMENTATION NEEDED
	signed int getSliderY() const;

	//! getVSliderX - DOCUMENTATION NEEDED
	signed int getVSliderX() const;

	//! getVSliderY - DOCUMENTATION NEEDED
	signed int getVSliderY() const;

	//! getASliderX - DOCUMENTATION NEEDED
	signed int getASliderX() const;

	//! getASliderY - DOCUMENTATION NEEDED
	signed int getASliderY() const;

	//! getFSliderX - DOCUMENTATION NEEDED
	signed int getFSliderX() const;

	//! getFSliderY - DOCUMENTATION NEEDED
	signed int getFSliderY() const;
	
	//! getPovDirection0 - DOCUMENTATION NEEDED
	PovDirection getPovDirection0();

	//! getPovDirection1 - DOCUMENTATION NEEDED
	PovDirection getPovDirection1();

	//! getPovDirection2 - DOCUMENTATION NEEDED
	PovDirection getPovDirection2();

	//! getPovDirection3 - DOCUMENTATION NEEDED
	PovDirection getPovDirection3();

protected:

	JoystickButton mButton;

	signed int mAxisX;		// x-axis position
	signed int mAxisY;		// y-axis position
	signed int mAxisZ;		// z-axis position

	signed int mAxisRX;	// x-axis rotation
	signed int mAxisRY;	// y-axis rotation
	signed int mAxisRZ;	// z-axis rotation

	signed int mAxisVX;	// x-axis velocity
	signed int mAxisVY;	// y-axis velocity
	signed int mAxisVZ;	// z-axis velocity

	signed int mAxisVRX;	// x-axis angular velocity
	signed int mAxisVRY;	// y-axis angular velocity
	signed int mAxisVRZ;	// z-axis angular velocity

	signed int mAxisAX;	// x-axis acceleration
	signed int mAxisAY;	// y-axis acceleration
	signed int mAxisAZ;	// z-axis acceleration

	signed int mAxisARX;	// x-axis angular acceleration
	signed int mAxisARY;	// y-axis angular acceleration
	signed int mAxisARZ;	// z-axis angular acceleration

	signed int mAxisFX;	// x-axis force
	signed int mAxisFY;	// y-axis force
	signed int mAxisFZ;	// z-axis force

	signed int mAxisFRX;	// x-axis torque
	signed int mAxisFRY;	// y-axis torque
	signed int mAxisFRZ;	// z-axis torque

	signed int mSliderX;	// extra x-axis position
	signed int mSliderY;	// extra y-axis position

	signed int mVSliderX;	// extra x-axis velocity
	signed int mVSliderY;	// extra y-axis velocity

	signed int mASliderX;	// extra x-axis acceleration
	signed int mASliderY;	// extra y-axis acceleration

	signed int mFSliderX;	// extra x-axis force
	signed int mFSliderY;	// extra y-axis force

	PovDirection	mPovDirection0;
	PovDirection	mPovDirection1;
	PovDirection	mPovDirection2;
	PovDirection	mPovDirection3;
};

} // end namespace input

#endif