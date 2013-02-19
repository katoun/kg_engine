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

#ifndef _CONSTRAINT_H_
#define _CONSTRAINT_H_

#include <EngineConfig.h>
#include <physics/ConstraintDefines.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class btGeneric6DofConstraint;
class btHingeConstraint;
class btSliderConstraint;

namespace physics
{

class Body;

//! Defines a physical joint.
//! Author: Kat'Oun
//! version: 1.0
class ENGINE_PUBLIC_EXPORT Constraint
{
public:

	Constraint();

	virtual ~Constraint();

	//! Returns the id of the game object.
	const unsigned int& getID() const;

	//! Gets joint type.
	const ConstraintType& getConstraintType() const;

	void setBodies(Body* body1, Body* body2);

	virtual void setAnchor(const glm::vec3& anchor);

	virtual void setAxis(const glm::vec3& axis);

	//! Initialize input device.
	void initialize();

	//! Uninitialize input device.
	void uninitialize();

protected:

	virtual void initializeImpl();
	virtual void uninitializeImpl();

	unsigned int mID;

	static unsigned int mIndexCounter;

	ConstraintType mConstraintType;

	Body* mBody1;
	Body* mBody2;

	glm::vec3 mAnchor;
	glm::vec3 mAxis;
};

//////////////////////////////////////////////////////////////////////////
//Spherical Constraint
class ENGINE_PUBLIC_EXPORT SphericalConstraint: public Constraint
{
public:

	SphericalConstraint();

	virtual ~SphericalConstraint();

	void setLimits(float coneLimit, float twistLimit);

protected:

	void initializeImpl();

	btGeneric6DofConstraint* mSphericalConstraint;

	float mConeLimit; // The maximum limit of rotational movement (radians)
	float mTwistLimit; // The maximum limit of twisting movement (radians)
};

//////////////////////////////////////////////////////////////////////////
//Hinge Constraint
class ENGINE_PUBLIC_EXPORT HingeConstraint: public Constraint
{
public:

	HingeConstraint();

	virtual ~HingeConstraint();

	void setLimits(float lowerLimit, float upperLimit);

protected:

	void initializeImpl();

	btHingeConstraint* mHingeConstraint;

	float mLowerLimit;
	float mUpperLimit;
};

//////////////////////////////////////////////////////////////////////////
//Slider Constraint
class ENGINE_PUBLIC_EXPORT SliderConstraint: public Constraint
{
public:

	SliderConstraint();

	virtual ~SliderConstraint();

protected:

	void initializeImpl();

	btSliderConstraint* mSliderConstraint;
};

//////////////////////////////////////////////////////////////////////////
//Generic Constraint
class ENGINE_PUBLIC_EXPORT GenericConstraint: public Constraint
{
public:

	GenericConstraint();

	virtual ~GenericConstraint();

protected:

	void initializeImpl();

	btGeneric6DofConstraint* mGenericConstraint;
};

//////////////////////////////////////////////////////////////////////////
//Rigid Constraint
class ENGINE_PUBLIC_EXPORT RigidConstraint: public Constraint
{
public:

	RigidConstraint();

	virtual ~RigidConstraint();

protected:

	void initializeImpl();

	btHingeConstraint* mRigidConstraint;
};

} // end namespace physics

#endif
