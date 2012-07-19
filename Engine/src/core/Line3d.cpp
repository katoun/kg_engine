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

#include <core/Line3d.h>

namespace core
{

line3d::line3d() : start(0.0f, 0.0f, 0.0f), end(1.0f, 1.0f, 1.0f) {}

line3d::line3d(float xa, float ya, float za, float xb, float yb, float zb) : start(xa, ya, za), end(xb, yb, zb) {}

line3d::line3d(const vector3d& nstart, const vector3d& nend) : start(nstart), end(nend) {}

line3d::line3d(const line3d& other) : start(other.start), end(other.end) {}

line3d line3d::operator+(const vector3d& point) const
{
	return line3d(start + point, end + point);
}

line3d& line3d::operator+=(const vector3d& point)
{
	start += point;
	end += point;
	return *this;
}

line3d line3d::operator-(const vector3d& point) const
{
	return line3d(start - point, end - point);
}

line3d& line3d::operator-=(const vector3d& point)
{
	start -= point;
	end -= point;
	return *this;
}

bool line3d::operator==(const line3d& other) const
{
	return ((start == other.start && end == other.end) || (end == other.start && start == other.end)) ? true : false;
}

bool line3d::operator!=(const line3d& other) const
{
	return ((start != other.start || end != other.end) && (end != other.start || start != other.end)) ? true : false;
}

void line3d::set(const float& xa, const float& ya, const float& za, const float& xb, const float& yb, const float& zb)
{
	start.set(xa, ya, za);
	end.set(xb, yb, zb);
}

void line3d::set(const vector3d& nstart, const vector3d& nend)
{
	start.set(nstart);
	end.set(nend);
}

void line3d::set(const line3d& other)
{
	start.set(other.start);
	end.set(other.end);
}

float line3d::getLength() const
{
	return start.getDistanceFrom(end);
};

float line3d::getLengthSQ() const
{
	return start.getDistanceFromSQ(end);
};

vector3d line3d::getMiddle() const
{
	return (start + end) * 0.5f;
}

vector3d line3d::getVector() const
{
	return end - start;
}

bool line3d::isPointBetweenStartAndEnd(const vector3d& point) const
{
	return point.isBetweenPoints(start, end);
}

vector3d line3d::getClosestPoint(const vector3d& point) const
{
	vector3d c = point - start;
	vector3d v = end - start;
	float d = v.getLength();
	v /= d;
	float t = v.dotProduct(c);

	if (t < 0.0f) return start;
	if (t > d) return end;

	v *= t;
	return start + v;
}

} // end namespace core
