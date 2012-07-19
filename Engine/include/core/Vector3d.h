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

#ifndef _VECTOR_3D_H_
#define _VECTOR_3D_H_

#include <core/Config.h>

namespace core
{

//! 3d vector class with lots of operators and methods.
class ENGINE_PUBLIC_EXPORT vector3d
{
public:

	vector3d();
	vector3d(float nx, float ny, float nz);
	vector3d(const vector3d& other);

	vector3d operator-() const;

	vector3d& operator=(const vector3d& other);

	vector3d operator+(const vector3d& other) const;
	vector3d& operator+=(const vector3d& other);

	vector3d operator-(const vector3d& other) const;
	vector3d& operator-=(const vector3d& other);

	vector3d operator*(const float scalar) const;
	vector3d& operator*=(const float scalar);

	vector3d operator/(const float scalar) const;
	vector3d& operator/=(const float scalar);

	bool operator==(const vector3d& other) const;
	bool operator!=(const vector3d& other) const;

	void set(const float nx, const float ny, const float nz);
	void set(const vector3d& other);

	float* get();
	const float* get() const;
	
	//! Inverts the vector.
	void invert();

	//! Normalizes the vector.
	vector3d& normalize();

	//! Returns length of the vector.
	float getLength() const;

	//! Returns squared length of the vector.
	//! This is useful because it is much faster then
	//! getLength().
	float getLengthSQ() const;

	//! Sets the length of the vector to a new value
	void setLength(float newLength);

	//! Returns the dot product with another vector.
	float dotProduct(const vector3d& other) const;

	//! Calculates the cross product with another vector
	//! \param p: vector to multiply with.
	//! \return Crossproduct of this vector with p.
	vector3d crossProduct(const vector3d& p) const;

	//! Returns distance from an other point.
	//! Here, the vector is interpreted as point in 3 dimensional space.
	float getDistanceFrom(const vector3d& other) const;

	//! Returns squared distance from an other point.
	//! Here, the vector is interpreted as point in 3 dimensional space.
	float getDistanceFromSQ(const vector3d& other) const;

	//! Returns if this vector interpreted as a point is on a line between two other points.
	//! It is assumed that the point is on the line.
	//! \param begin: Beginning vector to compare between.
	//! \param end: Ending vector to compare between.
	//! \return true if this vector is between begin and end.  false if not.
	bool isBetweenPoints(const vector3d& begin, const vector3d& end) const;

	//! ORIGIN_3D is a const vector3d that defines the 3d origin vector for the 3d space
	static const vector3d ORIGIN_3D;

	//! UNIT_X is a const vector3d that defines the 3d unit vector for the x-axis
	static const vector3d UNIT_X;

	//! UNIT_Y is a const vector3d that defines the 3d unit vector for the y-axis
	static const vector3d UNIT_Y;

	//! UNIT_Z is a const vector3d that defines the 3d unit vector for the z-axis
	static const vector3d UNIT_Z;

	//! NEGATIVE_UNIT_X is a const vector3d that defines the 3d unit vector for the negative x-axis
	static const vector3d NEGATIVE_UNIT_X;

	//! NEGATIVE_UNIT_Y is a const vector3d that defines the 3d unit vector for the negative y-axis
	static const vector3d NEGATIVE_UNIT_Y;

	//! NEGATIVE_UNIT_Z is a const vector3d that defines the 3d unit vector for the negative z-axis
	static const vector3d NEGATIVE_UNIT_Z;

	static const vector3d UNIT_SCALE;

	float X, Y, Z;
};

} // end namespace core

#endif