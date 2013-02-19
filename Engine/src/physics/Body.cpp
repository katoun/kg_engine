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

#include <physics/Body.h>
#include <physics/BodyData.h>
#include <physics/Material.h>
#include <physics/Shape.h>
#include <physics/Constraint.h>
#include <physics/PhysicsManager.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <game/MessageDefines.h>
#include <resource/ResourceEvent.h>
#include <resource/ResourceManager.h>
#include <core/Utils.h>

#include <btBulletDynamicsCommon.h>

namespace physics
{

Body::Body(): game::Component()
{
	mType = game::COMPONENT_TYPE_BODY;

	mRigidBody = nullptr;
	mMotionState = nullptr;

	mBodyType = BT_STATIC;

	mBodyData = nullptr;

	mSleeping = false;

	mSleepiness = 0.0f;

	mMass = 0.0f;

	mLinearDamping = 0.0f;

	mAngularDamping = 0.0f;

	mLinearVelocity = glm::vec3(0, 0, 0);

	mAngularVelocity = glm::vec3(0, 0, 0);

	mMaterial = nullptr;

	mEnabled = false;

	mBodyNeedsUpdate = true;

	mForce = glm::vec3(0, 0, 0);
	mTorque = glm::vec3(0, 0, 0);
	mLinearImpulse = glm::vec3(0, 0, 0);
	mAngularImpulse = glm::vec3(0, 0, 0);

	mConstraint = nullptr;
}

Body::~Body()
{
	if (mBodyData != nullptr)
	{
		mBodyData->removeResourceEventReceiver(this);
	}

	if (mMaterial != nullptr)
	{
		mMaterial->removeResourceEventReceiver(this);
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
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		BodyData* newBodyData = static_cast<BodyData*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_BODY_DATA, filename));
		assert(newBodyData != nullptr);
		if (newBodyData == nullptr)
			return;

		if (mBodyData != nullptr)
		{
			mBodyData->removeResourceEventReceiver(this);

			uninitialize();
		}

		mBodyData = newBodyData;
		mBodyData->addResourceEventReceiver(this);
	}
}

void Body::setBodyData(BodyData* bodyData)
{
	assert(bodyData != nullptr);
	if (bodyData == nullptr)
		return;

	if (mBodyData != nullptr)
	{
		mBodyData->removeResourceEventReceiver(this);

		uninitialize();
	}

	mBodyData = bodyData;
	mBodyData->addResourceEventReceiver(this);
}

BodyData* Body::getBodyData() const
{
	return mBodyData;
}

void Body::setMaterial(const std::string& filename)
{
	Material* pMaterial = nullptr;
	if (PhysicsManager::getInstance() != nullptr)
		pMaterial = PhysicsManager::getInstance()->createMaterial(filename);

	setMaterial(pMaterial);
}

void Body::setMaterial(Material* material)
{
	if (material == nullptr)
		return;

	if (mMaterial != nullptr)
	{
		mMaterial->removeResourceEventReceiver(this);
	}

	mMaterial = material;

	if (mMaterial != nullptr)
	{
		mMaterial->addResourceEventReceiver(this);
	}

	if (mRigidBody != nullptr)
	{
		if (mMaterial != nullptr)
		{
			mRigidBody->setFriction(mMaterial->getStaticFriction());
			mRigidBody->setRestitution(mMaterial->getRestitution());
		}
	}
}

Material* Body::getMaterial() const
{
	return mMaterial;
}

void Body::setEnabled(bool enabled)
{
	mEnabled = enabled;

	if (mRigidBody != nullptr)
	{
		if (mEnabled)
		{
			mRigidBody->setActivationState(ACTIVE_TAG);
			mRigidBody->setActivationState(DISABLE_DEACTIVATION);
		}
		else
		{
			mRigidBody->setActivationState(ISLAND_SLEEPING);
		}
	}
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

void Body::setLinearVelocity(const glm::vec3& velocity)
{
	mLinearVelocity = velocity;
}

const glm::vec3 Body::getLinearVelocity()
{
	return mLinearVelocity;
}

void Body::setAngularVelocity(const glm::vec3& velocity)
{
	mAngularVelocity = velocity;
}

const glm::vec3 Body::getAngularVelocity()
{
	return mAngularVelocity;
}

void Body::applyForce(const glm::vec3& force)
{
	if (mGameObject != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			mForce = pTransform->getOrientation() * force;
		}
	}

	if (mRigidBody != nullptr)
	{
		btVector3 f(mForce.x, mForce.y, mForce.z);
		mRigidBody->applyCentralForce(f);

		mForce = glm::vec3(0, 0, 0);//Reset value after it is set
	}
}

void Body::applyTorque(const glm::vec3& torque)
{
	//TODO!!!

	if (mRigidBody == nullptr)
	{
		btVector3 t(mTorque.x, mTorque.y, mTorque.z);
		mRigidBody->applyTorque(t);
	
		mTorque = glm::vec3(0, 0, 0);//Reset value after it is set
	}
}

