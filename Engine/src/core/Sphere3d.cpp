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

#include <core/Sphere3d.h>
#include <core/Math.h>

namespace core
{

sphere3d::sphere3d(): Center(0.0f, 0.0f, 0.0f), Radius(0.0f) {}

sphere3d::sphere3d(const vector3d& center, const float radius) : Center(center), Radius(radius) {}

sphere3d::sphere3d(const sphere3d& other) : Center(other.Center), Radius(other.Radius) {}

sphere3d& sphere3d::operator=(const sphere3d& other)
{
	Center = other.Center;
	Radius = other.Radius;
	return *this;
}

bool sphere3d::operator==(const sphere3d& other) const
{
	return ((other.Center == Center) && (other.Radius + EPSILON > Radius) && (other.Radius - EPSILON < Radius));
}

bool sphere3d::operator!=(const sphere3d& other) const
{
	return ((other.Center != Center) || (other.Radius + EPSILON < Radius) || (other.Radius - EPSILON > Radius));
}

inline void sphere3d::set(const vector3d center, const float radius)
{
	Center = center;
	Radius = radius;
}

inline void sphere3d::set(const sphere3d& s)
{
	Center = s.Center;
	Radius = s.Radius;
}

} // end namespace core
