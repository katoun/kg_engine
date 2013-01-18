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

#ifndef _LOAD_EVENT_RECEIVER_H_
#define _LOAD_EVENT_RECEIVER_H_

#include <core/Config.h>

namespace resource
{

struct LoadEvent;

class ENGINE_PUBLIC_EXPORT LoadEventReceiver
{	
public:

	virtual ~LoadEventReceiver();

	//! Called when the loading is about to begin.
	virtual void loadStarted(const LoadEvent& evt);

	//! Called after a resource has been loaded.
	virtual void loadUpdate(const LoadEvent& evt);

	//! Called after the loading has ended.
	virtual void loadEnded(const LoadEvent& evt);
};

} // end namespace resource

#endif