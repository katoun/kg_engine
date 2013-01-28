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

#ifndef _JOINT_H_
#define _JOINT_H_

#include <Config.h>
#include <core/Vector3d.h>
#include <core/Quaternion.h>
#include <physics/JointDefines.h>
#include <engine/Object.h>

namespace physics
{

class Body;

//! Defines a physical joint.
//! Author: Kat'Oun
//! version: 1.0
class ENGINE_PUBLIC_EXPORT Joint: public engine::Object
{
public:

	Joint();
	Joint(const std::string& name);

	virtual ~Joint();

	//! Gets joint type.
	const JointType& getJointType() const;

	void setActors(Body* body1, Body* body2);

	virtual void setAnchor(const core::vector3d& anchor);

	virtual void setAxis(const core::vector3d& axis);

protected:

	// Incremented count for next index
	static unsigned int msNextGeneratedJointIndex;

	JointType mJointType;

	Body* mActor1;
	Body* mActor2;

	core::vector3d mAnchor;
	core::vector3d mAxis;
};

//////////////////////////////////////////////////////////////////////////
//Spherical Joint
class ENGINE_PUBLIC_EXPORT SphericalJoint: public Joint
{
public:

	SphericalJoint();
	SphericalJoint(const std::string& name);

	virtual ~SphericalJoint();

	virtual void setLimits(float coneLimit, float twistLimit);

protected:

	float mConeLimit; // The maximum limit of rotational movement (radians)
	float mTwistLimit; // The maximum limit of twisting movement (radians)
};

//////////////////////////////////////////////////////////////////////////
//Hinge Joint
class ENGINE_PUBLIC_EXPORT HingeJoint: public Joint
{
public:

	HingeJoint();
	HingeJoint(const std::string& name);

	virtual ~HingeJoint();

	virtual void setLimits(float lowerLimit, float upperLimit);

protected:

	float mLowerLimit;
	float mUpperLimit;
};

//////////////////////////////////////////////////////////////////////////
//Slider Joint
class ENGINE_PUBLIC_EXPORT SliderJoint: public Joint
{
public:

	SliderJoint();
	SliderJoint(const std::string& name);

	virtual ~SliderJoint();
};

//////////////////////////////////////////////////////////////////////////
//Generic Joint
class ENGINE_PUBLIC_EXPORT GenericJoint: public Joint
{
public:

	GenericJoint();
	GenericJoint(const std::string& name);

	virtual ~GenericJoint();
};

//////////////////////////////////////////////////////////////////////////
//Rigid Joint
class ENGINE_PUBLIC_EXPORT RigidJoint: public Joint
{
public:

	RigidJoint();
	RigidJoint(const std::string& name);

	virtual ~RigidJoint();
};

} // end namespace physics

#endif
