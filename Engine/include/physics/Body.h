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

#ifndef _BODY_H_
#define _BODY_H_

#include <core/Config.h>
#include <scene/Node.h>
#include <resource/ResourceEventReceiver.h>

#include <string>

namespace physics
{

class Material;
class BodyData;
class Joint;
enum BodyType;

//! Defines an actor in the physics world.
//! Author: Kat'Oun
//! version: 1.0
class ENGINE_PUBLIC_EXPORT Body: public scene::Node, public resource::ResourceEventReceiver
{
public:

	Body(BodyData* bodyData);
	Body(const std::string& name, BodyData* bodyData);

	virtual ~Body();

	void setBodyType(BodyType type);
	BodyType getBodyType();

	//! Sets the body data this body will use.
	void setBodyData(const std::string& filename);
	void setBodyData(BodyData* bodyData);

	BodyData* getBodyData() const;

	//! Sets the name of the material which this body will use.
	virtual void setMaterial(const std::string& filename);
	virtual void setMaterial(Material* material);

	Material* getMaterial() const;

	//! Sets whether the Solid can collide with other Solids and be physically simulated.
	virtual void setEnabled(bool enabled);

	//! Returns true if the Solid is enabled.
	bool isEnabled() const;

	//! Sets whether the body is sleeping.
	void setSleeping(bool sleeping);

	//! Returns true if the Solid is sleeping.
	bool isSleeping() const;

	//! Sets the Solid's sleepiness level.
	void setSleepiness(float sleepiness);

	//! Returns the body's sleepiness level.
	float getSleepiness() const;

	//! Sets the mass.
	void setMass(float mass);
	//! Returns the mass.
	float getMass() const;

	//! Sets the amount of linear damping on this body.
	void setLinearDamping(float linearDamping);

	//! Returns the amount of linear damping on this body.
	float getLinearDamping() const;

	//! Sets the amount of angular damping on this body.
	void setAngularDamping(float angularDamping);

	//! Returns the amount of angular damping on this body.
	float getAngularDamping() const;

	//! Sets the linear velocity of the body.
	void setLinearVelocity(const core::vector3d& velocity);

	//! Gets the linear velocity of the body.
	const core::vector3d getLinearVelocity();

	//! Sets the angular velocity of the body.
	void setAngularVelocity(const core::vector3d& velocity);
	
	//! Gets the angular velocity of the body.
	const core::vector3d getAngularVelocity();

	//! Applies a force to the body.
	//! This applies a force with the direction and strength represented by the input vector.
	virtual void applyForce(const core::vector3d& force);

	//! Applies a torque on the body.
	//! In classical physics, torque is related to angular acceleration by \f$T=I.\alpha\f$ where T is the Torque, I is the moment of inertia, and \f$\alpha\f$ is angular acceleration.
	virtual void applyTorque(const core::vector3d& torque);

	//! Applys a linear impulse to the body.
	//! In classical physics, momentum is related to velocity by \f$p=m.v\f$ where p is the Momentum, m is the Mass, and v is the Velocity.
	//! An impulse is simply a change in momentum.
	virtual void applyLinearImpulse(const core::vector3d& linearImpulse);

	//! Applys an angular impulse to the body.
	//! This will cause a change in the angular momentum, and subsequently a change in the angular velocity.
	virtual void applyAngularImpulse(const core::vector3d& angularImpulse);

	void setJoint(Joint* joint);

	void resourceLoaded(const resource::ResourceEvent& evt);
	void resourceUnloaded(const resource::ResourceEvent& evt);

protected:

	// Incremented count for next index
	static unsigned int msNextGeneratedActorIndex;

	virtual void updateTransformImpl();

	BodyData* mBodyData;

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

	/// Determines whether the body is enabled.
	bool mEnabled;

	core::vector3d mForce;
	core::vector3d mTorque;
	core::vector3d mLinearImpulse;
	core::vector3d mAngularImpulse;

	Joint* mJoint;
};

} // end namespace physics

#endif