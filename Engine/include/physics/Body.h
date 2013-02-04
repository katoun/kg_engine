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

#ifndef _BODY_H_
#define _BODY_H_

#include <EngineConfig.h>
#include <game/Component.h>
#include <resource/ResourceEventReceiver.h>

#include <glm/glm.hpp>

#include <string>

namespace physics
{

class Material;
class BodyData;
class Joint;
enum BodyType;

//! Defines an actor in the physics world.
class ENGINE_PUBLIC_EXPORT Body: public game::Component, public resource::ResourceEventReceiver
{
public:

	Body();
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
	void setLinearVelocity(const glm::vec3& velocity);

	//! Gets the linear velocity of the body.
	const glm::vec3 getLinearVelocity();

	//! Sets the angular velocity of the body.
	void setAngularVelocity(const glm::vec3& velocity);
	
	//! Gets the angular velocity of the body.
	const glm::vec3 getAngularVelocity();

	//! Applies a force to the body.
	//! This applies a force with the direction and strength represented by the input vector.
	virtual void applyForce(const glm::vec3& force);

	//! Applies a torque on the body.
	//! In classical physics, torque is related to angular acceleration by \f$T=I.\alpha\f$ where T is the Torque, I is the moment of inertia, and \f$\alpha\f$ is angular acceleration.
	virtual void applyTorque(const glm::vec3& torque);

	//! Applys a linear impulse to the body.
	//! In classical physics, momentum is related to velocity by \f$p=m.v\f$ where p is the Momentum, m is the Mass, and v is the Velocity.
	//! An impulse is simply a change in momentum.
	virtual void applyLinearImpulse(const glm::vec3& linearImpulse);

	//! Applys an angular impulse to the body.
	//! This will cause a change in the angular momentum, and subsequently a change in the angular velocity.
	virtual void applyAngularImpulse(const glm::vec3& angularImpulse);

	void setJoint(Joint* joint);

	void resourceLoaded(const resource::ResourceEvent& evt);
	void resourceUnloaded(const resource::ResourceEvent& evt);

protected:

	virtual void setBodyDataImpl(BodyData* bodyData);

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
	glm::vec3 mLinearVelocity;

	/// The body's angular velocity.
	glm::vec3 mAngularVelocity;

	//! The material this body uses.
	Material* mMaterial;

	/// Determines whether the body is enabled.
	bool mEnabled;

	glm::vec3 mForce;
	glm::vec3 mTorque;
	glm::vec3 mLinearImpulse;
	glm::vec3 mAngularImpulse;

	Joint* mJoint;
};

} // end namespace physics

#endif