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

#include <core/Quaternion.h>
#include <core/Math.h>
#include <core/Matrix4.h>
#include <core/Vector3d.h>

namespace core
{

quaternion::quaternion(): x(0), y(0), z(0), w(1) {}

quaternion::quaternion(const matrix4& mat)
{
	(*this) = mat;
}

quaternion::quaternion(float nx, float ny, float nz)
{
	set(nx, ny, nz);
}

quaternion::quaternion(float nx, float ny, float nz, float nw): x(nx), y(ny), z(nz), w(nw) {}

quaternion::quaternion(const quaternion& other): x(other.x), y(other.y), z(other.z), w(other.w) {}

quaternion::quaternion(const vector3d& axis, const float& angle)
{
	fromAngleAxis(angle, axis);
}

quaternion quaternion::operator- () const
{
	return quaternion(-x, -y, -z, -w);
}

quaternion& quaternion::operator=(const quaternion& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
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
		x = (m(2, 1) - m(1, 2)) / scale;
		y = (m(0, 2) - m(2, 0)) / scale;
		z = (m(1, 0) - m(0, 1)) / scale;
		w = 0.25f * scale;
	}
	else
	{
		if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2))
		{
			// 1st element of diag is greatest value
			// find scale according to 1st element, and double it
			scale = sqrt(1.0f + m(0, 0) - m(1, 1) - m(2, 2)) * 2.0f;

			// TODO: speed this up
			x = 0.25f * scale;
			y = (m(0, 1) + m(1, 0)) / scale;
			z = (m(2, 0) + m(0, 2)) / scale;
			w = (m(2, 1) - m(1, 2)) / scale;
		}
		else if (m(1, 1) > m(2, 2))
		{
			// 2nd element of diag is greatest value
			// find scale according to 2nd element, and double it
			scale = sqrt(1.0f + m(1, 1) - m(0, 0) - m(2, 2)) * 2.0f;

			// TODO: speed this up
			x = (m(0, 1) + m(1, 0)) / scale;
			y = 0.25f * scale;
			z = (m(1, 2) + m(2, 1)) / scale;
			w = (m(0, 2) - m(2, 0)) / scale;
		}
		else
		{
			// 3rd element of diag is greatest value
			// find scale according to 3rd element, and double it
			scale  = sqrt(1.0f + m(2, 2) - m(0, 0) - m(1, 1)) * 2.0f;

			// TODO: speed this up
			x = (m(0, 2) + m(2, 0)) / scale;
			y = (m(1, 2) + m(2, 1)) / scale;
			z = 0.25f * scale;
			w = (m(1, 0) - m(0, 1)) / scale;
		}
	}

	normalize();
	return *this;
}

quaternion quaternion::operator+(const quaternion& b) const
{
	return quaternion(x + b.x, y + b.y, z + b.z, w + b.w);
}

quaternion quaternion::operator-(const quaternion& b) const
{
	return quaternion(x -b.x, y - b.y, z - b.z, w - b.w);
}

quaternion quaternion::operator*(const quaternion& other) const
{
	quaternion tmp;
	
	tmp.x = (w * other.x) + (x * other.w) + (y * other.z) - (z * other.y);
	tmp.y = (w * other.y) + (y * other.w) + (z * other.x) - (x * other.z);
	tmp.z = (w * other.z) + (z * other.w) + (x * other.y) - (y * other.x);
	tmp.w = (w * other.w) - (x * other.x) - (y * other.y) - (z * other.z);

	return tmp;
}

vector3d quaternion::operator*(const vector3d& v) const
{
	// nVidia SDK implementation

	vector3d uv, uuv;
	vector3d qvec(x, y, z);
	uv = qvec.crossProduct(v);
	uuv = qvec.crossProduct(uv);
	uv *= (2.0f * w);
	uuv *= 2.0f;

	return v + uv + uuv;
}

quaternion quaternion::operator*(float s) const
{
	return quaternion(s*x, s*y, s*z, s*w);
}

quaternion& quaternion::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

quaternion& quaternion::operator*=(const quaternion& other)
{
	*this = other * (*this);
	return *this;
}

bool quaternion::operator==(const quaternion& other) const
{
	return ((other.x + EPSILON > x) && (other.x - EPSILON < x) &&
			(other.y + EPSILON > y) && (other.y - EPSILON < y) &&
			(other.z + EPSILON > z) && (other.z - EPSILON < z) &&
			(other.w + EPSILON > w) && (other.w - EPSILON < w));
}

bool quaternion::operator!=(const quaternion& other) const
{
	return ((other.x + EPSILON < x) || (other.x - EPSILON > x) ||
			(other.y + EPSILON < y) || (other.y - EPSILON > y) ||
			(other.z + EPSILON < z) || (other.z - EPSILON > z) ||
			(other.w + EPSILON < w) || (other.w - EPSILON > w));
}

void quaternion::set(float x, float y, float z, float w)
{
	x = x;
	y = y;
	z = z;
	w = w;
}

