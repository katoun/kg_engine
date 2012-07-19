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

#ifndef _VECTOR_4D_H_
#define _VECTOR_4D_H_

#include <core/Config.h>

namespace core
{

class vector3d;

//! 4d vector class mostly to handle vertex components with 4 elements
//! just some basic operations defined for now
class ENGINE_PUBLIC_EXPORT vector4d
{
public:

	vector4d();
	vector4d(float nx, float ny, float nz, float nw);
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

	float X, Y, Z, W;
};

} // end namespace core

#endif