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

#ifndef _JOINT_H_
#define _JOINT_H_

#include <core/Config.h>
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
