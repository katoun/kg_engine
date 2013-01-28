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

#ifndef _JOYSTICK_EVENT_H_
#define _JOYSTICK_EVENT_H_

#include <Config.h>
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
			int axisX, int axisY, int axisZ, int axisRX, int axisRY, int axisRZ,
			int axisVX, int axisVY, int axisVZ, int axisVRX, int axisVRY, int axisVRZ,
			int axisAX, int axisAY, int axisAZ, int axisARX, int axisARY, int axisARZ,
			int axisFX, int axisFY, int axisFZ, int axisFRX, int axisFRY, int axisFRZ,
			int sliderX, int sliderY,
			int vsliderX, int vsliderY,
			int	asliderX, int asliderY,
			int fsliderX, int fsliderY,
			PovDirection povDirection0, PovDirection povDirection1, PovDirection povDirection2, PovDirection povDirection3,
			InputDevice *source);

	//! getButton - DOCUMENTATION NEEDED
	JoystickButton getButton() const;

	//! getAxisX - DOCUMENTATION NEEDED
	int getAxisX() const;

	//! getAxisY - DOCUMENTATION NEEDED
	int getAxisY() const;

	//! getAxisZ - DOCUMENTATION NEEDED
	int getAxisZ() const;

	//! getAxisRX - DOCUMENTATION NEEDED
	int getAxisRX() const;

	//! getAxisRY - DOCUMENTATION NEEDED
	int getAxisRY() const;

	//! getAxisRZ - DOCUMENTATION NEEDED
	int getAxisRZ() const;


	//! getAxisVX - DOCUMENTATION NEEDED
	int getAxisVX() const;

	//! getAxisVY - DOCUMENTATION NEEDED
	int getAxisVY() const;

	//! getAxisVZ - DOCUMENTATION NEEDED
	int getAxisVZ() const;

	//! getAxisVRX - DOCUMENTATION NEEDED
	int getAxisVRX() const;

	//! getAxisVRY - DOCUMENTATION NEEDED
	int getAxisVRY() const;

	//! getAxisVRZ - DOCUMENTATION NEEDED
	int getAxisVRZ() const;


	//! getAxisAX - DOCUMENTATION NEEDED
	int getAxisAX() const;

	//! getAxisAY - DOCUMENTATION NEEDED
	int getAxisAY() const;

	//! getAxisAZ - DOCUMENTATION NEEDED
	int getAxisAZ() const;

	//! getAxisARX - DOCUMENTATION NEEDED
	int getAxisARX() const;

	//! getAxisARY - DOCUMENTATION NEEDED
	int getAxisARY() const;

	//! getAxisARZ - DOCUMENTATION NEEDED
	int getAxisARZ() const;


	//! getAxisFX - DOCUMENTATION NEEDED
	int getAxisFX() const;

	//! getAxisFY - DOCUMENTATION NEEDED
	int getAxisFY() const;

	//! getAxisFZ - DOCUMENTATION NEEDED
	int getAxisFZ() const;

	//! getAxisFRX - DOCUMENTATION NEEDED
	int getAxisFRX() const;

	//! getAxisFRY - DOCUMENTATION NEEDED
	int getAxisFRY() const;

	//! getAxisFRZ - DOCUMENTATION NEEDED
	int getAxisFRZ() const;


	//! getSliderX - DOCUMENTATION NEEDED
	int getSliderX() const;

	//! getSliderY - DOCUMENTATION NEEDED
	int getSliderY() const;

	//! getVSliderX - DOCUMENTATION NEEDED
	int getVSliderX() const;

	//! getVSliderY - DOCUMENTATION NEEDED
	int getVSliderY() const;

	//! getASliderX - DOCUMENTATION NEEDED
	int getASliderX() const;

	//! getASliderY - DOCUMENTATION NEEDED
	int getASliderY() const;

	//! getFSliderX - DOCUMENTATION NEEDED
	int getFSliderX() const;

	//! getFSliderY - DOCUMENTATION NEEDED
	int getFSliderY() const;
	
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

	int mAxisX;		// x-axis position
	int mAxisY;		// y-axis position
	int mAxisZ;		// z-axis position

	int mAxisRX;	// x-axis rotation
	int mAxisRY;	// y-axis rotation
	int mAxisRZ;	// z-axis rotation

	int mAxisVX;	// x-axis velocity
	int mAxisVY;	// y-axis velocity
	int mAxisVZ;	// z-axis velocity

	int mAxisVRX;	// x-axis angular velocity
	int mAxisVRY;	// y-axis angular velocity
	int mAxisVRZ;	// z-axis angular velocity

	int mAxisAX;	// x-axis acceleration
	int mAxisAY;	// y-axis acceleration
	int mAxisAZ;	// z-axis acceleration

	int mAxisARX;	// x-axis angular acceleration
	int mAxisARY;	// y-axis angular acceleration
	int mAxisARZ;	// z-axis angular acceleration

	int mAxisFX;	// x-axis force
	int mAxisFY;	// y-axis force
	int mAxisFZ;	// z-axis force

	int mAxisFRX;	// x-axis torque
	int mAxisFRY;	// y-axis torque
	int mAxisFRZ;	// z-axis torque

	int mSliderX;	// extra x-axis position
	int mSliderY;	// extra y-axis position

	int mVSliderX;	// extra x-axis velocity
	int mVSliderY;	// extra y-axis velocity

	int mASliderX;	// extra x-axis acceleration
	int mASliderY;	// extra y-axis acceleration

	int mFSliderX;	// extra x-axis force
	int mFSliderY;	// extra y-axis force

	PovDirection	mPovDirection0;
	PovDirection	mPovDirection1;
	PovDirection	mPovDirection2;
	PovDirection	mPovDirection3;
};

} // end namespace input

#endif