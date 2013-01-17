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

#ifndef _WIN32_INPUT_DRIVER_H_
#define _WIN32_INPUT_DRIVER_H_

#include <Win32Config.h>
#include <core/Singleton.h>
#include <input/InputDriver.h>

namespace input
{

class Win32Keyboard;
class Win32Mouse;

class Win32InputDriver: public InputDriver, public core::Singleton<Win32InputDriver>
{
public:

	Win32InputDriver();

	~Win32InputDriver();

	signed int numJoySticks();

	signed int numMice();

	signed int numKeyboards();

	InputDevice* createInputDevice(InputType type, bool buffered);

	void removeInputDevice(InputDevice* device);

	bool getGrabMode();
	void setGrabMode(bool grabbed);

	static Win32InputDriver* getInstance();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void updateImpl(float elapsedTime);

	HWND mHWnd;

	Win32Keyboard* mWin32Keyboard;
	Win32Mouse* mWin32Mouse;

	bool mGrabbed;

	// internal class method for finding attached devices.
	void enumerateDevices();
};

} // end namespace platform

#endif