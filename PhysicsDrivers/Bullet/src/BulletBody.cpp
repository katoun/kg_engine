/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

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

#include <physics/BodyData.h>
#include <physics/Material.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <game/MessageDefines.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>
#include <platform/PlatformManager.h>
#include <BulletBody.h>
#include <BulletShape.h>
#include <BulletPhysicsDriver.h>

#include <list>

namespace physics
{

BulletBody::BulletBody(): Body()
{
	mRigidBody = NULL;
	mMotionState = NULL;

	mBodyNeedsUpdate = true;
}

BulletBody::~BulletBody() {}

void BulletBody::setEnabled(bool enabled)
{
	Body::setEnabled(enabled);

	if (mRigidBody != NULL)
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

void BulletBody::setMaterial(const std::string& filename)
{
	Body::setMaterial(filename);

	if (mRigidBody != NULL)
	{
		if (mMaterial != NULL)
		{
			mRigidBody->setFriction(mMaterial->getStaticFriction());
			mRigidBody->setRestitution(mMaterial->getRestitution());
		}
	}
}

void BulletBody::setMaterial(Material* material)
{
	if (material == NULL)
		return;

	if (mMaterial != material)
	{
		Body::setMaterial(material);

		if (mRigidBody != NULL)
		{
			if (mMaterial)
			{
				mRigidBody->setFriction(mMaterial->getStaticFriction());
				mRigidBody->setRestitution(mMaterial->getRestitution());
			}
		}
	}
}

void BulletBody::applyForce(const core::vector3d& force)
{
	Body::applyForce(force);

	if (mRigidBody != NULL)
	{
		btVector3 f(mForce.X, mForce.Y, mForce.Z);
		mRigidBody->applyCentralForce(f);

		mForce = core::vector3d::ORIGIN_3D;//Reset value after it is set
	}
}

void BulletBody::applyTorque(const core::vector3d& torque)
{
	Body::applyTorque(torque);
	
	if (mRigidBody == NULL)
	{
		btVector3 t(mTorque.X, mTorque.Y, mTorque.Z);
		mRigidBody->applyTorque(t);
	
		mTorque = core::vector3d::ORIGIN_3D;//Reset value after it is set
	}
}

void BulletBody::applyLinearImpulse(const core::vector3d& linearImpulse)
{
	Body::applyLinearImpulse(linearImpulse);
		
	if (mRigidBody != NULL)
	{
		btVector3 impulse(mLinearImpulse.X, mLinearImpulse.Y, mLinearImpulse.Z);
		mRigidBody->applyCentralImpulse(impulse);

		mLinearImpulse = core::vector3d::ORIGIN_3D;//Reset value after it is set
	}
}

void BulletBody::applyAngularImpulse(const core::vector3d& angularImpulse)
{
	Body::applyAngularImpulse(angularImpulse);
	
	if (mRigidBody != NULL)
	{
		btVector3 impulse(mAngularImpulse.X, mAngularImpulse.Y, mAngularImpulse.Z);
		mRigidBody->applyTorqueImpulse(impulse);

		mAngularImpulse = core::vector3d::ORIGIN_3D;//Reset value after it is set
	}
}

btRigidBody* BulletBody::getBulletRigidBody()
{
	return mRigidBody;
}

void BulletBody::initializeImpl()
{
	Body::initializeImpl();

	if (mBodyData == NULL)
		return;

	if (mMaterial == NULL)
		return;

	if (mBodyData->getState() != resource::RESOURCE_STATE_LOADED)
		return;

	if (mMaterial->getState() != resource::RESOURCE_STATE_LOADED)
		return;

	if (mRigidBody != NULL)
		return;
	
	btDynamicsWorld* pDynamicsWorld = BulletPhysicsDriver::getInstance().getDynamicsWorld();

	assert(pDynamicsWorld != NULL);
	if (pDynamicsWorld == NULL)
		return;

	if (mBodyData->getShapes().size() == 0)
		return;

	std::list<Shape*>::const_iterator i = mBodyData->getShapes().begin();
	btCollisionShape* pShape = addBulletShape((*i));

	if (pShape == NULL)
		return;

	btTransform trans;
	trans.setIdentity();
	
	if (mGameObject != NULL)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != NULL)
		{
			core::vector3d position = pTransform->getAbsolutePosition();
			core::quaternion orientation = pTransform->getAbsoluteOrientation();

			trans.setOrigin(btVector3(position.X, position.Y, position.Z));
			trans.setRotation(btQuaternion(orientation.X, orientation.Y, orientation.Z, orientation.W));
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
		mRigidBody->setLinearVelocity(btVector3(mLinearVelocity.X, mLinearVelocity.Y, mLinearVelocity.X));
		mRigidBody->setAngularVelocity(btVector3(mAngularVelocity.X, mAngularVelocity.Y, mAngularVelocity.X));
	}

	if (mForce != core::vector3d::ORIGIN_3D)
	{
		btVector3 f(mForce.X, mForce.Y, mForce.Z);
		mRigidBody->applyCentralForce(f);

		mForce = core::vector3d::ORIGIN_3D;//Reset value after it is set
	}

	if (mTorque != core::vector3d::ORIGIN_3D)
	{
		btVector3 t(mTorque.X, mTorque.Y, mTorque.Z);
		mRigidBody->applyTorque(t);
	
		mTorque = core::vector3d::ORIGIN_3D;//Reset value after it is set
	}

	if (mLinearImpulse != core::vector3d::ORIGIN_3D)
	{
		btVector3 impulse(mLinearImpulse.X, mLinearImpulse.Y, mLinearImpulse.Z);
		mRigidBody->applyCentralImpulse(impulse);

		mLinearImpulse = core::vector3d::ORIGIN_3D;//Reset value after it is set
	}

	if (mAngularImpulse != core::vector3d::ORIGIN_3D)
	{
		btVector3 impulse(mAngularImpulse.X, mAngularImpulse.Y, mAngularImpulse.Z);
		mRigidBody->applyTorqueImpulse(impulse);

		mAngularImpulse = core::vector3d::ORIGIN_3D;//Reset value after it is set
	}
}

void BulletBody::uninitializeImpl()
{
	btDynamicsWorld* pDynamicsWorld = BulletPhysicsDriver::getInstance().getDynamicsWorld();

	assert(pDynamicsWorld != NULL);
	if (pDynamicsWorld == NULL)
		return;
	
	if (mMotionState != NULL)
		delete mMotionState;

	if (mRigidBody != NULL)
	{
		pDynamicsWorld->removeRigidBody(mRigidBody);
		delete mRigidBody;
	}
}

void BulletBody::updateImpl(float elapsedTime)
{
	if (mRigidBody == NULL)
		return;

	/*if (mBodyNeedsUpdate)//TODO: who has priority?!!!
	{
		btTransform trans;
		trans.setIdentity();
		
		if (mGameObject != NULL)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != NULL)
			{
				core::vector3d position = pTransform->getAbsolutePosition();
				core::quaternion orientation = pTransform->getAbsoluteOrientation();

				trans.setOrigin(btVector3(position.X, position.Y, position.Z));
				trans.setRotation(btQuaternion(orientation.X, orientation.Y, orientation.Z, orientation.W));
			}
		}
	
		mRigidBody->setWorldTransform(trans);

		mBodyNeedsUpdate = false;
	}
	else */if (mRigidBody->isActive())
	{
		btTransform trans;
		mRigidBody->getMotionState()->getWorldTransform(trans);

		btVector3 globalPos = trans.getOrigin();
		btQuaternion globalOrient = trans.getRotation();

		if (mGameObject != NULL)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != NULL)
			{
				pTransform->setPosition(globalPos.getX(), globalPos.getY(), globalPos.getZ());

				pTransform->setOrientation(globalOrient.getX(), globalOrient.getY(), globalOrient.getZ(), globalOrient.getY());
			}
		}
	}
}

void BulletBody::onMessageImpl(unsigned int messageID)
{
	/*if (messageID == game::MESSAGE_TRANSFORM_NEEDS_UPDATE)
	{
		mBodyNeedsUpdate = true;
	}*/
}

btCollisionShape* BulletBody::addBulletShape(Shape* shape)
{
	if (shape == NULL)
		return NULL;

	switch(shape->getShapeType())
	{
	case SHAPE_TYPE_UNDEFINED:
		break;
	case SHAPE_TYPE_PLANE:
		{
			BulletPlaneShape* bulletPlaneShape = static_cast<BulletPlaneShape*>(shape);
			return bulletPlaneShape->getBulletCollisionShape();
		}
		break;
	case SHAPE_TYPE_SPHERE:
		{
			BulletSphereShape* bulletSphereShape = static_cast<BulletSphereShape*>(shape);
			return bulletSphereShape->getBulletCollisionShape();
		}
		break;
	case SHAPE_TYPE_BOX:
		{
			BulletBoxShape* bulletBoxShape = static_cast<BulletBoxShape*>(shape);
			return bulletBoxShape->getBulletCollisionShape();
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

	return NULL;
}

} // end namespace game
