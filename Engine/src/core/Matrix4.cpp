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

#include <core/Matrix4.h>
#include <core/Math.h>
#include <core/Vector3d.h>
#include <core/Vector4d.h>
#include <core/Plane3d.h>
#include <core/Aabox3d.h>

namespace core
{

float matrix4::empty = 0;

inline matrix4::matrix4()
{
	makeIdentity();
}

inline matrix4::matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
                        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	M[0] = m00;
	M[1] = m01;
	M[2] = m02;
	M[3] = m03;
	M[4] = m10;
	M[5] = m11;
	M[6] = m12;
	M[7] = m13;
	M[8] = m20;
	M[9] = m21;
	M[10] = m22;
	M[11] = m23;
	M[12] = m30;
	M[13] = m31;
	M[14] = m32;
	M[15] = m33;
}

inline matrix4& matrix4::operator=(const matrix4 &other)
{
	M[ 0] = other.M[ 0];
	M[ 1] = other.M[ 1];
	M[ 2] = other.M[ 2];
	M[ 3] = other.M[ 3];
	M[ 4] = other.M[ 4];
	M[ 5] = other.M[ 5];
	M[ 6] = other.M[ 6];
	M[ 7] = other.M[ 7];
	M[ 8] = other.M[ 8];
	M[ 9] = other.M[ 9];
	M[10] = other.M[10];
	M[11] = other.M[11];
	M[12] = other.M[12];
	M[13] = other.M[13];
	M[14] = other.M[14];
	M[15] = other.M[15];

	return *this;
}

float& matrix4::operator()(signed int row, signed int col)
{
	assert(row <= 3 && col <= 3);

	if (row > 3 || col > 3)
		return empty;

	return M[row * 4 + col];
}

const float& matrix4::operator()(signed int row, signed int col) const
{
	assert(row <= 3 && col <= 3);

	if (row > 3 || col > 3)
		return empty;

	return M[row * 4 + col];
}

float& matrix4::operator[](unsigned int index) 
{
	assert(index <= 15);

	if (index > 15)
		return empty;

	return M[index]; 
}

const float& matrix4::operator[](unsigned int index) const 
{
	assert(index <= 15);

	if (index > 15)
		return empty;

	return M[index]; 
}

inline bool matrix4::operator==(const matrix4 &other) const
{
	if( 
		M[ 0] != other.M[ 0] || M[ 1] != other.M[ 1] || M[ 2] != other.M[ 2] || M[ 3] != other.M[ 3] ||
		M[ 4] != other.M[ 4] || M[ 5] != other.M[ 5] || M[ 6] != other.M[ 6] || M[ 7] != other.M[ 7] ||
		M[ 8] != other.M[ 8] || M[ 9] != other.M[ 9] || M[10] != other.M[10] || M[11] != other.M[11] ||
		M[12] != other.M[12] || M[13] != other.M[13] || M[14] != other.M[14] || M[15] != other.M[15] )
		return false;
	return true;
}

inline bool matrix4::operator!=(const matrix4 &other) const
{
	if( 
		M[ 0] == other.M[ 0] || M[ 1] == other.M[ 1] || M[ 2] == other.M[ 2] || M[ 3] == other.M[ 3] ||
		M[ 4] == other.M[ 4] || M[ 5] == other.M[ 5] || M[ 6] == other.M[ 6] || M[ 7] == other.M[ 7] ||
		M[ 8] == other.M[ 8] || M[ 9] == other.M[ 9] || M[10] == other.M[10] || M[11] == other.M[11] ||
		M[12] == other.M[12] || M[13] == other.M[13] || M[14] == other.M[14] || M[15] == other.M[15] )
		return false;
	return true;
}

inline matrix4& matrix4::operator*=(const matrix4& other)
{	
	float newMatrix[16];
	const float *m1 = M, *m2 = other.M;

	newMatrix[ 0] = m1[ 0] * m2[ 0] + m1[ 1] * m2[ 4] + m1[ 2] * m2[ 8] + m1[ 3] * m2[12];
	newMatrix[ 1] = m1[ 0] * m2[ 1] + m1[ 1] * m2[ 5] + m1[ 2] * m2[ 9] + m1[ 3] * m2[13];
	newMatrix[ 2] = m1[ 0] * m2[ 2] + m1[ 1] * m2[ 6] + m1[ 2] * m2[10] + m1[ 3] * m2[14];
	newMatrix[ 3] = m1[ 0] * m2[ 3] + m1[ 1] * m2[ 7] + m1[ 2] * m2[11] + m1[ 3] * m2[15];

	newMatrix[ 4] = m1[ 4] * m2[ 0] + m1[ 5] * m2[ 4] + m1[ 6] * m2[ 8] + m1[ 7] * m2[12];
	newMatrix[ 5] = m1[ 4] * m2[ 1] + m1[ 5] * m2[ 5] + m1[ 6] * m2[ 9] + m1[ 7] * m2[13];
	newMatrix[ 6] = m1[ 4] * m2[ 2] + m1[ 5] * m2[ 6] + m1[ 6] * m2[10] + m1[ 7] * m2[14];
	newMatrix[ 7] = m1[ 4] * m2[ 3] + m1[ 5] * m2[ 7] + m1[ 6] * m2[11] + m1[ 7] * m2[15];

	newMatrix[ 8] = m1[ 8] * m2[ 0] + m1[ 9] * m2[ 4] + m1[10] * m2[ 8] + m1[11] * m2[12];
	newMatrix[ 9] = m1[ 8] * m2[ 1] + m1[ 9] * m2[ 5] + m1[10] * m2[ 9] + m1[11] * m2[12];
	newMatrix[10] = m1[ 8] * m2[ 2] + m1[ 9] * m2[ 6] + m1[10] * m2[10] + m1[11] * m2[14];
	newMatrix[11] = m1[ 8] * m2[ 3] + m1[ 9] * m2[ 7] + m1[10] * m2[11] + m1[11] * m2[14];

	newMatrix[12] = m1[12] * m2[ 0] + m1[13] * m2[ 4] + m1[14] * m2[ 8] + m1[15] * m2[12];
	newMatrix[13] = m1[12] * m2[ 1] + m1[13] * m2[ 5] + m1[14] * m2[ 9] + m1[15] * m2[13];
	newMatrix[14] = m1[12] * m2[ 2] + m1[13] * m2[ 6] + m1[14] * m2[10] + m1[15] * m2[14];
	newMatrix[15] = m1[12] * m2[ 3] + m1[13] * m2[ 7] + m1[14] * m2[11] + m1[15] * m2[15];

	for (signed int i = 0; i < 16; ++i)
		M[i] = newMatrix[i];

	return *this;
}

