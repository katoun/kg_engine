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

#include <render/Viewport.h>
#include <resource/LoadEvent.h>
#include <engine/EngineManager.h>
#include <win32/Win32Window.h>

namespace render
{

Win32Window::Win32Window()
{
	mIsFullScreen = false;
	mHWnd = 0;
	mGlrc = 0;
	mActive = false;
	mDisplayFrequency = 0;
}

Win32Window::~Win32Window()
{
	destroy();
}

void Win32Window::create(unsigned int width, unsigned int height, unsigned int colorDepth, bool fullScreen, unsigned int left, unsigned int top, bool depthBuffer, void* windowId)
{
	// Destroy current window if any
	if (mHWnd)
		destroy();
	
	HINSTANCE hInst = GetModuleHandle(NULL);

	char name[255];
	sprintf_s(name, "K_Game OpenGL window%d", mID);

	mHWnd = 0;
	mIsFullScreen = fullScreen;
	mIsExternal = false;

	// load window defaults
	mWidth = width;
	mHeight = height;
	mDisplayFrequency = 0;
	mIsDepthBuffered = depthBuffer;

	bool vsync = false;

	if (windowId)
	{
		mHWnd = (HWND)windowId;
		mIsExternal = true;
		mIsFullScreen = false;
	}

	if (mIsFullScreen)
	{
		mColorDepth = colorDepth;
		mLeft = 0;
		mTop = 0;
	}
	else
	{
		// Get color depth from display
		mColorDepth = GetDeviceCaps(GetDC(0), BITSPIXEL);
		mTop = top;
		mLeft = left;
	}
	
	if (!mIsExternal)
	{
		// Register the window class
		// NB allow 4 bytes of window data for Win32Window pointer
		WNDCLASS wndClass;

		wndClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				// Redraw On Size, And Own DC For Window.
		wndClass.lpfnWndProc	= (WNDPROC)WndProc;									// WndProc Handles Messages
		wndClass.cbClsExtra		= 0;												// No Extra Window Data
		wndClass.cbWndExtra		= 4;												// No Extra Window Data
		wndClass.hInstance		= hInst;											// Set The Instance
		wndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION/*IDI_WINLOGO*/);	// Load The Default Icon
		wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);						// Load The Arrow Pointer
		wndClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);				// No Background Required For GL
		wndClass.lpszMenuName	= NULL;												// We Don't Want A Menu
		wndClass.lpszClassName	= TEXT(name);										// Set The Class Name

		if (!RegisterClass(&wndClass))
		{
			MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		DWORD dwExStyle = 0;							// Window Extended Style
		DWORD dwStyle = WS_VISIBLE | WS_CLIPCHILDREN;	// Window Style
		int outerw, outerh;

		if (mIsFullScreen)
		{
			dwExStyle |= WS_EX_TOPMOST;					// Window Extended Style
			dwStyle |= WS_POPUP;						// Windows Style
			outerw = mWidth;
			outerh = mHeight;
		}
		else
		{
			dwStyle |= WS_OVERLAPPEDWINDOW;				// Windows Style

			int screenw = GetSystemMetrics(SM_CXSCREEN);
			int screenh = GetSystemMetrics(SM_CYSCREEN);

			// calculate overall dimensions for requested client area
			RECT rc = { 0, 0, mWidth, mHeight };
			AdjustWindowRect(&rc, dwStyle, false);

			// clamp window dimensions to screen size
			outerw = (rc.right-rc.left < screenw)? rc.right-rc.left : screenw;
			outerh = (rc.bottom-rc.top < screenh)? rc.bottom-rc.top : screenh;

			// center window if given negative coordinates
			if (mLeft < 0)
				mLeft = (screenw - outerw) / 2;
			if (mTop < 0)
				mTop = (screenh - outerh) / 2;

			// keep window contained in visible screen area
			if (mLeft > screenw - outerw)
				mLeft = screenw - outerw;
			if (mTop > screenh - outerh)
				mTop = screenh - outerh;
		}

		// Create our main window
		// Pass pointer to self
		mHWnd = CreateWindowEx(	dwExStyle,
									TEXT(name),
									TEXT(name),
									dwStyle,
									mLeft/*left*/,
									mTop/*top*/,
									outerw/*width*/,
									outerh/*height*/,
									0L,
									0L,
									hInst,
									this);

		if (!mHWnd)
		{
			destroy();
			MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		if (mIsFullScreen)
		{
			DEVMODE displayDeviceMode;

			memset(&displayDeviceMode, 0, sizeof(displayDeviceMode));
			displayDeviceMode.dmSize = sizeof(DEVMODE);
			displayDeviceMode.dmBitsPerPel = mColorDepth;
			displayDeviceMode.dmPelsWidth = mWidth;
			displayDeviceMode.dmPelsHeight = mHeight;
			displayDeviceMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if (mDisplayFrequency)
			{
				displayDeviceMode.dmDisplayFrequency = mDisplayFrequency;
				displayDeviceMode.dmFields |= DM_DISPLAYFREQUENCY;

				if(ChangeDisplaySettings(&displayDeviceMode, CDS_FULLSCREEN | CDS_TEST) != DISP_CHANGE_SUCCESSFUL)
				{
					MessageBox(NULL, "ChangeDisplaySettings with user display frequency failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
					displayDeviceMode.dmFields ^= DM_DISPLAYFREQUENCY;
				}	
			}

			if(ChangeDisplaySettings(&displayDeviceMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				MessageBox(NULL, "ChangeDisplaySettings failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			}
		}
	}

	RECT rc;
	// top and left represent outer window position
	GetWindowRect(mHWnd, &rc);
	mTop = rc.top;
	mLeft = rc.left;
	// width and height represent drawable area only
	GetClientRect(mHWnd, &rc);
	mWidth = rc.right;
	mHeight = rc.bottom;

	mHDC = GetDC(mHWnd);

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		mColorDepth,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	int iPixelFormat = ChoosePixelFormat(mHDC, &pfd);

	if(!iPixelFormat)
	{
		destroy();
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return ;
	}

	if(!SetPixelFormat(mHDC, iPixelFormat, &pfd))
	{
		destroy();
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	mGlrc = wglCreateContext(mHDC);
	if(!mGlrc)
	{
		destroy();
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	if(!wglMakeCurrent(mHDC, mGlrc))
	{
		destroy();
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Don't use wglew as if this is the first window, we won't have initialised yet
	/*PFNWGLSWAPINTERVALEXTPROC _wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (_wglSwapIntervalEXT)
		_wglSwapIntervalEXT(vsync? 1 : 0);*/

	mActive = true;
}

void Win32Window::setFullscreen(bool fullScreen, unsigned int width, unsigned int height)
{
	if (mIsFullScreen != fullScreen || mWidth != width || mHeight != height)
	{
		mIsFullScreen = fullScreen;
		DWORD dwStyle = WS_VISIBLE | WS_CLIPCHILDREN;

		if (mIsFullScreen)
		{
			dwStyle |= WS_POPUP;

			DEVMODE dm;
			dm.dmSize = sizeof(DEVMODE);
			dm.dmBitsPerPel = mColorDepth;
			dm.dmPelsWidth = width;
			dm.dmPelsHeight = height;
			dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			if (mDisplayFrequency)
			{
				dm.dmDisplayFrequency = mDisplayFrequency;
				dm.dmFields |= DM_DISPLAYFREQUENCY;
				if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN | CDS_TEST) != DISP_CHANGE_SUCCESSFUL)
				{
					MessageBox(NULL, "ChangeDisplaySettings with user display frequency failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
					dm.dmFields ^= DM_DISPLAYFREQUENCY;
				}
			}
			else
			{
				// try a few
				dm.dmDisplayFrequency = 100;
				dm.dmFields |= DM_DISPLAYFREQUENCY;
				if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN | CDS_TEST) != DISP_CHANGE_SUCCESSFUL)
				{
					dm.dmDisplayFrequency = 75;
					if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN | CDS_TEST) != DISP_CHANGE_SUCCESSFUL)
					{
						dm.dmFields ^= DM_DISPLAYFREQUENCY;
					}
				}

			}
			if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
				MessageBox(NULL, "ChangeDisplaySettings failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

			SetWindowLong(mHWnd, GWL_STYLE, dwStyle);
			SetWindowPos(mHWnd, HWND_TOPMOST, 0, 0, width, height,
				SWP_NOACTIVATE);
			mWidth = width;
			mHeight = height;
		}
		else
		{
			dwStyle |= WS_OVERLAPPEDWINDOW;

			// drop out of fullscreen
			ChangeDisplaySettings(NULL, 0);

			// calculate overall dimensions for requested client area
			RECT rc = { 0, 0, width, height };
			AdjustWindowRect(&rc, dwStyle, false);
			unsigned int winWidth = rc.right - rc.left;
			unsigned int winHeight = rc.bottom - rc.top;

			int screenw = GetSystemMetrics(SM_CXSCREEN);
			int screenh = GetSystemMetrics(SM_CYSCREEN);
			int left = (screenw - winWidth) / 2;
			int top = (screenh - winHeight) / 2;

			SetWindowLong(mHWnd, GWL_STYLE, dwStyle);
			SetWindowPos(mHWnd, HWND_NOTOPMOST, left, top, winWidth, winHeight,
				SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOACTIVATE);
			mWidth = width;
			mHeight = height;

			windowMovedOrResized();
		}
	}
}

bool Win32Window::isVisible() const
{
	return (mHWnd && !IsIconic(mHWnd)) ? true : false;
}

void Win32Window::setActive(bool state)
{
	mActive = state;

	if (mIsFullScreen)
	{
		if (!state)
		{
			//Restore Desktop
			ChangeDisplaySettings(NULL, 0);
			ShowWindow(mHWnd, SW_SHOWMINNOACTIVE);
		}
		else
		{
			//Restore App
			ShowWindow(mHWnd, SW_SHOWNORMAL);

			DEVMODE dm;
			dm.dmSize = sizeof(DEVMODE);
			dm.dmBitsPerPel = mColorDepth;
			dm.dmPelsWidth = mWidth;
			dm.dmPelsHeight = mHeight;
			dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			if (mDisplayFrequency)
			{
				dm.dmDisplayFrequency = mDisplayFrequency;
				dm.dmFields |= DM_DISPLAYFREQUENCY;
			}
			ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
		}
	}
}

void Win32Window::reposition(signed int top, signed int left)
{	
	if (mHWnd && !mIsFullScreen)
	{
		SetWindowPos(mHWnd, 0, left, top, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void Win32Window::resize(unsigned int width, unsigned int height)
{	
	if (mHWnd && !mIsFullScreen)
	{
		RECT rc = { 0, 0, width, height };
		AdjustWindowRect(&rc, GetWindowLong(mHWnd, GWL_STYLE), false);
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
		SetWindowPos(mHWnd, 0, 0, 0, width, height,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void Win32Window::setCaption(const std::string& text)
{
	SetWindowText(mHWnd, text.c_str());
}

void Win32Window::windowMovedOrResized()
{
	if (!mHWnd || IsIconic(mHWnd))
		return;

	RECT rc;
	BOOL result;

	// Update top left parameters
	result = GetWindowRect(mHWnd, &rc);
	if (result == FALSE)
	{
		mTop = 0;
		mLeft = 0;
		mWidth = 0;
		mHeight = 0;
		return;
	}

	mTop = rc.top;
	mLeft = rc.left;

	// width and height represent drawable area only
	result = GetClientRect(mHWnd, &rc);
	if (result == FALSE)
	{
		mTop = 0;
		mLeft = 0;
		mWidth = 0;
		mHeight = 0;
		return;
	}
	unsigned int width = rc.right - rc.left;
	unsigned int height = rc.bottom - rc.top;

	// Case window resized.
	if (width != mWidth || height != mHeight)
	{
		mWidth  = rc.right - rc.left;
		mHeight = rc.bottom - rc.top;

		// Notify viewports of resize
		std::list<Viewport*>::const_iterator i;
		for (i = mViewports.begin(); i != mViewports.end(); ++i)
		{
			Viewport* pViewport = (*i);
			if (pViewport != NULL)
			{
				pViewport->setDimenionsChanged();
			}
		}
	}
}

void Win32Window::destroy()
{
	if (!mHWnd)	return;

	if (mGlrc)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(mGlrc))
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		mGlrc = NULL;
	}

	mHDC = GetDC(mHWnd);
	if (mHDC)
	{
		if (!ReleaseDC(mHWnd, mHDC))				// Are We Able To Release The DC
		{
			MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		mHDC = NULL;
	}

	if (!mIsExternal)
	{
		if (mIsFullScreen)									// Are We In Fullscreen Mode?
		{
			ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
			ShowCursor(true);								// Show Mouse Pointer
		}

		if (mHWnd && !DestroyWindow(mHWnd))					// Are We Able To Destroy The Window?
		{
			MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
			mHWnd = NULL;
		}
	}
	else
	{
		ShowCursor(true);									// Show Mouse Pointer
	}

	mHWnd = 0;
	mHDC = 0;
	mGlrc = 0;
	mActive = false;
}

void Win32Window::updateImpl(float elapsedTime)
{
	RenderWindow::updateImpl(elapsedTime);

	SwapBuffers(mHDC);
}

// Window procedure callback
// This is a static member, so applies to all windows but we store the
// Win32Window instance in the window data GetWindowLog/SetWindowLog
LRESULT Win32Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE)
	{
		// Store pointer to Win32Window in user data area
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams));
		return 0;
	}

	// look up window instance
	Win32Window* win = (Win32Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (win == NULL)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	switch (message)
	{
		case WM_ACTIVATE:
			if(LOWORD(wParam) != WA_INACTIVE)				
				win->setActive(true);
#ifndef _DEBUG			
			else
				win->setActive(false);
#endif
			break;

		case WM_SYSKEYDOWN:
			switch (wParam)
			{
			case VK_CONTROL:
			case VK_SHIFT:
			case VK_MENU: //ALT
				//return zero to bypass defProc and signal we processed the message
				return 0;
			}
			break;
		case WM_SYSKEYUP:
			switch (wParam)
			{
			case VK_CONTROL:
			case VK_SHIFT:
			case VK_MENU: //ALT
			case VK_F10:
				//return zero to bypass defProc and signal we processed the message
				return 0;
			}
			break;
		case WM_SYSCHAR:
			// return zero to bypass defProc and signal we processed the message, unless it's an ALT-space
			if (wParam != VK_SPACE)
				return 0;
			break;

		case WM_MOVE:
			win->windowMovedOrResized();
			break;

		case WM_ENTERSIZEMOVE:
			// Previent rendering while moving / sizing
			win->setActive(false);
			break;

		case WM_EXITSIZEMOVE:
			win->windowMovedOrResized();
			win->setActive(true);
			break;

		case WM_DISPLAYCHANGE:
			win->windowMovedOrResized();
			break;

		case WM_SIZE:
			win->windowMovedOrResized();
			break;

		case WM_GETMINMAXINFO:
			// Prevent the window from going smaller than some minimum size
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
			break;

		case WM_CLOSE:
			engine::EngineManager::getInstance().stop();
			return 0;

		case WM_SYSCOMMAND:
			// prevent screensaver or monitor powersave mode from starting
			if ((wParam == SC_SCREENSAVE) || (wParam == SC_MONITORPOWER) ||	(wParam == SC_KEYMENU))
				return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

} // end namespace render
