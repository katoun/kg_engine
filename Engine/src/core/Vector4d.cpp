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

#include<core/Vector4d.h>
#include <core/Math.h>
#include <core/Vector3d.h>

namespace core
{

vector4d::vector4d(): x(0), y(0), z(0), w(0) {}

vector4d::vector4d(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw) {}

vector4d::vector4d(const vector4d& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

vector4d::vector4d(const vector3d& other) : x(other.x), y(other.y), z(other.z), w(0) {}

vector4d vector4d::operator-() const
{
	return vector4d(-x, -y, -z, -w);
}

vector4d& vector4d::operator=(const vector4d& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}

vector4d vector4d::operator+(const vector4d& other) const
{
	return vector4d(x + other.x, y + other.y, z + other.z, w + other.w);
}

vector4d& vector4d::operator+=(const vector4d& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

vector4d vector4d::operator-(const vector4d& other) const
{
	return vector4d(x - other.x, y - other.y, z - other.z, w - other.w);
}
vector4d& vector4d::operator-=(const vector4d& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

vector4d vector4d::operator*(const float scalar) const
{
	return vector4d(x * scalar, y * scalar, z * scalar, w * scalar);
}

vector4d& vector4d::operator*=(const float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

vector4d vector4d::operator/(const float scalar) const
{
	float i = 1.0f / scalar;
	return vector4d(x * i, y * i, z * i, w * i);
}

vector4d& vector4d::operator/=(const float scalar)
{
	float i = 1.0f / scalar;
	x *= i;
	y *= i;
	z *= i;
	w *= i;
	return *this;
}

bool vector4d::operator==(const vector4d& other) const
{
	return ((other.x + EPSILON > x) && (other.x - EPSILON < x) &&
			(other.y + EPSILON > y) && (other.y - EPSILON < y) &&
			(other.z + EPSILON > z) && (other.z - EPSILON < z) &&
			(other.w + EPSILON > w) && (other.w - EPSILON < w));
}

bool vector4d::operator!=(const vector4d& other) const
{
	return ((other.x + EPSILON < x) || (other.x - EPSILON > x) ||
			(other.y + EPSILON < y) || (other.y - EPSILON > y) ||
			(other.z + EPSILON < z) || (other.z - EPSILON > z) ||
			(other.w + EPSILON < w) || (other.w - EPSILON > w));
}

void vector4d::set(const float nx, const float ny, const float nz, const float nw)
{
	x = nx;
	y = ny;
	z = nz;
	w = nw;
}

void vector4d::set(const vector4d& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

float* vector4d::get()
{
	return &x;
}

const float* vector4d::get() const
{
	return &x;
}

void vector4d::invert()
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;
}

vector4d& vector4d::normalize()
{
	float l = getLength();
	if (l == 0.0f)
	{
		return *this;
	}

	l = 1.0f / l;
	x *= l;
	y *= l;
	z *= l;
	w *= l;
	return *this;
}

float vector4d::getLength() const
{
	return sqrt(x*x + y*y + z*z + w*w);
}

float vector4d::getLengthSQ() const
{
	return x*x + y*y + z*z + w*w;
}

void vector4d::setLength(float newLength)
{
	float l = getLength();
	if (l == 0)
	{
		return;
	}

	l = newLength / l;
	x *= l;
	y *= l;
	z *= l;
	w *= l;
}

float vector4d::dotProduct(const vector4d& other) const
{
	return x*other.x + y*other.y + z*other.z + w*other.w;
}

float vector4d::getDistanceFrom(const vector4d& other) const
{
	float vx = x - other.x;
	float vy = y - other.y;
	float vz = z - other.z;
	float vw = w - other.w;
	return sqrt(vx*vx + vy*vy + vz*vz + vw*vw);
}

float vector4d::getDistanceFromSQ(const vector4d& other) const
{
	float vx = x - other.x;
	float vy = y - other.y;
	float vz = z - other.z;
	float vw = w - other.w;
	return (vx*vx + vy*vy + vz*vz + vw * vw);
}

} // end namespace core
