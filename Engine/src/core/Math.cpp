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
