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

#include <platform/PlatformManager.h>
#include <BulletShape.h>

namespace physics
{

//////////////////////////////////////////////////////////////////////////
//Plane Shape

BulletPlaneShape::BulletPlaneShape(): PlaneShape()
{
	mPlaneShape = nullptr;
}

BulletPlaneShape::~BulletPlaneShape()
{
	SAFE_DELETE(mPlaneShape);
}

void BulletPlaneShape::setDimension(const glm::vec3& normal, float d)
{
	PlaneShape::setDimension(normal, d);

	btVector3 planeNorm(normal.x, normal.y, normal.z);
	btScalar planeConst(d);

	mPlaneShape = new btStaticPlaneShape(planeNorm , planeConst);
}

btCollisionShape* BulletPlaneShape::getBulletCollisionShape()
{
	return mPlaneShape;
}

//////////////////////////////////////////////////////////////////////////
//Sphere Shape

BulletSphereShape::BulletSphereShape(): SphereShape()
{
	mSphereShape = nullptr;
}

BulletSphereShape::~BulletSphereShape()
{
	SAFE_DELETE(mSphereShape);
}

void BulletSphereShape::setDimension(float radius)
{
	SphereShape::setDimension(radius);

	btScalar rad(radius);

	mSphereShape = new btSphereShape(rad);
}


btCollisionShape* BulletSphereShape::getBulletCollisionShape()
{
	return mSphereShape;
}

//////////////////////////////////////////////////////////////////////////
//Box Shape

BulletBoxShape::BulletBoxShape(): BoxShape()
{
	mBoxShape = nullptr;
}

BulletBoxShape::~BulletBoxShape()
{
	SAFE_DELETE(mBoxShape);
}

void BulletBoxShape::setDimension(const glm::vec3& dimensions)
{
	BoxShape::setDimension(dimensions);

	btVector3 dim(dimensions.x, dimensions.y, dimensions.z);

	mBoxShape = new btBoxShape(dim);
}

btCollisionShape* BulletBoxShape::getBulletCollisionShape()
{
	return mBoxShape;
}

} // end namespace game
