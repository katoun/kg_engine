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
	mLastTimeStamp.update();
	mCurrentTimeStamp.update();

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

void RenderTarget::update(float elapsedTime)
{
	updateImpl(elapsedTime);
}

Viewport* RenderTarget::createViewport(Camera* cam, float left, float top , float width , float height)
{
	Viewport* newViewport = new Viewport(cam, this, left, top, width, height);
	mViewports.push_back(newViewport);

	return newViewport;
}

void RenderTarget::removeViewport(Viewport* viewport)
{
	if (viewport == NULL)
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
		if (pViewport != NULL)
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

signed int RenderTarget::getTriangleCount() const
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
	mLastTimeStamp.update();
}

void RenderTarget::setPriority(unsigned int priority)
{
	mPriority = priority;
}

unsigned int RenderTarget::getPriority() const
{
	return mPriority;
}

bool RenderTarget::isActive() const
{
	return mActive;
}

void RenderTarget::setActive(bool state)
{
	mActive = state;
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
	updateStats();
}

void RenderTarget::updateStats()
{
	++mFrameCount;

	mCurrentTimeStamp.update();

	if (mCurrentTimeStamp != NULL && mLastTimeStamp != NULL)
	{
		Poco::Timestamp::TimeDiff diff = mCurrentTimeStamp - mLastTimeStamp;

		// Update the frame rate once per second
		if (diff > Poco::Timestamp::resolution())
		{
			mStats.lastFPS = (float)(mFrameCount * Poco::Timestamp::resolution()) / (float)(diff);

			if (mStats.avgFPS == 0)
				mStats.avgFPS = mStats.lastFPS;
			else
				mStats.avgFPS = (mStats.avgFPS + mStats.lastFPS) / 2; // not strictly correct, but good enough

			mStats.bestFPS = core::max(mStats.bestFPS, mStats.lastFPS);
			mStats.worstFPS = core::min(mStats.worstFPS, mStats.lastFPS);

			mLastTimeStamp.swap(mCurrentTimeStamp);
			mFrameCount  = 0;
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
