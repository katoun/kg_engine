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

#include <core/Utils.h>

#include <algorithm>

namespace core
{

void stringTrim(std::string& str, const std::string& whitespace, bool left, bool right)
{
	if (right)
		str.erase(str.find_last_not_of(whitespace)+1); // trim right
	if (left)
		str.erase(0, str.find_first_not_of(whitespace)); // trim left
}

void stringToLower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(),	tolower);
}

void stringToUpper(std::string& str)
{
	std::transform(str.begin(),	str.end(), str.begin(),	toupper);
}

signed int stringToInt(std::string& str)
{
	//If the first sequence of non-whitespace characters in str is not a valid integral number,
	//or if no such sequence exists because either str is empty or it contains only whitespace characters,
	//no conversion is performed.
	if (str.size() != 0)
		return atoi(str.c_str());

	return 0;
}

float stringToFloat(std::string& str)
{
	//If the first sequence of non-whitespace characters in str does not form a valid floating-point number as just defined,
	//or if no such sequence exists because either str is empty or contains only whitespace characters,
	//no conversion is performed.
	if (str.size() != 0)
		return (float)atof(str.c_str());

	return 0.0f;
}

std::string intToString(unsigned int i)
{
	char str[256];
	snprintf(str, 256, "%d", i);
	std::string ret = str;

	return ret;
}

std::string intToString(signed int i)
{
	char str[256];
	snprintf(str, 256, "%d", i);
	std::string ret = str;

	return ret;
}

std::string floatToString(float f)
{
	char str[255];
	snprintf(str, 255, "%f", f);
	std::string ret = str;

	return ret;
}

std::vector<std::string> splitString(const std::string& str, const std::string& delims, unsigned int maxSplits)
{
	std::vector<std::string> ret;
	// Pre-allocate some space for performance
	ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

	unsigned int numSplits = 0;

	// Use STL methods 
	size_t start, pos;
	start = 0;
	do 
	{
		pos = str.find_first_of(delims, start);
		if (pos == start)
		{
			// Do nothing
			start = pos + 1;
		}
		else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
		{
			// Copy the rest of the string
			ret.push_back(str.substr(start));
			break;
		}
		else
		{
			// Copy up to delimiter
			ret.push_back(str.substr(start, pos - start));
			start = pos + 1;
		}
		// parse up to next real data
		start = str.find_first_not_of(delims, start);
		++numSplits;
	} 
	while (pos != std::string::npos);

	return ret;
}

bool powerOfTwo(unsigned int num)
{
	return ((num & (num - 1)) == 0);
}

unsigned int fixedToFixed(unsigned int value, unsigned int n, unsigned int p)
{
	if(n > p)
	{
		// Less bits required than available; this is easy
		value >>= n-p;
	} 
	else if(n < p)
	{
		// More bits required than are there, do the fill
		// Use old fashioned division, probably better than a loop
		if(value == 0)
			value = 0;
		else if(value == (((unsigned int)(1))<<n)-1)
			value = (1<<p)-1;
		else
			value = value*(1<<p)/((1<<n)-1);
	}
	return value;
}

unsigned int floatToFixed(const float value, const unsigned int bits)
{
	if(value <= 0.0f)
		return 0;
	else if (value >= 1.0f)
		return (1<<bits)-1;
	else 
		return (unsigned int)(value * (1<<bits));
}

float fixedToFloat(unsigned int value, unsigned int bits)
{
	return (float)value/(float)((1<<bits)-1);
}

unsigned short int floatToHalf(float i)
{
	union
	{
		float f;
		unsigned int i;
	}v;

	v.f = i;

	register int s =  (v.i >> 16)	& 0x00008000;
	register int e = ((v.i >> 23)	& 0x000000ff) - (127 - 15);
	register int m =   v.i			& 0x007fffff;

	if (e <= 0)
	{
		if (e < -10)
		{
			return 0;
		}
		m = (m | 0x00800000) >> (1 - e);

		return s | (m >> 13);
	}
	else if (e == 0xff - (127 - 15))
	{
		if (m == 0) // Inf
		{
			return s | 0x7c00;
		} 
		else // NAN
		{
			m >>= 13;
			return s | 0x7c00 | m | (m == 0);
		}
	}
	else
	{
		if (e > 30) // Overflow
		{
			return s | 0x7c00;
		}

		return s | (e << 10) | (m >> 13);
	}
}

float halfToFloat(unsigned short int y)
{
	union
	{
		float f;
		unsigned int i;
	}v;

	register int s = (y >> 15)	& 0x00000001;
	register int e = (y >> 10)	& 0x0000001f;
	register int m =  y			& 0x000003ff;

	if (e == 0)
	{
		if (m == 0) // Plus or minus zero
		{
			v.i = s << 31;
			return v.f;
		}
		else // Denormalized number -- renormalize it
		{
			while (!(m & 0x00000400))
			{
				m <<= 1;
				e -=  1;
			}

			e += 1;
			m &= ~0x00000400;
		}
	}
	else if (e == 31)
	{
		if (m == 0) // Inf
		{
			v.i = (s << 31) | 0x7f800000;
			return v.f;
		}
		else // NaN
		{
			v.i = (s << 31) | 0x7f800000 | (m << 13);
			return v.f;
		}
	}

	e = e + (127 - 15);
	m = m << 13;

	v.i = (s << 31) | (e << 23) | m;
	return v.f;
}

} // end namespace core