void quaternion::set(float nx, float ny, float nz)
{
	float angle;

	angle = nx * 0.5f;
	float sr = (float)sin(angle);
	float cr = (float)cos(angle);

	angle = ny * 0.5f;
	float sp = (float)sin(angle);
	float cp = (float)cos(angle);

	angle = nz * 0.5f;
	float sy = (float)sin(angle);
	float cy = (float)cos(angle);

	float cpcy = cp * cy;
	float spcy = sp * cy;
	float cpsy = cp * sy;
	float spsy = sp * sy; //quaternion fix by jox

	x = sr * cpcy - cr * spsy;
	y = cr * spcy + sr * cpsy;
	z = cr * cpsy - sr * spcy;
	w = cr * cpcy + sr * spsy;

	normalize();
}

void quaternion::set(vector3d rot)
{
	set(rot.x, rot.y, rot.z);
}

void quaternion::setDegrees(float x, float y, float z)
{
	set((float)(x * DEGTORAD), (float)(y * DEGTORAD), (float)(z * DEGTORAD));
}

void quaternion::setDegrees(vector3d rot)
{
	set((float)(rot.x * DEGTORAD), (float)(rot.y * DEGTORAD), (float)(rot.z * DEGTORAD));
}

void quaternion::setDegrees(const vector3d& axis, const float& angle)
{
	fromAngleAxis((float)(angle * DEGTORAD), axis);
}

void quaternion::invert()
{
	float fNorm = x * x + y * y + z * z + w * w;
	if (fNorm > 0.0)
	{
		float fInvNorm = 1.0f / fNorm;
		x = -x * fInvNorm;
		y = -y * fInvNorm;
		z = -z * fInvNorm;
		w = w * fInvNorm;
	}
	else
	{
		// return an invalid result to flag the error
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}
}

void quaternion::unitInvert()
{
	x = -x;
	y = -y;
	z = -z;
}

quaternion quaternion::getInverse() const
{
	float fNorm = x * x + y * y + z * z + w * w;
	if (fNorm > 0.0)
	{
		float fInvNorm = 1.0f / fNorm;
		return quaternion(-x*fInvNorm, -y*fInvNorm, -z*fInvNorm, w*fInvNorm);
	}
	else
	{
		// return an invalid result to flag the error
		return quaternion(0.0, 0.0, 0.0, 0.0);
	}
}

quaternion quaternion::getUnitInverse() const
{
	return quaternion(-x, -y, -z);
}

quaternion& quaternion::normalize()
{
	float n = x * x + y * y + z * z + w * w;

	if (n == 1)
		return *this;

	n = 1.0f / sqrt(n);
	x *= n;
	y *= n;
	z *= n;
	w *= n;

	return *this;
}

float quaternion::dotProduct(const quaternion& q2) const
{
	return (x * q2.x) + (y * q2.y) + (z * q2.z) + (w * q2.w);
}

void quaternion::fromAngleAxis(const float& angle, const vector3d& axis)
{
	// assert:  axis is unit length
	//
	// The quaternion representing the rotation is
	//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)
	// assert commented out since vector.normalize cant even create 100% normalized f32s
	// assert(sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z) == 1);

	float fHalfAngle = 0.5f * angle;
	float fSin = (float)sin(fHalfAngle);
	w = (float)cos(fHalfAngle);
	x = fSin * axis.x;
	y = fSin * axis.y;
	z = fSin * axis.z;
}

void  quaternion::fromDegreeAxis(const float& degree, const vector3d& axis)
{
	float fHalfAngle = 0.5f * (degree * DEGTORAD);
	float fSin = (float)sin(fHalfAngle);
	w = (float)cos(fHalfAngle);
	x = fSin * axis.x;
	y = fSin * axis.y;
	z = fSin * axis.z;

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
		x = IDENTITY.x;
		y = IDENTITY.y;
		z = IDENTITY.z;
		w = IDENTITY.w;
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

		x = c.x * invs;
		y = c.y * invs;
		z = c.z * invs;
		w = s * 0.5f;
		normalize();
	}
}

void quaternion::fromRotationMatrix(const matrix4& m)
{
	float T = 1 + m(0, 0) + m(1, 1) + m(2, 2);

	if (T > 0.00000001f)
	{
		float S = 0.5f / (float)sqrt(T);
		x = (m(2, 1) - m(1, 2)) * S;
		y =	(m(0, 2) - m(2, 0)) * S;
		z =	(m(1, 0) - m(0, 1)) * S;
		w = 0.25f / S;
	}
	else if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2))
	{
		float S = 1.0f / (2.0f * (float)sqrt(1.0f + m(0, 0) - m(1, 1) - m(2, 2)));
		x = 0.25f / S;
		y = (m(1, 0) + m(0, 1)) * S;
		z = (m(0, 2) + m(2, 0)) * S;
		w = (m(2, 1) - m(1, 2)) * S;
	}
	else if (m(1, 1) > m(2, 2))
	{
		float S = 1.0f / (2.0f * (float)sqrt(1.0f - m(0, 0) + m(1, 1) - m(2, 2)));
		x = (m(1, 0) + m(0, 1)) * S;
		y = 0.25f / S;
		z = (m(2, 1) + m(1, 2)) * S;
		w = (m(0, 2) - m(2, 0)) * S;
	}
	else
	{
		float S = 1.0f / (2.0f * (float)sqrt(1.0f - m(0, 0) - m(1, 1) + m(2, 2)));
		x = (m(0, 2) + m(2, 0)) * S;
		y = (m(2, 1) + m(1, 2)) * S;
		z = 0.25f / S;
		w = (m(1, 0) - m(0, 1)) * S;
	}
}

