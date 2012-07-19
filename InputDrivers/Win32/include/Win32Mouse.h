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

#ifndef _WIN32_MOUSE_H_
#define _WIN32_MOUSE_H_

#include <Win32Config.h>
#include <input/Mouse.h>

#include <map>

namespace input
{

enum MouseButton;

class Win32Mouse : public Mouse
{
public:

	//! Constructor
	Win32Mouse(bool buffered);

	//! Destructor
	~Win32Mouse();

	void sendButtonDown(oi_mouse button);

	void sendButtonUp(oi_mouse button);

	void sendMove(signed int axisX, signed int axisY);

	void setGrab(bool grabbed);

protected:

	void initializeImpl();
	void uninitializeImpl();

	void updateImpl(float elapsedTime);

	std::map<oi_mouse, MouseButton> mButtonMap;
};

} // end namespace input

#endif