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

#include <render/Frustum.h>
#include <render/FrustumDefines.h>
#include <render/CameraDefines.h>
#include <core/Math.h>
#include <core/Matrix4.h>
#include <core/Sphere3d.h>
#include <core/Aabox3d.h>

namespace render
{

Frustum::Frustum() {}

Frustum::~Frustum() {}

inline void Frustum::buildViewFrustum(const core::matrix4& projMat, const core::matrix4& viewMat, ProjectionType projType, float fov, float aspect, float near, float far)
{
	core::matrix4 mat = projMat * viewMat;

	mPlanes[FRUSTUM_PLANE_LEFT].Normal.x = mat(3,0) - mat(0,0);
	mPlanes[FRUSTUM_PLANE_LEFT].Normal.y = mat(3,1) - mat(0,1);
	mPlanes[FRUSTUM_PLANE_LEFT].Normal.z = mat(3,2) - mat(0,2);
	mPlanes[FRUSTUM_PLANE_LEFT].D = mat(3,3) - mat(0,3);

	mPlanes[FRUSTUM_PLANE_RIGHT].Normal.x = mat(3,0) + mat(0,0);
	mPlanes[FRUSTUM_PLANE_RIGHT].Normal.y = mat(3,1) + mat(0,1);
	mPlanes[FRUSTUM_PLANE_RIGHT].Normal.z = mat(3,2) + mat(0,2);
	mPlanes[FRUSTUM_PLANE_RIGHT].D = mat(3,3) + mat(0,3);

	mPlanes[FRUSTUM_PLANE_TOP].Normal.x = mat(3,0) + mat(1,0);
	mPlanes[FRUSTUM_PLANE_TOP].Normal.y = mat(3,1) + mat(1,1);
	mPlanes[FRUSTUM_PLANE_TOP].Normal.z = mat(3,2) + mat(1,2);
	mPlanes[FRUSTUM_PLANE_TOP].D = mat(3,3) + mat(1,3);

	mPlanes[FRUSTUM_PLANE_BOTTOM].Normal.x = mat(3,0) - mat(1,0);
	mPlanes[FRUSTUM_PLANE_BOTTOM].Normal.y = mat(3,1) - mat(1,1);
	mPlanes[FRUSTUM_PLANE_BOTTOM].Normal.z = mat(3,2) - mat(1,2);
	mPlanes[FRUSTUM_PLANE_BOTTOM].D = mat(3,3) - mat(1,3);

	mPlanes[FRUSTUM_PLANE_NEAR].Normal.x = mat(3,0) + mat(2,0);
	mPlanes[FRUSTUM_PLANE_NEAR].Normal.y = mat(3,1) + mat(2,1);
	mPlanes[FRUSTUM_PLANE_NEAR].Normal.z = mat(3,2) + mat(2,2);
	mPlanes[FRUSTUM_PLANE_NEAR].D = mat(3,3) + mat(2,3);

	mPlanes[FRUSTUM_PLANE_FAR].Normal.x = mat(3,0) - mat(2,0);
	mPlanes[FRUSTUM_PLANE_FAR].Normal.y =mat(3,1) - mat(2,1);
	mPlanes[FRUSTUM_PLANE_FAR].Normal.z = mat(3,2) - mat(2,2);
	mPlanes[FRUSTUM_PLANE_FAR].D = mat(3,3) - mat(2,3);

	for (unsigned char i = 0; i < 6; ++i)
	{
		float invLen = 1.0f / mPlanes[i].Normal.getLength();
		mPlanes[i].Normal *= invLen;
		mPlanes[i].D *= invLen;
	}

	//Near plane
	mPlanes[FRUSTUM_PLANE_NEAR].getIntersectionWithPlanes(mPlanes[FRUSTUM_PLANE_TOP], mPlanes[FRUSTUM_PLANE_LEFT], mCorners[0]);
	mPlanes[FRUSTUM_PLANE_NEAR].getIntersectionWithPlanes(mPlanes[FRUSTUM_PLANE_BOTTOM], mPlanes[FRUSTUM_PLANE_LEFT], mCorners[1]);
	mPlanes[FRUSTUM_PLANE_NEAR].getIntersectionWithPlanes(mPlanes[FRUSTUM_PLANE_BOTTOM], mPlanes[FRUSTUM_PLANE_RIGHT], mCorners[2]);
	mPlanes[FRUSTUM_PLANE_NEAR].getIntersectionWithPlanes(mPlanes[FRUSTUM_PLANE_TOP], mPlanes[FRUSTUM_PLANE_RIGHT], mCorners[3]);

	//Far plane
	mPlanes[FRUSTUM_PLANE_FAR].getIntersectionWithPlanes(mPlanes[FRUSTUM_PLANE_TOP], mPlanes[FRUSTUM_PLANE_LEFT], mCorners[4]);
	mPlanes[FRUSTUM_PLANE_FAR].getIntersectionWithPlanes(mPlanes[FRUSTUM_PLANE_BOTTOM], mPlanes[FRUSTUM_PLANE_LEFT], mCorners[5]);
	mPlanes[FRUSTUM_PLANE_FAR].getIntersectionWithPlanes(mPlanes[FRUSTUM_PLANE_BOTTOM], mPlanes[FRUSTUM_PLANE_RIGHT], mCorners[6]);
	mPlanes[FRUSTUM_PLANE_FAR].getIntersectionWithPlanes(mPlanes[FRUSTUM_PLANE_TOP], mPlanes[FRUSTUM_PLANE_RIGHT], mCorners[7]);
}

inline void Frustum::transform(const core::matrix4& mat)
{
	for (unsigned int i=0; i < 6; ++i)
		mat.transformPlane(mPlanes[i]);
}

const core::vector3d* Frustum::getCorners()
{
	return mCorners;
}

bool Frustum::isVisible(const core::vector3d& point)
{
	// Go through all the sides of the frustum
	for (unsigned char i = 0; i < 6; ++i)
	{
		// Calculate the plane equation and check if the point is behind a side of the frustum
		if (mPlanes[i].getDistanceTo(point) > 0)
		{
			return true;
		}
	}

	return false;
}

bool Frustum::isVisible(const core::sphere3d& sphere)
{
	// Go through all the sides of the frustum
	for (unsigned char i = 0; i < 6; ++i)
	{
		if (mPlanes[i].getDistanceTo(sphere.Center) > sphere.Radius)
			return true;
	}

	return false;
}

bool Frustum::isVisible(const core::aabox3d& box)
{
	// Get corners of the box
	core::vector3d edges[8];
	box.getEdges(edges);
	
	// Go through all the sides of the frustum
	for (unsigned char i = 0; i < 6; ++i)
	{		
		// Calculate the plane equation and check if the box is behind a side of the frustum
		if ((mPlanes[i].getDistanceTo(edges[0]) > 0) &&
			(mPlanes[i].getDistanceTo(edges[1]) > 0) &&
			(mPlanes[i].getDistanceTo(edges[2]) > 0) &&
			(mPlanes[i].getDistanceTo(edges[3]) > 0) &&
			(mPlanes[i].getDistanceTo(edges[4]) > 0) &&
			(mPlanes[i].getDistanceTo(edges[5]) > 0) &&
			(mPlanes[i].getDistanceTo(edges[6]) > 0) &&
			(mPlanes[i].getDistanceTo(edges[7]) > 0))
		{
			return true;
		}
	}
	
	return false;
}

} //namespace render
