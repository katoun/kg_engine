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

#include <render/Color.h>

namespace render
{

Color::Color(): r(1), g(1), b(1), a(1) {}

Color::Color(float red, float green, float blue): r(red), g(green), b(blue), a(1) {}

Color::Color(float red, float green, float blue, float alpha): r(red), g(green), b(blue), a(alpha) {}

Color::Color(const Color& other): r(other.r), g(other.g), b(other.b), a(other.a) {}

inline Color& Color::operator=(const Color& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	a = other.a;
	return *this;
}

inline Color Color::operator+(const Color& other) const
{
	return Color(r + other.r, g + other.g, b + other.b, a + other.a);
}

inline Color& Color::operator+=(const Color& other)
{
	r += other.r;
	g += other.g;
	b += other.b;
	a += other.a;

	return *this;
}

inline Color Color::operator-(const Color& other) const
{	
	return Color(r - other.r, g - other.g, b - other.b, a - other.a);
}

inline Color& Color::operator-=(const Color& other)
{
	r -= other.r;
	g -= other.g;
	b -= other.b;
	a -= other.a;

	return *this;
}

inline Color Color::operator*(const Color& other) const
{
	return Color(other.r * r, other.g * g, other.b * b, other.a * a);
}

inline Color Color::operator*(const float v) const
{
	return Color(v * r, v * g, v * b, v * a);
}

inline Color& Color::operator*=(const float v )
{
	r *= v;
	g *= v;
	b *= v;
	a *= v;
	return *this;
}

inline Color Color::operator/(const Color& other) const
{	
	return Color(other.r / r, other.g / g, other.b / b, other.a / a);
}

inline Color Color::operator/(const float v) const
{
	assert(v != 0.0f);

	float i = 1.0f / v;
	return Color(r * i, g * i, b * i, a * i);
}

inline Color& Color::operator/=(const float v)
{
	assert(v != 0.0f);

	float i = 1.0f / v;
	r *= i;
	g *= i;
	b *= i;
	a *= i;
	return *this;
}

inline bool Color::operator==(const Color& other) const
{
	return (r == other.r && g == other.g && b == other.b && a == other.a);
}

inline bool Color::operator!=(const Color& other) const
{
	return !(*this == other);
}

float* Color::get()
{
	return &r;
}

const float* Color::get() const
{
	return &r;
}

#if GAME_ENDIAN == ENDIAN_BIG
void Color::setAsABGR(const unsigned int val)
#else
void Color::setAsRGBA(const unsigned int val)
#endif
{
	unsigned int val32 = val;

	// Convert from 32bit pattern
	// (rgBA = 8888)

	// red
	r = ((val32 >> 24) & 0xFF) / 255.0f;

	// green
	g = ((val32 >> 16) & 0xFF) / 255.0f;

	// Blue
	b = ((val32 >> 8) & 0xFF) / 255.0f;

	// Alpha
	a = (val32 & 0xFF) / 255.0f;
}

#if GAME_ENDIAN == ENDIAN_BIG
void Color::setAsBGRA(const unsigned int val)
#else
void Color::setAsARGB(const unsigned int val)
#endif
{
	unsigned int val32 = val;

	// Convert from 32bit pattern
	// (ArgB = 8888)

	// Alpha
	a = ((val32 >> 24) & 0xFF) / 255.0f;

	// red
	r = ((val32 >> 16) & 0xFF) / 255.0f;

	// green
	g = ((val32 >> 8) & 0xFF) / 255.0f;

	// Blue
	b = (val32 & 0xFF) / 255.0f;
}

#if GAME_ENDIAN == ENDIAN_BIG
void Color::setAsARGB(const unsigned int val)
#else
void Color::setAsBGRA(const unsigned int val)
#endif
{
	unsigned int val32 = val;

	// Convert from 32bit pattern
	// (ARGB = 8888)

	// Blue
	b = ((val32 >> 24) & 0xFF) / 255.0f;

	// green
	g = ((val32 >> 16) & 0xFF) / 255.0f;

	// red
	r = ((val32 >> 8) & 0xFF) / 255.0f;

	// Alpha
	a = (val32 & 0xFF) / 255.0f;
}

#if GAME_ENDIAN == ENDIAN_BIG
void Color::setAsRGBA(const unsigned int val)
#else
void Color::setAsABGR(const unsigned int val)
#endif
{
	unsigned int val32 = val;

	// Convert from 32bit pattern
	// (ABGR = 8888)

	// Alpha
	a = ((val32 >> 24) & 0xFF) / 255.0f;

	// Blue
	b = ((val32 >> 16) & 0xFF) / 255.0f;

	// green
	g = ((val32 >> 8) & 0xFF) / 255.0f;

	// red
	r = (val32 & 0xFF) / 255.0f;
}

void Color::setHSB(float hue, float saturation, float brightness)
{
	//Not implemented yet!TODO
}

#if GAME_ENDIAN == ENDIAN_BIG
unsigned int Color::getAsABGR() const
#else
unsigned int Color::getAsRGBA() const
#endif
{
	unsigned char val8;
	unsigned int val32 = 0;

	// Convert to 32bit pattern
	// (RGBA = 8888)

	// red
	val8 = (unsigned char)(r * 255);
	val32 = val8 << 24;

	// green
	val8 = (unsigned char)(g * 255);
	val32 += val8 << 16;

	// Blue
	val8 = (unsigned char)(b * 255);
	val32 += val8 << 8;

	// Alpha
	val8 = (unsigned char)(a * 255);
	val32 += val8;

	return val32;
}

#if GAME_ENDIAN == ENDIAN_BIG
unsigned int Color::getAsBGRA() const
#else
unsigned int Color::getAsARGB() const
#endif
{
	unsigned char val8;
	unsigned int val32 = 0;

	// Convert to 32bit pattern
	// (ARGB = 8888)

	// Alpha
	val8 = (unsigned char)(a * 255);
	val32 = val8 << 24;

	// red
	val8 = (unsigned char)(r * 255);
	val32 += val8 << 16;

	// green
	val8 = (unsigned char)(g * 255);
	val32 += val8 << 8;

	// Blue
	val8 = (unsigned char)(b * 255);
	val32 += val8;


	return val32;
}

#if GAME_ENDIAN == ENDIAN_BIG
unsigned int Color::getAsARGB() const
#else
unsigned int Color::getAsBGRA() const
#endif
{
	unsigned char val8;
	unsigned int val32 = 0;

	// Convert to 32bit pattern
	// (ARGB = 8888)

	// Blue
	val8 = (unsigned char)(b * 255);
	val32 = val8 << 24;

	// Green
	val8 = (unsigned char)(g * 255);
	val32 += val8 << 16;

	// red
	val8 = (unsigned char)(r * 255);
	val32 += val8 << 8;

	// Alpha
	val8 = (unsigned char)(a * 255);
	val32 += val8;


	return val32;
}

#if GAME_ENDIAN == ENDIAN_BIG
unsigned int Color::getAsRGBA() const
#else
unsigned int Color::getAsABGR() const
#endif
{
	unsigned char val8;
	unsigned int val32 = 0;

	// Convert to 32bit pattern
	// (ABRG = 8888)

	// Alpha
	val8 = (unsigned char)(a * 255);
	val32 = val8 << 24;

	// Blue
	val8 = (unsigned char)(b * 255);
	val32 += val8 << 16;

	// Green
	val8 = (unsigned char)(g * 255);
	val32 += val8 << 8;

	// red
	val8 = (unsigned char)(r * 255);
	val32 += val8;

	return val32;
}

const Color Color::ZERO		= Color(0, 0, 0, 0);
const Color Color::Black	= Color(0, 0, 0);
const Color Color::White	= Color(1, 1, 1);
const Color Color::Red		= Color(1, 0, 0);
const Color Color::Green	= Color(0, 1, 0);
const Color Color::Blue		= Color(0, 0, 1);

} // end namespace render
