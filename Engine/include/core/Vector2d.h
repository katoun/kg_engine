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

#ifndef _VECTOR_2D_H_
#define _VECTOR_2D_H_

#include <core/Config.h>

namespace core
{

//! 2d vector class with lots of operators and methods.
class ENGINE_PUBLIC_EXPORT vector2d
{
public:

	vector2d();
	vector2d(float nx, float ny);
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

	float X, Y;
};

} // end namespace core

#endif