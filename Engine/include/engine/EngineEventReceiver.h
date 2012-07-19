/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

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

#ifndef _ENGINE_EVENT_RECEIVER_H_
#define _ENGINE_EVENT_RECEIVER_H_

#include <core/Config.h>

namespace engine
{

struct EngineEvent;

class ENGINE_PUBLIC_EXPORT EngineEventReceiver
{
public:

	virtual ~EngineEventReceiver();

	//! Called just after the engine has been initialized.
	virtual void engineInitialized();
	//! Called just after the engine has been uninitialized.
	virtual void engineUninitialized();

	//! Called just after the engine has started.
	virtual void engineStarted();
	//! Called just before the engine has stopped.
	virtual void engineStopped();
	
	//! Called when the engine is about to begin updating.
	virtual void engineUpdateStarted(const EngineEvent& evt);
	//! Called just after the engine has been updated.
	virtual void engineUpdateEnded(const EngineEvent& evt);
};

} // end namespace engine

#endif