void Body::applyLinearImpulse(const glm::vec3& linearImpulse)
{
	if (mGameObject != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			mLinearImpulse = pTransform->getOrientation() * linearImpulse;
		}
	}

	if (mRigidBody != nullptr)
	{
		btVector3 impulse(mLinearImpulse.x, mLinearImpulse.y, mLinearImpulse.z);
		mRigidBody->applyCentralImpulse(impulse);

		mLinearImpulse = glm::vec3(0, 0, 0);//Reset value after it is set
	}
}

void Body::applyAngularImpulse(const glm::vec3& angularImpulse)
{
	if (mGameObject != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			mAngularImpulse = pTransform->getOrientation() * angularImpulse;
		}
	}

	if (mRigidBody != nullptr)
	{
		btVector3 impulse(mAngularImpulse.x, mAngularImpulse.y, mAngularImpulse.z);
		mRigidBody->applyTorqueImpulse(impulse);

		mAngularImpulse = glm::vec3(0, 0, 0);//Reset value after it is set
	}
}

void Body::setConstraint(Constraint* joint)
{
	mConstraint = joint;
}

void Body::resourceLoaded(const resource::ResourceEvent& evt)
{
	if (evt.source != nullptr)
	{
		if (evt.source == mBodyData)
		{
			if (mMaterial == nullptr && mBodyData->getMaterial() != nullptr)
			{
				mMaterial = mBodyData->getMaterial();

				if (mMaterial != nullptr)
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

			if (mConstraint != nullptr)
				mConstraint->initialize();
		}

		if (evt.source == mMaterial)
		{
			initialize();

			if (mConstraint != nullptr)
				mConstraint->initialize();
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

		mLinearVelocity = glm::vec3(0, 0, 0);

		mAngularVelocity = glm::vec3(0, 0, 0);

		mMaterial = nullptr;

		mEnabled = false;

		mForce = glm::vec3(0, 0, 0);
		mTorque = glm::vec3(0, 0, 0);
		mLinearImpulse = glm::vec3(0, 0, 0);
		mAngularImpulse = glm::vec3(0, 0, 0);

		uninitialize();
	}
}

btRigidBody* Body::getBulletRigidBody()
{
	return mRigidBody;
}

void Body::initializeImpl()
{
	if (mBodyData == nullptr)
		return;

	if (mMaterial == nullptr)
		return;

	if (mBodyData->getState() != resource::RESOURCE_STATE_LOADED)
		return;

	if (mMaterial->getState() != resource::RESOURCE_STATE_LOADED)
		return;

	if (mRigidBody != nullptr)
		return;
	
	btDynamicsWorld* pDynamicsWorld = nullptr;
	if (PhysicsManager::getInstance() != nullptr)
		pDynamicsWorld = PhysicsManager::getInstance()->getDynamicsWorld();

	assert(pDynamicsWorld != nullptr);
	if (pDynamicsWorld == nullptr)
		return;

	if (mBodyData->getShapes().size() == 0)
		return;

	std::list<Shape*>::const_iterator i = mBodyData->getShapes().begin();
	btCollisionShape* pShape = addBulletShape((*i));

	if (pShape == nullptr)
		return;

	btTransform trans;
	trans.setIdentity();
	
	if (mGameObject != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			glm::vec3 position = pTransform->getAbsolutePosition();
			glm::quat orientation = pTransform->getAbsoluteOrientation();

			trans.setOrigin(btVector3(position.x, position.y, position.z));
			trans.setRotation(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
		}
	}

	mMotionState = new btDefaultMotionState(trans);

	btVector3 localInertia(0,0,0);

	btScalar mass(mMass);

	if (mBodyType != BT_DYNAMIC)
		mass = 0;

	if (mBodyType == BT_DYNAMIC)
		pShape->calculateLocalInertia(mass, localInertia);

	mRigidBody = new btRigidBody(mass, mMotionState, pShape, localInertia);

	mRigidBody->setFriction(mMaterial->getStaticFriction());
	mRigidBody->setRestitution(mMaterial->getRestitution());

	int currFlags = mRigidBody->getCollisionFlags();

	switch (mBodyType)
	{
	case BT_DYNAMIC:
		currFlags &= (~btCollisionObject::CF_STATIC_OBJECT);
		currFlags &= (~btCollisionObject::CF_KINEMATIC_OBJECT);
		break;
	case BT_STATIC:
		currFlags |= btCollisionObject::CF_STATIC_OBJECT;
		currFlags &= (~btCollisionObject::CF_KINEMATIC_OBJECT);
		break;
	case BT_KINEMATIC:
		currFlags &= (~btCollisionObject::CF_STATIC_OBJECT);
		currFlags |= btCollisionObject::CF_KINEMATIC_OBJECT;
		break;
	}
	
	currFlags |= btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK;
	mRigidBody->setCollisionFlags(currFlags);
	mRigidBody->updateInertiaTensor();

	pDynamicsWorld->addRigidBody(mRigidBody);

	if (mEnabled)
	{
		mRigidBody->setActivationState(ACTIVE_TAG);
		mRigidBody->setActivationState(DISABLE_DEACTIVATION);
	}
	else
	{
		mRigidBody->setActivationState(ISLAND_SLEEPING);
	}

	mRigidBody->setUserPointer(static_cast<Body*>(this));

	if (mBodyType != BT_STATIC)
	{
		mRigidBody->setDamping(btScalar(mLinearDamping), btScalar(mAngularDamping));
		mRigidBody->setLinearVelocity(btVector3(mLinearVelocity.x, mLinearVelocity.y, mLinearVelocity.x));
		mRigidBody->setAngularVelocity(btVector3(mAngularVelocity.x, mAngularVelocity.y, mAngularVelocity.x));
	}

	if (mForce != glm::vec3(0, 0, 0))
	{
		btVector3 f(mForce.x, mForce.y, mForce.z);
		mRigidBody->applyCentralForce(f);

		mForce = glm::vec3(0, 0, 0);//Reset value after it is set
	}

	if (mTorque != glm::vec3(0, 0, 0))
	{
		btVector3 t(mTorque.x, mTorque.y, mTorque.z);
		mRigidBody->applyTorque(t);
	
		mTorque = glm::vec3(0, 0, 0);//Reset value after it is set
	}

	if (mLinearImpulse != glm::vec3(0, 0, 0))
	{
		btVector3 impulse(mLinearImpulse.x, mLinearImpulse.y, mLinearImpulse.z);
		mRigidBody->applyCentralImpulse(impulse);

		mLinearImpulse = glm::vec3(0, 0, 0);//Reset value after it is set
	}

	if (mAngularImpulse != glm::vec3(0, 0, 0))
	{
		btVector3 impulse(mAngularImpulse.x, mAngularImpulse.y, mAngularImpulse.z);
		mRigidBody->applyTorqueImpulse(impulse);

		mAngularImpulse = glm::vec3(0, 0, 0);//Reset value after it is set
	}
}

void Body::uninitializeImpl()
{
	btDynamicsWorld* pDynamicsWorld = nullptr;
	if (PhysicsManager::getInstance() != nullptr)
		pDynamicsWorld = PhysicsManager::getInstance()->getDynamicsWorld();

	assert(pDynamicsWorld != nullptr);
	if (pDynamicsWorld == nullptr)
		return;
	
	SAFE_DELETE(mMotionState);

	if (mRigidBody != nullptr)
	{
		pDynamicsWorld->removeRigidBody(mRigidBody);
	}
	SAFE_DELETE(mRigidBody);
}

void Body::updateImpl(float elapsedTime)
{
	if (mRigidBody == nullptr)
		return;

	if (mRigidBody->isActive())
	{
		btTransform trans;
		mRigidBody->getMotionState()->getWorldTransform(trans);

		btVector3 globalPos = trans.getOrigin();
		btQuaternion globalOrient = trans.getRotation();

		if (mGameObject != nullptr)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != nullptr)
			{
				pTransform->setPosition(globalPos.getX(), globalPos.getY(), globalPos.getZ());

				pTransform->setOrientation(globalOrient.getX(), globalOrient.getY(), globalOrient.getZ(), globalOrient.getW());
			}
		}

		mBodyNeedsUpdate = false;
	}
	else if (mBodyNeedsUpdate)//you can manually set the transform of the game object only if the body is inactive!
	{
		btTransform trans;
		trans.setIdentity();
		
		if (mGameObject != nullptr)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != nullptr)
			{
				glm::vec3 position = pTransform->getAbsolutePosition();
				glm::quat orientation = pTransform->getAbsoluteOrientation();

				trans.setOrigin(btVector3(position.x, position.y, position.z));
				trans.setRotation(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
			}
		}
	
		mRigidBody->setWorldTransform(trans);

		mBodyNeedsUpdate = false;
	}
}

