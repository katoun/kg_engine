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

#include<core/Vector4d.h>
#include <core/Math.h>
#include <core/Vector3d.h>

namespace core
{

vector4d::vector4d(): X(0), Y(0), Z(0), W(0) {}

vector4d::vector4d(float nx, float ny, float nz, float nw) : X(nx), Y(ny), Z(nz), W(nw) {}

vector4d::vector4d(const vector4d& other) : X(other.X), Y(other.Y), Z(other.Z), W(other.W) {}

vector4d::vector4d(const vector3d& other) : X(other.X), Y(other.Y), Z(other.Z), W(0) {}

vector4d vector4d::operator-() const
{
	return vector4d(-X, -Y, -Z, -W);
}

vector4d& vector4d::operator=(const vector4d& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	W = other.W;
	return *this;
}

vector4d vector4d::operator+(const vector4d& other) const
{
	return vector4d(X + other.X, Y + other.Y, Z + other.Z, W + other.W);
}

vector4d& vector4d::operator+=(const vector4d& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;
	W += other.W;
	return *this;
}

vector4d vector4d::operator-(const vector4d& other) const
{
	return vector4d(X - other.X, Y - other.Y, Z - other.Z, W - other.W);
}
vector4d& vector4d::operator-=(const vector4d& other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;
	W -= other.W;
	return *this;
}

vector4d vector4d::operator*(const float scalar) const
{
	return vector4d(X * scalar, Y * scalar, Z * scalar, W * scalar);
}

vector4d& vector4d::operator*=(const float scalar)
{
	X *= scalar;
	Y *= scalar;
	Z *= scalar;
	W *= scalar;
	return *this;
}

vector4d vector4d::operator/(const float scalar) const
{
	float i = 1.0f / scalar;
	return vector4d(X * i, Y * i, Z * i, W * i);
}

vector4d& vector4d::operator/=(const float scalar)
{
	float i = 1.0f / scalar;
	X *= i;
	Y *= i;
	Z *= i;
	W *= i;
	return *this;
}

bool vector4d::operator==(const vector4d& other) const
{
	return ((other.X + EPSILON > X) && (other.X - EPSILON < X) &&
			(other.Y + EPSILON > Y) && (other.Y - EPSILON < Y) &&
			(other.Z + EPSILON > Z) && (other.Z - EPSILON < Z) &&
			(other.W + EPSILON > W) && (other.W - EPSILON < W));
}

bool vector4d::operator!=(const vector4d& other) const
{
	return ((other.X + EPSILON < X) || (other.X - EPSILON > X) ||
			(other.Y + EPSILON < Y) || (other.Y - EPSILON > Y) ||
			(other.Z + EPSILON < Z) || (other.Z - EPSILON > Z) ||
			(other.W + EPSILON < W) || (other.W - EPSILON > W));
}

void vector4d::set(const float nx, const float ny, const float nz, const float nw)
{
	X = nx;
	Y = ny;
	Z = nz;
	W = nw;
}

void vector4d::set(const vector4d& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	W = other.W;
}

float* vector4d::get()
{
	return &X;
}

const float* vector4d::get() const
{
	return &X;
}

void vector4d::invert()
{
	X = -X;
	Y = -Y;
	Z = -Z;
	W = -W;
}

vector4d& vector4d::normalize()
{
	float l = getLength();
	if (l == 0.0f)
	{
		return *this;
	}

	l = 1.0f / l;
	X *= l;
	Y *= l;
	Z *= l;
	W *= l;
	return *this;
}

float vector4d::getLength() const
{
	return sqrt(X*X + Y*Y + Z*Z + W*W);
}

float vector4d::getLengthSQ() const
{
	return X*X + Y*Y + Z*Z + W*W;
}

void vector4d::setLength(float newLength)
{
	float l = getLength();
	if (l == 0)
	{
		return;
	}

	l = newLength / l;
	X *= l;
	Y *= l;
	Z *= l;
	W *= l;
}

float vector4d::dotProduct(const vector4d& other) const
{
	return X*other.X + Y*other.Y + Z*other.Z + W*other.W;
}

float vector4d::getDistanceFrom(const vector4d& other) const
{
	float vx = X - other.X;
	float vy = Y - other.Y;
	float vz = Z - other.Z;
	float vw = W - other.W;
	return sqrt(vx*vx + vy*vy + vz*vz + vw*vw);
}

float vector4d::getDistanceFromSQ(const vector4d& other) const
{
	float vx = X - other.X;
	float vy = Y - other.Y;
	float vz = Z - other.Z;
	float vw = W - other.W;
	return (vx*vx + vy*vy + vz*vz + vw * vw);
}

} // end namespace core
