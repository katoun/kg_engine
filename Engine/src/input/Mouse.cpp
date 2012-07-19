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

#include <input/Mouse.h>
#include <input/MouseEvent.h>

namespace input
{

Mouse::Mouse()
{
	mInputType = INPUT_TYPE_MOUSE;
	
	mEvent = new MouseEvent();
}

Mouse::~Mouse()
{
	delete mEvent;
}

void Mouse::addMouseEventReceiver(input::MouseEventReceiver* newEventReceiver)
{
	mMouseEventReceivers.push_back(newEventReceiver);
}

void Mouse::removeMouseEventReceiver(input::MouseEventReceiver* oldEventReceiver)
{
	std::list<input::MouseEventReceiver*>::iterator i;
	for (i = mMouseEventReceivers.begin(); i != mMouseEventReceivers.end(); ++i)
	{
		if ((*i) == oldEventReceiver)
		{
			mMouseEventReceivers.erase(i);
			return;
		}
	}
}

} // end namespace input
