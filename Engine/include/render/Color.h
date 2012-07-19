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

#ifndef _COLOR_H_
#define _COLOR_H_

#include <core/Config.h>

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
	Color(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f);
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

	float R, G, B, A;

	static const Color ZERO;
	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
};

} // end namespace render

#endif