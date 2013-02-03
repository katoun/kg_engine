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

#include <core/Plane3d.h>
#include <core/Math.h>

namespace core
{

plane3d::plane3d(): Normal(0, 1, 0)
{
	recalculateD(glm::vec3(0, 0, 0));
}

plane3d::plane3d(const glm::vec3& MPoint, const glm::vec3& Normal) : Normal(Normal)
{
	recalculateD(MPoint);
}

plane3d::plane3d(float px, float py, float pz, float nx, float ny, float nz) : Normal(nx, ny, nz)
{
	recalculateD(glm::vec3(px, py, pz));
}

plane3d::plane3d(const plane3d& other) : Normal(other.Normal), D(other.D) {};

plane3d::plane3d(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3)
{
	setPlane(point1, point2, point3);
}

inline bool plane3d::operator==(const plane3d& other) const
{
	return ((other.Normal == Normal) && (other.D + EPSILON > D) && (other.D - EPSILON < D));
}

inline bool plane3d::operator!=(const plane3d& other) const
{
	return ((other.Normal != Normal) || (other.D + EPSILON < D) || (other.D - EPSILON > D));
}

void plane3d::setPlane(const glm::vec3& point, const glm::vec3& nvector)
{
	Normal = glm::normalize(nvector);
	recalculateD(point);
}

void plane3d::setPlane(const glm::vec3& nvect, float d)
{
	Normal = nvect;
	D = d;
}

void plane3d::setPlane(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3)
{
	// creates the plane from 3 member points
	Normal = glm::normalize(glm::cross((point2 - point1), (point3 - point1)));

	recalculateD(point1);
}

void plane3d::recalculateD(const glm::vec3& MPoint)
{
	D = - glm::dot(MPoint, Normal);
}

glm::vec3 plane3d::getMemberPoint() const
{
	return Normal * -D;
}

bool plane3d::isFrontFacing(const glm::vec3& lookDirection) const
{
	return (glm::dot(Normal, lookDirection) <= 0.0f);
}

float plane3d::getDistanceTo(const glm::vec3& point) const
{
	return glm::dot(Normal, point) + D;
}

bool plane3d::getIntersectionWithLine(const glm::vec3& linePoint, const glm::vec3& lineVect, glm::vec3& outIntersection) const
{
	float t2 = glm::dot(Normal, lineVect);

	if (t2 == 0.0f)
		return false;

	float t = - (glm::dot(Normal, linePoint) + D) / t2;
	outIntersection = linePoint + (lineVect * t);

	return true;
}

bool plane3d::getIntersectionWithPlane(const plane3d& other, glm::vec3& outLinePoint, glm::vec3& outLineVect) const
{
	const float fn00 = sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z);
	const float fn01 = glm::dot(Normal, other.Normal);
	const float fn11 = sqrt(other.Normal.x*other.Normal.x + other.Normal.y*other.Normal.y + other.Normal.z*other.Normal.z);
	const float det = fn00*fn11 - fn01*fn01;

	if (abs(det) < EPSILON)
		return false;

	const float invdet = 1.0f / det;
	const float fc0 = (fn11*-D + fn01*other.D) * invdet;
	const float fc1 = (fn00*-other.D + fn01*D) * invdet;

	outLineVect = glm::cross(Normal, other.Normal);
	outLinePoint = Normal*fc0 + other.Normal*fc1;

	return true;
}

bool plane3d::getIntersectionWithPlanes(const plane3d& o1, const plane3d& o2, glm::vec3& outPoint) const
{
	glm::vec3 linePoint, lineVect;
	if (getIntersectionWithPlane(o1, linePoint, lineVect))
		return o2.getIntersectionWithLine(linePoint, lineVect, outPoint);

	return false;
}

} // end namespace core
