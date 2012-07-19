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
#include <render/Camera.h>
#include <render/RenderTarget.h>
#include <core/Utils.h>

namespace render
{

Viewport::Viewport(Camera* camera, RenderTarget* target, float left, float top, float width, float height)
{
	mCamera = camera;
	mTarget = target;

	mRelLeft = left;
	mRelTop = top;
	mRelWidth = width;
	mRelHeight = height;

	mBackColor = render::Color::Black;
	mClearEveryFrame = true;

	mIsVisible = true;

	mShowOverlays = true;

	mModifiedDimentions = true;
}

Viewport::~Viewport() {}

void Viewport::setVisible(bool enabled)
{
	mIsVisible = enabled;
}
bool Viewport::isVisible()
{
	return mIsVisible;
}

void Viewport::notifyRenderedFaces(unsigned int numfaces)
{
	mRenderedFaces = numfaces;
}

unsigned int Viewport::getNumRenderedFaces() const
{
	return mRenderedFaces;
}

void Viewport::setShowOverlays(bool enabled)
{
	mShowOverlays = enabled;
}
bool Viewport::getShowOverlays()
{
	return mShowOverlays;
}

RenderTarget* Viewport::getTarget()
{
	return mTarget;
}

Camera* Viewport::getCamera()
{
	return mCamera;
}

float Viewport::getLeft()
{
	return mRelLeft;
}

float Viewport::getTop()
{
	return mRelTop;
}

float Viewport::getWidth()
{
	return mRelWidth;
}

float Viewport::getHeight()
{
	return mRelHeight;
}

signed int Viewport::getActualLeft()
{
	return mActLeft;
}

signed int Viewport::getActualTop()
{
	return mActTop;
}

signed int Viewport::getActualWidth()
{
	return mActWidth;
}

signed int Viewport::getActualHeight()
{
	return mActHeight;
}

void Viewport::setDimensions(float left, float top, float width, float height)
{
	mRelLeft = left;
	mRelTop = top;
	mRelWidth = width;
	mRelHeight = height;

	mModifiedDimentions = true;

	//updateDimensions();
}
void Viewport::setDimenionsChanged()
{
}

void Viewport::setBackgroundColor(Color& color)
{
	mBackColor = color;
}

Color Viewport::getBackgroundColor()
{
	return mBackColor;
}

void Viewport::setClearEveryFrame(bool clear)
{
	mClearEveryFrame = clear;
}

bool Viewport::getClearEveryFrame()
{
	return mClearEveryFrame;
}

void Viewport::getActualDimensions(signed int& left, signed int& top, signed int& width, signed int& height)
{
	left = mActLeft;
	top = mActTop;
	width = mActWidth;
	height = mActHeight;
}

void Viewport::update(float elapsedTime)
{
	if (mModifiedDimentions)
	{
		if (mTarget != NULL)
		{
			signed int height = mTarget->getHeight();
			signed int width = mTarget->getWidth();

			mActLeft = (signed int)(mRelLeft * width);
			mActTop = (signed int)(mRelTop * height);
			mActWidth = (signed int)(mRelWidth * width);
			mActHeight = (signed int)(mRelHeight * height);
		}

		// This allows cameras to be used to render to many viewports,
		// which can have their own dimensions and aspect ratios.

		if (mCamera != NULL) 
		{
			mCamera->setAspectRatio((float)mActWidth / (float)mActHeight);
		}

		mModifiedDimentions = false;
	}
}

} // end namespace render
