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

vector3d::vector3d() : X(0), Y(0), Z(0) {}

vector3d::vector3d(float nx, float ny, float nz) : X(nx), Y(ny), Z(nz) {}

vector3d::vector3d(const vector3d& other) : X(other.X), Y(other.Y), Z(other.Z) {}

vector3d vector3d::operator-() const
{
	return vector3d(-X, -Y, -Z);
}

vector3d& vector3d::operator=(const vector3d& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	return *this;
}

vector3d vector3d::operator+(const vector3d& other) const
{
	return vector3d(X + other.X, Y + other.Y, Z + other.Z);
}

vector3d& vector3d::operator+=(const vector3d& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;
	return *this;
}

vector3d vector3d::operator-(const vector3d& other) const
{
	return vector3d(X - other.X, Y - other.Y, Z - other.Z);
}

vector3d& vector3d::operator-=(const vector3d& other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;
	return *this;
}

vector3d vector3d::operator*(const float scalar) const
{
	return vector3d(X * scalar, Y * scalar, Z * scalar);
}

vector3d& vector3d::operator*=(const float scalar)
{
	X *= scalar;
	Y *= scalar;
	Z *= scalar;
	return *this;
}

vector3d vector3d::operator/(const float scalar) const
{
	assert(scalar != 0.0f);

	float i = 1.0f / scalar;
	return vector3d(X * i, Y * i, Z * i);
}

vector3d& vector3d::operator/=(const float scalar)
{
	assert(scalar != 0.0f);

	float i = 1.0f / scalar;
	X *= i;
	Y *= i;
	Z *= i;
	return *this;
}

bool vector3d::operator==(const vector3d& other) const
{
	return ((other.X + EPSILON > X) && (other.X - EPSILON < X) &&
			(other.Y + EPSILON > Y) && (other.Y - EPSILON < Y) &&
			(other.Z + EPSILON > Z) && (other.Z - EPSILON < Z));
}

bool vector3d::operator!=(const vector3d& other) const
{
	return ((other.X + EPSILON < X) || (other.X - EPSILON > X) ||
			(other.Y + EPSILON < Y) || (other.Y - EPSILON > Y) ||
			(other.Z + EPSILON < Z) || (other.Z - EPSILON > Z));
}

vector3d& vector3d::normalize()
{
	float ln = getLength();
	if (ln > EPSILON)
	{
		ln = 1.0f / ln;
		X *= ln;
		Y *= ln;
		Z *= ln;
	}
	return *this;
}

void vector3d::invert()
{
	X = -X;
	Y = -Y;
	Z = -Z;
}

void vector3d::set(const float nx, const float ny, const float nz)
{
	X = nx;
	Y = ny;
	Z = nz;
}

void vector3d::set(const vector3d& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
}

float* vector3d::get()
{
	return &X;
}

const float* vector3d::get() const
{
	return &X;
}

float vector3d::getLength() const
{
	return sqrt(X*X + Y*Y + Z*Z);
}

float vector3d::getLengthSQ() const
{
	return X*X + Y*Y + Z*Z;
}

void vector3d::setLength(float newLength)
{
	float ln = getLength();
	if (ln > EPSILON)
	{
		ln = newLength / ln;
		X *= ln;
		Y *= ln;
		Z *= ln;
	}
}

float vector3d::dotProduct(const vector3d& other) const
{
	return X*other.X + Y*other.Y + Z*other.Z;
}

vector3d vector3d::crossProduct(const vector3d& p) const
{
	return vector3d(Y * p.Z - Z * p.Y, Z * p.X - X * p.Z, X * p.Y - Y * p.X);
}

float vector3d::getDistanceFrom(const vector3d& other) const
{
	float vx = X - other.X;
	float vy = Y - other.Y;
	float vz = Z - other.Z;
	return sqrt(vx*vx + vy*vy + vz*vz);
}

float vector3d::getDistanceFromSQ(const vector3d& other) const
{
	float vx = X - other.X;
	float vy = Y - other.Y;
	float vz = Z - other.Z;
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
