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

#ifndef _BODY_DATA_H_
#define _BODY_DATA_H_

#include <core/Config.h>
#include <core/Vector3d.h>
#include <resource/Resource.h>

#include <string>
#include <list>

namespace resource
{
class Serializer;
}

namespace physics
{

class Material;
class Body;
class Shape;

enum BodyType
{
	BT_DYNAMIC,
	BT_STATIC,
	BT_KINEMATIC,
	BT_COUNT
};

//!  Defines a body resource.
//!  Author: Kat'Oun
//!  version: 1.0
class ENGINE_PUBLIC_EXPORT BodyData: public resource::Resource
{
	friend class Body;

public:

	BodyData(const std::string& name, resource::Serializer* serializer);
	virtual ~BodyData();

	//! Sets what type the Body should be: static, dynamic or kinetic.
	void setBodyType(BodyType type);

	//! Gets what type the Body is: static, dynamic or kinetic.
	const BodyType& getBodyType() const;

	//! Sets whether the body is sleeping.
	void setSleeping(bool sleeping);

	//! Gets whether the body is sleeping.
	bool getSleeping();

	//! Sets the Solid's sleepiness level.
	void setSleepiness(float sleepiness);

	//! Gets the Solid's sleepiness level.
	float getSleepiness();

	//! Sets the mass.
	void setMass(float mass);

	//! Gets the mass.
	float getMass();

	//! Sets the amount of linear damping on this body.
	void setLinearDamping(float linearDamping);

	//! Gets the amount of linear damping on this body.
	float getLinearDamping();

	//! Sets the amount of angular damping on this body.
	void setAngularDamping(float angularDamping);

	//! Gets the amount of angular damping on this body.
	float getAngularDamping();

	//! Sets the linear velocity of the body.
	void setLinearVelocity(const core::vector3d& velocity);

	//! Gets the linear velocity of the body.
	const core::vector3d& getLinearVelocity() const;

	//! Sets the angular velocity of the body.
	void setAngularVelocity(const core::vector3d& velocity);

	//! Gets the angular velocity of the body.
	const core::vector3d& getAngularVelocity() const;

	//! Sets the name of the material which this body will use.
	void setMaterial(const std::string& filename);

	Material* getMaterial();
	
	void addShape(Shape* shape);

	std::list<Shape*>& getShapes();

	//! Removes all texture unit.
	void removeAllShapes();

private:

	void unloadImpl();

	/// Determines what type the body is.
	BodyType mBodyType;

	/// Determines whether the body is sleeping.
	bool mSleeping;

	/// The body's sleepiness level which determines how fast falls asleep.
	float mSleepiness;

	///The total mass of the body.
	float mMass;

	/// The amount of damping applied to the body's linear motion.
	float mLinearDamping;

	/// The amount of damping applied to the body's angular motion.
	float mAngularDamping;

	/// The body's linear velocity in global coordinates.
	core::vector3d mLinearVelocity;

	/// The body's angular velocity.
	core::vector3d mAngularVelocity;

	//! The material this body uses.
	Material* mMaterial;

	std::list<Shape*> mShapes;
};

} // end namespace physics

#endif