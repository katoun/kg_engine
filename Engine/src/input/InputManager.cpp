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

#include <input/InputManager.h>
#include <input/Cursor.h>
#include <input/InputDevice.h>
#include <input/InputDeviceDefines.h>
#include <input/InputDriver.h>

template<> input::InputManager* core::Singleton<input::InputManager>::m_Singleton = nullptr;

namespace input
{

InputManager::InputManager(): core::System("InputManager")
{
	mCursor = nullptr;

	mCursorCenter = glm::vec2(0, 0);

	mInputDriver = nullptr;
}

InputManager::~InputManager() {}

int InputManager::numJoySticks()
{
	if (mInputDriver != nullptr)
	{
		return mInputDriver->numJoySticks();
	}

	return 0;
}

int InputManager::numMice()
{
	if (mInputDriver != nullptr)
	{
		return mInputDriver->numMice();
	}

	return 0;
}

int InputManager::numKeyboards()
{
	if (mInputDriver != nullptr)
	{
		return mInputDriver->numKeyboards();
	}

	return 0;
}

InputDevice* InputManager::createInputDevice(InputType type, bool buffered)
{
	if (mInputDriver != nullptr)
	{
		InputDevice* newDevice = mInputDriver->createInputDevice(type, buffered);
		if (newDevice != nullptr)
			mInputDevices.push_back(newDevice);

		return newDevice;
	}

	return nullptr;
}

void InputManager::removeInputDevice(InputDevice* device)
{
	if (device == nullptr)
		return;

	std::list<InputDevice*>::iterator i;
	for (i = mInputDevices.begin(); i != mInputDevices.end(); ++i)
	{
		if (device == (*i))
		{
			mInputDevices.erase(i);

			if (mInputDriver != nullptr)
				mInputDriver->removeInputDevice(device);
		}
	}
}

void InputManager::removeAllInputDevices()
{
	std::list<InputDevice*>::iterator i;
	for (i = mInputDevices.begin(); i != mInputDevices.end(); ++i)
	{
		if (mInputDriver != nullptr)
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
	if (mInputDriver != nullptr)
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
	if (mCursor != nullptr)
		mCursor->update(elapsedTime);

	std::list<InputDevice*>::iterator i;
	for (i = mInputDevices.begin(); i != mInputDevices.end(); ++i)
	{
		InputDevice* device = (*i);
		if (device != nullptr)
			device->update(elapsedTime);
	}
}

void InputManager::setSystemDriverImpl(core::SystemDriver* systemDriver)
{
	mInputDriver = static_cast<InputDriver*>(systemDriver);

	if (mInputDriver != nullptr)
		mCursor = mInputDriver->getCursor();
}

void InputManager::removeSystemDriverImpl()
{
	mInputDriver = nullptr;
}

InputManager* InputManager::getInstance()
{
	return core::Singleton<InputManager>::getInstance();
}
} // end namespace input
