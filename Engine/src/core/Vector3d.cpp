/*
-----------------------------------------------------------------------------
Copyright (c) 2006-2012 Catalin Alexandru Nastase

KG game engine (http://k-game.sourceforge.net) is made available under the LGPL License (http://www.gnu.org/copyleft/lgpl.html)

Under the LGPL you may use KG game engine for any purpose you wish, as long as you:
1. Release any modifications to the KG game engine source back to the community
2. Pass on the source to KG game engine with all the copyrights intact, or link back to a place where the source code can be obtained (http://k-game.sourceforge.net)
3. Make it clear where you have customised it.
The above is a precis, please do read the full license agreement.
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
