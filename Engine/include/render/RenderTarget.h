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

#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include <core/Config.h>
#include <platform/PlatformManager.h>
#include <render/RenderTargetEventReceiver.h>

#include <Poco/Timestamp.h>

#include <list>

namespace render
{

class Camera;
class Viewport;
struct RenderTargetEvent;
class RenderTargetViewportEvent;

//! A 'canvas' which can receive the results of a rendering
//! operation.
//!
//! This abstract class defines a common root to all targets of rendering operations.
//! A render target could be a window on a screen, or another off screen surface like a texture or bump map etc.
class ENGINE_PUBLIC_EXPORT RenderTarget
{
public:

	struct FrameStats
	{
		float lastFPS;
		float avgFPS;
		float bestFPS;
		float worstFPS;
		signed int triangleCount;
	};

	RenderTarget();
	virtual ~RenderTarget();

	//! Returns the id of the resource.
	const unsigned int& getID() const;

	//! Retrieve information about the render target.
	virtual unsigned int getWidth() const;
	virtual unsigned int getHeight() const;
	virtual unsigned int getColorDepth() const;

	void update(float elapsedTime);

	//! Create a viewport to the rendering target.
	//!
	//!	A viewport is the rectangle into which redering output is sent. This method adds
	//!	a viewport to the render target, rendering from the supplied camera. The
	//!	rest of the parameters are only required if you wish to add more than one viewport
	//!	to a single rendering target. Note that size information passed to this method is
	//!	passed as a parametric, i.e. it is relative rather than absolute. This is to allow
	//!	viewports to automatically resize along with the target.
	//! \param cam: The camera from which the viewport contents will be rendered (mandatory)
	//!	viewports i.e. picture-in-picture). Higher ZOrders are on top of lower ones. The actual number
	//!	is irrelevant, only the relative ZOrder matters (you can leave gaps in the numbering)
	//! \param left: The relative position of the left of the viewport on the target, as a value between 0 and 1.
	//! \param top: The relative position of the top of the viewport on the target, as a value between 0 and 1.
	//! \param width: The relative width of the viewport on the target, as a value between 0 and 1.
	//! \param height: The relative height of the viewport on the target, as a value between 0 and 1.
	virtual Viewport* createViewport(Camera* cam, float left = 0.0f, float top = 0.0f , float width = 1.0f, float height = 1.0f);
	
	//! Removes a viewport from the rendering target.
	virtual void removeViewport(Viewport* viewport);

	//! Removes all viewports on this target.
	virtual void removeAllViewports();

	virtual const FrameStats& getStatistics() const;
	
	//! Individual stats access - gets the number of frames per second (FPS) based on the last frame rendered.
	virtual float getLastFPS() const;
	//! Individual stats access - gets the average frames per second (FPS) since call to RenderSystem::startRendering.
	virtual float getAverageFPS() const;
	//! Individual stats access - gets the best frames per second (FPS) since call to RenderSystem::startRendering.
	virtual float getBestFPS() const;
	//! Individual stats access - gets the worst frames per second (FPS) since call to RenderSystem::startRendering.
	virtual float getWorstFPS() const;
	//! Gets the number of triangles rendered in the last update() call.
	virtual signed int getTriangleCount() const;

	//! Resets saved frame-rate statistices.
	virtual void resetStatistics();

	//! Sets the priority of this render target in relation to the others.
	//!
	//! This can be used in order to schedule render target updates. Lower
	//! priorities will be rendered first. Note that the priority must be set
	//! at the time the render target is attached to the render system, changes
	//! afterwards will not affect the ordering.
	virtual void setPriority(unsigned int priority);
	//! Gets the priority of a render target.
	virtual unsigned int getPriority() const;

	//! Used to retrieve or set the active state of the render target.
	virtual bool isActive() const;

	//! Used to set the active state of the render target.
	virtual void setActive(bool state);

	void addRenderTargetEventReceiver(RenderTargetEventReceiver* newEventReceiver);
	void removeRenderTargetEventReceiver(RenderTargetEventReceiver* oldEventReceiver);

	std::list<Viewport*>& getViewports();
	
protected:

	unsigned int mID;

	// Incremented count for next index
	static unsigned int mIndexCounter;

	virtual void updateImpl(float elapsedTime);

	//! Central list of viewports - for easy memory management and lookup.
	std::list<Viewport*> mViewports;

	// The priority of the render target.
	unsigned int mPriority;

	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mColorDepth;
	bool mIsDepthBuffered;

	// Stats
	FrameStats mStats;

	signed int mFrameCount;
	Poco::Timestamp mLastTimeStamp;
	Poco::Timestamp mCurrentTimeStamp;

	bool mActive;
	bool mAutoUpdate;

	RenderTargetEvent* mRenderTargetEvent;

	std::list<RenderTargetEventReceiver*> mRenderTargetEventReceivers;

	void updateStats();

	//!  internal method for firing events.
	virtual void firePreUpdate();
	//!  internal method for firing events.
	virtual void firePostUpdate();
};

} // end namespace render

#endif