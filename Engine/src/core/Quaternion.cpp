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

#include <core/Quaternion.h>
#include <core/Math.h>
#include <core/Matrix4.h>
#include <core/Vector3d.h>

namespace core
{

quaternion::quaternion(): X(0.0f), Y(0.0f), Z(0.0f), W(1.0f) {}

quaternion::quaternion(const matrix4& mat)
{
	(*this) = mat;
}

quaternion::quaternion(float x, float y, float z)
{
	set(x, y, z);
}

quaternion::quaternion(float x, float y, float z, float w): X(x), Y(y), Z(z), W(w) {}

quaternion::quaternion(const quaternion& other): X(other.X), Y(other.Y), Z(other.Z), W(other.W) {}

quaternion::quaternion(const vector3d& axis, const float& angle)
{
	fromAngleAxis(angle, axis);
}

quaternion quaternion::operator- () const
{
	return quaternion(-X, -Y, -Z, -W);
}

quaternion& quaternion::operator=(const quaternion& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	W = other.W;
	return *this;
}

quaternion& quaternion::operator=(const matrix4& m)
{
	float diag = m(0, 0) + m(1, 1) + m(2, 2) + 1;
	float scale = 0.0f;

	if (diag > 0.0f)
	{
		scale = sqrt(diag) * 2.0f; // get scale from diagonal

		// TODO: speed this up
		X = (m(2, 1) - m(1, 2)) / scale;
		Y = (m(0, 2) - m(2, 0)) / scale;
		Z = (m(1, 0) - m(0, 1)) / scale;
		W = 0.25f * scale;
	}
	else
	{
		if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2))
		{
			// 1st element of diag is greatest value
			// find scale according to 1st element, and double it
			scale = sqrt(1.0f + m(0, 0) - m(1, 1) - m(2, 2)) * 2.0f;

			// TODO: speed this up
			X = 0.25f * scale;
			Y = (m(0, 1) + m(1, 0)) / scale;
			Z = (m(2, 0) + m(0, 2)) / scale;
			W = (m(2, 1) - m(1, 2)) / scale;
		}
		else if (m(1, 1) > m(2, 2))
		{
			// 2nd element of diag is greatest value
			// find scale according to 2nd element, and double it
			scale = sqrt(1.0f + m(1, 1) - m(0, 0) - m(2, 2)) * 2.0f;

			// TODO: speed this up
			X = (m(0, 1) + m(1, 0)) / scale;
			Y = 0.25f * scale;
			Z = (m(1, 2) + m(2, 1)) / scale;
			W = (m(0, 2) - m(2, 0)) / scale;
		}
		else
		{
			// 3rd element of diag is greatest value
			// find scale according to 3rd element, and double it
			scale  = sqrt(1.0f + m(2, 2) - m(0, 0) - m(1, 1)) * 2.0f;

			// TODO: speed this up
			X = (m(0, 2) + m(2, 0)) / scale;
			Y = (m(1, 2) + m(2, 1)) / scale;
			Z = 0.25f * scale;
			W = (m(1, 0) - m(0, 1)) / scale;
		}
	}

	normalize();
	return *this;
}

quaternion quaternion::operator+(const quaternion& b) const
{
	return quaternion(X + b.X, Y + b.Y, Z + b.Z, W + b.W);
}

quaternion quaternion::operator-(const quaternion& b) const
{
	return quaternion(X -b.X, Y - b.Y, Z - b.Z, W - b.W);
}

quaternion quaternion::operator*(const quaternion& other) const
{
	quaternion tmp;
	
	tmp.X = (W * other.X) + (X * other.W) + (Y * other.Z) - (Z * other.Y);
	tmp.Y = (W * other.Y) + (Y * other.W) + (Z * other.X) - (X * other.Z);
	tmp.Z = (W * other.Z) + (Z * other.W) + (X * other.Y) - (Y * other.X);
	tmp.W = (W * other.W) - (X * other.X) - (Y * other.Y) - (Z * other.Z);

	return tmp;
}

vector3d quaternion::operator*(const vector3d& v) const
{
	// nVidia SDK implementation

	vector3d uv, uuv;
	vector3d qvec(X, Y, Z);
	uv = qvec.crossProduct(v);
	uuv = qvec.crossProduct(uv);
	uv *= (2.0f * W);
	uuv *= 2.0f;

	return v + uv + uuv;
}

quaternion quaternion::operator*(float s) const
{
	return quaternion(s*X, s*Y, s*Z, s*W);
}

