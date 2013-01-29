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

#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include <EngineConfig.h>
#include <core/Vector3d.h>

namespace core
{

class matrix4;
//class vector3d;

//! Quaternion class.
class ENGINE_PUBLIC_EXPORT quaternion
{

public:

	quaternion();
	quaternion(const matrix4& mat);
	quaternion(float x, float y, float z);
	quaternion(float X, float Y, float Z, float W);
	quaternion(const quaternion& other);
	quaternion(const vector3d& axis, const float& angle);
	
	quaternion operator- () const;

	//! assignment operator
	quaternion& operator=(const quaternion& other);

	//! matrix assignment operator
	quaternion& operator=(const matrix4& other);

	//! add operator
	quaternion operator+(const quaternion& other) const;

	//! subtract operator
	quaternion operator-(const quaternion& other) const;

	//! multiplication operator
	quaternion operator*(const quaternion& other) const;

	//! multiplication operator
	quaternion operator*(float s) const;

	//! multiplication operator
	quaternion& operator*=(float s);

	//! multiplication operator
	vector3d operator*(const vector3d& v) const;

	//! multiplication operator
	quaternion& operator*=(const quaternion& other);

	//! equal operator
	bool operator==(const quaternion& other) const;

	bool operator!=(const quaternion& other) const;

	//! Sets new quaternion
	void set(float x, float y, float z, float w);

	//! Sets new quaternion based on Euler angles
	void set(float x, float y, float z);

	//! Sets new quaternion based on Euler angles
	void set(vector3d rot);

	//! Sets new quaternion based on Euler angles in degrees instead of radians
	void setDegrees(float x, float y, float z);

	//! Sets new quaternion based on Euler angles in degrees instead of radians
	void setDegrees(vector3d rot);

	//! Sets new quaternion based on axis and angle where the angle is in degrees instead of radians
	void setDegrees(const vector3d& axis, const float& angle);

	//! Inverts the quaternion.
	void invert();//non-zero quaternion
	void unitInvert();//unit-length quaternion

	//! Get the Invert of this quaternion
	quaternion getInverse() const;//non-zero quaternion
	quaternion getUnitInverse() const;//unit-length quaternion

	//! Normalizes the quaternion.
	quaternion& normalize();

	//! Returns the dot product.
	float dotProduct(const quaternion& other) const;

	//from angle in radians and axis
	void fromAngleAxis(const float& angle, const vector3d& axis);

	//from degrees and axis
	void fromDegreeAxis(const float& degree, const vector3d& axis);

	void fromAxisToAxis(const vector3d& src, const vector3d& dest, const vector3d& fallbackAxis = vector3d::ORIGIN_3D);

	void fromRotationMatrix(const matrix4& m);

	matrix4 toRotationMatrix() const;

	//! convert the quaternion to Euler angles
	vector3d toEulerAngles() const;

	//! convert the quaternion to Euler degrees
	vector3d toEulerDegrees() const;

	//! Calculate the local X element of this quaternion
	float getXDegrees() const;

	//! Calculate the local Y element of this quaternion
	float getYDegrees() const;

	//! Calculate the local Z element of this quaternion
	float getZDegrees() const;

	static const quaternion ZERO;
	static const quaternion IDENTITY;

	float X, Y, Z, W;
};

} // end namespace core

#endif