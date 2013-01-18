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

#include <input/JoystickEvent.h>

namespace input
{

JoystickEvent::JoystickEvent()
{
	mButton = JOYSTICK_BUTTON_UNKNOWN;

	mAxisX = 0;
	mAxisY = 0;
	mAxisZ = 0;

	mAxisRX = 0;
	mAxisRY = 0;
	mAxisRZ = 0;

	mAxisVX = 0;
	mAxisVY = 0;
	mAxisVZ = 0;

	mAxisVRX = 0;
	mAxisVRY = 0;
	mAxisVRZ = 0;

	mAxisAX = 0;
	mAxisAY = 0;
	mAxisAZ = 0;

	mAxisARX = 0;
	mAxisARY = 0;
	mAxisARZ = 0;

	mAxisFX = 0;
	mAxisFY = 0;
	mAxisFZ = 0;

	mAxisFRX = 0;
	mAxisFRY = 0;
	mAxisFRZ = 0;


	mSliderX = 0;
	mSliderY = 0;

	mVSliderX = 0;
	mVSliderY = 0;

	mASliderX = 0;
	mASliderY = 0;

	mFSliderX = 0;
	mFSliderY = 0;

	mPovDirection0 = POV_DIRECTION_CENTER;
	mPovDirection1 = POV_DIRECTION_CENTER;
	mPovDirection2 = POV_DIRECTION_CENTER;
	mPovDirection3 = POV_DIRECTION_CENTER;

}

JoystickEvent::~JoystickEvent() {}

void JoystickEvent::set(JoystickButton button,
						int axisX, int axisY, int axisZ, int axisRX, int axisRY, int axisRZ,
						int axisVX, int axisVY, int axisVZ, int axisVRX, int axisVRY, int axisVRZ,
						int axisAX, int axisAY, int axisAZ, int axisARX, int axisARY, int axisARZ,
						int axisFX, int axisFY, int axisFZ, int axisFRX, int axisFRY, int axisFRZ,
						int sliderX, int sliderY,
						int vsliderX, int vsliderY,
						int	asliderX, int asliderY,
						int fsliderX, int fsliderY,
						PovDirection povDirection0, PovDirection povDirection1, PovDirection povDirection2, PovDirection povDirection3,
						InputDevice *source)
{
	InputEvent::set(source);
	mButton = button;

	mAxisX = axisX;
	mAxisY = axisY;
	mAxisZ = axisZ;

	mAxisRX = axisRX;
	mAxisRY = axisRY;
	mAxisRZ = axisRZ;

	mAxisVX = axisVX;
	mAxisVY = axisVY;
	mAxisVZ = axisVZ;

	mAxisVRX = axisVRX;
	mAxisVRY = axisVRY;
	mAxisVRZ = axisVRZ;

	mAxisAX = axisAX;
	mAxisAY = axisAY;
	mAxisAZ = axisAZ;

	mAxisARX = axisARX;
	mAxisARY = axisARY;
	mAxisARZ = axisARZ;

	mAxisFX = axisFX;
	mAxisFY = axisFY;
	mAxisFZ = axisFZ;

	mAxisFRX = axisFRX;
	mAxisFRY = axisFRY;
	mAxisFRZ = axisFRZ;

	mSliderX = sliderX;
	mSliderY = sliderY;

	mVSliderX = vsliderX;
	mVSliderY = vsliderY;

	mASliderX = asliderX;
	mASliderY = asliderY;

	mFSliderX = fsliderX;
	mFSliderY = fsliderY;

	mPovDirection0 = povDirection0;
	mPovDirection1 = povDirection1;
	mPovDirection2 = povDirection2;
	mPovDirection3 = povDirection3;
}

JoystickButton JoystickEvent::getButton() const
{
	return mButton;
}

int JoystickEvent::getAxisX() const
{
	return mAxisX;
}

int JoystickEvent::getAxisY() const
{
	return mAxisY;
}

int JoystickEvent::getAxisZ() const
{
	return mAxisZ;
}

int JoystickEvent::getAxisRX() const
{
	return mAxisRX;
}

int JoystickEvent::getAxisRY() const
{
	return mAxisRY;
}

int JoystickEvent::getAxisRZ() const
{
	return mAxisRZ;
}


int JoystickEvent::getAxisVX() const
{
	return mAxisVX;
}

int JoystickEvent::getAxisVY() const
{
	return mAxisVY;
}

int JoystickEvent::getAxisVZ() const
{
	return mAxisVZ;
}

int JoystickEvent::getAxisVRX() const
{
	return mAxisVRX;
}

int JoystickEvent::getAxisVRY() const
{
	return mAxisVRY;
}

int JoystickEvent::getAxisVRZ() const
{
	return mAxisVRZ;
}


int JoystickEvent::getAxisAX() const
{
	return mAxisAX;
}

int JoystickEvent::getAxisAY() const
{
	return mAxisAY;
}

int JoystickEvent::getAxisAZ() const
{
	return mAxisAZ;
}

int JoystickEvent::getAxisARX() const
{
	return mAxisARX;
}

int JoystickEvent::getAxisARY() const
{
	return mAxisARY;
}

int JoystickEvent::getAxisARZ() const
{
	return mAxisARZ;
}


int JoystickEvent::getAxisFX() const
{
	return mAxisFX;
}

int JoystickEvent::getAxisFY() const
{
	return mAxisFY;
}

int JoystickEvent::getAxisFZ() const
{
	return mAxisFZ;
}

int JoystickEvent::getAxisFRX() const
{
	return mAxisFRX;
}

int JoystickEvent::getAxisFRY() const
{
	return mAxisFRY;
}

int JoystickEvent::getAxisFRZ() const
{
	return mAxisFRZ;
}

int JoystickEvent::getSliderX() const
{
	return mSliderX;
}

int JoystickEvent::getSliderY() const
{
	return mSliderY;
}

int JoystickEvent::getVSliderX() const
{
	return mVSliderX;
}

int JoystickEvent::getVSliderY() const
{
	return mVSliderY;
}

int JoystickEvent::getASliderX() const
{
	return mASliderX;
}

int JoystickEvent::getASliderY() const
{
	return mASliderY;
}

int JoystickEvent::getFSliderX() const
{
	return mFSliderX;
}

int JoystickEvent::getFSliderY() const
{
	return mFSliderY;
}

PovDirection JoystickEvent::getPovDirection0()
{
	return mPovDirection0;
}

PovDirection JoystickEvent::getPovDirection1()
{
	return mPovDirection1;
}

PovDirection JoystickEvent::getPovDirection2()
{
	return mPovDirection2;
}

PovDirection JoystickEvent::getPovDirection3()
{
	return mPovDirection3;
}

} // end namespace input
