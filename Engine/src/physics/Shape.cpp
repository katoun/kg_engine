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
