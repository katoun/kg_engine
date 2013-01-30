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

#include <core/Vector3d.h>
#include <core/Math.h>

namespace core
{

vector3d::vector3d() : x(0), y(0), z(0) {}

vector3d::vector3d(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

vector3d::vector3d(const vector3d& other) : x(other.x), y(other.y), z(other.z) {}

vector3d vector3d::operator-() const
{
	return vector3d(-x, -y, -z);
}

vector3d& vector3d::operator=(const vector3d& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

vector3d vector3d::operator+(const vector3d& other) const
{
	return vector3d(x + other.x, y + other.y, z + other.z);
}

vector3d& vector3d::operator+=(const vector3d& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

vector3d vector3d::operator-(const vector3d& other) const
{
	return vector3d(x - other.x, y - other.y, z - other.z);
}

vector3d& vector3d::operator-=(const vector3d& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

vector3d vector3d::operator*(const float scalar) const
{
	return vector3d(x * scalar, y * scalar, z * scalar);
}

vector3d& vector3d::operator*=(const float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

vector3d vector3d::operator/(const float scalar) const
{
	assert(scalar != 0.0f);

	float i = 1.0f / scalar;
	return vector3d(x * i, y * i, z * i);
}

vector3d& vector3d::operator/=(const float scalar)
{
	assert(scalar != 0.0f);

	float i = 1.0f / scalar;
	x *= i;
	y *= i;
	z *= i;
	return *this;
}

bool vector3d::operator==(const vector3d& other) const
{
	return ((other.x + EPSILON > x) && (other.x - EPSILON < x) &&
			(other.y + EPSILON > y) && (other.y - EPSILON < y) &&
			(other.z + EPSILON > z) && (other.z - EPSILON < z));
}

bool vector3d::operator!=(const vector3d& other) const
{
	return ((other.x + EPSILON < x) || (other.x - EPSILON > x) ||
			(other.y + EPSILON < y) || (other.y - EPSILON > y) ||
			(other.z + EPSILON < z) || (other.z - EPSILON > z));
}

vector3d& vector3d::normalize()
{
	float ln = getLength();
	if (ln > EPSILON)
	{
		ln = 1.0f / ln;
		x *= ln;
		y *= ln;
		z *= ln;
	}
	return *this;
}

void vector3d::invert()
{
	x = -x;
	y = -y;
	z = -z;
}

void vector3d::set(const float nx, const float ny, const float nz)
{
	x = nx;
	y = ny;
	z = nz;
}

void vector3d::set(const vector3d& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

float* vector3d::get()
{
	return &x;
}

const float* vector3d::get() const
{
	return &x;
}

float vector3d::getLength() const
{
	return sqrt(x*x + y*y + z*z);
}

float vector3d::getLengthSQ() const
{
	return x*x + y*y + z*z;
}

void vector3d::setLength(float newLength)
{
	float ln = getLength();
	if (ln > EPSILON)
	{
		ln = newLength / ln;
		x *= ln;
		y *= ln;
		z *= ln;
	}
}

float vector3d::dotProduct(const vector3d& other) const
{
	return x*other.x + y*other.y + z*other.z;
}

vector3d vector3d::crossProduct(const vector3d& p) const
{
	return vector3d(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

float vector3d::getDistanceFrom(const vector3d& other) const
{
	float vx = x - other.x;
	float vy = y - other.y;
	float vz = z - other.z;
	return sqrt(vx*vx + vy*vy + vz*vz);
}

float vector3d::getDistanceFromSQ(const vector3d& other) const
{
	float vx = x - other.x;
	float vy = y - other.y;
	float vz = z - other.z;
	return (vx*vx + vy*vy + vz*vz);
}

bool vector3d::isBetweenPoints(const vector3d& begin, const vector3d& end) const
{
	float f = (end - begin).getLengthSQ();
	return (getDistanceFromSQ(begin) < f && getDistanceFromSQ(end) < f);
}

const vector3d vector3d::ORIGIN_3D = vector3d(0.0f, 0.0f, 0.0f);

const vector3d vector3d::UNIT_X = vector3d(1.0f, 0.0f, 0.0f);

const vector3d vector3d::UNIT_Y = vector3d(0.0f, 1.0f, 0.0f);

const vector3d vector3d::UNIT_Z = vector3d(0.0f, 0.0f, 1.0f);

const vector3d vector3d::NEGATIVE_UNIT_X = vector3d(-1.0f, 0.0f, 0.0f);

const vector3d vector3d::NEGATIVE_UNIT_Y = vector3d(0.0f, -1.0f, 0.0f);

const vector3d vector3d::NEGATIVE_UNIT_Z = vector3d(0.0f, 0.0f, -1.0f);

const vector3d vector3d::UNIT_SCALE = vector3d(1.0f, 1.0f, 1.0f);

} // end namespace core
