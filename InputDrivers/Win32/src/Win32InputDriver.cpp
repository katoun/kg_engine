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

#include <core/Position2d.h>
#include <engine/EngineSettings.h>
#include <Win32InputDriver.h>
#include <Win32Cursor.h>
#include <Win32Mouse.h>
#include <Win32Keyboard.h>
#include <Win32Joystick.h>

template<> input::Win32InputDriver& core::Singleton<input::Win32InputDriver>::ms_Singleton = input::Win32InputDriver();

namespace input
{

Win32InputDriver::Win32InputDriver(): InputDriver("Win32 InputDriver")
{
	mCursor = new Win32Cursor();

	mWin32Keyboard = NULL;
	mWin32Mouse = NULL;

	mGrabbed = false;
}

Win32InputDriver::~Win32InputDriver() {}

signed int Win32InputDriver::numJoySticks()
{
	return 0;
}

signed int Win32InputDriver::numMice()
{
	return 1;
}

signed int Win32InputDriver::numKeyboards()
{
	return 1;
}

InputDevice* Win32InputDriver::createInputDevice(InputType type, bool buffered)
{
	InputDevice* device = NULL;
	switch (type)
	{
		case INPUT_TYPE_KEYBOARD:
			mWin32Keyboard = new Win32Keyboard(buffered);
			device = mWin32Keyboard;
			break;
		case INPUT_TYPE_MOUSE:
			mWin32Mouse = new Win32Mouse(buffered);
			device = mWin32Mouse;
			break;
		case INPUT_TYPE_JOYSTICK:
			device = NULL;
			break;
		default:
			device = NULL;
	}

	if (device != NULL)
	{
		try
		{
			device->initialize();
		}
		catch(...)
		{
			delete device;
			throw;
		}
	}

	return device;
}

void Win32InputDriver::removeInputDevice(InputDevice* device)
{
	assert(device);
	if (device == NULL)
		return;

	delete device;
}

bool Win32InputDriver::getGrabMode()
{
	return mGrabbed;
}

void Win32InputDriver::setGrabMode(bool grabbed)
{
	mGrabbed = grabbed;
}

void Win32InputDriver::initializeImpl()
{
	engine::EngineSettings& engineOptions = engine::EngineSettings::getInstance();

	// Get HWND
	if (engineOptions.getMainWindowId())
		mHWnd = (HWND)engineOptions.getMainWindowId();
	else
		mHWnd = GetActiveWindow();

	if (IsWindow(mHWnd) == 0)
	{
		MessageBox(NULL, "Win32InputSystem: The HWND is not valid!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Get HINST
	HINSTANCE hInst = GetModuleHandle(0);

	RECT rect;
	GetClientRect(mHWnd, &rect);
	mCursorCenter.X = (signed int)((rect.right - rect.left) / 2);
	mCursorCenter.Y = (signed int)((rect.bottom - rect.top) / 2);

	if (mCursor != NULL)
		mCursor->setPosition(mCursorCenter);

#ifdef _DEBUG
	//std::cout <<"mCursorCenterX : "<<mCursorCenter.X<< std::endl;
	//std::cout <<"mCursorCenterY : "<<mCursorCenter.Y<< std::endl;
#endif

	//TODO init input system here
	char csw[100];
	sprintf(csw, "c:%u s:%u w:%u", 0, 0, (unsigned int)mHWnd);
	int r = oi_init(csw, 0);

	enumerateDevices();
}

void Win32InputDriver::uninitializeImpl()
{
	if (mCursor != NULL)
		delete mCursor;

	int r = oi_close();
}

void Win32InputDriver::updateImpl(float elapsedTime)
{
	if(mCursor != NULL && mCursor->isAutoCenter())
	{
		//Only update cursor position if the window has the focus
		if(mHWnd == GetForegroundWindow() && IsWindowVisible(mHWnd))
			mCursor->setPosition(mCursorCenter);
	}

	oi_event ev;
	oi_events_poll(&ev);

	switch(ev.type)
	{
		case OI_KEYDOWN:
		{
			if (mWin32Keyboard != NULL)
				mWin32Keyboard->sendKeyDown(ev.key.keysym.sym);
		}
		break;
		case OI_KEYUP:
		{
			if (mWin32Keyboard != NULL)
				mWin32Keyboard->sendKeyUp(ev.key.keysym.sym);
		}
		break;
		case OI_MOUSEBUTTONDOWN:
		{
			if (mWin32Mouse != NULL)
				mWin32Mouse->sendButtonDown((oi_mouse)ev.button.button);
		}
		break;
		case OI_MOUSEBUTTONUP:
		{
			if (mWin32Mouse != NULL)
				mWin32Mouse->sendButtonUp((oi_mouse)ev.button.button);
		}
		break;
		case OI_MOUSEMOVE:
		{
			if (mWin32Mouse != NULL)
				mWin32Mouse->sendMove(ev.move.relx, ev.move.rely);
		}
		break;
	}
}

void Win32InputDriver::enumerateDevices() {}

Win32InputDriver& Win32InputDriver::getInstance()
{
	return core::Singleton<Win32InputDriver>::getInstance();
}

Win32InputDriver* Win32InputDriver::getInstancePtr()
{
	return core::Singleton<Win32InputDriver>::getInstancePtr();
}

} // end namespace platform
