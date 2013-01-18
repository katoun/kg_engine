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