inline matrix4 matrix4::operator*(const matrix4& other) const
{
	matrix4 tmtrx;
	const float *m1 = M, *m2 = other.M;
	float *m3 = tmtrx.M;

	m3[ 0] = m1[ 0] * m2[ 0] + m1[ 1] * m2[ 4] + m1[ 2] * m2[ 8] + m1[3] * m2[12];
	m3[ 1] = m1[ 0] * m2[ 1] + m1[ 1] * m2[ 5] + m1[ 2] * m2[ 9] + m1[3] * m2[13];
	m3[ 2] = m1[ 0] * m2[ 2] + m1[ 1] * m2[ 6] + m1[ 2] * m2[10] + m1[3] * m2[14];
	m3[ 3] = m1[ 0] * m2[ 3] + m1[ 1] * m2[ 7] + m1[ 2] * m2[11] + m1[3] * m2[15];

	m3[ 4] = m1[ 4] * m2[ 0] + m1[ 5] * m2[ 4] + m1[ 6] * m2[8] + m1[7] * m2[12];
	m3[ 5] = m1[ 4] * m2[ 1] + m1[ 5] * m2[ 5] + m1[ 6] * m2[9] + m1[7] * m2[13];
	m3[ 6] = m1[ 4] * m2[ 2] + m1[ 5] * m2[ 6] + m1[ 6] * m2[10] + m1[7] * m2[14];
	m3[ 7] = m1[ 4] * m2[ 3] + m1[ 5] * m2[ 7] + m1[ 6] * m2[11] + m1[7] * m2[15];

	m3[ 8] = m1[ 8] * m2[ 0] + m1[ 9] * m2[ 4] + m1[10] * m2[ 8] + m1[11] * m2[12];
	m3[ 9] = m1[ 8] * m2[ 1] + m1[ 9] * m2[ 5] + m1[10] * m2[ 9] + m1[11] * m2[13];
	m3[10] = m1[ 8] * m2[ 2] + m1[ 9] * m2[ 6] + m1[10] * m2[10] + m1[11] * m2[14];
	m3[11] = m1[ 8] * m2[ 3] + m1[ 9] * m2[ 7] + m1[10] * m2[11] + m1[11] * m2[15];

	m3[12] = m1[12] * m2[ 0] + m1[13] * m2[ 4] + m1[14] * m2[ 8] + m1[15] * m2[12];
	m3[13] = m1[12] * m2[ 1] + m1[13] * m2[ 5] + m1[14] * m2[ 9] + m1[15] * m2[13];
	m3[14] = m1[12] * m2[ 2] + m1[13] * m2[ 6] + m1[14] * m2[10] + m1[15] * m2[14];
	m3[15] = m1[12] * m2[ 3] + m1[13] * m2[ 7] + m1[14] * m2[11] + m1[15] * m2[15];

	return tmtrx;
}

inline matrix4& matrix4::operator+=(const matrix4& other)
{
	M[ 0] = M[ 0] + other.M[ 0];
	M[ 1] = M[ 1] + other.M[ 1];
	M[ 2] = M[ 2] + other.M[ 2];
	M[ 3] = M[ 3] + other.M[ 3];

	M[ 4] = M[ 4] + other.M[ 4];
	M[ 5] = M[ 5] + other.M[ 5];
	M[ 6] = M[ 6] + other.M[ 6];
	M[ 7] = M[ 7] + other.M[ 7];

	M[ 8] = M[ 8] + other.M[ 8];
	M[ 9] = M[ 9] + other.M[ 9];
	M[10] = M[10] + other.M[10];
	M[11] = M[11] + other.M[11];

	M[12] = M[12] + other.M[12];
	M[13] = M[13] + other.M[13];
	M[14] = M[14] + other.M[14];
	M[15] = M[15] + other.M[15];

	return *this;
}

