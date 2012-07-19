/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

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

#ifndef _WIN32_WINDOW_H_
#define _WIN32_WINDOW_H_

#include <GLConfig.h>
#include <render/RenderWindow.h>

namespace render
{

class GL_PUBLIC_EXPORT Win32Window: public RenderWindow
{
public:

	Win32Window();
	~Win32Window();

	void create(unsigned int width, unsigned int height, unsigned int colorDepth, bool fullScreen, unsigned int left, unsigned int top, bool depthBuffer, void* windowId = NULL);
	void setFullscreen(bool fullScreen, unsigned int width, unsigned int height);

	void destroy();
	bool isVisible() const;
	bool isClosed() const;
	void reposition(signed int left, signed int top);
	void resize(unsigned int width, unsigned int height);
	void swapBuffers(bool waitForVSync);
	void setCaption(const std::string& text);

	HWND getWindowHandle() const;
	HDC getHDC() const;

	//! Method for dealing with resize / move & 3d library.
	void windowMovedOrResized();
	
	//! Used to set the active state of the render target.
	void setActive(bool state);

protected:

	HWND	mHWnd;					// Win32 Window handle
	HDC		mHDC;
	HGLRC	mGlrc;
	bool    mIsExternal;
	int     mDisplayFrequency;      // fullscreen only, to restore display

	bool mClosed;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

} // end namespace render

#endif