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
