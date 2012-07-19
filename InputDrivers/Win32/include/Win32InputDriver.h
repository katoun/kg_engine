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

#ifndef _WIN32_INPUT_DRIVER_H_
#define _WIN32_INPUT_DRIVER_H_

#include <Win32Config.h>
#include <core/Singleton.h>
#include <input/InputDriver.h>

namespace input
{

class Win32Keyboard;
class Win32Mouse;

class Win32InputDriver : public InputDriver, public core::Singleton<Win32InputDriver>
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

	static Win32InputDriver& getInstance();
	static Win32InputDriver* getInstancePtr();

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