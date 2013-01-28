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

#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include <Config.h>

namespace core
{

class vector3d;
class vector4d;
class quaternion;
class plane3d;
class aabox3d;

//! 4x4 matrix. Mostly used as transformation matrix for 3d calculations.
//! Matrix4 is mainly used by the K_Game engine for doing transformations.
//! The matrix is a OpenGL style matrix, row major with translations in the 4th row.
//! The generic form M * V which shows the layout of the matrix entries is shown below:

//!     [ m[ 0]  m[ 1]  m[ 2]  m[ 3] ]   {x}
//!     | m[ 4]  m[ 5]  m[ 6]  m[ 7] | * {y}
//!     | m[ 8]  m[ 9]  m[10]  m[11] |   {z}
//!     [ m[12]  m[13]  m[14]  m[15] ]   {1}
class ENGINE_PUBLIC_EXPORT matrix4
{
public:

	matrix4();

	matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);

	//! Sets this matrix equal to the other matrix.
	matrix4& operator=(const matrix4 &other);
	
	//! Simple operator for directly accessing every element of the matrix.
	float& operator()(int row, int col);

	//! Simple operator for directly accessing every element of the matrix.
	const float& operator()(int row, int col) const;

	//! Simple operator for linearly accessing every element of the matrix.
	float& operator[](unsigned int index);

	//! Simple operator for linearly accessing every element of the matrix.
	const float& operator[](unsigned int index) const;

	//! Returns true if other matrix is equal to this matrix.
	bool operator==(const matrix4 &other) const;

	//! Returns true if other matrix is not equal to this matrix.
	bool operator!=(const matrix4 &other) const;

	//! Multiply by another matrix.
	matrix4& operator*=(const matrix4& other);

	//! Multiply by another matrix.
	matrix4 operator*(const matrix4& other) const;

	//! Add by another matrix.
	matrix4& operator+=(const matrix4& other);

	//! Add by another matrix.
	matrix4 operator+(const matrix4& other) const;

	//! Subtract by another matrix.
	matrix4& operator-=(const matrix4& other);

	//! Subtract by another matrix.
	matrix4 operator-(const matrix4& other) const;

	matrix4 operator- () const;

	void set(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
	void set(const matrix4& other);

	float* get();
	const float* get() const;

	//! Set matrix to identity.
	void makeIdentity();

	//! Returns true if the matrix is the identity matrix
	bool isIdentity();

	//! Returns if the matrix has an inverse.
	bool hasInverse();

	//! Returns the inversed matrix is exists else it returns the current matrix
	matrix4 getInverse() const;

	//! Returns transposed matrix
	matrix4 getTransposed() const;

	//! Set the translation of the current matrix. Will erase any previous values.
	void setTranslation(const vector3d& translation);

	//! Gets the current translation
	vector3d getTranslation() const;

	//! Set the inverse translation of the current matrix. Will erase any previous values.
	void setInverseTranslation(const vector3d& translation);

	//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
	void setRotationRadians(const vector3d& rotation);

	//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
	void setRotationDegrees(const vector3d& rotation);

	//! Returns the rotation, as set by setRotation(). This code was originally written by Chev.
	vector3d getRotationDegrees() const;

	//! Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified.
	void setInverseRotationRadians(const vector3d& rotation);

	//! Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified.
	void setInverseRotationDegrees(const vector3d& rotation);

	//! Set Scale
	void setScale(const vector3d& scale);

	//! Get Scale
	vector3d getScale() const;

	//! Set the inverse scale of the current matrix. The 4th row and column are unmodified.
	void setInverseScale(const vector3d& scale);

	//! Translate a vector by the translation part of this matrix.
	void translateVector(vector3d& vect) const;

	//! Translate a vector by the inverse of the translation part of this matrix.
	void inverseTranslateVector(vector3d& vect) const;

	//! Rotate a vector by the rotation part of this matrix.
	void rotateVector(vector3d& vect) const;

	//! Rotate a vector by the inverse of the rotation part of this matrix.
	void inverseRotateVect(vector3d& vect) const;

	//! Transforms the vector by this matrix
	void transformVector(vector3d& vect) const;

	//! Transforms input vector by this matrix and stores result in output vector
	void transformVector(const vector3d& in, vector3d& out) const;

	//! Transforms the vector by this matrix
	void transformVector(vector4d& vect) const;

	//! Transforms input vector by this matrix and stores result in output vector
	void transformVector(const vector4d& in, vector4d& out) const;

	//! Transforms a plane by this matrix
	void transformPlane(plane3d &plane) const;

	//! Transforms a plane by this matrix
	void transformPlane(const plane3d &in, plane3d &out) const;

	//! Transforms a axis aligned bounding box
	void transformBox(aabox3d& box) const;

	//! Builds a right-handed infinite perspective projection matrix based on a field of view
	void buildProjectionMatrixInfinitePerspectiveFov(float fieldOfViewRadians, float aspectRatio, float zNear);
	
	//! Builds a right-handed perspective projection matrix based on a field of view
	void buildProjectionMatrixPerspectiveFov(float fieldOfViewRadians, float aspectRatio, float zNear, float zFar);
	
	//! Builds a right-handed perspective projection matrix.
	void buildProjectionMatrixPerspective(float widthOfViewVolume, float heightOfViewVolume, float zNear, float zFar);

	//! Builds a right-handed orthogonal projection matrix.
	void buildProjectionMatrixOrtho(float widthOfViewVolume, float heightOfViewVolume, float zNear, float zFar);

	//! Builds a right-handed orthogonal projection matrix with left/right and bottom/top instead of only width and height
	void buildProjectionMatrixOrtho(float left, float right, float bottom, float top, float zNear, float zFar);

	//! Builds a right-handed look-at matrix.
	void buildViewMatrix(const vector3d& position, const vector3d& target, const vector3d& upVector);

	//! Builds a right-handed world matrix.
	void buildWorldMatrix(const vector3d& position, const vector3d& scale, const quaternion& orientation);

	//! Builds a matrix that flattens geometry into a plane.
	//! \param light: light source
	//! \param plane: plane into which the geometry if flattened into
	//! \param point: value between 0 and 1, describing the light source.
	//! If this is 1, it is a point light, if it is 0, it is a directional light.
	void buildShadowMatrix(vector3d light, plane3d plane, float point = 1.0f);

	static const matrix4 ZERO;

	static const matrix4 IDENTITY;

private:

	static float empty;

	//! Matrix data, stored in row-major order
	float M[16];
};

} // end namespace core

#endif