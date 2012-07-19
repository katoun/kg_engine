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