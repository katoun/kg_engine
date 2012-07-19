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