quaternion& quaternion::operator*=(float s)
{
	X *= s;
	Y *= s;
	Z *= s;
	W *= s;
	return *this;
}

quaternion& quaternion::operator*=(const quaternion& other)
{
	*this = other * (*this);
	return *this;
}

bool quaternion::operator==(const quaternion& other) const
{
	return ((other.X + EPSILON > X) && (other.X - EPSILON < X) &&
			(other.Y + EPSILON > Y) && (other.Y - EPSILON < Y) &&
			(other.Z + EPSILON > Z) && (other.Z - EPSILON < Z) &&
			(other.W + EPSILON > W) && (other.W - EPSILON < W));
}

bool quaternion::operator!=(const quaternion& other) const
{
	return ((other.X + EPSILON < X) || (other.X - EPSILON > X) ||
			(other.Y + EPSILON < Y) || (other.Y - EPSILON > Y) ||
			(other.Z + EPSILON < Z) || (other.Z - EPSILON > Z) ||
			(other.W + EPSILON < W) || (other.W - EPSILON > W));
}

void quaternion::set(float x, float y, float z, float w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

void quaternion::set(float x, float y, float z)
{
	float angle;

	angle = x * 0.5f;
	float sr = (float)sin(angle);
	float cr = (float)cos(angle);

	angle = y * 0.5f;
	float sp = (float)sin(angle);
	float cp = (float)cos(angle);

	angle = z * 0.5f;
	float sy = (float)sin(angle);
	float cy = (float)cos(angle);

	float cpcy = cp * cy;
	float spcy = sp * cy;
	float cpsy = cp * sy;
	float spsy = sp * sy; //quaternion fix by jox

	X = sr * cpcy - cr * spsy;
	Y = cr * spcy + sr * cpsy;
	Z = cr * cpsy - sr * spcy;
	W = cr * cpcy + sr * spsy;

	normalize();
}

void quaternion::set(vector3d rot)
{
	set(rot.X, rot.Y, rot.Z);
}

void quaternion::setDegrees(float x, float y, float z)
{
	set((float)(x * DEGTORAD), (float)(y * DEGTORAD), (float)(z * DEGTORAD));
}

void quaternion::setDegrees(vector3d rot)
{
	set((float)(rot.X * DEGTORAD), (float)(rot.Y * DEGTORAD), (float)(rot.Z * DEGTORAD));
}

void quaternion::setDegrees(const vector3d& axis, const float& angle)
{
	fromAngleAxis((float)(angle * DEGTORAD), axis);
}

void quaternion::invert()
{
	float fNorm = X * X + Y * Y + Z * Z + W * W;
	if (fNorm > 0.0)
	{
		float fInvNorm = 1.0f / fNorm;
		X = -X * fInvNorm;
		Y = -Y * fInvNorm;
		Z = -Z * fInvNorm;
		W = W * fInvNorm;
	}
	else
	{
		// return an invalid result to flag the error
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
		W = 0.0f;
	}
}

void quaternion::unitInvert()
{
	X = -X;
	Y = -Y;
	Z = -Z;
}

quaternion quaternion::getInverse() const
{
	float fNorm = X * X + Y * Y + Z * Z + W * W;
	if (fNorm > 0.0)
	{
		float fInvNorm = 1.0f / fNorm;
		return quaternion(-X*fInvNorm, -Y*fInvNorm, -Z*fInvNorm, W*fInvNorm);
	}
	else
	{
		// return an invalid result to flag the error
		return quaternion(0.0, 0.0, 0.0, 0.0);
	}
}

quaternion quaternion::getUnitInverse() const
{
	return quaternion(-X, -Y, -Z);
}

quaternion& quaternion::normalize()
{
	float n = X * X + Y * Y + Z * Z + W * W;

	if (n == 1)
		return *this;

	n = 1.0f / sqrt(n);
	X *= n;
	Y *= n;
	Z *= n;
	W *= n;

	return *this;
}

float quaternion::dotProduct(const quaternion& q2) const
{
	return (X * q2.X) + (Y * q2.Y) + (Z * q2.Z) + (W * q2.W);
}

void quaternion::fromAngleAxis(const float& angle, const vector3d& axis)
{
	// assert:  axis is unit length
	//
	// The quaternion representing the rotation is
	//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)
	// assert commented out since vector.normalize cant even create 100% normalized f32s
	// assert(sqrt(axis.X * axis.X + axis.Y * axis.Y + axis.Z * axis.Z) == 1);

	float fHalfAngle = 0.5f * angle;
	float fSin = (float)sin(fHalfAngle);
	W = (float)cos(fHalfAngle);
	X = fSin * axis.X;
	Y = fSin * axis.Y;
	Z = fSin * axis.Z;
}

void  quaternion::fromDegreeAxis(const float& degree, const vector3d& axis)
{
	float fHalfAngle = 0.5f * (degree * DEGTORAD);
	float fSin = (float)sin(fHalfAngle);
	W = (float)cos(fHalfAngle);
	X = fSin * axis.X;
	Y = fSin * axis.Y;
	Z = fSin * axis.Z;

}

void quaternion::fromAxisToAxis(const vector3d& src, const vector3d& dest, const vector3d& fallbackAxis)
{
	// Based on Stan Melax's article in Game Programming Gems

	// Copy, since cannot modify local
	vector3d v0 = src;
	vector3d v1 = dest;
	v0.normalize();
	v1.normalize();

	float d = v0.dotProduct(v1);
	// If dot == 1, vectors are the same
	if (d >= 1.0f)
	{
		X = IDENTITY.X;
		Y = IDENTITY.Y;
		Z = IDENTITY.Z;
		W = IDENTITY.W;
	}
	
	if (d < (1e-6f - 1.0f))
	{
		if (fallbackAxis != vector3d::ORIGIN_3D)
		{
			// rotate 180 degrees about the fallback axis
			fromAngleAxis(PI, fallbackAxis);
		}
		else
		{
			// Generate an axis
			vector3d axis = vector3d::UNIT_X.crossProduct(src);
			if (axis.getLength() == 0) // pick another if colinear
				axis = vector3d::UNIT_Y.crossProduct(src);
			axis.normalize();
			fromAngleAxis(PI, axis);
		}
	}
	else
	{
		float s = sqrt((1+d)*2);
		float invs = 1 / s;

		vector3d c = v0.crossProduct(v1);

		X = c.X * invs;
		Y = c.Y * invs;
		Z = c.Z * invs;
		W = s * 0.5f;
		normalize();
	}
}

void quaternion::fromRotationMatrix(const matrix4& m)
{
	float T = 1 + m(0, 0) + m(1, 1) + m(2, 2);

	if (T > 0.00000001f)
	{
		float S = 0.5f / (float)sqrt(T);
		X = (m(2, 1) - m(1, 2)) * S;
		Y =	(m(0, 2) - m(2, 0)) * S;
		Z =	(m(1, 0) - m(0, 1)) * S;
		W = 0.25f / S;
	}
	else if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2))
	{
		float S = 1.0f / (2.0f * (float)sqrt(1.0f + m(0, 0) - m(1, 1) - m(2, 2)));
		X = 0.25f / S;
		Y = (m(1, 0) + m(0, 1)) * S;
		Z = (m(0, 2) + m(2, 0)) * S;
		W = (m(2, 1) - m(1, 2)) * S;
	}
	else if (m(1, 1) > m(2, 2))
	{
		float S = 1.0f / (2.0f * (float)sqrt(1.0f - m(0, 0) + m(1, 1) - m(2, 2)));
		X = (m(1, 0) + m(0, 1)) * S;
		Y = 0.25f / S;
		Z = (m(2, 1) + m(1, 2)) * S;
		W = (m(0, 2) - m(2, 0)) * S;
	}
	else
	{
		float S = 1.0f / (2.0f * (float)sqrt(1.0f - m(0, 0) - m(1, 1) + m(2, 2)));
		X = (m(0, 2) + m(2, 0)) * S;
		Y = (m(2, 1) + m(1, 2)) * S;
		Z = 0.25f / S;
		W = (m(1, 0) - m(0, 1)) * S;
	}
}

