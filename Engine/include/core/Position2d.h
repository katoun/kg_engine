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

#ifndef _POSITION_2D_H_
#define _POSITION_2D_H_

#include <core/Config.h>

namespace core
{

//! Simple class for holding 2d coordinates.
class ENGINE_PUBLIC_EXPORT position2d
{
public:

	position2d();
	position2d(signed int x, signed int y);
	position2d(const position2d& other);

	inline position2d& operator=(const position2d& other);

	inline position2d operator-(const position2d& other) const;
	inline position2d& operator+=(const position2d& other);

	inline position2d operator+(const position2d& other) const;
	inline position2d& operator-=(const position2d& other);

	inline bool operator == (const position2d& other) const;
	inline bool operator != (const position2d& other) const;

	//! Returns distance from an other point.
	float getDistanceFrom(const position2d& other) const;

	//! Returns distance from an other point.
	float getDistanceFromSQ(const position2d& other) const;

	//! ORIGIN_2D is a const position2d that defines the 2d origin position for the 2d space
	static const position2d ORIGIN_2D;

	signed int X, Y;
};

} // end namespace core

#endif