/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

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

#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <core/Config.h>
#include <core/Vector3d.h>
#include <core/Quaternion.h>
#include <physics/ShapeDefines.h>

namespace physics
{

//!  Defines a physical shape.
//!  Author: Kat'Oun
//!  version: 1.0
class ENGINE_PUBLIC_EXPORT Shape
{
public:

	Shape();
	virtual ~Shape();

	//! Gets hape type.
	const ShapeType& getShapeType() const;

	//! Sets the position of the shape relative to it's body.
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(const core::vector3d& pos);

	//! Gets the position of the joint relative to it's body.
	const core::vector3d& getPosition() const;

	//!Sets the orientation applied to this shape.
	virtual void setOrientation(const core::quaternion& q);

	//!Gets the orientation of this shape.
	const core::quaternion& getOrientation() const;

protected:

	ShapeType mShapeType;

	//! Stores the position/translation of the shape relative to its body.
	core::vector3d mPosition;

	//! Stores the orientation of the shape relative to it's body.
	core::quaternion mOrientation;
};

//////////////////////////////////////////////////////////////////////////
//Plane Shape

class ENGINE_PUBLIC_EXPORT PlaneShape: public Shape
{
public:

	PlaneShape();
	virtual ~PlaneShape();

	virtual void setDimension(const core::vector3d& normal, float d);

	const core::vector3d& getNormal();
	float getD();

protected:

	core::vector3d mNormal;
	float mD;
};

//////////////////////////////////////////////////////////////////////////
//Sphere Shape

class ENGINE_PUBLIC_EXPORT SphereShape: public Shape
{
public:

	SphereShape();
	virtual ~SphereShape();

	virtual void setDimension(float radius);

	float getRadius();

protected:

	float mRadius;
};

//////////////////////////////////////////////////////////////////////////
//Box Shape

class ENGINE_PUBLIC_EXPORT BoxShape: public Shape
{
public:

	BoxShape();
	virtual ~BoxShape();

	virtual void setDimension(const core::vector3d& dimensions);

	const core::vector3d& getDimensions();

protected:

	core::vector3d mDimensions;
};

} // end namespace physics

#endif