inline matrix4 matrix4::operator+(const matrix4& other) const
{
	matrix4 r;
	
	r.M[ 0] = M[ 0] + other.M[ 0];
	r.M[ 1] = M[ 1] + other.M[ 1];
	r.M[ 2] = M[ 2] + other.M[ 2];
	r.M[ 3] = M[ 3] + other.M[ 3];

	r.M[ 4] = M[ 4] + other.M[ 4];
	r.M[ 5] = M[ 5] + other.M[ 5];
	r.M[ 6] = M[ 6] + other.M[ 6];
	r.M[ 7] = M[ 7] + other.M[ 7];

	r.M[ 8] = M[ 8] + other.M[ 8];
	r.M[ 9] = M[ 9] + other.M[ 9];
	r.M[10] = M[10] + other.M[10];
	r.M[11] = M[11] + other.M[11];

	r.M[12] = M[12] + other.M[12];
	r.M[13] = M[13] + other.M[13];
	r.M[14] = M[14] + other.M[14];
	r.M[15] = M[15] + other.M[15];

	return r;
}

inline matrix4& matrix4::operator-=(const matrix4& other)
{
	M[ 0] = M[ 0] - other.M[ 0];
	M[ 1] = M[ 1] - other.M[ 1];
	M[ 2] = M[ 2] - other.M[ 2];
	M[ 3] = M[ 3] - other.M[ 3];

	M[ 4] = M[ 4] - other.M[ 4];
	M[ 5] = M[ 5] - other.M[ 5];
	M[ 6] = M[ 6] - other.M[ 6];
	M[ 7] = M[ 7] - other.M[ 7];

	M[ 8] = M[ 8] - other.M[ 8];
	M[ 9] = M[ 9] - other.M[ 9];
	M[10] = M[10] - other.M[10];
	M[11] = M[11] - other.M[11];

	M[12] = M[12] - other.M[12];
	M[13] = M[13] - other.M[13];
	M[14] = M[14] - other.M[14];
	M[15] = M[15] - other.M[15];

	return *this;
}

inline matrix4 matrix4::operator-(const matrix4& other) const
{
	matrix4 r;
	
	r.M[ 0] = M[ 0] - other.M[ 0];
	r.M[ 1] = M[ 1] - other.M[ 1];
	r.M[ 2] = M[ 2] - other.M[ 2];
	r.M[ 3] = M[ 3] - other.M[ 3];

	r.M[ 4] = M[ 4] - other.M[ 4];
	r.M[ 5] = M[ 5] - other.M[ 5];
	r.M[ 6] = M[ 6] - other.M[ 6];
	r.M[ 7] = M[ 7] - other.M[ 7];

	r.M[ 8] = M[ 8] - other.M[ 8];
	r.M[ 9] = M[ 9] - other.M[ 9];
	r.M[10] = M[10] - other.M[10];
	r.M[11] = M[11] - other.M[11];

	r.M[12] = M[12] - other.M[12];
	r.M[13] = M[13] - other.M[13];
	r.M[14] = M[14] - other.M[14];
	r.M[15] = M[15] - other.M[15];

	return r;
}

inline matrix4 matrix4::operator- () const
{
	matrix4 r;
	
	r.M[ 0] = -M[ 0];
	r.M[ 1] = -M[ 1];
	r.M[ 2] = -M[ 2];
	r.M[ 3] = -M[ 3];

	r.M[ 4] = -M[ 4];
	r.M[ 5] = -M[ 5];
	r.M[ 6] = -M[ 6];
	r.M[ 7] = -M[ 7];

	r.M[ 8] = -M[ 8];
	r.M[ 9] = -M[ 9];
	r.M[10] = -M[10];
	r.M[11] = -M[11];

	r.M[12] = -M[12];
	r.M[13] = -M[13];
	r.M[14] = -M[14];
	r.M[15] = -M[15];

	return r;
}

void matrix4::set(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
		 float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	M[0] = m00;
	M[1] = m01;
	M[2] = m02;
	M[3] = m03;
	M[4] = m10;
	M[5] = m11;
	M[6] = m12;
	M[7] = m13;
	M[8] = m20;
	M[9] = m21;
	M[10] = m22;
	M[11] = m23;
	M[12] = m30;
	M[13] = m31;
	M[14] = m32;
	M[15] = m33;
}

void matrix4::set(const matrix4& other)
{
	M[ 0] = other.M[ 0];
	M[ 1] = other.M[ 1];
	M[ 2] = other.M[ 2];
	M[ 3] = other.M[ 3];
	M[ 4] = other.M[ 4];
	M[ 5] = other.M[ 5];
	M[ 6] = other.M[ 6];
	M[ 7] = other.M[ 7];
	M[ 8] = other.M[ 8];
	M[ 9] = other.M[ 9];
	M[10] = other.M[10];
	M[11] = other.M[11];
	M[12] = other.M[12];
	M[13] = other.M[13];
	M[14] = other.M[14];
	M[15] = other.M[15];
}

float* matrix4::get()
{
	return &M[0];
}

const float* matrix4::get() const
{
	return &M[0];
}

inline void matrix4::makeIdentity()
{
	M[0] = 1.0f;
	M[1] = 0.0f;
	M[2] = 0.0f;
	M[3] = 0.0f;
	M[4] = 0.0f;
	M[5] = 1.0f;
	M[6] = 0.0f;
	M[7] = 0.0f;
	M[8] = 0.0f;
	M[9] = 0.0f;
	M[10] = 1.0f;
	M[11] = 0.0f;
	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = 0.0f;
	M[15] = 1.0f;
}