matrix4 quaternion::toRotationMatrix() const
{
	core::matrix4 m;
	
	m(0, 0) = 1.0f - (2.0f * Y * Y + 2.0f * Z * Z);
	m(0, 1) = 2.0f * X * Y - 2.0f * Z * W;
	m(0, 2) = 2.0f * X * Z + 2.0f * Y * W;
	m(0, 3) = 0.0f;

	m(1, 0) = 2.0f * X * Y + 2.0f * Z * W;
	m(1, 1) = 1.0f - (2.0f * X * X + 2.0f * Z * Z);
	m(1, 2) = 2.0f * Z * Y - 2.0f * X * W;
	m(1, 3) = 0.0f;

	m(2, 0) = 2.0f * X * Z - 2.0f * Y * W;
	m(2, 1) = 2.0f * Z * Y + 2.0f * X * W;
	m(2, 2) = 1.0f - (2.0f * X * X + 2.0f * Y * Y);
	m(2, 3) = 0.0f;

	m(3, 0) = 0.0f;
	m(3, 1) = 0.0f;
	m(3, 2) = 0.0f;
	m(3, 3) = 1.0f;

	return m;
}

vector3d quaternion::toEulerAngles() const
{
	/* new version after euclideanspace.com by Matthias Meyer
	old version had a problem where the initial quaternion
	would return -0 for Y value and NaN for rotation of 1.5708
	around Y-axis when using fromangleaxis() */

	vector3d euler;
	float testValue = X * Y + Z * W;
	if (testValue > 0.499f) // north pole singularity
	{
		euler.Y = (float)(2.0f * atan2(X, W));
		euler.Z = HALF_PI;
		euler.X = 0.0f;
		return euler;
	}
	if (testValue < -0.499f) // south pole singularity
	{
		euler.Y = (float)(-2.0f * atan2(X, W));
		euler.Z = -HALF_PI;
		euler.X = 0.0f;
		return euler;
	}
	float sqx = X * X;
	float sqy = Y * Y;
	float sqz = Z * Z;
	float sqw = W * W;
	float unit = sqx + sqy + sqz + sqw;

	euler.Y = (float)atan2(2.0f * Y * W - 2.0f * X * Z, sqx - sqy - sqz + sqw);
	euler.Z = (float)asin(2.0f * testValue / unit);
	euler.X = (float)atan2(2.0f * X * W - 2.0f * Y * Z, -sqx + sqy - sqz + sqw);

	return euler;
}


