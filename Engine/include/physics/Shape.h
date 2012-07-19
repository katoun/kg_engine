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
