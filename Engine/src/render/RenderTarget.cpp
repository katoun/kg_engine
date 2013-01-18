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

#include <render/RenderTarget.h>
#include <render/Camera.h>
#include <render/Viewport.h>
#include <render/RenderTargetEvent.h>
#include <core/Math.h>
#include <core/Utils.h>

namespace render
{

unsigned int RenderTarget::mIndexCounter = 0;

RenderTarget::RenderTarget()
{
	mID = mIndexCounter++;

	mRenderTargetEvent = new RenderTargetEvent();

	mFrameCount = 0;
	mLastElapsedTime = 0;

	resetStatistics();
}

RenderTarget::~RenderTarget()
{
	// Delete all Viewports
	removeAllViewports();

	delete mRenderTargetEvent;
}

const unsigned int& RenderTarget::getID() const
{
	return mID;
}

unsigned int RenderTarget::getWidth() const
{
	return mWidth;
}

unsigned int RenderTarget::getHeight() const
{
	return mHeight;
}

unsigned int RenderTarget::getColorDepth() const
{
	return mColorDepth;
}

Viewport* RenderTarget::createViewport(Camera* cam, float top, float left, float width , float height)
{
	Viewport* pViewport = new Viewport(cam, this);
	if (pViewport == nullptr)
		return nullptr;

	pViewport->setTop(top);
	pViewport->setLeft(left);
	pViewport->setWidth(width);
	pViewport->setHeight(height);
	
	mViewports.push_back(pViewport);

	return pViewport;
}

void RenderTarget::removeViewport(Viewport* viewport)
{
	if (viewport == nullptr)
		return;

	std::list<Viewport*>::iterator i;
	for(i = mViewports.begin(); i != mViewports.end(); ++i)
	{
		if(viewport == (*i))
		{
			mViewports.erase(i);
			delete viewport;
		}
	}
}

void RenderTarget::removeAllViewports()
{
	std::list<Viewport*>::iterator i;
	for (i = mViewports.begin(); i != mViewports.end(); ++i)
	{
		Viewport* pViewport = (*i);
		if (pViewport != nullptr)
		{
			delete pViewport;
		}
	}

	mViewports.clear();
}

const RenderTarget::FrameStats& RenderTarget::getStatistics() const
{
	return mStats;
}

float RenderTarget::getLastFPS() const
{
	return mStats.lastFPS;
}
float RenderTarget::getAverageFPS() const
{
	return mStats.avgFPS;
}
float RenderTarget::getBestFPS() const
{
	return mStats.bestFPS;
}
float RenderTarget::getWorstFPS() const
{
	return mStats.worstFPS;
}

int RenderTarget::getTriangleCount() const
{
	return mStats.triangleCount;
}

void RenderTarget::resetStatistics()
{
	mStats.avgFPS = 0.0;
	mStats.bestFPS = 0.0;
	mStats.lastFPS = 0.0;
	mStats.worstFPS = 999.0;
	mStats.triangleCount = 0;

	mFrameCount = 0;
	mLastElapsedTime = 0;
}

bool RenderTarget::isActive() const
{
	return mActive;
}

void RenderTarget::setActive(bool state)
{
	mActive = state;
}

void RenderTarget::update(float elapsedTime)
{
	updateImpl(elapsedTime);
}

void RenderTarget::addRenderTargetEventReceiver(RenderTargetEventReceiver* newEventReceiver)
{
	mRenderTargetEventReceivers.push_back(newEventReceiver);
}

void RenderTarget::removeRenderTargetEventReceiver(RenderTargetEventReceiver* oldEventReceiver)
{
	std::list<RenderTargetEventReceiver*>::iterator i;
	for (i = mRenderTargetEventReceivers.begin(); i != mRenderTargetEventReceivers.end(); ++i)
	{
		if ((*i) == oldEventReceiver)
		{
			mRenderTargetEventReceivers.erase(i);
			return;
		}
	}
}

std::list<Viewport*>& RenderTarget::getViewports()
{
	return mViewports;
}

void RenderTarget::updateImpl(float elapsedTime)
{
	// notify listeners (pre)
	firePreUpdate();

	mStats.triangleCount = 0;
	// Go through viewports in Z-order
	// Tell each to refresh
	std::list<Viewport*>::const_iterator i;
	for (i = mViewports.begin(); i != mViewports.end(); ++i)
	{
		Viewport* pViewport = (*i);
		if (pViewport)
		{
			pViewport->update(elapsedTime);

			if (pViewport->isVisible())
				mStats.triangleCount += pViewport->getNumRenderedFaces();
		}
	}

	// notify listeners (post)
	firePostUpdate();

	// Update statistics
	++mFrameCount;
	mLastElapsedTime += elapsedTime;
	if (mLastElapsedTime != 0.0f)
	{
		if (mLastElapsedTime > 1000.0f)
		{
			mStats.lastFPS = (float)(mFrameCount * 1000.0f) / (float)(mLastElapsedTime);

			if (mStats.avgFPS == 0)
				mStats.avgFPS = mStats.lastFPS;
			else
				mStats.avgFPS = (mStats.avgFPS + mStats.lastFPS) / 2; // not strictly correct, but good enough

			mStats.bestFPS = core::max(mStats.bestFPS, mStats.lastFPS);
			mStats.worstFPS = core::min(mStats.worstFPS, mStats.lastFPS);

			mFrameCount = 0;
			mLastElapsedTime = 0;
		}
	}
}

void RenderTarget::firePreUpdate()
{
	// Do pre update event
	mRenderTargetEvent->source = this;

	// Tell all listeners
	std::list<RenderTargetEventReceiver*>::const_iterator i;
	for (i = mRenderTargetEventReceivers.begin(); i != mRenderTargetEventReceivers.end(); ++i)
	{
		(*i)->preRenderTargetUpdate(*mRenderTargetEvent);
	}
}

void RenderTarget::firePostUpdate()
{
	// Do post update event
	mRenderTargetEvent->source = this;

	// Tell all listeners
	std::list<RenderTargetEventReceiver*>::const_iterator i;
	for (i = mRenderTargetEventReceivers.begin(); i != mRenderTargetEventReceivers.end(); ++i)
	{
		(*i)->postRenderTargetUpdate(*mRenderTargetEvent);
	}
}

} // end namespace render
