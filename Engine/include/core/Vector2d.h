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

#ifndef _VECTOR_2D_H_
#define _VECTOR_2D_H_

#include <EngineConfig.h>

namespace core
{

//! 2d vector class with lots of operators and methods.
class ENGINE_PUBLIC_EXPORT vector2d
{
public:

	vector2d();
	vector2d(float x, float y);
	vector2d(const vector2d& other);

	vector2d operator-() const;

	vector2d& operator=(const vector2d& other);

	vector2d operator+(const vector2d& other) const;
	vector2d& operator+=(const vector2d& other);

	vector2d operator-(const vector2d& other) const;
	vector2d& operator-=(const vector2d& other);

	vector2d operator*(const float scalar) const;
	vector2d& operator*=(const float scalar);

	vector2d operator/(const float scalar) const;
	vector2d& operator/=(const float scalar);

	bool operator==(const vector2d& other) const;
	bool operator!=(const vector2d& other) const;

	//! Inverts the vector.
	void invert();

	//! normalizes the vector.
	vector2d& normalize();

	void set(const float& nx, const float& ny);
	void set(const vector2d& other);

	float* get();
	const float* get() const;

	//! Returns the length of the vector
	float getLength() const;

	//! Returns squared length of the vector.
	//! This is useful because it is much faster then
	//! getLength().
	float getLengthSQ() const;

	//! Sets the length of the vector to a new value
	void setLength(float newLength);

	//! Returns the dot product of this vector with an other.
	float dotProduct(const vector2d& other) const;

	//! Returns distance from an other point. Here, the vector is interpreted as
	//! point in 2 dimensional space.
	float getDistanceFrom(const vector2d& other) const;

	//! Returns squared distance from an other point.
	//! Here, the vector is interpreted as point in 2 dimensional space.
	float getDistanceFromSQ(const vector2d& other) const;

	//! rotates the point around a center by an amount of degrees.
	void rotateBy(float degrees, const vector2d& center);

	//! Calculates the angle of this vector in grad in the trigonometric sense.
	//! This method has been suggested by Pr3t3nd3r.
	//! \return Returns a value between 0 and 360.
	float getAngleTrig() const;

	//! Calculates the angle of this vector in grad in the counter trigonometric sense.
	//! \return Returns a value between 0 and 360.
	float getAngle() const;

	//! Calculates the angle between this vector and another one in grad.
	//! \return Returns a value between 0 and 90.
	float getAngleWith(const vector2d& b) const;

	//! ORIGIN_2D is a const vector2d that defines the 2d origin vector for the 2d space
	static const vector2d ORIGIN_2D;

	float x, y;
};

} // end namespace core

#endif