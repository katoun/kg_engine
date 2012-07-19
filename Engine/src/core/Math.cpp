/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2012 Catalin Alexandru Nastase

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

#include <core/Math.h>
#include <cmath>

namespace core
{

float min(const float a, const float b)
{
	if (a < b)
		return a;
	else
		return b;
}

float max(const float a, const float b)
{
	if (a > b)
		return a;
	else
		return b;
}

float min(const float a, const float b, const float c)
{
	if (a < b && a < c)
		return a;
	else if (b < a && b < c)
		return b;
	else
		return c;
}

float max(const float a, const float b, const float c)
{	
	if (a > b && a > c)
		return a;
	else if (b > a && b > c)
		return b;
	else
		return c;
}

bool equals(const float a, const float b)
{
	return ((a + EPSILON > b) && (a - EPSILON < b));
}

float abs(float val)
{
	return std::abs(val);
}

float acos(float val)
{
	if (-1.0 < val)
	{
		if (val < 1.0)
			return std::acos(val);
		else
			return 0.0;
	}
	else
	{
		return PI;
	}
}

float asin(float val)
{
	if (-1.0 < val)
	{
		if (val < 1.0)
			return std::asin(val);
		else
			return -HALF_PI;
	}
	else
	{
		return HALF_PI;
	}
}

float atan(float val)
{
	return std::atan(val);
}

float atan2(float valy, float valx)
{
	return std::atan2(valy, valx);
}

float cos(float val)
{
	return std::cos(val);
}

float sin(float val)
{
	return std::sin(val);
}

float tan(float val)
{
	return std::tan(val);
}

float ceil(float val)
{
	return std::ceil(val);
}

float floor(float val)
{
	return std::floor(val);
}

float clamp(float val, float min, float max)
{
	assert(min <= max);

	if(val <= min)
		return min;
	if(val >= max)
		return max;

	return val;
}

float exp(float val)
{
	return std::exp(val);
}

float log(float val)
{
	return std::log(val);
}

float sqr(float val)
{
	return val*val;
}

float sqrt(float val)
{
	return std::sqrt(val);
}

} // end namespace core
