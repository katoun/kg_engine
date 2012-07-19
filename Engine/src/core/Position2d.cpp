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

#include <core/Position2d.h>
#include <core/Math.h>

namespace core
{

position2d::position2d() : X(0), Y(0) {}

position2d::position2d(signed int x, signed int y) : X(x), Y(y) {}

position2d::position2d(const position2d& other) : X(other.X), Y(other.Y) {}

position2d& position2d::operator=(const position2d& other)
{
	X = other.X;
	Y = other.Y;
	return *this;
}

position2d position2d::operator+(const position2d& other) const
{
	return position2d(X + other.X, Y + other.Y);
}

position2d& position2d::operator+=(const position2d& other)
{
	X += other.X;
	Y += other.Y;
	return *this;
}

position2d position2d::operator-(const position2d& other) const
{
	return position2d(X -other.X, Y - other.Y);
}

position2d& position2d::operator-=(const position2d& other)
{
	X -= other.X;
	Y -= other.Y;
	return *this;
}

bool position2d::operator == (const position2d& other) const
{
	return (X == other.X && Y == other.Y);
}

bool position2d::operator != (const position2d& other) const
{
	return (X != other.X || Y != other.Y);
}

float position2d::getDistanceFrom(const position2d& other) const
{
	signed int vx = X - other.X;
	signed int vy = Y - other.Y;
	return sqrt((float)(vx*vx + vy*vy));
}

float position2d::getDistanceFromSQ(const position2d& other) const
{
	signed int vx = X - other.X;
	signed int vy = Y - other.Y;
	return (float)(vx*vx + vy*vy);
}

const position2d position2d::ORIGIN_2D = position2d(0, 0);

} // end namespace core