vector3d quaternion::toEulerDegrees() const
{
	vector3d eulerRad = toEulerAngles();
	eulerRad.X *= RADTODEG;
	eulerRad.Y *= RADTODEG;
	eulerRad.Z *= RADTODEG;

	return eulerRad;
}

float quaternion::getXDegrees(void) const
{
	float eulerX;
	float testValue = X * Y + Z * W;
	if (testValue > 0.499f) // north pole singularity
	{
		eulerX = 0.0f;
		return eulerX;
	}
	if (testValue < -0.499f) // south pole singularity
	{
		eulerX = 0.0f;
		return eulerX;
	}

	float sqx = X * X;
	float sqy = Y * Y;
	float sqz = Z * Z;
	float sqw = W * W;
	//float unit = sqx + sqy + sqz + sqw;

	eulerX = atan2(2.0f * X * W - 2.0f * Y * Z, -sqx + sqy - sqz + sqw);
	eulerX *= RADTODEG;

	return eulerX;
}

float quaternion::getYDegrees(void) const
{
	float eulerY;
	float testValue = X * Y + Z * W;
	if (testValue > 0.499f) // north pole singularity
	{
		eulerY = (float)(2.0f * atan2(X, W));;
		return eulerY;
	}
	if (testValue < -0.499f) // south pole singularity
	{
		eulerY = (float)(-2.0f * atan2(X, W));
		return eulerY;
	}

	float sqx = X * X;
	float sqy = Y * Y;
	float sqz = Z * Z;
	float sqw = W * W;
	//float unit = sqx + sqy + sqz + sqw;

	eulerY = (float)atan2(2.0f * Y * W - 2.0f * X * Z, sqx - sqy - sqz + sqw);
	eulerY *= RADTODEG;

	return eulerY;
}

float quaternion::getZDegrees(void) const
{
	float eulerZ;
	float testValue = X * Y + Z * W;
	if (testValue > 0.499f) // north pole singularity
	{
		eulerZ = HALF_PI;
		return eulerZ;
	}
	if (testValue < -0.499f) // south pole singularity
	{
		eulerZ = -HALF_PI;
		return eulerZ;
	}

	float sqx = X * X;
	float sqy = Y * Y;
	float sqz = Z * Z;
	float sqw = W * W;
	float unit = sqx + sqy + sqz + sqw;

	eulerZ = (float)asin(2.0f * testValue / unit);
	eulerZ *= RADTODEG;

	return eulerZ;
}

const quaternion quaternion::ZERO = quaternion(0.0f, 0.0f, 0.0f, 0.0f);
const quaternion quaternion::IDENTITY = quaternion(0.0f, 0.0f, 0.0f, 1.0f);

} // end namespace core
