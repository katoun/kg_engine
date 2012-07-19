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
						signed int axisX, signed int axisY, signed int axisZ, signed int axisRX, signed int axisRY, signed int axisRZ,
						signed int axisVX, signed int axisVY, signed int axisVZ, signed int axisVRX, signed int axisVRY, signed int axisVRZ,
						signed int axisAX, signed int axisAY, signed int axisAZ, signed int axisARX, signed int axisARY, signed int axisARZ,
						signed int axisFX, signed int axisFY, signed int axisFZ, signed int axisFRX, signed int axisFRY, signed int axisFRZ,
						signed int sliderX, signed int sliderY,
						signed int vsliderX, signed int vsliderY,
						signed int	asliderX, signed int asliderY,
						signed int fsliderX, signed int fsliderY,
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

signed int JoystickEvent::getAxisX() const
{
	return mAxisX;
}

signed int JoystickEvent::getAxisY() const
{
	return mAxisY;
}

signed int JoystickEvent::getAxisZ() const
{
	return mAxisZ;
}

signed int JoystickEvent::getAxisRX() const
{
	return mAxisRX;
}

signed int JoystickEvent::getAxisRY() const
{
	return mAxisRY;
}

signed int JoystickEvent::getAxisRZ() const
{
	return mAxisRZ;
}


signed int JoystickEvent::getAxisVX() const
{
	return mAxisVX;
}

signed int JoystickEvent::getAxisVY() const
{
	return mAxisVY;
}

signed int JoystickEvent::getAxisVZ() const
{
	return mAxisVZ;
}

signed int JoystickEvent::getAxisVRX() const
{
	return mAxisVRX;
}

signed int JoystickEvent::getAxisVRY() const
{
	return mAxisVRY;
}

signed int JoystickEvent::getAxisVRZ() const
{
	return mAxisVRZ;
}


signed int JoystickEvent::getAxisAX() const
{
	return mAxisAX;
}

signed int JoystickEvent::getAxisAY() const
{
	return mAxisAY;
}

signed int JoystickEvent::getAxisAZ() const
{
	return mAxisAZ;
}

signed int JoystickEvent::getAxisARX() const
{
	return mAxisARX;
}

signed int JoystickEvent::getAxisARY() const
{
	return mAxisARY;
}

signed int JoystickEvent::getAxisARZ() const
{
	return mAxisARZ;
}


signed int JoystickEvent::getAxisFX() const
{
	return mAxisFX;
}

signed int JoystickEvent::getAxisFY() const
{
	return mAxisFY;
}

signed int JoystickEvent::getAxisFZ() const
{
	return mAxisFZ;
}

signed int JoystickEvent::getAxisFRX() const
{
	return mAxisFRX;
}

signed int JoystickEvent::getAxisFRY() const
{
	return mAxisFRY;
}

signed int JoystickEvent::getAxisFRZ() const
{
	return mAxisFRZ;
}

signed int JoystickEvent::getSliderX() const
{
	return mSliderX;
}

signed int JoystickEvent::getSliderY() const
{
	return mSliderY;
}

signed int JoystickEvent::getVSliderX() const
{
	return mVSliderX;
}

signed int JoystickEvent::getVSliderY() const
{
	return mVSliderY;
}

signed int JoystickEvent::getASliderX() const
{
	return mASliderX;
}

signed int JoystickEvent::getASliderY() const
{
	return mASliderY;
}

signed int JoystickEvent::getFSliderX() const
{
	return mFSliderX;
}

signed int JoystickEvent::getFSliderY() const
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
