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

#include <Win32Cursor.h>

namespace input
{

Win32Cursor::Win32Cursor()
{
}

Win32Cursor::~Win32Cursor()
{
}

void Win32Cursor::setVisible(bool set)
{
	oi_app_cursor(set && !mAutoCenter ? OI_ENABLE : OI_DISABLE);
}

void Win32Cursor::setAutoCenter(bool set)
{
	Cursor::setAutoCenter(set);
	oi_app_cursor(set ? OI_DISABLE : OI_ENABLE);
}

void Win32Cursor::setPosition(const glm::vec2& pos)
{
	setPosition((int)pos.x, (int)pos.y);
}

void Win32Cursor::setPosition(int x, int y)
{
	oi_mouse_warp(0, x, y);
}

void Win32Cursor::updateImpl(float elapsedTime)
{
	oi_mouse_relative(0, (int*)&mPosition.x, (int*)&mPosition.y);

	oi_mouse_absolute(0, (int*)&mAbsolutePosition.x, (int*)&mAbsolutePosition.y);
}

} // end namespace input
