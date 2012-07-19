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

#include <core/Position2d.h>
#include <core/Math.h>

namespace core
{

position2d::position2d() : X(0), Y(0) {}

position2d::position2d(signed int x, signed int y) : X(x), Y(y) {}

position2d::position2d(const position2d& other) : X(other.X), Y(other.Y) {}

position2d& position2d::operator=(const position2d& other)
{
	X = other.X;
	Y = other.Y;
	return *this;
}

position2d position2d::operator+(const position2d& other) const
{
	return position2d(X + other.X, Y + other.Y);
}

position2d& position2d::operator+=(const position2d& other)
{
	X += other.X;
	Y += other.Y;
	return *this;
}

position2d position2d::operator-(const position2d& other) const
{
	return position2d(X -other.X, Y - other.Y);
}

position2d& position2d::operator-=(const position2d& other)
{
	X -= other.X;
	Y -= other.Y;
	return *this;
}

bool position2d::operator == (const position2d& other) const
{
	return (X == other.X && Y == other.Y);
}

bool position2d::operator != (const position2d& other) const
{
	return (X != other.X || Y != other.Y);
}

float position2d::getDistanceFrom(const position2d& other) const
{
	signed int vx = X - other.X;
	signed int vy = Y - other.Y;
	return sqrt((float)(vx*vx + vy*vy));
}

float position2d::getDistanceFromSQ(const position2d& other) const
{
	signed int vx = X - other.X;
	signed int vy = Y - other.Y;
	return (float)(vx*vx + vy*vy);
}

const position2d position2d::ORIGIN_2D = position2d(0, 0);

} // end namespace core
