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

#include <input/InputDevice.h>
#include <core/Utils.h>
#include <core/Utils.h>

namespace input
{

InputDevice::InputDevice()
{
	mInputType = INPUT_TYPE_UNKNOWN;

	mBuffered = false;
	mVendor = core::STRING_BLANK;
}

InputDevice::~InputDevice() {}

InputType InputDevice::getInputType()
{
	return mInputType;
}

bool InputDevice::getBuffered()
{
	return mBuffered;
}

void InputDevice::setBuffered(bool buffered)
{
	mBuffered = buffered;
}

const std::string& InputDevice::getVendor() const
{
	return mVendor;
}

void InputDevice::initialize()
{
	initializeImpl();
}

void InputDevice::uninitialize()
{
	uninitializeImpl();
}

void InputDevice::update(float elapsedTime)
{
	updateImpl(elapsedTime);
}

void InputDevice::initializeImpl() {}

void InputDevice::uninitializeImpl() {}

void InputDevice::updateImpl(float elapsedTime) {}

} // end namespace input
