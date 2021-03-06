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

#ifndef _COLOR_H_
#define _COLOR_H_

#include <EngineConfig.h>

namespace render
{
//! Class representing color.

//! Color is represented as 4 components, each of which is a
//! floating-point value from 0.0 to 1.0.
//! \par:
//! The 3 'normal' color components are red, green and blue, a higher
//! number indicating greater amounts of that component in the color.
//! The forth component is the 'alpha' value, which represents
//! transparency. In this case, 0.0 is completely transparent and 1.0 is
//! fully opaque.
//! \todo: Add more formats e.g. 16-bit 5551 etc.
class ENGINE_PUBLIC_EXPORT Color
{
public:
	Color();
	Color(float red, float green, float blue);
	Color(float red, float green, float blue, float alpha);
	Color(const Color& other);

	inline Color& operator=(const Color& other);

	// arithmetic operations
	inline Color operator+(const Color& other) const;
	inline Color& operator+=(const Color& other);

	inline Color operator-(const Color& other) const;
	inline Color& operator-=(const Color& other);

	inline Color operator*(const Color& other) const;
	inline Color operator*(const float v) const;
	inline Color& operator*=(const float v);

	inline Color operator/(const Color& other) const;
	inline Color operator/(const float v) const;
	inline Color& operator/=(const float v);

	inline bool operator==(const Color& other) const;
	inline bool operator!=(const Color& other) const;

	float* get();
	const float* get() const;

	//! Sets color as RGBA.
	void setAsRGBA(const unsigned int val);

	//! Sets color as ARGB.
	void setAsARGB(const unsigned int val);

	//! Sets color as BGRA.
	void setAsBGRA(const unsigned int val);

	//! Sets color as ABGR.
	void setAsABGR(const unsigned int val);

	//! Set a color value from Hue, Saturation and Brightness.
	//! \param hue: Hue value, scaled to the [0,1] range as opposed to the 0-360
	//! \param saturation: Saturation level, [0,1]
	//! \param brightness: Brightness level, [0,1]
	void setHSB(float hue, float saturation, float brightness);

	//! Retrieves color as RGBA.
	unsigned int getAsRGBA() const;

	//! Retrieves color as ARGB.
	unsigned int getAsARGB() const;

	//! Retrieves color as BGRA.
	unsigned int getAsBGRA() const;

	//! Retrieves colors as ABGR.
	unsigned int getAsABGR() const;

	float r, g, b, a;

	static const Color ZERO;
	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
};

} // end namespace render

#endif