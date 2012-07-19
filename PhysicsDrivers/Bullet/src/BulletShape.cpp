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

#include <platform/PlatformManager.h>
#include <BulletShape.h>

namespace physics
{

//////////////////////////////////////////////////////////////////////////
//Plane Shape

BulletPlaneShape::BulletPlaneShape(): PlaneShape()
{
	mPlaneShape = NULL;
}

BulletPlaneShape::~BulletPlaneShape()
{
	if (mPlaneShape)
		delete mPlaneShape;
}

void BulletPlaneShape::setDimension(const core::vector3d& normal, float d)
{
	PlaneShape::setDimension(normal, d);

	btVector3 planeNorm(normal.X, normal.Y, normal.Z);
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
	mSphereShape = NULL;
}

BulletSphereShape::~BulletSphereShape()
{
	if (mSphereShape)
		delete mSphereShape;
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
	mBoxShape = NULL;
}

BulletBoxShape::~BulletBoxShape()
{
	if (mBoxShape)
		delete mBoxShape;
}

void BulletBoxShape::setDimension(const core::vector3d& dimensions)
{
	BoxShape::setDimension(dimensions);

	btVector3 dim(dimensions.X, dimensions.Y, dimensions.Z);

	mBoxShape = new btBoxShape(dim);
}

btCollisionShape* BulletBoxShape::getBulletCollisionShape()
{
	return mBoxShape;
}

} // end namespace game
