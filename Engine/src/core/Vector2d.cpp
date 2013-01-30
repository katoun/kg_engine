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

vector2d::vector2d() : x(0), y(0) {}

vector2d::vector2d(float nx, float ny) : x(nx), y(ny) {}

vector2d::vector2d(const vector2d& other) : x(other.x), y(other.y) {}

vector2d vector2d::operator-() const
{
	return vector2d(-x, -y);
}

vector2d& vector2d::operator=(const vector2d& other)
{
	x = other.x;
	y = other.y;
	return *this;
}

vector2d vector2d::operator+(const vector2d& other) const
{
	return vector2d(x + other.x, y + other.y);
}

vector2d& vector2d::operator+=(const vector2d& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

vector2d vector2d::operator-(const vector2d& other) const
{
	return vector2d(x - other.x, y - other.y);
}

vector2d& vector2d::operator-=(const vector2d& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

vector2d vector2d::operator*(const float scalar) const
{
	return vector2d(x * scalar, y * scalar);
}

vector2d& vector2d::operator*=(const float v)
{
	x *= v;
	y *= v;
	return *this;
}

vector2d vector2d::operator/(const float scalar) const
{
	return vector2d(x / scalar, y / scalar);
}

vector2d& vector2d::operator/=(const float scalar)
{
	x /= scalar;
	y /= scalar;
	return *this;
}

bool vector2d::operator==(const vector2d& other) const
{
	return ((other.x + EPSILON > x) && (other.x - EPSILON < x) &&
			(other.y + EPSILON > y) && (other.y - EPSILON < y));
}

bool vector2d::operator!=(const vector2d& other) const
{
	return ((other.x + EPSILON < x) || (other.x - EPSILON > x) ||
			(other.y + EPSILON < y) || (other.y - EPSILON > y));
}

void vector2d::invert()
{
	x = -x;
	y = -y;
}

vector2d& vector2d::normalize()
{
	float ln = (float)getLength();
	if (ln > EPSILON)
	{
		ln = (float)1.0 / ln;
		x *= ln;
		y *= ln;
	}
	return *this;
}

void vector2d::set(const float& nx, const float& ny)
{
	x = nx;
	y = ny;
}

void vector2d::set(const vector2d& other)
{
	x = other.x;
	y = other.y;
}

float* vector2d::get()
{
	return &x;
}

const float* vector2d::get() const
{
	return &x;
}

float vector2d::getLength() const
{
	return sqrt(x*x + y*y);
}

float vector2d::getLengthSQ() const
{
	return x*x + y*y;
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
		x *= ln;
		y *= ln;
	}
}

float vector2d::dotProduct(const vector2d& other) const
{
	return x*other.x + y*other.y;
}

float vector2d::getDistanceFrom(const vector2d& other) const
{
	float vx = x - other.x;
	float vy = y - other.y;
	return sqrt(vx*vx + vy*vy);
}

float vector2d::getDistanceFromSQ(const vector2d& other) const
{
	float vx = x - other.x;
	float vy = y - other.y;
	return (vx*vx + vy*vy);
}

void vector2d::rotateBy(float degrees, const vector2d& center)
{
	degrees *= DEGTORAD;
	float cs = cos(degrees);
	float sn = sin(degrees);

	x -= center.x;
	y -= center.y;

	set(x*cs - y*sn, x*sn + y*cs);

	x += center.x;
	y += center.y;
}

float vector2d::getAngleTrig() const
{
	if (x == 0.0f)
		return y < 0.0f ? 270.0f : 90.0f;
	else
		if (y == 0.0f)
			return x < 0.0f ? 180.0f : 0.0f;

	if (y > 0.0f)
		if (x > 0.0f)
			return atan(y / x) * RADTODEG;
		else
			return 180.0f -atan(y / -x) * RADTODEG;
	else
		if (x > 0.0f)
			return 360.0f -atan(-y / x) * RADTODEG;
		else
			return 180.0f + atan(-y / -x) * RADTODEG;
}

float vector2d::getAngle() const
{
	if (y == 0.0f)  // corrected thanks to a suggestion by Jox
		return x < 0.0f ? 180.0f : 0.0f;
	else if (x == 0.0f)
		return y < 0.0f ? 90.0f : 270.0f;

	float tmp = y / getLength();
	tmp = atan(sqrt(1 - tmp * tmp) / tmp) * RADTODEG;

	if (x > 0.0f && y > 0.0f)
		return tmp + 270.0f;
	else if (x > 0.0f && y < 0.0f)
		return tmp + 90.0f;
	else if (x < 0.0f && y < 0.0f)
		return 90.0f - tmp;
	else if (x < 0.0f && y > 0.0f)
		return 270.0f - tmp;

	return tmp;
}

float vector2d::getAngleWith(const vector2d& b) const
{
	float tmp = x * b.x + y * b.y;

	if (tmp == 0.0f)
		return 90.0f;

	tmp = tmp / sqrt((x * x + y * y) * (b.x * b.x + b.y * b.y));
	if (tmp < 0.0f)
		tmp = -tmp;

	tmp = atan(sqrt(1 - tmp*tmp) / tmp) * RADTODEG;

	return tmp;
}

const vector2d vector2d::ORIGIN_2D = vector2d(0.0f, 0.0f);

} // end namespace core
