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
#include <render/RenderManager.h>
#include <core/Utils.h>

namespace render
{

Viewport::Viewport(Camera* camera, RenderTarget* target)
{
	mCamera = camera;
	mTarget = target;

	mRelativeTop = 0.0f;
	mRelativeLeft = 0.0f;
	mRelativeWidth = 1.0f;
	mRelativeHeight = 1.0f;

	mBackColor = render::Color::Black;
	mClearEveryFrame = true;

	mIsVisible = true;

	mShowOverlays = true;

	mDimentionsNeedsUpdate = true;
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

float Viewport::getTop()
{
	return mRelativeTop;
}

float Viewport::getLeft()
{
	return mRelativeLeft;
}

float Viewport::getWidth()
{
	return mRelativeWidth;
}

float Viewport::getHeight()
{
	return mRelativeHeight;
}

void Viewport::setTop(float top)
{
	mRelativeTop = top;
	mDimentionsNeedsUpdate = true;
}

void Viewport::setLeft(float left)
{
	mRelativeLeft = left;
	mDimentionsNeedsUpdate = true;
}

void Viewport::setWidth(float width)
{
	mRelativeWidth = width;
	mDimentionsNeedsUpdate = true;
}

void Viewport::setHeight(float height)
{
	mRelativeHeight = height;
	mDimentionsNeedsUpdate = true;
}

signed int Viewport::getActualLeft()
{
	return mActualLeft;
}

signed int Viewport::getActualTop()
{
	return mActualTop;
}

signed int Viewport::getActualWidth()
{
	return mActualWidth;
}

signed int Viewport::getActualHeight()
{
	return mActualHeight;
}

void Viewport::setDimenionsChanged()
{
	mDimentionsNeedsUpdate = true;
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

void Viewport::update(float elapsedTime)
{
	if (mDimentionsNeedsUpdate)
	{
		if (mTarget != nullptr)
		{
			float width = (float)mTarget->getWidth();
			float height = (float)mTarget->getHeight();

			mActualLeft = (signed int)(mRelativeLeft * width);
			mActualTop = (signed int)(mRelativeTop * height);
			mActualWidth = (signed int)(mRelativeWidth * width);
			mActualHeight = (signed int)(mRelativeHeight * height);
		}

		// This allows cameras to be used to render to many viewports,
		// which can have their own dimensions and aspect ratios.
		if (mCamera != nullptr) 
		{
			mCamera->setAspectRatio((float)mActualWidth / (float)mActualHeight);
		}

		//Add this viewport to the render manager to be counted as a viewport state change
		if (RenderManager::getInstance() != nullptr)
			RenderManager::getInstance()->addUpdatedViewport(this);

		mDimentionsNeedsUpdate = false;
	}
}

} // end namespace render
