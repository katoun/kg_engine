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

#ifndef _VECTOR_4D_H_
#define _VECTOR_4D_H_

#include <EngineConfig.h>

namespace core
{

class vector3d;

//! 4d vector class mostly to handle vertex components with 4 elements
//! just some basic operations defined for now
class ENGINE_PUBLIC_EXPORT vector4d
{
public:

	vector4d();
	vector4d(float x, float y, float z, float w);
	vector4d(const vector4d& other);
	vector4d(const vector3d& other);

	vector4d operator-() const;

	vector4d& operator=(const vector4d& other);

	vector4d operator+(const vector4d& other) const;
	vector4d& operator+=(const vector4d& other);

	vector4d operator-(const vector4d& other) const;
	vector4d& operator-=(const vector4d& other);

	vector4d operator*(const float scalar) const;
	vector4d& operator*=(const float scalar);

	vector4d operator/(const float scalar) const;
	vector4d& operator/=(const float scalar);

	bool operator==(const vector4d& other) const;
	bool operator!=(const vector4d& other) const;

	void set(const float nx, const float ny, const float nz, const float nw);
	void set(const vector4d& other);

	float* get();
	const float* get() const;

	//! Inverts the vector.
	void invert();

	//! Normalizes the vector.
	vector4d& normalize();

	//! Returns length of the vector.
	float getLength() const;

	//! Returns squared length of the vector.
	//! This is useful because it is much faster then
	//! getLength().
	float getLengthSQ() const;

	//! Sets the lenght of the vector to a new value
	void setLength(float newLength);

	//! Returns the dot product with another vector.
	float dotProduct(const vector4d& other) const;

	//! Returns distance from an other point. Here, the vector is interpreted as
	//! point in 3 dimensional space.
	float getDistanceFrom(const vector4d& other) const;

	//! Returns squared distance from an other point.
	//! Here, the vector is interpreted as point in 3 dimensional space.
	float getDistanceFromSQ(const vector4d& other) const;

	float x, y, z, w;
};

} // end namespace core

#endif