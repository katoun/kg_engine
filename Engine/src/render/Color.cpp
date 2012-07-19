/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

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

#include <render/Color.h>

namespace render
{

Color::Color(float red, float green, float blue, float alpha): R(red), G(green), B(blue), A(alpha) {}

Color::Color(const Color& other): R(other.R), G(other.G), B(other.B), A(other.A) {}

inline Color& Color::operator=(const Color& other)
{
	R = other.R;
	G = other.G;
	B = other.B;
	A = other.A;
	return *this;
}

inline Color Color::operator+(const Color& other) const
{
	return Color(R + other.R, G + other.G, B + other.B, A + other.A);
}

inline Color& Color::operator+=(const Color& other)
{
	R += other.R;
	G += other.G;
	B += other.B;
	A += other.A;

	return *this;
}

inline Color Color::operator-(const Color& other) const
{	
	return Color(R - other.R, G - other.G, B - other.B, A - other.A);
}

inline Color& Color::operator-=(const Color& other)
{
	R -= other.R;
	G -= other.G;
	B -= other.B;
	A -= other.A;

	return *this;
}

inline Color Color::operator*(const Color& other) const
{
	return Color(other.R * R, other.G * G, other.B * B, other.A * A);
}

inline Color Color::operator*(const float v) const
{
	return Color(v * R, v * G, v * B, v * A);
}

inline Color& Color::operator*=(const float v )
{
	R *= v;
	G *= v;
	B *= v;
	A *= v;
	return *this;
}

inline Color Color::operator/(const Color& other) const
{	
	return Color(other.R / R, other.G / G, other.B / B, other.A / A);
}

inline Color Color::operator/(const float v) const
{
	assert(v != 0.0f);

	float i = 1.0f / v;
	return Color(R * i, G * i, B * i, A * i);
}

inline Color& Color::operator/=(const float v)
{
	assert(v != 0.0f);

	float i = 1.0f / v;
	R *= i;
	G *= i;
	B *= i;
	A *= i;
	return *this;
}

inline bool Color::operator==(const Color& other) const
{
	return (R == other.R && G == other.G && B == other.B && A == other.A);
}

inline bool Color::operator!=(const Color& other) const
{
	return !(*this == other);
}

float* Color::get()
{
	return &R;
}

const float* Color::get() const
{
	return &R;
}

#if GAME_ENDIAN == ENDIAN_BIG
void Color::setAsABGR(const unsigned int val)
#else
void Color::setAsRGBA(const unsigned int val)
#endif
{
	unsigned int val32 = val;

	// Convert from 32bit pattern
	// (RGBA = 8888)

	// Red
	R = ((val32 >> 24) & 0xFF) / 255.0f;

	// Green
	G = ((val32 >> 16) & 0xFF) / 255.0f;

	// Blue
	B = ((val32 >> 8) & 0xFF) / 255.0f;

	// Alpha
	A = (val32 & 0xFF) / 255.0f;
}

#if GAME_ENDIAN == ENDIAN_BIG
void Color::setAsBGRA(const unsigned int val)
#else
void Color::setAsARGB(const unsigned int val)
#endif
{
	unsigned int val32 = val;

	// Convert from 32bit pattern
	// (ARGB = 8888)

	// Alpha
	A = ((val32 >> 24) & 0xFF) / 255.0f;

	// Red
	R = ((val32 >> 16) & 0xFF) / 255.0f;

	// Green
	G = ((val32 >> 8) & 0xFF) / 255.0f;

	// Blue
	B = (val32 & 0xFF) / 255.0f;
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
	B = ((val32 >> 24) & 0xFF) / 255.0f;

	// Green
	G = ((val32 >> 16) & 0xFF) / 255.0f;

	// Red
	R = ((val32 >> 8) & 0xFF) / 255.0f;

	// Alpha
	A = (val32 & 0xFF) / 255.0f;
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
	A = ((val32 >> 24) & 0xFF) / 255.0f;

	// Blue
	B = ((val32 >> 16) & 0xFF) / 255.0f;

	// Green
	G = ((val32 >> 8) & 0xFF) / 255.0f;

	// Red
	R = (val32 & 0xFF) / 255.0f;
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

	// Red
	val8 = (unsigned char)(R * 255);
	val32 = val8 << 24;

	// Green
	val8 = (unsigned char)(G * 255);
	val32 += val8 << 16;

	// Blue
	val8 = (unsigned char)(B * 255);
	val32 += val8 << 8;

	// Alpha
	val8 = (unsigned char)(A * 255);
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
	val8 = (unsigned char)(A * 255);
	val32 = val8 << 24;

	// Red
	val8 = (unsigned char)(R * 255);
	val32 += val8 << 16;

	// Green
	val8 = (unsigned char)(G * 255);
	val32 += val8 << 8;

	// Blue
	val8 = (unsigned char)(B * 255);
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
	val8 = (unsigned char)(B * 255);
	val32 = val8 << 24;

	// Green
	val8 = (unsigned char)(G * 255);
	val32 += val8 << 16;

	// Red
	val8 = (unsigned char)(R * 255);
	val32 += val8 << 8;

	// Alpha
	val8 = (unsigned char)(A * 255);
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
	val8 = (unsigned char)(A * 255);
	val32 = val8 << 24;

	// Blue
	val8 = (unsigned char)(B * 255);
	val32 += val8 << 16;

	// Green
	val8 = (unsigned char)(G * 255);
	val32 += val8 << 8;

	// Red
	val8 = (unsigned char)(R * 255);
	val32 += val8;

	return val32;
}

const Color Color::ZERO = Color(0.0f, 0.0f, 0.0f, 0.0f);
const Color Color::Black = Color(0.0f, 0.0f, 0.0f);
const Color Color::White = Color(1.0f, 1.0f, 1.0f);
const Color Color::Red = Color(1.0f, 0.0f, 0.0f);
const Color Color::Green = Color(0.0f, 1.0f, 0.0f);
const Color Color::Blue = Color(0.0f, 0.0f, 1.0f);

} // end namespace render
