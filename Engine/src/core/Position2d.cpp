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

#include <core/Position2d.h>
#include <core/Math.h>

namespace core
{

position2d::position2d() : x(0), y(0) {}

position2d::position2d(int nx, int ny) : x(nx), y(ny) {}

position2d::position2d(const position2d& other) : x(other.x), y(other.y) {}

position2d& position2d::operator=(const position2d& other)
{
	x = other.x;
	y = other.y;
	return *this;
}

position2d position2d::operator+(const position2d& other) const
{
	return position2d(x + other.x, y + other.y);
}

position2d& position2d::operator+=(const position2d& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

position2d position2d::operator-(const position2d& other) const
{
	return position2d(x -other.x, y - other.y);
}

position2d& position2d::operator-=(const position2d& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

bool position2d::operator == (const position2d& other) const
{
	return (x == other.x && y == other.y);
}

bool position2d::operator != (const position2d& other) const
{
	return (x != other.x || y != other.y);
}

float position2d::getDistanceFrom(const position2d& other) const
{
	int vx = x - other.x;
	int vy = y - other.y;
	return sqrt((float)(vx*vx + vy*vy));
}

float position2d::getDistanceFromSQ(const position2d& other) const
{
	int vx = x - other.x;
	int vy = y - other.y;
	return (float)(vx*vx + vy*vy);
}

const position2d position2d::ORIGIN_2D = position2d(0, 0);

} // end namespace core
