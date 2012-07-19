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