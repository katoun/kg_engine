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

#include <core/Aabox3d.h>
#include <core/Math.h>

namespace core
{

aabox3d::aabox3d(): MinEdge(-1.0f, -1.0f, -1.0f), MaxEdge(1.0f, 1.0f, 1.0f) {}

aabox3d::aabox3d(const vector3d& min, const vector3d& max): MinEdge(min), MaxEdge(max) {}

aabox3d::aabox3d(const vector3d& init): MinEdge(init), MaxEdge(init) {}

aabox3d::aabox3d(float minx, float miny, float minz, float maxx, float maxy, float maxz): MinEdge(minx, miny, minz), MaxEdge(maxx, maxy, maxz) {}

inline bool aabox3d::operator==(const aabox3d& other) const
{
	return (MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);
}

inline bool aabox3d::operator!=(const aabox3d& other) const
{
	return !(MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);
}

void aabox3d::addInternalPoint(float x, float y, float z)
{
	if (x > MaxEdge.X) MaxEdge.X = x;
	if (y > MaxEdge.Y) MaxEdge.Y = y;
	if (z > MaxEdge.Z) MaxEdge.Z = z;

	if (x < MinEdge.X) MinEdge.X = x;
	if (y < MinEdge.Y) MinEdge.Y = y;
	if (z < MinEdge.Z) MinEdge.Z = z;
}

void aabox3d::addInternalPoint(const vector3d& p)
{
	addInternalPoint(p.X, p.Y, p.Z);
}

void aabox3d::addInternalBox(const aabox3d& b)
{
	addInternalPoint(b.MaxEdge);
	addInternalPoint(b.MinEdge);
}

void aabox3d::reset(float x, float y, float z)
{
	MaxEdge.set(x, y, z);
	MinEdge = MaxEdge;
}

void aabox3d::reset(const aabox3d& initValue)
{
	*this = initValue;
}

void aabox3d::reset(const vector3d& initValue)
{
	MaxEdge = initValue;
	MinEdge = initValue;
}

vector3d aabox3d::getCenter() const
{
	return (MinEdge + MaxEdge) / 2;
}

vector3d aabox3d::getExtent() const
{
	return MaxEdge - MinEdge;
}

void aabox3d::getEdges(vector3d *edges) const
{
	vector3d middle = (MinEdge + MaxEdge) / 2;
	vector3d diag = middle - MaxEdge;

	//! Edges are stored in this way:
	//!       /1--------/3
	//!      /  |      / |
	//!     /   |     /  |
	//!     5---------7  |
	//!     |   0- - -| -2
	//!     |  /      |  /
	//!     |/        | /
	//!     4---------6/

	edges[0].set(middle.X + diag.X, middle.Y + diag.Y, middle.Z + diag.Z);
	edges[1].set(middle.X + diag.X, middle.Y - diag.Y, middle.Z + diag.Z);
	edges[2].set(middle.X + diag.X, middle.Y + diag.Y, middle.Z - diag.Z);
	edges[3].set(middle.X + diag.X, middle.Y - diag.Y, middle.Z - diag.Z);
	edges[4].set(middle.X - diag.X, middle.Y + diag.Y, middle.Z + diag.Z);
	edges[5].set(middle.X - diag.X, middle.Y - diag.Y, middle.Z + diag.Z);
	edges[6].set(middle.X - diag.X, middle.Y + diag.Y, middle.Z - diag.Z);
	edges[7].set(middle.X - diag.X, middle.Y - diag.Y, middle.Z - diag.Z);
}

bool aabox3d::isEmpty() const
{
	vector3d d = MinEdge - MaxEdge;
	if (d.X < 0.0f) d.X = -d.X;
	if (d.Y < 0.0f) d.Y = -d.Y;
	if (d.Z < 0.0f) d.Z = -d.Z;

	return (d.X < EPSILON && d.Y < EPSILON && d.Z < EPSILON);
}

void aabox3d::repair()
{
	float t;

	if (MinEdge.X > MaxEdge.X)
	{
		t = MinEdge.X;
		MinEdge.X = MaxEdge.X;
		MaxEdge.X = t;
	}
	if (MinEdge.Y > MaxEdge.Y)
	{
		t = MinEdge.Y;
		MinEdge.Y = MaxEdge.Y;
		MaxEdge.Y = t;
	}
	if (MinEdge.Z > MaxEdge.Z)
	{
		t = MinEdge.Z;
		MinEdge.Z = MaxEdge.Z;
		MaxEdge.Z = t;
	}
}

} // end namespace core
