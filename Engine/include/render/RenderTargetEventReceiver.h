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

#ifndef _RENDER_TARGET_EVENT_RECEIVER_H_
#define _RENDER_TARGET_EVENT_RECEIVER_H_

#include <Config.h>

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