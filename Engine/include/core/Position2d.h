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

#ifndef _POSITION_2D_H_
#define _POSITION_2D_H_

#include <core/Config.h>

namespace core
{

//! Simple class for holding 2d coordinates.
class ENGINE_PUBLIC_EXPORT position2d
{
public:

	position2d();
	position2d(signed int x, signed int y);
	position2d(const position2d& other);

	inline position2d& operator=(const position2d& other);

	inline position2d operator-(const position2d& other) const;
	inline position2d& operator+=(const position2d& other);

	inline position2d operator+(const position2d& other) const;
	inline position2d& operator-=(const position2d& other);

	inline bool operator == (const position2d& other) const;
	inline bool operator != (const position2d& other) const;

	//! Returns distance from an other point.
	float getDistanceFrom(const position2d& other) const;

	//! Returns distance from an other point.
	float getDistanceFromSQ(const position2d& other) const;

	//! ORIGIN_2D is a const position2d that defines the 2d origin position for the 2d space
	static const position2d ORIGIN_2D;

	signed int X, Y;
};

} // end namespace core

#endif