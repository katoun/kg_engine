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

#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include <core/Config.h>
#include <render/Color.h>

namespace render
{

class Camera;
class RenderTarget;

//! An abstraction of a viewport, i.e. a rendering region on a render target.
//!
//! A viewport is the meeting of a camera and a rendering surface - the camera renders the
//! scene from a viewpoint, and places it's results into some subset of a rendering target,
//! which may be the whole surface or just a part of the surface. Each viewport has a single
//! camera as source and a single target as destination. A camera only has 1 viewport, but a
//! render target may have several. A viewport also has a Z-order, i.e. if there is more than
//! one viewport on a single render target and they overlap, one must obscure the other in some
//! predetermined way.<BR>
class ENGINE_PUBLIC_EXPORT Viewport
{
public:

	//! The standard constructor.
	//! \param camera: Pointer to a camera to be the source for the image.
	//! \param target: Pointer to the render target to be the destination for the rendering.
	//! \param left/top/width/height: Dimensions of the viewport, expressed as decimals
	//! between 0 and 1. This allows the dimensions to apply irrespective of changes in the target's
	//! size: e.g. to fill the whole area, values of 0,0,1,1 are appropriate.
	Viewport(Camera* camera, RenderTarget* target, float left, float top, float width, float height);
	~Viewport();

	//! Retrieves a pointer to the render target for this viewport.
	RenderTarget* getTarget();
	//! Retrieves a pointer to the camera for this viewport.
	Camera* getCamera();
	//! Gets one of the relative dimensions of the viewport, a value between 0.0 and 1.0.
	float getLeft();
	//! Gets one of the relative dimensions of the viewport, a value between 0.0 and 1.0.
	float getTop();
	//! Gets one of the relative dimensions of the viewport, a value between 0.0 and 1.0.
	float getWidth();
	//! Gets one of the relative dimensions of the viewport, a value between 0.0 and 1.0.
	float getHeight();
	//! Gets one of the actual dimensions of the viewport, a value in pixels.
	signed int getActualLeft();
	//! Gets one of the actual dimensions of the viewport, a value in pixels.
	signed int getActualTop();
	//! Gets one of the actual dimensions of the viewport, a value in pixels.
	signed int getActualWidth();
	//! Gets one of the actual dimensions of the viewport, a value in pixels.
	signed int getActualHeight();
	
	//!	Sets the dimensions (after creation).

	//!	\param left/top/width/height: Dimensions relative to the size of the
	//!	target, represented as real values between 0 and 1. E.g. the full target
	//!	area is 0,0,1,1.
	void setDimensions(float left, float top, float width, float height);

	void setDimenionsChanged();

	//! Sets the initial background color of the viewport (before rendering).
	void setBackgroundColor(Color& color);
	//! Gets the background color.
	Color getBackgroundColor();

	//! Determines whether to clear the viewport before rendering.
	//!
	//! If you expecting every pixel on the viewport to be redrawn
	//! every frame, you can save a little time by not clearing the
	//! viewport before every frame. Do so by passing 'false' to
	//! this method (the default is to clear every frame).
	void setClearEveryFrame(bool clear);

	//! Determines if the viewport is cleared before every frame.
	bool getClearEveryFrame();

	//! Access to actual dimensions (based on target size).
	void getActualDimensions(signed int& left, signed int& top, signed int& width, signed int& height);

	void setVisible(bool enabled);
	bool isVisible();

	//! Internal method to notify camera of the visible faces in the last render.
	void notifyRenderedFaces(unsigned int numfaces);
	//! Gets the number of rendered faces in the last update.
	unsigned int getNumRenderedFaces() const;

	void setShowOverlays(bool enabled);
	bool getShowOverlays();

	void update(float elapsedTime);

protected:

	Camera* mCamera;
	RenderTarget* mTarget;

	//! Flag indicating derived transform was modified.
	bool mModifiedDimentions;

	// Relative dimensions, irrespective of target dimensions (0..1)
	float mRelLeft, mRelTop, mRelWidth, mRelHeight;
	// Actual dimensions, based on target dimensions
	signed int mActLeft, mActTop, mActWidth, mActHeight;
	
	// Stored number of visible faces in the last render
	unsigned int mRenderedFaces;

	// Background options
	Color mBackColor;
	bool mClearEveryFrame;

	bool mIsVisible;

	bool mShowOverlays;
};

} // end namespace render

#endif