/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2012 Catalin Alexandru Nastase

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

#ifndef _PLANE_3D_H_
#define _PLANE_3D_H_

#include <core/Vector3d.h>

namespace core
{

//! Template plane class with some intersection testing methods.
class ENGINE_PUBLIC_EXPORT plane3d
{
public:

	plane3d();
	plane3d(float px, float py, float pz, float nx, float ny, float nz);
	plane3d(const vector3d& point1, const vector3d& point2, const vector3d& point3);
	plane3d(const vector3d& MPoint, const vector3d& Normal);
	plane3d(const plane3d& other);

	inline bool operator==(const plane3d& other) const;
	inline bool operator!=(const plane3d& other) const;

	void setPlane(const vector3d& point, const vector3d& nvector);

	void setPlane(const vector3d& nvect, float d);

	void setPlane(const vector3d& point1, const vector3d& point2, const vector3d& point3);

	//! Recalculates the distance from origin by applying
	//! a new member point to the plane.
	void recalculateD(const vector3d& MPoint);

	//! Returns a member point of the plane.
	vector3d getMemberPoint() const;

	//! Returns if the plane is front of backfacing. Note that this only
	//! works if the normal is Normalized.
	//! \param lookDirection: Look direction.
	//! \return Returns true if the plane is front facing, which mean it would
	//! be visible, and false if it is backfacing.
	bool isFrontFacing(const vector3d& lookDirection) const;

	//! Returns the distance to a point.  Note that this only
	//! works if the normal is Normalized.
	float getDistanceTo(const vector3d& point) const;

	//! Returns an intersection with a 3d line.
	//! \param lineVect: Vector of the line to intersect with.
	//! \param linePoint: Point of the line to intersect with.
	//! \param outIntersection: Place to store the intersection point, if there is one.
	bool getIntersectionWithLine(const vector3d& linePoint, const vector3d& lineVect, vector3d& outIntersection) const;
	
	//! Intersects this plane with another.
	//! \param other: Other plane to intersect with.
	//! \param outLinePoint: Base point of intersection line.
	//! \param outLineVect: Vector of intersection.
	bool getIntersectionWithPlane(const plane3d& other, vector3d& outLinePoint, vector3d& outLineVect) const;

	//! Returns the intersection point with two other planes if there is one.
	bool getIntersectionWithPlanes(const plane3d& o1, const plane3d& o2, vector3d& outPoint) const;

	vector3d Normal;
	float D;	// distance from origin
};

} // end namespace core

#endif