matrix4 quaternion::toRotationMatrix() const
{
	core::matrix4 m;
	
	m(0, 0) = 1.0f - (2.0f * y * y + 2.0f * z * z);
	m(0, 1) = 2.0f * x * y - 2.0f * z * w;
	m(0, 2) = 2.0f * x * z + 2.0f * y * w;
	m(0, 3) = 0.0f;

	m(1, 0) = 2.0f * x * y + 2.0f * z * w;
	m(1, 1) = 1.0f - (2.0f * x * x + 2.0f * z * z);
	m(1, 2) = 2.0f * z * y - 2.0f * x * w;
	m(1, 3) = 0.0f;

	m(2, 0) = 2.0f * x * z - 2.0f * y * w;
	m(2, 1) = 2.0f * z * y + 2.0f * x * w;
	m(2, 2) = 1.0f - (2.0f * x * x + 2.0f * y * y);
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
	would return -0 for y value and NaN for rotation of 1.5708
	around y-axis when using fromangleaxis() */

	vector3d euler;
	float testValue = x * y + z * w;
	if (testValue > 0.499f) // north pole singularity
	{
		euler.y = (float)(2.0f * atan2(x, w));
		euler.z = HALF_PI;
		euler.x = 0.0f;
		return euler;
	}
	if (testValue < -0.499f) // south pole singularity
	{
		euler.y = (float)(-2.0f * atan2(x, w));
		euler.z = -HALF_PI;
		euler.x = 0.0f;
		return euler;
	}
	float sqx = x * x;
	float sqy = y * y;
	float sqz = z * z;
	float sqw = w * w;
	float unit = sqx + sqy + sqz + sqw;

	euler.y = (float)atan2(2.0f * y * w - 2.0f * x * z, sqx - sqy - sqz + sqw);
	euler.z = (float)asin(2.0f * testValue / unit);
	euler.x = (float)atan2(2.0f * x * w - 2.0f * y * z, -sqx + sqy - sqz + sqw);

	return euler;
}


vector3d quaternion::toEulerDegrees() const
{
	vector3d eulerRad = toEulerAngles();
	eulerRad.x *= RADTODEG;
	eulerRad.y *= RADTODEG;
	eulerRad.z *= RADTODEG;

	return eulerRad;
}

float quaternion::getXDegrees() const
{
	float eulerx;
	float testValue = x * y + z * w;
	if (testValue > 0.499f) // north pole singularity
	{
		eulerx = 0.0f;
		return eulerx;
	}
	if (testValue < -0.499f) // south pole singularity
	{
		eulerx = 0.0f;
		return eulerx;
	}

	float sqx = x * x;
	float sqy = y * y;
	float sqz = z * z;
	float sqw = w * w;
	//float unit = sqx + sqy + sqz + sqw;

	eulerx = atan2(2.0f * x * w - 2.0f * y * z, -sqx + sqy - sqz + sqw);
	eulerx *= RADTODEG;

	return eulerx;
}

float quaternion::getYDegrees() const
{
	float eulery;
	float testValue = x * y + z * w;
	if (testValue > 0.499f) // north pole singularity
	{
		eulery = (float)(2.0f * atan2(x, w));;
		return eulery;
	}
	if (testValue < -0.499f) // south pole singularity
	{
		eulery = (float)(-2.0f * atan2(x, w));
		return eulery;
	}

	float sqx = x * x;
	float sqy = y * y;
	float sqz = z * z;
	float sqw = w * w;
	//float unit = sqx + sqy + sqz + sqw;

	eulery = (float)atan2(2.0f * y * w - 2.0f * x * z, sqx - sqy - sqz + sqw);
	eulery *= RADTODEG;

	return eulery;
}

float quaternion::getZDegrees() const
{
	float eulerz;
	float testValue = x * y + z * w;
	if (testValue > 0.499f) // north pole singularity
	{
		eulerz = HALF_PI;
		return eulerz;
	}
	if (testValue < -0.499f) // south pole singularity
	{
		eulerz = -HALF_PI;
		return eulerz;
	}

	float sqx = x * x;
	float sqy = y * y;
	float sqz = z * z;
	float sqw = w * w;
	float unit = sqx + sqy + sqz + sqw;

	eulerz = (float)asin(2.0f * testValue / unit);
	eulerz *= RADTODEG;

	return eulerz;
}

const quaternion quaternion::ZERO = quaternion(0.0f, 0.0f, 0.0f, 0.0f);
const quaternion quaternion::IDENTITY = quaternion(0.0f, 0.0f, 0.0f, 1.0f);

} // end namespace core
