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

#include <physics/Joint.h>
#include <physics/Body.h>
#include <platform/PlatformManager.h>
#include <core/Utils.h>
#include <core/Math.h>

namespace physics
{

unsigned int Joint::msNextGeneratedJointIndex = 0;

Joint::Joint(): engine::Object("Joint_" + core::intToString(msNextGeneratedJointIndex++))
{
	mObjectType = engine::OT_JOINT;
	mJointType = JOINT_TYPE_UNDEFINED;

	mActor1 = NULL;
	mActor2 = NULL;

	mAnchor = core::vector3d::ORIGIN_3D;
	mAxis = core::vector3d::NEGATIVE_UNIT_Y;
}

Joint::Joint(const std::string& name): engine::Object(name)
{
	mObjectType = engine::OT_JOINT;
	mJointType = JOINT_TYPE_UNDEFINED;

	mActor1 = NULL;
	mActor2 = NULL;

	mAnchor = core::vector3d::ORIGIN_3D;
	mAxis = core::vector3d::NEGATIVE_UNIT_Y;
}

Joint::~Joint()
{
	if (mActor1 != NULL)
		mActor1->setJoint(NULL);
	if (mActor2 != NULL)
		mActor2->setJoint(NULL);
}

void Joint::setActors(Body* actor1, Body* actor2)
{
	mActor1 = actor1;
	mActor2 = actor2;

	if (mActor1 != NULL)
		mActor1->setJoint(this);
	if (mActor2 != NULL)
		mActor2->setJoint(this);
}

const JointType& Joint::getJointType() const
{
	return mJointType;
}

void Joint::setAnchor(const core::vector3d& anchor)
{
	mAnchor = anchor;
}

void Joint::setAxis(const core::vector3d& axis)
{
	mAxis = axis;
}

//////////////////////////////////////////////////////////////////////////
//Spherical Joint
SphericalJoint::SphericalJoint(): Joint()
{
	mJointType = JOINT_TYPE_SPHERICAL;

	mConeLimit = 0.0f;
	mTwistLimit = 0.0f;
}

SphericalJoint::SphericalJoint(const std::string& name): Joint(name)
{
	mJointType = JOINT_TYPE_SPHERICAL;

	mConeLimit = 0.0f;
	mTwistLimit = 0.0f;
}

SphericalJoint::~SphericalJoint() {}

void SphericalJoint::setLimits(float coneLimit, float twistLimit)
{
	mConeLimit = coneLimit;
	mTwistLimit = twistLimit;
}

//////////////////////////////////////////////////////////////////////////
//Hinge Joint
HingeJoint::HingeJoint(): Joint()
{
	mJointType = JOINT_TYPE_HINGE;

	mLowerLimit = -core::FLOAT_MAX;
	mUpperLimit = core::FLOAT_MAX;
}

HingeJoint::HingeJoint(const std::string& name): Joint(name)
{
	mJointType = JOINT_TYPE_HINGE;

	mLowerLimit = -core::FLOAT_MAX;
	mUpperLimit = core::FLOAT_MAX;
}

HingeJoint::~HingeJoint() {}

void HingeJoint::setLimits(float lowerLimit, float upperLimit)
{
	mLowerLimit = lowerLimit;
	mUpperLimit = upperLimit;
}

//////////////////////////////////////////////////////////////////////////
//Slider Joint
SliderJoint::SliderJoint(): Joint()
{
	mJointType = JOINT_TYPE_SLIDER;
}

SliderJoint::SliderJoint(const std::string& name): Joint(name)
{
	mJointType = JOINT_TYPE_SLIDER;

}

SliderJoint::~SliderJoint() {}

//////////////////////////////////////////////////////////////////////////
//Generic Joint
GenericJoint::GenericJoint(): Joint()
{
	mJointType = JOINT_TYPE_GENERIC;
}

GenericJoint::GenericJoint(const std::string& name): Joint(name)
{
	mJointType = JOINT_TYPE_GENERIC;
}

GenericJoint::~GenericJoint() {}

//////////////////////////////////////////////////////////////////////////
//Rigid Joint
RigidJoint::RigidJoint(): Joint()
{
	mJointType = JOINT_TYPE_RIGID;
}

RigidJoint::RigidJoint(const std::string& name): Joint(name)
{
	mJointType = JOINT_TYPE_RIGID;
}

RigidJoint::~RigidJoint() {}

} // end namespace physics
