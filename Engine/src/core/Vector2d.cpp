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

#include <core/Vector2d.h>
#include <core/Math.h>

namespace core
{

vector2d::vector2d() : X(0), Y(0) {}

vector2d::vector2d(float nx, float ny) : X(nx), Y(ny) {}

vector2d::vector2d(const vector2d& other) : X(other.X), Y(other.Y) {}

vector2d vector2d::operator-() const
{
	return vector2d(-X, -Y);
}

vector2d& vector2d::operator=(const vector2d& other)
{
	X = other.X;
	Y = other.Y;
	return *this;
}

vector2d vector2d::operator+(const vector2d& other) const
{
	return vector2d(X + other.X, Y + other.Y);
}

vector2d& vector2d::operator+=(const vector2d& other)
{
	X += other.X;
	Y += other.Y;
	return *this;
}

vector2d vector2d::operator-(const vector2d& other) const
{
	return vector2d(X - other.X, Y - other.Y);
}

vector2d& vector2d::operator-=(const vector2d& other)
{
	X -= other.X;
	Y -= other.Y;
	return *this;
}

vector2d vector2d::operator*(const float scalar) const
{
	return vector2d(X * scalar, Y * scalar);
}

vector2d& vector2d::operator*=(const float v)
{
	X *= v;
	Y *= v;
	return *this;
}

vector2d vector2d::operator/(const float scalar) const
{
	return vector2d(X / scalar, Y / scalar);
}

vector2d& vector2d::operator/=(const float scalar)
{
	X /= scalar;
	Y /= scalar;
	return *this;
}

bool vector2d::operator==(const vector2d& other) const
{
	return ((other.X + EPSILON > X) && (other.X - EPSILON < X) &&
			(other.Y + EPSILON > Y) && (other.Y - EPSILON < Y));
}

bool vector2d::operator!=(const vector2d& other) const
{
	return ((other.X + EPSILON < X) || (other.X - EPSILON > X) ||
			(other.Y + EPSILON < Y) || (other.Y - EPSILON > Y));
}

void vector2d::invert()
{
	X = -X;
	Y = -Y;
}

vector2d& vector2d::normalize()
{
	float ln = (float)getLength();
	if (ln > EPSILON)
	{
		ln = (float)1.0 / ln;
		X *= ln;
		Y *= ln;
	}
	return *this;
}

void vector2d::set(const float& nx, const float& ny)
{
	X = nx;
	Y = ny;
}

void vector2d::set(const vector2d& other)
{
	X = other.X;
	Y = other.Y;
}

float* vector2d::get()
{
	return &X;
}

const float* vector2d::get() const
{
	return &X;
}

float vector2d::getLength() const
{
	return sqrt(X*X + Y*Y);
}

float vector2d::getLengthSQ() const
{
	return X*X + Y*Y;
}

void vector2d::setLength(float newLength)
{
	//normalize();
	//*this *= newlength;

	// patch by calimero, changes by niko, combined by mm
	float ln = getLength();
	if (ln > EPSILON)
	{
		ln = newLength / ln;
		X *= ln;
		Y *= ln;
	}
}

float vector2d::dotProduct(const vector2d& other) const
{
	return X*other.X + Y*other.Y;
}

float vector2d::getDistanceFrom(const vector2d& other) const
{
	float vx = X - other.X;
	float vy = Y - other.Y;
	return sqrt(vx*vx + vy*vy);
}

float vector2d::getDistanceFromSQ(const vector2d& other) const
{
	float vx = X - other.X;
	float vy = Y - other.Y;
	return (vx*vx + vy*vy);
}

void vector2d::rotateBy(float degrees, const vector2d& center)
{
	degrees *= DEGTORAD;
	float cs = cos(degrees);
	float sn = sin(degrees);

	X -= center.X;
	Y -= center.Y;

	set(X*cs - Y*sn, X*sn + Y*cs);

	X += center.X;
	Y += center.Y;
}

float vector2d::getAngleTrig() const
{
	if (X == 0.0f)
		return Y < 0.0f ? 270.0f : 90.0f;
	else
		if (Y == 0.0f)
			return X < 0.0f ? 180.0f : 0.0f;

	if (Y > 0.0f)
		if (X > 0.0f)
			return atan(Y / X) * RADTODEG;
		else
			return 180.0f -atan(Y / -X) * RADTODEG;
	else
		if (X > 0.0f)
			return 360.0f -atan(-Y / X) * RADTODEG;
		else
			return 180.0f + atan(-Y / -X) * RADTODEG;
}

float vector2d::getAngle() const
{
	if (Y == 0.0f)  // corrected thanks to a suggestion by Jox
		return X < 0.0f ? 180.0f : 0.0f;
	else if (X == 0.0f)
		return Y < 0.0f ? 90.0f : 270.0f;

	float tmp = Y / getLength();
	tmp = atan(sqrt(1 - tmp * tmp) / tmp) * RADTODEG;

	if (X > 0.0f && Y > 0.0f)
		return tmp + 270.0f;
	else if (X > 0.0f && Y < 0.0f)
		return tmp + 90.0f;
	else if (X < 0.0f && Y < 0.0f)
		return 90.0f - tmp;
	else if (X < 0.0f && Y > 0.0f)
		return 270.0f - tmp;

	return tmp;
}

float vector2d::getAngleWith(const vector2d& b) const
{
	float tmp = X * b.X + Y * b.Y;

	if (tmp == 0.0f)
		return 90.0f;

	tmp = tmp / sqrt((X * X + Y * Y) * (b.X * b.X + b.Y * b.Y));
	if (tmp < 0.0f)
		tmp = -tmp;

	tmp = atan(sqrt(1 - tmp*tmp) / tmp) * RADTODEG;

	return tmp;
}

const vector2d vector2d::ORIGIN_2D = vector2d(0.0f, 0.0f);

} // end namespace core
