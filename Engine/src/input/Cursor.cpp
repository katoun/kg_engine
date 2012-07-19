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

#include <input/Cursor.h>

namespace input
{

Cursor::Cursor()
{
	mAutoCenter = false;

	mPosition = core::position2d::ORIGIN_2D;
	mAbsolutePosition = core::position2d::ORIGIN_2D;
}

Cursor::~Cursor() {}

void Cursor::setAutoCenter(bool set)
{
	mAutoCenter = set;
}

bool Cursor::isAutoCenter()
{
	return mAutoCenter;
}

const core::position2d& Cursor::getPosition() const
{
	return mPosition;
}

const core::position2d& Cursor::getAbsolutePosition() const
{
	return mAbsolutePosition;
}

void Cursor::update(float elapsedTime)
{
	updateImpl(elapsedTime);
}

void Cursor::updateImpl(float elapsedTime) {}

} // end namespace input
