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

#include <physics/Body.h>
#include <physics/BodyData.h>
#include <physics/Material.h>
#include <physics/Joint.h>
#include <physics/PhysicsManager.h>
#include <resource/ResourceEvent.h>
#include <resource/ResourceManager.h>
#include <core/Utils.h>

namespace physics
{

unsigned int Body::msNextGeneratedActorIndex = 0;

Body::Body(BodyData* bodyData): scene::Node("Body_" + core::intToString(msNextGeneratedActorIndex++))
{
	mNodeType = scene::NT_BODY;

	mBodyType = BT_STATIC;

	mSleeping = false;

	mSleepiness = 0.0f;

	mMass = 0.0f;

	mLinearDamping = 0.0f;

	mAngularDamping = 0.0f;

	mLinearVelocity = core::vector3d::ORIGIN_3D;

	mAngularVelocity = core::vector3d::ORIGIN_3D;

	mMaterial = NULL;

	mEnabled = false;

	mForce = core::vector3d::ORIGIN_3D;
	mTorque = core::vector3d::ORIGIN_3D;
	mLinearImpulse = core::vector3d::ORIGIN_3D;
	mAngularImpulse = core::vector3d::ORIGIN_3D;

	mJoint = NULL;

	assert(bodyData != NULL);
	if (bodyData == NULL)
		return;

	mBodyData = bodyData;
	mBodyData->addResourceEventReceiver(this);
}

Body::Body(const std::string& name, BodyData* bodyData): scene::Node(name)
{
	mNodeType = scene::NT_BODY;

	mBodyType = BT_STATIC;

	mSleeping = false;

	mSleepiness = 0.0f;

	mMass = 0.0f;

	mLinearDamping = 0.0f;

	mAngularDamping = 0.0f;

	mLinearVelocity = core::vector3d::ORIGIN_3D;

	mAngularVelocity = core::vector3d::ORIGIN_3D;

	mMaterial = NULL;

	mEnabled = false;

	mForce = core::vector3d::ORIGIN_3D;
	mTorque = core::vector3d::ORIGIN_3D;
	mLinearImpulse = core::vector3d::ORIGIN_3D;
	mAngularImpulse = core::vector3d::ORIGIN_3D;

	mJoint = NULL;

	assert(bodyData != NULL);
	if (bodyData == NULL)
		return;

	mBodyData = bodyData;
	mBodyData->addResourceEventReceiver(this);
}

Body::~Body()
{
	if (mBodyData != NULL)
	{
		mBodyData->removeResourceEventReceiver(this);
	}
}

void Body::setBodyType(BodyType type)
{
	mBodyType = type;
}

BodyType Body::getBodyType()
{
	return mBodyType;
}

void Body::setBodyData(const std::string& filename)
{
	BodyData* newBodyData = static_cast<BodyData*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_BODY_DATA, filename));
	assert(newBodyData != NULL);
	if (newBodyData == NULL)
		return;

	if (mBodyData != NULL)
	{
		mBodyData->removeResourceEventReceiver(this);
	}

	uninitialize();

	mBodyData = newBodyData;
	mBodyData->addResourceEventReceiver(this);
}

void Body::setBodyData(BodyData* bodyData)
{
	assert(bodyData != NULL);
	if (bodyData == NULL)
		return;

	if (mBodyData != NULL)
	{
		mBodyData->removeResourceEventReceiver(this);
	}

	uninitialize();

	mBodyData = bodyData;
	mBodyData->addResourceEventReceiver(this);
}

BodyData* Body::getBodyData() const
{
	return mBodyData;
}

void Body::setMaterial(const std::string& filename)
{
	std::string materialname = filename;
	mMaterial = PhysicsManager::getInstance().createMaterial(materialname);

	if (mMaterial != NULL)
	{
		mMaterial->addResourceEventReceiver(this);
	}
}

void Body::setMaterial(Material* material)
{
	if (material == NULL)
		return;

	mMaterial = material;

	if (mMaterial != NULL)
	{
		mMaterial->addResourceEventReceiver(this);
	}
}

Material* Body::getMaterial() const
{
	return mMaterial;
}

