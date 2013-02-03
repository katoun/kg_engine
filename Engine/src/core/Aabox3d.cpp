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

#include <core/Aabox3d.h>
#include <core/Math.h>

namespace core
{

aabox3d::aabox3d(): MinEdge(-1, -1, -1), MaxEdge(1, 1, 1) {}

aabox3d::aabox3d(const glm::vec3& min, const glm::vec3& max): MinEdge(min), MaxEdge(max) {}

aabox3d::aabox3d(const glm::vec3& init): MinEdge(init), MaxEdge(init) {}

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
	if (x > MaxEdge.x) MaxEdge.x = x;
	if (y > MaxEdge.y) MaxEdge.y = y;
	if (z > MaxEdge.z) MaxEdge.z = z;

	if (x < MinEdge.x) MinEdge.x = x;
	if (y < MinEdge.y) MinEdge.y = y;
	if (z < MinEdge.z) MinEdge.z = z;
}

void aabox3d::addInternalPoint(const glm::vec3& p)
{
	addInternalPoint(p.x, p.y, p.z);
}

void aabox3d::addInternalBox(const aabox3d& b)
{
	addInternalPoint(b.MaxEdge);
	addInternalPoint(b.MinEdge);
}

void aabox3d::reset(float x, float y, float z)
{
	MaxEdge = glm::vec3(x, y, z);
	MinEdge = MaxEdge;
}

void aabox3d::reset(const aabox3d& initValue)
{
	*this = initValue;
}

void aabox3d::reset(const glm::vec3& initValue)
{
	MaxEdge = initValue;
	MinEdge = initValue;
}

glm::vec3 aabox3d::getCenter() const
{
	return (MinEdge + MaxEdge) / glm::vec3(2);
}

glm::vec3 aabox3d::getExtent() const
{
	return MaxEdge - MinEdge;
}

void aabox3d::getEdges(glm::vec3 *edges) const
{
	glm::vec3 middle = (MinEdge + MaxEdge) / glm::vec3(2);
	glm::vec3 diag = middle - MaxEdge;

	//! Edges are stored in this way:
	//!       /1--------/3
	//!      /  |      / |
	//!     /   |     /  |
	//!     5---------7  |
	//!     |   0- - -| -2
	//!     |  /      |  /
	//!     |/        | /
	//!     4---------6/

	edges[0] = glm::vec3(middle.x + diag.x, middle.y + diag.y, middle.z + diag.z);
	edges[1] = glm::vec3(middle.x + diag.x, middle.y - diag.y, middle.z + diag.z);
	edges[2] = glm::vec3(middle.x + diag.x, middle.y + diag.y, middle.z - diag.z);
	edges[3] = glm::vec3(middle.x + diag.x, middle.y - diag.y, middle.z - diag.z);
	edges[4] = glm::vec3(middle.x - diag.x, middle.y + diag.y, middle.z + diag.z);
	edges[5] = glm::vec3(middle.x - diag.x, middle.y - diag.y, middle.z + diag.z);
	edges[6] = glm::vec3(middle.x - diag.x, middle.y + diag.y, middle.z - diag.z);
	edges[7] = glm::vec3(middle.x - diag.x, middle.y - diag.y, middle.z - diag.z);
}

bool aabox3d::isEmpty() const
{
	glm::vec3 d = MinEdge - MaxEdge;
	if (d.x < 0.0f) d.x = -d.x;
	if (d.y < 0.0f) d.y = -d.y;
	if (d.z < 0.0f) d.z = -d.z;

	return (d.x < EPSILON && d.y < EPSILON && d.z < EPSILON);
}

void aabox3d::repair()
{
	float t;

	if (MinEdge.x > MaxEdge.x)
	{
		t = MinEdge.x;
		MinEdge.x = MaxEdge.x;
		MaxEdge.x = t;
	}
	if (MinEdge.y > MaxEdge.y)
	{
		t = MinEdge.y;
		MinEdge.y = MaxEdge.y;
		MaxEdge.y = t;
	}
	if (MinEdge.z > MaxEdge.z)
	{
		t = MinEdge.z;
		MinEdge.z = MaxEdge.z;
		MaxEdge.z = t;
	}
}

} // end namespace core
