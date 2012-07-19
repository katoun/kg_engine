/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

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

#include <platform/PlatformManager.h>
#include <BulletJoint.h>
#include <BulletBody.h>
#include <BulletPhysicsDriver.h>

namespace physics
{

/////////////////////////////////////////////////////////////////////////
//Spherical Joint
BulletSphericalJoint::BulletSphericalJoint(): SphericalJoint()
{
	mSphericalConstraint = NULL;
}

BulletSphericalJoint::BulletSphericalJoint(const std::string& name): SphericalJoint(name)
{
	mSphericalConstraint = NULL;
}

BulletSphericalJoint::~BulletSphericalJoint()
{
	btDynamicsWorld* pDynamicsWorld = BulletPhysicsDriver::getInstance().getDynamicsWorld();

	assert(pDynamicsWorld != NULL);
	if (pDynamicsWorld == NULL)
		return;

	if (mSphericalConstraint)
	{
		pDynamicsWorld->removeConstraint(mSphericalConstraint);
		delete mSphericalConstraint;
	}
}

void BulletSphericalJoint::initializeImpl()
{
	if ((mActor1 && mActor1->isInitialized() && mActor2 && mActor2->isInitialized()) ||
		(mActor1 && mActor1->isInitialized()) ||
		(mActor2 && mActor2->isInitialized()))
	{
		btDynamicsWorld* pDynamicsWorld = BulletPhysicsDriver::getInstance().getDynamicsWorld();

		assert(pDynamicsWorld != NULL);
		if (pDynamicsWorld == NULL)
			return;

		BulletBody* bulletBody1 = static_cast<BulletBody*>(mActor1);
		BulletBody* bulletBody2 = static_cast<BulletBody*>(mActor2);

		btVector3 pivotInA(mAnchor.X, mAnchor.Y, mAnchor.Z);
		btVector3 pivotInB = bulletBody1->getBulletRigidBody()->getCenterOfMassTransform().inverse()(bulletBody2->getBulletRigidBody()->getCenterOfMassTransform()(pivotInA));

		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInB = btTransform::getIdentity();
		frameInA.setOrigin(pivotInA);
		frameInB.setOrigin(pivotInB);

		mSphericalConstraint = new btGeneric6DofConstraint(*(bulletBody1->getBulletRigidBody()), *(bulletBody2->getBulletRigidBody()), frameInA, frameInB, true);

		pDynamicsWorld->addConstraint(mSphericalConstraint, true);
	}
}

void BulletSphericalJoint::setLimits(float coneLimit, float twistLimit)
{
	if (mSphericalConstraint == NULL)
		return;

	SphericalJoint::setLimits(coneLimit, twistLimit);

	btVector3 limit(coneLimit,coneLimit,twistLimit);
	mSphericalConstraint->setAngularLowerLimit(-limit);
	mSphericalConstraint->setAngularUpperLimit(limit);
}

//////////////////////////////////////////////////////////////////////////
//Hinge Joint
BulletHingeJoint::BulletHingeJoint(): HingeJoint()
{
	mHingeConstraint = NULL;
}

BulletHingeJoint::BulletHingeJoint(const std::string& name): HingeJoint(name)
{
	mHingeConstraint = NULL;
}

BulletHingeJoint::~BulletHingeJoint()
{
	btDynamicsWorld* pDynamicsWorld = BulletPhysicsDriver::getInstance().getDynamicsWorld();

	assert(pDynamicsWorld != NULL);
	if (pDynamicsWorld == NULL)
		return;

	if (mHingeConstraint)
	{
		pDynamicsWorld->removeConstraint(mHingeConstraint);
		delete mHingeConstraint;
	}
}

void BulletHingeJoint::initializeImpl()
{
	if ((mActor1 && mActor1->isInitialized() && mActor2 && mActor2->isInitialized()) ||
		(mActor1 && mActor1->isInitialized()) ||
		(mActor2 && mActor2->isInitialized()))
	{
		btDynamicsWorld* pDynamicsWorld = BulletPhysicsDriver::getInstance().getDynamicsWorld();

		assert(pDynamicsWorld != NULL);
		if (pDynamicsWorld == NULL)
			return;

		BulletBody* bulletBody1 = static_cast<BulletBody*>(mActor1);
		BulletBody* bulletBody2 = static_cast<BulletBody*>(mActor2);

		btVector3 pivotInA(mAnchor.X, mAnchor.Y, mAnchor.Z);
		btVector3 pivotInB = bulletBody1->getBulletRigidBody()->getCenterOfMassTransform().inverse()(bulletBody2->getBulletRigidBody()->getCenterOfMassTransform()(pivotInA));

		btVector3 axisInA(mAxis.X, mAxis.Y, mAxis.Z);
		btVector3 axisInB(mAxis.X, mAxis.Y, mAxis.Z);

		mHingeConstraint = new btHingeConstraint(*(bulletBody1->getBulletRigidBody()), *(bulletBody2->getBulletRigidBody()), pivotInA, pivotInB, axisInA, axisInB);

		pDynamicsWorld->addConstraint(mHingeConstraint, true);
	}
}

void BulletHingeJoint::setLimits(float lowerLimit, float upperLimit)
{
	if (mHingeConstraint == NULL)
		return;

	HingeJoint::setLimits(lowerLimit, upperLimit);

	mHingeConstraint->setLimit(lowerLimit, upperLimit);
}

//////////////////////////////////////////////////////////////////////////
//Slider Joint
BulletSliderJoint::BulletSliderJoint(): SliderJoint()
{
	mSliderConstraint = NULL;
}

BulletSliderJoint::BulletSliderJoint(const std::string& name): SliderJoint(name)
{
	mSliderConstraint = NULL;
}

BulletSliderJoint::~BulletSliderJoint()
{
	btDynamicsWorld* pDynamicsWorld = BulletPhysicsDriver::getInstance().getDynamicsWorld();

	assert(pDynamicsWorld != NULL);
	if (pDynamicsWorld == NULL)
		return;

	if (mSliderConstraint)
	{
		pDynamicsWorld->removeConstraint(mSliderConstraint);
		delete mSliderConstraint;
	}
}

void BulletSliderJoint::initializeImpl()
{
	if ((mActor1 && mActor1->isInitialized() && mActor2 && mActor2->isInitialized()) ||
		(mActor1 && mActor1->isInitialized()) ||
		(mActor2 && mActor2->isInitialized()))
	{
		btDynamicsWorld* pDynamicsWorld = BulletPhysicsDriver::getInstance().getDynamicsWorld();

		assert(pDynamicsWorld != NULL);
		if (pDynamicsWorld == NULL)
			return;

		BulletBody* bulletBody1 = static_cast<BulletBody*>(mActor1);
		BulletBody* bulletBody2 = static_cast<BulletBody*>(mActor2);

		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInB = btTransform::getIdentity();

		mSliderConstraint = new btSliderConstraint(*(bulletBody1->getBulletRigidBody()), *(bulletBody2->getBulletRigidBody()), frameInA, frameInB, true);

		//Constraint the angular movement
		mSliderConstraint->setLowerAngLimit(0.0f);
		mSliderConstraint->setUpperAngLimit(0.0f);

		pDynamicsWorld->addConstraint(mSliderConstraint);
	}
}

/////////////////////////////////////////////////////////////////////////
//Generic Joint
BulletGenericJoint::BulletGenericJoint(): GenericJoint()
{
	mGenericConstraint = NULL;
}

BulletGenericJoint::BulletGenericJoint(const std::string& name): GenericJoint(name)
{
	mGenericConstraint = NULL;
}

BulletGenericJoint::~BulletGenericJoint()
{
	btDynamicsWorld* pDynamicsWorld = BulletPhysicsDriver::getInstance().getDynamicsWorld();

	assert(pDynamicsWorld != NULL);
	if (pDynamicsWorld == NULL)
		return;

	if (mGenericConstraint)
	{
		pDynamicsWorld->removeConstraint(mGenericConstraint);
		delete mGenericConstraint;
	}
}

void BulletGenericJoint::initializeImpl()
{
	if ((mActor1 && mActor1->isInitialized() && mActor2 && mActor2->isInitialized()) ||
		(mActor1 && mActor1->isInitialized()) ||
		(mActor2 && mActor2->isInitialized()))
	{
		btDynamicsWorld* pDynamicsWorld = BulletPhysicsDriver::getInstance().getDynamicsWorld();

		assert(pDynamicsWorld != NULL);
		if (pDynamicsWorld == NULL)
			return;

		BulletBody* bulletBody1 = static_cast<BulletBody*>(mActor1);
		BulletBody* bulletBody2 = static_cast<BulletBody*>(mActor2);

		btVector3 pivotInA(mAnchor.X, mAnchor.Y, mAnchor.Z);
		btVector3 pivotInB = bulletBody1->getBulletRigidBody()->getCenterOfMassTransform().inverse()(bulletBody2->getBulletRigidBody()->getCenterOfMassTransform()(pivotInA));

		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInB = btTransform::getIdentity();
		frameInA.setOrigin(pivotInA);
		frameInB.setOrigin(pivotInB);

		mGenericConstraint = new btGeneric6DofConstraint(*(bulletBody1->getBulletRigidBody()), *(bulletBody2->getBulletRigidBody()), frameInA, frameInB, true);

		pDynamicsWorld->addConstraint(mGenericConstraint);
	}
}

//////////////////////////////////////////////////////////////////////////
//Rigid Joint
BulletRigidJoint::BulletRigidJoint(): RigidJoint()
{
	mRigidConstraint = NULL;
}

BulletRigidJoint::BulletRigidJoint(const std::string& name): RigidJoint(name)
{
	mRigidConstraint = NULL;
}

BulletRigidJoint::~BulletRigidJoint()
{
	btDynamicsWorld* pDynamicsWorld = BulletPhysicsDriver::getInstance().getDynamicsWorld();

	assert(pDynamicsWorld != NULL);
	if (pDynamicsWorld == NULL)
		return;

	if (mRigidConstraint)
	{
		pDynamicsWorld->removeConstraint(mRigidConstraint);
		delete mRigidConstraint;
	}
}

void BulletRigidJoint::initializeImpl()
{
	if ((mActor1 && mActor1->isInitialized() && mActor2 && mActor2->isInitialized()) ||
		(mActor1 && mActor1->isInitialized()) ||
		(mActor2 && mActor2->isInitialized()))
	{
		btDynamicsWorld* pDynamicsWorld = BulletPhysicsDriver::getInstance().getDynamicsWorld();

		assert(pDynamicsWorld != NULL);
		if (pDynamicsWorld == NULL)
			return;

		BulletBody* bulletBody1 = static_cast<BulletBody*>(mActor1);
		BulletBody* bulletBody2 = static_cast<BulletBody*>(mActor2);

		btVector3 pivotInA(mAnchor.X, mAnchor.Y, mAnchor.Z);
		btVector3 pivotInB = bulletBody1->getBulletRigidBody()->getCenterOfMassTransform().inverse()(bulletBody2->getBulletRigidBody()->getCenterOfMassTransform()(pivotInA));

		btVector3 axisInA(mAxis.X, mAxis.Y, mAxis.Z);
		btVector3 axisInB(mAxis.X, mAxis.Y, mAxis.Z);

		mRigidConstraint = new btHingeConstraint(*(bulletBody1->getBulletRigidBody()), *(bulletBody2->getBulletRigidBody()), pivotInA, pivotInB, axisInA, axisInB);

		pDynamicsWorld->addConstraint(mRigidConstraint, true);

		const float TOLERANCE = 0.01f;

		// Bullet can get into weird states with angles exactly at its boundaries (PI and -PI) if the limits are exactly equal.
		btScalar angle = mRigidConstraint->getHingeAngle();
		btScalar lowerLimit = angle - TOLERANCE;
		btScalar upperLimit = angle + TOLERANCE;

		// clamp it to make sure it's in the valid range for Bullet
		if (lowerLimit < -SIMD_PI)
			lowerLimit = -SIMD_PI;
		// clamp it to make sure it's in the valid range for Bullet
		if (upperLimit > SIMD_PI)
			upperLimit = SIMD_PI;

		mRigidConstraint->setLimit(lowerLimit, upperLimit);
	}
}

} // end namespace game