inline bool matrix4::isIdentity()
{
	for (signed int i = 0; i < 4; ++i)
	{
		for (signed int j = 0; j < 4; ++j)
		{
			if (j != i)
			{
				if ((*this)(i, j) < -0.0000001f || (*this)(i, j) >  0.0000001f)
					return false;
			}
			else
			{
				if ((*this)(i, j) < 0.9999999f || (*this)(i, j) > 1.0000001f)
					return false;
			}
		}
	}
	return true;
}

inline bool matrix4::hasInverse()
{
	const matrix4 &m = *this;

	float d = (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) * (m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3))	- (m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1)) * (m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3))
		+ (m(0, 0) * m(3, 1) - m(3, 0) * m(0, 1)) * (m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3))	+ (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) * (m(0, 2) * m(3, 3) - m(3, 2) * m(0, 3))
		- (m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1)) * (m(0, 2) * m(2, 3) - m(2, 2) * m(0, 3))	+ (m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1)) * (m(0, 2) * m(1, 3) - m(1, 2) * m(0, 3));

	if (d == 0.f) return false;

	return false;
}

inline matrix4 matrix4::getInverse() const
{
	/// Calculates the inverse of this Matrix
	/// The inverse is calculated using Cramers rule.
	/// If no inverse exists then 'false' is returned.

	matrix4 temp = *this;

	float d = ((*this)(0, 0) * (*this)(1, 1) - (*this)(1, 0) * (*this)(0, 1)) * ((*this)(2, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(2, 3))	- ((*this)(0, 0) * (*this)(2, 1) - (*this)(2, 0) * (*this)(0, 1)) * ((*this)(1, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(1, 3))
			+ ((*this)(0, 0) * (*this)(3, 1) - (*this)(3, 0) * (*this)(0, 1)) * ((*this)(1, 2) * (*this)(2, 3) - (*this)(2, 2) * (*this)(1, 3))	+ ((*this)(1, 0) * (*this)(2, 1) - (*this)(2, 0) * (*this)(1, 1)) * ((*this)(0, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(0, 3))
			- ((*this)(1, 0) * (*this)(3, 1) - (*this)(3, 0) * (*this)(1, 1)) * ((*this)(0, 2) * (*this)(2, 3) - (*this)(2, 2) * (*this)(0, 3))	+ ((*this)(2, 0) * (*this)(3, 1) - (*this)(3, 0) * (*this)(2, 1)) * ((*this)(0, 2) * (*this)(1, 3) - (*this)(1, 2) * (*this)(0, 3));

	if (d != 0.f)
	{
		d = 1.f / d;

		temp(0, 0) = d * ((*this)(1, 1) * ((*this)(2, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(2, 3)) + (*this)(2, 1) * ((*this)(3, 2) * (*this)(1, 3) - (*this)(1, 2) * (*this)(3, 3)) + (*this)(3, 1) * ((*this)(1, 2) * (*this)(2, 3) - (*this)(2, 2) * (*this)(1, 3)));
		temp(1, 0) = d * ((*this)(1, 2) * ((*this)(2, 0) * (*this)(3, 3) - (*this)(3, 0) * (*this)(2, 3)) + (*this)(2, 2) * ((*this)(3, 0) * (*this)(1, 3) - (*this)(1, 0) * (*this)(3, 3)) + (*this)(3, 2) * ((*this)(1, 0) * (*this)(2, 3) - (*this)(2, 0) * (*this)(1, 3)));
		temp(2, 0) = d * ((*this)(1, 3) * ((*this)(2, 0) * (*this)(3, 1) - (*this)(3, 0) * (*this)(2, 1)) + (*this)(2, 3) * ((*this)(3, 0) * (*this)(1, 1) - (*this)(1, 0) * (*this)(3, 1)) + (*this)(3, 3) * ((*this)(1, 0) * (*this)(2, 1) - (*this)(2, 0) * (*this)(1, 1)));
		temp(3, 0) = d * ((*this)(1, 0) * ((*this)(3, 1) * (*this)(2, 2) - (*this)(2, 1) * (*this)(3, 2)) + (*this)(2, 0) * ((*this)(1, 1) * (*this)(3, 2) - (*this)(3, 1) * (*this)(1, 2)) + (*this)(3, 0) * ((*this)(2, 1) * (*this)(1, 2) - (*this)(1, 1) * (*this)(2, 2)));
		temp(0, 1) = d * ((*this)(2, 1) * ((*this)(0, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(0, 3)) + (*this)(3, 1) * ((*this)(2, 2) * (*this)(0, 3) - (*this)(0, 2) * (*this)(2, 3)) + (*this)(0, 1) * ((*this)(3, 2) * (*this)(2, 3) - (*this)(2, 2) * (*this)(3, 3)));
		temp(1, 1) = d * ((*this)(2, 2) * ((*this)(0, 0) * (*this)(3, 3) - (*this)(3, 0) * (*this)(0, 3)) + (*this)(3, 2) * ((*this)(2, 0) * (*this)(0, 3) - (*this)(0, 0) * (*this)(2, 3)) + (*this)(0, 2) * ((*this)(3, 0) * (*this)(2, 3) - (*this)(2, 0) * (*this)(3, 3)));
		temp(2, 1) = d * ((*this)(2, 3) * ((*this)(0, 0) * (*this)(3, 1) - (*this)(3, 0) * (*this)(0, 1)) + (*this)(3, 3) * ((*this)(2, 0) * (*this)(0, 1) - (*this)(0, 0) * (*this)(2, 1)) + (*this)(0, 3) * ((*this)(3, 0) * (*this)(2, 1) - (*this)(2, 0) * (*this)(3, 1)));
		temp(3, 1) = d * ((*this)(2, 0) * ((*this)(3, 1) * (*this)(0, 2) - (*this)(0, 1) * (*this)(3, 2)) + (*this)(3, 0) * ((*this)(0, 1) * (*this)(2, 2) - (*this)(2, 1) * (*this)(0, 2)) + (*this)(0, 0) * ((*this)(2, 1) * (*this)(3, 2) - (*this)(3, 1) * (*this)(2, 2)));
		temp(0, 2) = d * ((*this)(3, 1) * ((*this)(0, 2) * (*this)(1, 3) - (*this)(1, 2) * (*this)(0, 3)) + (*this)(0, 1) * ((*this)(1, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(1, 3)) + (*this)(1, 1) * ((*this)(3, 2) * (*this)(0, 3) - (*this)(0, 2) * (*this)(3, 3)));
		temp(1, 2) = d * ((*this)(3, 2) * ((*this)(0, 0) * (*this)(1, 3) - (*this)(1, 0) * (*this)(0, 3)) + (*this)(0, 2) * ((*this)(1, 0) * (*this)(3, 3) - (*this)(3, 0) * (*this)(1, 3)) + (*this)(1, 2) * ((*this)(3, 0) * (*this)(0, 3) - (*this)(0, 0) * (*this)(3, 3)));
		temp(2, 2) = d * ((*this)(3, 3) * ((*this)(0, 0) * (*this)(1, 1) - (*this)(1, 0) * (*this)(0, 1)) + (*this)(0, 3) * ((*this)(1, 0) * (*this)(3, 1) - (*this)(3, 0) * (*this)(1, 1)) + (*this)(1, 3) * ((*this)(3, 0) * (*this)(0, 1) - (*this)(0, 0) * (*this)(3, 1)));
		temp(3, 2) = d * ((*this)(3, 0) * ((*this)(1, 1) * (*this)(0, 2) - (*this)(0, 1) * (*this)(1, 2)) + (*this)(0, 0) * ((*this)(3, 1) * (*this)(1, 2) - (*this)(1, 1) * (*this)(3, 2)) + (*this)(1, 0) * ((*this)(0, 1) * (*this)(3, 2) - (*this)(3, 1) * (*this)(0, 2)));
		temp(0, 3) = d * ((*this)(0, 1) * ((*this)(2, 2) * (*this)(1, 3) - (*this)(1, 2) * (*this)(2, 3)) + (*this)(1, 1) * ((*this)(0, 2) * (*this)(2, 3) - (*this)(2, 2) * (*this)(0, 3)) + (*this)(2, 1) * ((*this)(1, 2) * (*this)(0, 3) - (*this)(0, 2) * (*this)(1, 3)));
		temp(1, 3) = d * ((*this)(0, 2) * ((*this)(2, 0) * (*this)(1, 3) - (*this)(1, 0) * (*this)(2, 3)) + (*this)(1, 2) * ((*this)(0, 0) * (*this)(2, 3) - (*this)(2, 0) * (*this)(0, 3)) + (*this)(2, 2) * ((*this)(1, 0) * (*this)(0, 3) - (*this)(0, 0) * (*this)(1, 3)));
		temp(2, 3) = d * ((*this)(0, 3) * ((*this)(2, 0) * (*this)(1, 1) - (*this)(1, 0) * (*this)(2, 1)) + (*this)(1, 3) * ((*this)(0, 0) * (*this)(2, 1) - (*this)(2, 0) * (*this)(0, 1)) + (*this)(2, 3) * ((*this)(1, 0) * (*this)(0, 1) - (*this)(0, 0) * (*this)(1, 1)));
		temp(3, 3) = d * ((*this)(0, 0) * ((*this)(1, 1) * (*this)(2, 2) - (*this)(2, 1) * (*this)(1, 2)) + (*this)(1, 0) * ((*this)(2, 1) * (*this)(0, 2) - (*this)(0, 1) * (*this)(2, 2)) + (*this)(2, 0) * ((*this)(0, 1) * (*this)(1, 2) - (*this)(1, 1) * (*this)(0, 2)));
	}

	return temp;
}

inline matrix4 matrix4::getTransposed() const
{
	matrix4 temp;
	
	temp[ 0] = M[ 0];
	temp[ 1] = M[ 4];
	temp[ 2] = M[ 8];
	temp[ 3] = M[12];

	temp[ 4] = M[ 1];
	temp[ 5] = M[ 5];
	temp[ 6] = M[ 9];
	temp[ 7] = M[13];

	temp[ 8] = M[ 2];
	temp[ 9] = M[ 6];
	temp[10] = M[10];
	temp[11] = M[14];

	temp[12] = M[ 3];
	temp[13] = M[ 7];
	temp[14] = M[11];
	temp[15] = M[15];

	return temp;
}

inline void matrix4::setTranslation(const vector3d& translation)
{
	M[ 3] = translation.X;
	M[ 7] = translation.Y;
	M[11] = translation.Z;
}

inline vector3d matrix4::getTranslation() const
{	
	return vector3d(M[ 3], M[ 7], M[11]);
}

inline void matrix4::setInverseTranslation(const vector3d& translation)
{
	M[ 3] = -translation.X;
	M[ 7] = -translation.Y;
	M[11] = -translation.Z;
}

inline void matrix4::setRotationRadians(const vector3d& rotation)
{
	// preserve scale
	vector3d scale = getScale();
	
	float cr = cos(rotation.X);
	float sr = sin(rotation.X);
	float cp = cos(rotation.Y);
	float sp = sin(rotation.Y);
	float cy = cos(rotation.Z);
	float sy = sin(rotation.Z);
	
	M[ 0] = cp * cy;
	M[ 4] = cp * sy;
	M[ 8] = -sp;

	float srsp = sr * sp;
	float crsp = cr * sp;

	M[ 1] = srsp * cy - cr * sy;
	M[ 5] = srsp * sy + cr * cy;
	M[ 9] = sr * cp;

	M[ 2] = crsp * cy + sr * sy;
	M[ 6] = crsp * sy - sr * cy;
	M[10] = cr * cp;

	// preserve scale
	setScale(scale);
}

inline void matrix4::setRotationDegrees(const vector3d& rotation)
{
	setRotationRadians(rotation * DEGTORAD);
}

inline vector3d matrix4::getRotationDegrees() const
{
	const matrix4 &mat = *this;

	float Y = -asin(mat(2, 0));
	float D = Y;
	float C = cos(Y);
	Y *= RADTODEG;

	float rotx, roty, X, Z;

	if (abs(C) > 0.0005f)
	{
		rotx = mat(2, 2) / C;
		roty = mat(2, 1)  / C;
		X = atan2(roty, rotx) * RADTODEG;
		rotx = mat(0, 0) / C;
		roty = mat(1, 0) / C;
		Z = atan2(roty, rotx) * RADTODEG;
	}
	else
	{
		X  = 0.0f;
		rotx = mat(1, 1);
		roty = -mat(0, 1);
		Z  = atan2(roty, rotx) * RADTODEG;
	}

	// fix values that get below zero
	// before it would set (!) values to 360
	// that where above 360:
	if (X < 0.00) X += 360.00f;
	if (Y < 0.00) Y += 360.00f;
	if (Z < 0.00) Z += 360.00f;

	return vector3d((float)X, (float)Y, (float)Z);
}

inline void matrix4::setInverseRotationRadians(const vector3d& rotation)
{
	// preserve scale
	vector3d scale = getScale();
		
	float cr = cos(rotation.X);
	float sr = sin(rotation.X);
	float cp = cos(rotation.Y);
	float sp = sin(rotation.Y);
	float cy = cos(rotation.Z);
	float sy = sin(rotation.Z);

	M[ 0] = cp * cy;
	M[ 1] = cp * sy;
	M[ 2] = -sp;

	float srsp = sr * sp;
	float crsp = cr * sp;

	M[ 4] = srsp * cy - cr * sy;
	M[ 5] = srsp * sy + cr * cy;
	M[ 6] = sr * cp;

	M[ 8] = crsp * cy + sr * sy;
	M[ 9] = crsp * sy - sr * cy;
	M[10] = cr * cp;

	// preserve scale
	setScale(scale);
}

inline void matrix4::setInverseRotationDegrees(const vector3d& rotation)
{
	setInverseRotationRadians(rotation * DEGTORAD);
}

inline void matrix4::setScale(const vector3d& scale)
{
	M[ 0] *= scale.X;
	M[ 4] *= scale.X;
	M[ 8] *= scale.X;

	M[ 1] *= scale.Y;
	M[ 5] *= scale.Y;
	M[ 9] *= scale.Y;

	M[ 2] *= scale.Z;
	M[ 6] *= scale.Z;
	M[10] *= scale.Z;
}

inline vector3d matrix4::getScale() const
{
	vector3d scale;

	scale.X = vector3d(M[ 0],M[ 4],M[ 8]).getLength();
	scale.Y = vector3d(M[ 1],M[ 5],M[ 9]).getLength();
	scale.Z = vector3d(M[ 2],M[ 6],M[10]).getLength();

	return scale;
}

inline void matrix4::setInverseScale(const vector3d& scale)
{
	M[ 0] /= scale.X;
	M[ 4] /= scale.X;
	M[ 8] /= scale.X;

	M[ 1] /= scale.Y;
	M[ 5] /= scale.Y;
	M[ 9] /= scale.Y;

	M[ 2] /= scale.Z;
	M[ 6] /= scale.Z;
	M[10] /= scale.Z;
}

inline void matrix4::translateVector(vector3d& vect) const
{
	vect.X = vect.X + M[ 3];
	vect.Y = vect.Y + M[ 7];
	vect.Z = vect.Z + M[11];
}

inline void matrix4::inverseTranslateVector(vector3d& vect) const
{
	vect.X = vect.X - M[ 3];
	vect.Y = vect.Y - M[ 7];
	vect.Z = vect.Z - M[11];
}

inline void matrix4::rotateVector(vector3d& vect) const
{
	vector3d tmp = vect;

	vect.X = M[ 0] * tmp.X + M[ 1] * tmp.Y + M[ 2] * tmp.Z + M[ 3];
	vect.Y = M[ 4] * tmp.X + M[ 5] * tmp.Y + M[ 6] * tmp.Z + M[ 7];
	vect.Z = M[ 8] * tmp.X + M[ 9] * tmp.Y + M[10] * tmp.Z + M[11];
}

inline void matrix4::inverseRotateVect(vector3d& vect) const
{
	vector3d tmp = vect;

	vect.X = M[ 0] * tmp.X + M[ 4] * tmp.Y + M[ 8] * tmp.Z + M[12];
	vect.Y = M[ 1] * tmp.X + M[ 5] * tmp.Y + M[ 9] * tmp.Z + M[13];
	vect.Z = M[ 2] * tmp.X + M[ 6] * tmp.Y + M[10] * tmp.Z + M[11];
}

inline void matrix4::transformVector(vector3d& vect) const
{
	float vector[3];

	vector[0] = M[ 0] * vect.X + M[ 1] * vect.Y + M[ 2] * vect.Z + M[ 3];
	vector[1] = M[ 4] * vect.X + M[ 5] * vect.Y + M[ 6] * vect.Z + M[ 7];
	vector[2] = M[ 8] * vect.X + M[ 9] * vect.Y + M[10] * vect.Z + M[11];

	vect.X = vector[0];
	vect.Y = vector[1];
	vect.Z = vector[2];
}

inline void matrix4::transformVector(const vector3d& in, vector3d& out) const
{	
	out.X = M[ 0] * in.X + M[ 1] * in.Y + M[ 2] * in.Z + M[ 3];
	out.Y = M[ 4] * in.X + M[ 5] * in.Y + M[ 6] * in.Z + M[ 7];
	out.Z = M[ 8] * in.X + M[ 9] * in.Y + M[10] * in.Z + M[11];
}

inline void matrix4::transformVector(vector4d& vect) const
{
	float vector[4];

	vector[0] = M[ 0] * vect.X + M[ 1] * vect.Y + M[ 2] * vect.Z + M[ 3] * vect.W;
	vector[1] = M[ 4] * vect.X + M[ 5] * vect.Y + M[ 6] * vect.Z + M[ 7] * vect.W;
	vector[2] = M[ 8] * vect.X + M[ 9] * vect.Y + M[10] * vect.Z + M[11] * vect.W;
	vector[3] = M[12] * vect.X + M[13] * vect.Y + M[14] * vect.Z + M[15] * vect.W;

	vect.X = vector[0];
	vect.Y = vector[1];
	vect.Z = vector[2];
	vect.W = vector[3];
}

inline void matrix4::transformVector(const vector4d& in, vector4d& out) const
{	
	out.X = M[ 0] * in.X + M[ 1] * in.Y + M[ 2] * in.Z + M[ 3] * in.W;
	out.Y = M[ 4] * in.X + M[ 5] * in.Y + M[ 6] * in.Z + M[ 7] * in.W;
	out.Z = M[ 8] * in.X + M[ 9] * in.Y + M[10] * in.Z + M[11] * in.W;
	out.W = M[12] * in.X + M[13] * in.Y + M[14] * in.Z + M[15] * in.W;
}

inline void matrix4::transformPlane(plane3d &plane) const
{
	vector3d member;
	transformVector(plane.getMemberPoint(), member);

	vector3d origin(0, 0, 0);
	transformVector(plane.Normal);
	transformVector(origin);

	plane.Normal -= origin;
	plane.D = - member.dotProduct(plane.Normal);
}

inline void matrix4::transformPlane(const plane3d &in, plane3d &out) const
{
	out = in;
	transformPlane(out);
}

inline void matrix4::transformBox(aabox3d& box) const
{
	core::vector3d edges[8];
	box.getEdges(edges);

	unsigned char i;
	for (i = 0; i < 8; ++i)
		transformVector(edges[i]);

	box.reset(edges[0]);

	for (i = 1; i < 8; ++i)
		box.addInternalPoint(edges[i]);
}

inline void matrix4::buildProjectionMatrixInfinitePerspectiveFov(float fieldOfViewRadians, float aspectRatio, float zNear)
{
	float h = (float)tan(fieldOfViewRadians / 2);
	float w = h * aspectRatio;
	
	M[ 0] = 2.0f * zNear / w;
	M[ 1] = 0.0f;
	M[ 2] = 0.0f;
	M[ 3] = 0.0f;

	M[ 4] = 0.0f;
	M[ 5] = 2.0f * zNear / h;
	M[ 6] = 0.0f;
	M[ 7] = 0.0f;

	M[ 8] = 0.0f;
	M[ 9] = 0.0f;
	M[10] = -1.0f;
	M[11] = -2.0f * zNear;

	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = -1.0f;
	M[15] = 0.0f;
}

inline void matrix4::buildProjectionMatrixPerspectiveFov(float fieldOfViewRadians, float aspectRatio, float zNear, float zFar)
{
	float h = (float)tan(fieldOfViewRadians / 2);
	float w = h * aspectRatio;
	
	M[ 0] = 2.0f * zNear / w;
	M[ 1] = 0.0f;
	M[ 2] = 0.0f;
	M[ 3] = 0.0f;

	M[ 4] = 0.0f;
	M[ 5] = 2.0f * zNear / h;
	M[ 6] = 0.0f;
	M[ 7] = 0.0f;

	M[ 8] = 0.0f;
	M[ 9] = 0.0f;
	M[10] = -(zFar + zNear) / (zFar - zNear);
	M[11] = -2.0f * zNear * zFar / (zFar - zNear);

	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = -1.0f;
	M[15] = 0.0f;
}

inline void matrix4::buildProjectionMatrixPerspective(float widthOfViewVolume, float heightOfViewVolume, float zNear, float zFar)
{	
	M[ 0] = 2.0f * zNear / widthOfViewVolume;
	M[ 1] = 0.0f;
	M[ 2] = 0.0f;
	M[ 3] = 0.0f;

	M[ 4] = 0.0f;
	M[ 5] = 2.0f * zNear / heightOfViewVolume;
	M[ 6] = 0.0f;
	M[ 7] = 0.0f;

	M[ 8] = 0.0f;
	M[ 9] = 0.0f;
	M[10] = -(zFar + zNear) / (zFar - zNear);
	M[11] = -2.0f * zNear * zFar / (zFar - zNear);

	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = -1.0f;
	M[15] = 0.0f;
}

inline void matrix4::buildProjectionMatrixOrtho(float widthOfViewVolume, float heightOfViewVolume, float zNear, float zFar)
{	
	M[ 0] = 2.0f / widthOfViewVolume;
	M[ 1] = 0.0f;
	M[ 2] = 0.0f;
	M[ 3] = 0.0f;

	M[ 4] = 0.0f;
	M[ 5] = 2.0f / heightOfViewVolume;
	M[ 6] = 0.0f;
	M[ 7] = 0.0f;

	M[ 8] = 0.0f;
	M[ 9] = 0.0f;
	M[10] = -2.0f / (zFar - zNear);
	M[11] = (zFar + zNear) / (zFar - zNear);

	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = 0.0f;
	M[15] = 1.0f;
}

inline void matrix4::buildProjectionMatrixOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{	
	M[ 0] = 2.0f / (right - left);
	M[ 1] = 0.0f;
	M[ 2] = 0.0f;
	M[ 3] = -(right + left) / (right - left);

	M[ 4] = 0.0f;
	M[ 5] = 2.0f / (top - bottom);
	M[ 6] = 0.0f;
	M[ 7] = -(top + bottom) / (top - bottom);

	M[ 8] = 0.0f;
	M[ 9] = 0.0f;
	M[10] = -2.0f / (zFar - zNear);
	M[11] = (zFar + zNear) / (zFar - zNear);

	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = 0.0f;
	M[15] = 1.0f;
}

inline void matrix4::buildViewMatrix(const vector3d& position, const vector3d& target, const vector3d& upVector)
{
	vector3d zaxis = position - target;
	zaxis.normalize();

	vector3d xaxis = upVector.crossProduct(zaxis);
	xaxis.normalize();

	vector3d yaxis = zaxis.crossProduct(xaxis);

	vector3d trans;

	trans.X = -xaxis.dotProduct(position);
	trans.Y = -yaxis.dotProduct(position);
	trans.Z = -zaxis.dotProduct(position);

	M[ 0] = xaxis.X;
	M[ 1] = xaxis.Y;
	M[ 2] = xaxis.Z;
	M[ 3] = trans.X;

	M[ 4] = yaxis.X;
	M[ 5] = yaxis.Y;
	M[ 6] = yaxis.Z;
	M[ 7] = trans.Y;

	M[ 8] = zaxis.X;
	M[ 9] = zaxis.Y;
	M[10] = zaxis.Z;
	M[11] = trans.Z;

	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = 0.0f;
	M[15] = 1.0f;
}

inline void matrix4::buildShadowMatrix(vector3d light, plane3d plane, float point)
{
	plane.Normal.normalize();
	float d = plane.Normal.dotProduct(light);

	(*this)(0,0) = plane.Normal.X * light.X + d;
	(*this)(1,0) = plane.Normal.X * light.Y;
	(*this)(2,0) = plane.Normal.X * light.Z;
	(*this)(3,0) = plane.Normal.X * point;

	(*this)(0,1) = plane.Normal.Y * light.X;
	(*this)(1,1) = plane.Normal.Y * light.Y + d;
	(*this)(2,1) = plane.Normal.Y * light.Z;
	(*this)(3,1) = plane.Normal.Y * point;

	(*this)(0,2) = plane.Normal.Z * light.X;
	(*this)(1,2) = plane.Normal.Z * light.Y;
	(*this)(2,2) = plane.Normal.Z * light.Z + d;
	(*this)(3,2) = plane.Normal.Z * point;

	(*this)(0,3) = plane.D * light.X;
	(*this)(1,3) = plane.D * light.Y;
	(*this)(2,3) = plane.D * light.Z;
	(*this)(3,3) = plane.D * point + d;
}

const matrix4 matrix4::ZERO(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

const matrix4 matrix4::IDENTITY(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

} // end namespace core
