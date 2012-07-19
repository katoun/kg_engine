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

#include <physics/Shape.h>

namespace physics
{

Shape::Shape()
{
	mShapeType = SHAPE_TYPE_UNDEFINED;

	mPosition = core::vector3d::ORIGIN_3D;
	mOrientation = core::quaternion::IDENTITY;
}

Shape::~Shape() {}

const ShapeType& Shape::getShapeType() const
{
	return mShapeType;
}

void Shape::setPosition(float x, float y, float z)
{
	mPosition.X = x;
	mPosition.Y = y;
	mPosition.Z = z;
}

void Shape::setPosition(const core::vector3d& pos)
{
	mPosition = pos;
}

const core::vector3d& Shape::getPosition() const
{
	return mPosition;
}

void Shape::setOrientation(const core::quaternion& q)
{
	mOrientation = q;
}

const core::quaternion& Shape::getOrientation() const
{
	return mOrientation;
}

//////////////////////////////////////////////////////////////////////////
//Plane Shape

PlaneShape::PlaneShape()
{
	mShapeType = SHAPE_TYPE_PLANE;
	mNormal = core::vector3d::UNIT_Y;
	mD = 0.0f;
}

PlaneShape::~PlaneShape() {}

void PlaneShape::setDimension(const core::vector3d& normal, float d)
{
	mNormal = normal;
	mD = d;
}

const core::vector3d& PlaneShape::getNormal()
{
	return mNormal;
}

float PlaneShape::getD()
{
	return mD;
}

//////////////////////////////////////////////////////////////////////////
//Sphere Shape

SphereShape::SphereShape()
{
	mShapeType = SHAPE_TYPE_SPHERE;
	mRadius = 1.0f;
}

SphereShape::~SphereShape() {}

void SphereShape::setDimension(float radius)
{
	mRadius = radius;
}

float SphereShape::getRadius()
{
	return mRadius;
}

//////////////////////////////////////////////////////////////////////////
//Box Shape

BoxShape::BoxShape()
{
	mShapeType = SHAPE_TYPE_BOX;
	mDimensions = core::vector3d::UNIT_SCALE;
}

BoxShape::~BoxShape() {}

void BoxShape::setDimension(const core::vector3d& dimensions)
{
	mDimensions = dimensions;
}

const core::vector3d& BoxShape::getDimensions()
{
	return mDimensions;
}

} // end namespace physics