void Body::onMessageImpl(unsigned int messageID)
{
	if (messageID == game::MESSAGE_TRANSFORM_NEEDS_UPDATE)
	{
		mBodyNeedsUpdate = true;
	}
}

btCollisionShape* Body::addBulletShape(Shape* shape)
{
	if (shape == nullptr)
		return nullptr;

	switch(shape->getShapeType())
	{
	case SHAPE_TYPE_UNDEFINED:
		break;
	case SHAPE_TYPE_PLANE:
		{
			PlaneShape* pPlaneShape = static_cast<PlaneShape*>(shape);
			return pPlaneShape->getBulletCollisionShape();
		}
		break;
	case SHAPE_TYPE_SPHERE:
		{
			SphereShape* pSphereShape = static_cast<SphereShape*>(shape);
			return pSphereShape->getBulletCollisionShape();
		}
		break;
	case SHAPE_TYPE_BOX:
		{
			BoxShape* pBoxShape = static_cast<BoxShape*>(shape);
			return pBoxShape->getBulletCollisionShape();
		}
		break;
	case SHAPE_TYPE_CAPSULE:
		break;
	case SHAPE_TYPE_CONVEX:
		break;
	case SHAPE_TYPE_MESH:
		break;
	default:
		break;	
	}

	return nullptr;
}

} // end namespace physics