void Body::setEnabled(bool enabled)
{
	mEnabled = enabled;
}

bool Body::isEnabled() const
{
	return mEnabled;
}

void Body::setSleeping(bool sleeping)
{
	mSleeping = sleeping;
}

bool Body::isSleeping() const
{
	return mSleeping;
}

void Body::setSleepiness(float sleepiness)
{
	mSleepiness = sleepiness;
}

float Body::getSleepiness() const
{
	return mSleepiness;
}

void Body::setMass(float mass)
{
	mMass = mass;
}

float Body::getMass() const
{
	return mMass;
}

void Body::setLinearDamping(float linearDamping)
{
	assert(linearDamping >= 0.0f);

	mLinearDamping = linearDamping;
}

float Body::getLinearDamping() const
{
	return mLinearDamping;
}

void Body::setAngularDamping(float angularDamping)
{
	assert(angularDamping >= 0.0f);

	mAngularDamping = angularDamping;
}

float Body::getAngularDamping() const
{
	return mAngularDamping;
}

void Body::setLinearVelocity(const core::vector3d& velocity)
{
	mLinearVelocity = velocity;
}

const core::vector3d Body::getLinearVelocity()
{
	return mLinearVelocity;
}

void Body::setAngularVelocity(const core::vector3d& velocity)
{
	mAngularVelocity = velocity;
}

const core::vector3d Body::getAngularVelocity()
{
	return mAngularVelocity;
}

void Body::applyForce(const core::vector3d& force)
{
	mForce = mOrientation * force;
}

void Body::applyTorque(const core::vector3d& torque)
{
	mTorque = mOrientation * torque;
}

void Body::applyLinearImpulse(const core::vector3d& linearImpulse)
{
	mLinearImpulse = mOrientation * linearImpulse;
}

void Body::applyAngularImpulse(const core::vector3d& angularImpulse)
{
	mAngularImpulse = mOrientation * angularImpulse;
}

void Body::setJoint(Joint* joint)
{
	mJoint = joint;
}

void Body::resourceLoaded(const resource::ResourceEvent& evt)
{
	if (evt.source != NULL)
	{
		if (evt.source == mBodyData)
		{
			if (mMaterial == NULL && mBodyData->getMaterial() != NULL)
			{
				mMaterial = mBodyData->getMaterial();

				if (mMaterial != NULL)
				{
					mMaterial->addResourceEventReceiver(this);
				}
			}

			mBodyType = mBodyData->getBodyType();

			mSleeping = mBodyData->getSleeping();
			mSleepiness = mBodyData->getSleepiness();

			mMass = mBodyData->getMass();

			mLinearDamping = mBodyData->getLinearDamping();
			mAngularDamping = mBodyData->getAngularDamping();

			mLinearVelocity = mBodyData->getLinearVelocity();
			mAngularVelocity = mBodyData->getAngularVelocity();
		
			initialize();

			if (mJoint != NULL)
				mJoint->initialize();
		}

		if (evt.source == mMaterial)
		{
			initialize();

			if (mJoint != NULL)
				mJoint->initialize();
		}
	}
}

void Body::resourceUnloaded(const resource::ResourceEvent& evt)
{
	if (evt.source && (evt.source == mBodyData || evt.source == mMaterial))
	{
		mBodyType = BT_STATIC;

		mSleeping = false;

		mSleepiness = 0.0f;

		mMass = 0.0f;

		mLinearDamping = 0.0f;

		mAngularDamping = 0.0f;

		mLinearVelocity = core::vector3d::ORIGIN_3D;

		mAngularVelocity = core::vector3d::ORIGIN_3D;

		mMaterial = NULL;

		mEnabled = false;

		mForce = core::vector3d::ORIGIN_3D;
		mTorque = core::vector3d::ORIGIN_3D;
		mLinearImpulse = core::vector3d::ORIGIN_3D;
		mAngularImpulse = core::vector3d::ORIGIN_3D;

		uninitialize();
	}
}

void Body::updateTransformImpl()
{
	Node::updateTransformImpl();
}

} // end namespace physics
