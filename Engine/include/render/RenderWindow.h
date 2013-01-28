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

#ifndef _RENDER_WINDOW_H_
#define _RENDER_WINDOW_H_

#include <Config.h>
#include <render/RenderTarget.h>

#include <string>

namespace render
{

//!  Manages the target rendering window.
//!
//! This class handles a window into which the contents
//! of a scene are rendered. There is a many-to-1 relationship
//! between instances of this class an instance of RenderSystem
//! which controls the rendering of the scene. There may be
//! more than one window in the case of level editor tools etc.
//! This class is abstract since there may be
//! different implementations for different windowing systems.
//!
//! Instances are created and communicated with by the render system
//! although client programs can get a reference to it from
//! the render system if required for resizing or moving.
//! Note that you can have multiple viewpoints
//! in the window for effects like rear-view mirrors and
//! picture-in-picture views (see Viewport and Camera).
class ENGINE_PUBLIC_EXPORT RenderWindow: public RenderTarget
{
public:

	RenderWindow();
	
	//! Creates & displays the new window.
	//! \param width: The width of the window in pixels.
	//! \param height: The height of the window in pixels.
	//! \param colorDepth: The color depth in bits. Ignored if fullScreen is false since the desktop depth is used.
	//! \param fullScreen: If true, the window fills the screen, with no title bar or border.
	//! \param left: The x-position of the window. Ignored if fullScreen = true.
	//! \param top: The y-position of the window. Ignored if fullScreen = true.
	//! \param depthBuffer: Specify true to include a depth-buffer.
	//! \param windowId: Specify the windowID of the external window if needed.
	virtual void create(unsigned int width, unsigned int height, unsigned int colorDepth, bool fullScreen, unsigned int left, unsigned int top, bool depthBuffer, void* windowId = nullptr) = 0;

	//! Retrieve information about the render window.
	virtual unsigned int getTop() const;
	virtual unsigned int getLeft() const;
	
	//! Returns true if window is running in fullscreen mode.
	virtual bool isFullScreen();
	
	//! Alter fullscreen mode options. 
	//! \param fullScreen: Whether to use fullscreen mode or not. 
	//! \param width: The new width to use
	//! \param height: The new height to use
	virtual void setFullscreen(bool fullScreen, unsigned int width, unsigned int height);

	//! Reposition the window.
	virtual void reposition(int top, int left) = 0;

	//! Alter the size of the window.
	virtual void resize(unsigned int width, unsigned int height) = 0;

	virtual bool isActive() const;
	
	//! Determine if the window is visible (not minimized or obscured).
	//!
	//! This method returns true if the window is active
	//! i.e. is visible somewhere on screen, therefore should
	//! receive more images.
	//! The render system will use this to determine if it
	//! should bother rendering. If the window is minimised,
	//! the render will simply update the world and not
	//! bother rendering anything, to save CPU time.
	virtual bool isVisible() const;

	//! Set the visibility state.
	virtual void setVisible(bool visible);

	virtual void setCaption(const std::string& text) = 0;

protected:

	virtual void updateImpl(float elapsedTime);

	bool mIsFullScreen;

	int mTop;
	int mLeft;
};

} // end namespace render

#endif