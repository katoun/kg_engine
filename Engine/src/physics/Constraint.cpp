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

#include <physics/Constraint.h>
#include <physics/Body.h>
#include <physics/PhysicsManager.h>
#include <core/Utils.h>
#include <core/Math.h>

#include <btBulletDynamicsCommon.h>

namespace physics
{

unsigned int Constraint::mIndexCounter = 0;

Constraint::Constraint()
{
	mID = mIndexCounter++;

	mConstraintType = CONSTRAINT_TYPE_UNDEFINED;

	mBody1 = nullptr;
	mBody2 = nullptr;

	mAnchor = glm::vec3(0, 0, 0);
	mAxis = glm::vec3(0, 1, 0);
}

Constraint::~Constraint()
{
	if (mBody1 != nullptr)
		mBody1->setConstraint(nullptr);
	if (mBody2 != nullptr)
		mBody2->setConstraint(nullptr);
}

const unsigned int& Constraint::getID() const
{
	return mID;
}

void Constraint::setBodies(Body* body1, Body* body2)
{
	mBody1 = body1;
	mBody2 = body2;

	if (mBody1 != nullptr)
		mBody1->setConstraint(this);
	if (mBody2 != nullptr)
		mBody2->setConstraint(this);
}

const ConstraintType& Constraint::getConstraintType() const
{
	return mConstraintType;
}

void Constraint::setAnchor(const glm::vec3& anchor)
{
	mAnchor = anchor;
}

void Constraint::setAxis(const glm::vec3& axis)
{
	mAxis = axis;
}

void Constraint::initialize()
{
	initializeImpl();
}

void Constraint::uninitialize()
{
	uninitializeImpl();
}

void Constraint::initializeImpl() {}

void Constraint::uninitializeImpl() {}

//////////////////////////////////////////////////////////////////////////
//Spherical Constraint
SphericalConstraint::SphericalConstraint(): Constraint()
{
	mConstraintType = CONSTRAINT_TYPE_SPHERICAL;

	mSphericalConstraint = nullptr;

	mConeLimit = 0.0f;
	mTwistLimit = 0.0f;
}

SphericalConstraint::~SphericalConstraint()
{
	btDynamicsWorld* pDynamicsWorld = nullptr;
	if (PhysicsManager::getInstance() != nullptr)
		pDynamicsWorld = PhysicsManager::getInstance()->getDynamicsWorld();

	assert(pDynamicsWorld != nullptr);
	if (pDynamicsWorld == nullptr)
		return;

	if (mSphericalConstraint)
	{
		pDynamicsWorld->removeConstraint(mSphericalConstraint);
	}
	SAFE_DELETE(mSphericalConstraint);
}

void SphericalConstraint::setLimits(float coneLimit, float twistLimit)
{
	mConeLimit = coneLimit;
	mTwistLimit = twistLimit;

	if (mSphericalConstraint == nullptr)
		return;

	SphericalConstraint::setLimits(coneLimit, twistLimit);

	btVector3 limit(coneLimit,coneLimit,twistLimit);
	mSphericalConstraint->setAngularLowerLimit(-limit);
	mSphericalConstraint->setAngularUpperLimit(limit);
}

void SphericalConstraint::initializeImpl()
{
	if ((mBody1 != nullptr && mBody1->isInitialized() && mBody2 != nullptr && mBody2->isInitialized()) ||
		(mBody1 != nullptr && mBody1->isInitialized()) ||
		(mBody2 != nullptr && mBody2->isInitialized()))
	{
		btDynamicsWorld* pDynamicsWorld = nullptr;
		if (PhysicsManager::getInstance() != nullptr)
			pDynamicsWorld = PhysicsManager::getInstance()->getDynamicsWorld();

		assert(pDynamicsWorld != nullptr);
		if (pDynamicsWorld == nullptr)
			return;

		btVector3 pivotInA(mAnchor.x, mAnchor.y, mAnchor.z);
		btVector3 pivotInB = mBody1->getBulletRigidBody()->getCenterOfMassTransform().inverse()(mBody2->getBulletRigidBody()->getCenterOfMassTransform()(pivotInA));

		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInB = btTransform::getIdentity();
		frameInA.setOrigin(pivotInA);
		frameInB.setOrigin(pivotInB);

		mSphericalConstraint = new btGeneric6DofConstraint(*(mBody1->getBulletRigidBody()), *(mBody2->getBulletRigidBody()), frameInA, frameInB, true);

		pDynamicsWorld->addConstraint(mSphericalConstraint, true);
	}
}

//////////////////////////////////////////////////////////////////////////
//Hinge Constraint
HingeConstraint::HingeConstraint(): Constraint()
{
	mConstraintType = CONSTRAINT_TYPE_HINGE;

	mHingeConstraint = nullptr;

	mLowerLimit = -core::FLOAT_MAX;
	mUpperLimit = core::FLOAT_MAX;
}

HingeConstraint::~HingeConstraint()
{
	btDynamicsWorld* pDynamicsWorld = nullptr;
	if (PhysicsManager::getInstance() != nullptr)
		pDynamicsWorld = PhysicsManager::getInstance()->getDynamicsWorld();

	assert(pDynamicsWorld != nullptr);
	if (pDynamicsWorld == nullptr)
		return;

	if (mHingeConstraint)
	{
		pDynamicsWorld->removeConstraint(mHingeConstraint);
	}
	SAFE_DELETE(mHingeConstraint);
}

void HingeConstraint::setLimits(float lowerLimit, float upperLimit)
{
	mLowerLimit = lowerLimit;
	mUpperLimit = upperLimit;

	if (mHingeConstraint == nullptr)
		return;

	HingeConstraint::setLimits(lowerLimit, upperLimit);

	mHingeConstraint->setLimit(lowerLimit, upperLimit);
}

void HingeConstraint::initializeImpl()
{
	if ((mBody1 != nullptr && mBody1->isInitialized() && mBody2 != nullptr && mBody2->isInitialized()) ||
		(mBody1 != nullptr && mBody1->isInitialized()) ||
		(mBody2 != nullptr && mBody2->isInitialized()))
	{
		btDynamicsWorld* pDynamicsWorld = nullptr;
		if (PhysicsManager::getInstance() != nullptr)
			pDynamicsWorld = PhysicsManager::getInstance()->getDynamicsWorld();

		assert(pDynamicsWorld != nullptr);
		if (pDynamicsWorld == nullptr)
			return;

		btVector3 pivotInA(mAnchor.x, mAnchor.y, mAnchor.z);
		btVector3 pivotInB = mBody1->getBulletRigidBody()->getCenterOfMassTransform().inverse()(mBody2->getBulletRigidBody()->getCenterOfMassTransform()(pivotInA));

		btVector3 axisInA(mAxis.x, mAxis.y, mAxis.z);
		btVector3 axisInB(mAxis.x, mAxis.y, mAxis.z);

		mHingeConstraint = new btHingeConstraint(*(mBody1->getBulletRigidBody()), *(mBody2->getBulletRigidBody()), pivotInA, pivotInB, axisInA, axisInB);

		pDynamicsWorld->addConstraint(mHingeConstraint, true);
	}
}

//////////////////////////////////////////////////////////////////////////
//Slider Constraint
SliderConstraint::SliderConstraint(): Constraint()
{
	mConstraintType = CONSTRAINT_TYPE_SLIDER;

	mSliderConstraint = nullptr;
}

SliderConstraint::~SliderConstraint()
{
	btDynamicsWorld* pDynamicsWorld = nullptr;
	if (PhysicsManager::getInstance() != nullptr)
		pDynamicsWorld = PhysicsManager::getInstance()->getDynamicsWorld();

	assert(pDynamicsWorld != nullptr);
	if (pDynamicsWorld == nullptr)
		return;

	if (mSliderConstraint)
	{
		pDynamicsWorld->removeConstraint(mSliderConstraint);
	}
	SAFE_DELETE(mSliderConstraint);
}

void SliderConstraint::initializeImpl()
{
	if ((mBody1 != nullptr && mBody1->isInitialized() && mBody2 != nullptr && mBody2->isInitialized()) ||
		(mBody1 != nullptr && mBody1->isInitialized()) ||
		(mBody2 != nullptr && mBody2->isInitialized()))
	{
		btDynamicsWorld* pDynamicsWorld = nullptr;
		if (PhysicsManager::getInstance() != nullptr)
			pDynamicsWorld = PhysicsManager::getInstance()->getDynamicsWorld();

		assert(pDynamicsWorld != nullptr);
		if (pDynamicsWorld == nullptr)
			return;

		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInB = btTransform::getIdentity();

		mSliderConstraint = new btSliderConstraint(*(mBody1->getBulletRigidBody()), *(mBody2->getBulletRigidBody()), frameInA, frameInB, true);

		//Constraint the angular movement
		mSliderConstraint->setLowerAngLimit(0.0f);
		mSliderConstraint->setUpperAngLimit(0.0f);

		pDynamicsWorld->addConstraint(mSliderConstraint);
	}
}

//////////////////////////////////////////////////////////////////////////
//Generic Constraint
GenericConstraint::GenericConstraint(): Constraint()
{
	mConstraintType = CONSTRAINT_TYPE_GENERIC;

	mGenericConstraint = nullptr;
}

GenericConstraint::~GenericConstraint()
{
	btDynamicsWorld* pDynamicsWorld = nullptr;
	if (PhysicsManager::getInstance() != nullptr)
		pDynamicsWorld = PhysicsManager::getInstance()->getDynamicsWorld();

	assert(pDynamicsWorld != nullptr);
	if (pDynamicsWorld == nullptr)
		return;

	if (mGenericConstraint)
	{
		pDynamicsWorld->removeConstraint(mGenericConstraint);
	}
	SAFE_DELETE(mGenericConstraint);
}

void GenericConstraint::initializeImpl()
{
	if ((mBody1 != nullptr && mBody1->isInitialized() && mBody2 != nullptr && mBody2->isInitialized()) ||
		(mBody1 != nullptr && mBody1->isInitialized()) ||
		(mBody2 != nullptr && mBody2->isInitialized()))
	{
		btDynamicsWorld* pDynamicsWorld = nullptr;
		if (PhysicsManager::getInstance() != nullptr)
			pDynamicsWorld = PhysicsManager::getInstance()->getDynamicsWorld();

		assert(pDynamicsWorld != nullptr);
		if (pDynamicsWorld == nullptr)
			return;

		btVector3 pivotInA(mAnchor.x, mAnchor.y, mAnchor.z);
		btVector3 pivotInB = mBody1->getBulletRigidBody()->getCenterOfMassTransform().inverse()(mBody2->getBulletRigidBody()->getCenterOfMassTransform()(pivotInA));

		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInB = btTransform::getIdentity();
		frameInA.setOrigin(pivotInA);
		frameInB.setOrigin(pivotInB);

		mGenericConstraint = new btGeneric6DofConstraint(*(mBody1->getBulletRigidBody()), *(mBody2->getBulletRigidBody()), frameInA, frameInB, true);

		pDynamicsWorld->addConstraint(mGenericConstraint);
	}
}

//////////////////////////////////////////////////////////////////////////
//Rigid Constraint
RigidConstraint::RigidConstraint(): Constraint()
{
	mConstraintType = CONSTRAINT_TYPE_RIGID;

	mRigidConstraint = nullptr;
}

RigidConstraint::~RigidConstraint()
{
	btDynamicsWorld* pDynamicsWorld = nullptr;
	if (PhysicsManager::getInstance() != nullptr)
		pDynamicsWorld = PhysicsManager::getInstance()->getDynamicsWorld();

	assert(pDynamicsWorld != nullptr);
	if (pDynamicsWorld == nullptr)
		return;

	if (mRigidConstraint)
	{
		pDynamicsWorld->removeConstraint(mRigidConstraint);
	}
	SAFE_DELETE(mRigidConstraint);
}

void RigidConstraint::initializeImpl()
{
	if ((mBody1 != nullptr && mBody1->isInitialized() && mBody2 != nullptr && mBody2->isInitialized()) ||
		(mBody1 != nullptr && mBody1->isInitialized()) ||
		(mBody2 != nullptr && mBody2->isInitialized()))
	{
		btDynamicsWorld* pDynamicsWorld = nullptr;
		if (PhysicsManager::getInstance() != nullptr)
			pDynamicsWorld = PhysicsManager::getInstance()->getDynamicsWorld();

		assert(pDynamicsWorld != nullptr);
		if (pDynamicsWorld == nullptr)
			return;

		btVector3 pivotInA(mAnchor.x, mAnchor.y, mAnchor.z);
		btVector3 pivotInB = mBody1->getBulletRigidBody()->getCenterOfMassTransform().inverse()(mBody2->getBulletRigidBody()->getCenterOfMassTransform()(pivotInA));

		btVector3 axisInA(mAxis.x, mAxis.y, mAxis.z);
		btVector3 axisInB(mAxis.x, mAxis.y, mAxis.z);

		mRigidConstraint = new btHingeConstraint(*(mBody1->getBulletRigidBody()), *(mBody2->getBulletRigidBody()), pivotInA, pivotInB, axisInA, axisInB);

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

} // end namespace physics
