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

	mActor1 = nullptr;
	mActor2 = nullptr;

	mAnchor = core::vector3d::ORIGIN_3D;
	mAxis = core::vector3d::NEGATIVE_UNIT_Y;
}

Joint::Joint(const std::string& name): engine::Object(name)
{
	mObjectType = engine::OT_JOINT;
	mJointType = JOINT_TYPE_UNDEFINED;

	mActor1 = nullptr;
	mActor2 = nullptr;

	mAnchor = core::vector3d::ORIGIN_3D;
	mAxis = core::vector3d::NEGATIVE_UNIT_Y;
}

Joint::~Joint()
{
	if (mActor1 != nullptr)
		mActor1->setJoint(nullptr);
	if (mActor2 != nullptr)
		mActor2->setJoint(nullptr);
}

void Joint::setActors(Body* actor1, Body* actor2)
{
	mActor1 = actor1;
	mActor2 = actor2;

	if (mActor1 != nullptr)
		mActor1->setJoint(this);
	if (mActor2 != nullptr)
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
