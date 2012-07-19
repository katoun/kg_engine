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

#include <core/Line2d.h>

namespace core
{

line2d::line2d() : start(0, 0), end(1, 1) {}

line2d::line2d(float xa, float ya, float xb, float yb) : start(xa, ya), end(xb, yb) {}

line2d::line2d(const vector2d& nstart, const vector2d& nend) : start(nstart), end(nend) {}

line2d::line2d(const line2d& other) : start(other.start), end(other.end) {}

line2d line2d::operator+(const vector2d& point) const
{
	return line2d(start + point, end + point);
}

line2d& line2d::operator+=(const vector2d& point)
{
	start += point;
	end += point;
	return *this;
}

line2d line2d::operator-(const vector2d& point) const
{
	return line2d(start - point, end - point);
}

line2d& line2d::operator-=(const vector2d& point)
{
	start -= point;
	end -= point;
	return *this;
}

bool line2d::operator==(const line2d& other) const
{
	return ((start == other.start && end == other.end) || (end == other.start && start == other.end));
}

bool line2d::operator!=(const line2d& other) const
{
	return ((start != other.start || end != other.end) && (end != other.start || start != other.end));
}

void line2d::set(const float& xa, const float& ya, const float& xb, const float& yb)
{
	start.set(xa, ya);
	end.set(xb, yb);
}

void line2d::set(const vector2d& nstart, const vector2d& nend)
{
	start.set(nstart);
	end.set(nend);
}

void line2d::set(const line2d& other)
{
	start.set(other.start);
	end.set(other.end);
}

float line2d::getLength() const
{
	return start.getDistanceFrom(end);
}

vector2d line2d::getVector() const
{
	return vector2d(start.X - end.X, start.Y - end.Y);
}

vector2d line2d::getUnitVector()
{
	float len = (float)(1.0 / getLength());
	return vector2d((end.X - start.X) * len, (end.Y - start.Y) * len);
}

float line2d::getAngleWith(const line2d& l)
{
	vector2d vect = getVector();
	vector2d vect2 = l.getVector();
	return vect.getAngleWith(vect2);
}

} // end namespace core
