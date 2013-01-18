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
