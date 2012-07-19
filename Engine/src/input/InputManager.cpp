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

#include <input/InputManager.h>
#include <input/Cursor.h>
#include <input/InputDevice.h>
#include <input/InputDeviceDefines.h>
#include <input/InputDriver.h>

template<> input::InputManager& core::Singleton<input::InputManager>::ms_Singleton = input::InputManager();

namespace input
{

InputManager::InputManager(): core::System("InputManager")
{
	mCursor = NULL;

	mCursorCenter.X = 0;
	mCursorCenter.Y = 0;

	mInputDriver = NULL;
}

InputManager::~InputManager() {}

signed int InputManager::numJoySticks()
{
	if (mInputDriver != NULL)
	{
		return mInputDriver->numJoySticks();
	}

	return 0;
}

signed int InputManager::numMice()
{
	if (mInputDriver != NULL)
	{
		return mInputDriver->numMice();
	}

	return 0;
}

signed int InputManager::numKeyboards()
{
	if (mInputDriver != NULL)
	{
		return mInputDriver->numKeyboards();
	}

	return 0;
}

InputDevice* InputManager::createInputDevice(InputType type, bool buffered)
{
	if (mInputDriver != NULL)
	{
		InputDevice* newDevice = mInputDriver->createInputDevice(type, buffered);
		if (newDevice != NULL)
			mInputDevices.push_back(newDevice);

		return newDevice;
	}

	return NULL;
}

void InputManager::removeInputDevice(InputDevice* device)
{
	if (device == NULL)
		return;

	std::list<InputDevice*>::iterator i;
	for (i = mInputDevices.begin(); i != mInputDevices.end(); ++i)
	{
		if (device == (*i))
		{
			mInputDevices.erase(i);

			if (mInputDriver != NULL)
				mInputDriver->removeInputDevice(device);
		}
	}
}

void InputManager::removeAllInputDevices()
{
	std::list<InputDevice*>::iterator i;
	for (i = mInputDevices.begin(); i != mInputDevices.end(); ++i)
	{
		if (mInputDriver != NULL)
			mInputDriver->removeInputDevice(*i);
	}
	mInputDevices.clear();
}

Cursor* InputManager::getCursor()
{
	return mCursor;
}

void InputManager::initializeImpl()
{
	if (mInputDriver != NULL)
	{
		mCursorCenter = mInputDriver->getCursorCenter();
	}
}

void InputManager::uninitializeImpl()
{
	// Remove all InputDevices
	removeAllInputDevices();
}

void InputManager::startImpl() {}

void InputManager::stopImpl() {}

void InputManager::updateImpl(float elapsedTime)
{
	if (mCursor != NULL)
		mCursor->update(elapsedTime);

	std::list<InputDevice*>::iterator i;
	for (i = mInputDevices.begin(); i != mInputDevices.end(); ++i)
	{
		InputDevice* device = (*i);
		if (device != NULL)
			device->update(elapsedTime);
	}
}

void InputManager::setSystemDriverImpl(core::SystemDriver* systemDriver)
{
	mInputDriver = static_cast<InputDriver*>(systemDriver);

	if (mInputDriver != NULL)
		mCursor = mInputDriver->getCursor();
}

void InputManager::removeSystemDriverImpl()
{
	mInputDriver = NULL;
}

InputManager& InputManager::getInstance()
{
	return core::Singleton<InputManager>::getInstance();
}

InputManager* InputManager::getInstancePtr()
{
	return core::Singleton<InputManager>::getInstancePtr();
}

} // end namespace input
