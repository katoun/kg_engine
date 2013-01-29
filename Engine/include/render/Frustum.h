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

#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include <EngineConfig.h>
#include <core/Plane3d.h>
#include <core/Vector3d.h>

namespace core
{
class matrix4;
class sphere3d;
class aabox3d;
}

namespace render
{

enum ProjectionType;

//! A frustum represents a pyramid, capped at the near and far end which is
//! used to represent either a visible area or a projection area. Can be used
//! for a number of applications.
class ENGINE_PUBLIC_EXPORT Frustum
{
public:

	Frustum();

	~Frustum();

	//! build the view frustum
	void buildViewFrustum(const core::matrix4& projMat, const core::matrix4& viewMat, ProjectionType projType, float fov, float aspect, float near, float far);

	//! transforms the frustum by the matrix
	//! \param mat: Matrix by which the view frustum is transformed.
	void transform(const core::matrix4& mat);

	const core::vector3d* getCorners();

	//! Tests whether the given point is visible in the Frustum.
	bool isVisible(const core::vector3d& point);

	//! Tests whether the given sphere is visible in the Frustum.
	bool isVisible(const core::sphere3d& sphere);

	//! Tests whether the given aabox is visible in the Frustum.
	bool isVisible(const core::aabox3d& box);

protected:

	core::plane3d mPlanes[6];
	core::vector3d mCorners[8];
};

} //namespace render

#endif