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

#ifndef _RENDER_TARGET_EVENT_RECEIVER_H_
#define _RENDER_TARGET_EVENT_RECEIVER_H_

#include <core/Config.h>

namespace render
{

struct RenderTargetEvent;

class ENGINE_PUBLIC_EXPORT RenderTargetEventReceiver
{
public:

	virtual ~RenderTargetEventReceiver();

	//! Called just before a RenderTarget is about to be rendered into.
	//! This event is raised just before any of the viewports on the target
	//! are rendered to. You can perform manual rendering operations here if
	//! you want.
	virtual void preRenderTargetUpdate(const RenderTargetEvent& evt);

	//! Called just after a RenderTarget has been rendered to.
	//! This event is called just after all the viewports attached to the target
	//! in question have been rendered to. You can perform your own manual rendering
	//! commands in this event handler if you like.
	virtual void postRenderTargetUpdate(const RenderTargetEvent& evt);
};

} // end namespace render

#endif