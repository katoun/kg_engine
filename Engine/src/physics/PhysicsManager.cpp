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

#include <physics/PhysicsManager.h>
#include <physics/PhysicsDriver.h>
#include <physics/CollisionEvent.h>
#include <physics/Body.h>
#include <physics/BodyFactory.h>
#include <physics/BodyData.h>
#include <physics/BodyDataFactory.h>
#include <physics/Shape.h>
#include <physics/ShapeFactory.h>
#include <physics/Joint.h>
#include <physics/JointFactory.h>
#include <physics/Material.h>
#include <game/GameObject.h>
#include <game/ComponentDefines.h>
#include <game/ComponentFactory.h>
#include <game/GameManager.h>

template<> physics::PhysicsManager* core::Singleton<physics::PhysicsManager>::m_Singleton = nullptr;

namespace physics
{

CollisionEvent* PhysicsManager::mCollisionEvent = nullptr;
std::list<CollisionEventReceiver*> PhysicsManager::mCollisionEventReceivers;

PhysicsManager::PhysicsManager(): core::System("PhysicsManager")
{
	mDefaultBodyDataFactory = new BodyDataFactory();

	mPhysicsDriver = nullptr;
	mDefaultBodyFactory = nullptr;

	mHardware = true;
	mCollisionAccuracy = 1.0f;
	mSolverAccuracy = 1.0f;

	mGravity = glm::vec3(0, -9.81f, 0);

	mCollisionEvent = new CollisionEvent();

	mShapeFactory = nullptr;
	mJointFactory = nullptr;
}

PhysicsManager::~PhysicsManager()
{
	SAFE_DELETE(mCollisionEvent);
	SAFE_DELETE(mDefaultBodyDataFactory);
}

void PhysicsManager::setHardware(bool state)
{
	mHardware = state;
	
	if (mPhysicsDriver != nullptr)
		mPhysicsDriver->setHardware(state);
}

void PhysicsManager::setCollisionAccuracy(float accuracy)
{
	mCollisionAccuracy = accuracy;

	if (mPhysicsDriver != nullptr)
		mPhysicsDriver->setCollisionAccuracy(accuracy);
}

void PhysicsManager::setSolverAccuracy(float accuracy)
{
	mSolverAccuracy = accuracy;

	if (mPhysicsDriver != nullptr)
		mPhysicsDriver->setSolverAccuracy(accuracy);
}

void PhysicsManager::setGravity(const glm::vec3& gravity)
{
	mGravity = gravity;

	if (mPhysicsDriver != nullptr)
		mPhysicsDriver->setGravity(gravity);
}

void PhysicsManager::addBody(Body* body)
{
	if (body == nullptr)
		return;

	mBodies[body->getID()] = body;
}

void PhysicsManager::removeBody(Body *actor)
{
	if (actor == nullptr)
		return;

	removeBody(actor->getID());
}

void PhysicsManager::removeBody(const unsigned int& id)
{
	std::map<unsigned int, Body*>::iterator i = mBodies.find(id);
	if (i != mBodies.end())
		mBodies.erase(i);
}

void PhysicsManager::removeAllBodies()
{
	mBodies.clear();
}

Shape* PhysicsManager::createShape(ShapeType type)
{
	if (mShapeFactory == nullptr)
		return nullptr;
		
	return mShapeFactory->createShape(type);

	return nullptr;
}

Joint* PhysicsManager::createJoint(JointType type)
{
	if (mJointFactory == nullptr)
		return nullptr;

	Joint* newJoint = nullptr;
	newJoint = mJointFactory->createJoint(type);

	if (newJoint == nullptr)
		return nullptr;

	mJoints[newJoint->getID()] = newJoint;

	return newJoint;
}

Joint* PhysicsManager::createJoint(const std::string& name, JointType type)
{
	if (mJointFactory == nullptr)
		return nullptr;

	Joint* newJoint = nullptr;
	newJoint = mJointFactory->createJoint(name, type);

	if (newJoint == nullptr)
		return nullptr;

	mJoints[newJoint->getID()] = newJoint;

	return newJoint;
}

Joint* PhysicsManager::getJoint(const unsigned int& id)
{
	std::map<unsigned int, Joint*>::const_iterator i = mJoints.find(id);
	if (i != mJoints.end())
		return i->second;

	return nullptr;
}

unsigned int PhysicsManager::getNumberOfJoints() const
{
	return mJoints.size();
}

void PhysicsManager::removeJoint(Joint *joint)
{
	if (joint == nullptr)
		return;

	removeJoint(joint->getID());
}

void PhysicsManager::removeJoint(const unsigned int& id)
{
	std::map<unsigned int, Joint*>::iterator i = mJoints.find(id);
	if (i != mJoints.end())
	{
		SAFE_DELETE(i->second);
		mJoints.erase(i);
	}
}

void PhysicsManager::removeAllJoints()
{
	std::map<unsigned int, Joint*>::iterator i;
	for (i = mJoints.begin(); i != mJoints.end(); ++i)
	{
		SAFE_DELETE(i->second);
	}

	mJoints.clear();
}

Material* PhysicsManager::createMaterial(const std::string& materialFilename)
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		Material*  newMaterial = static_cast<Material*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_PHYSICS_MATERIAL, materialFilename));
	
		if (newMaterial == nullptr)
			return nullptr;
	
		mMaterials[newMaterial->getID()] = newMaterial;

		return newMaterial;
	}

	return nullptr;
}

void PhysicsManager::addCollisionEventReceiver(CollisionEventReceiver* newEventReceiver)
{
	mCollisionEventReceivers.push_back(newEventReceiver);
}

void PhysicsManager::removeCollisionEventReceiver(CollisionEventReceiver* oldEventReceiver)
{
	std::list<CollisionEventReceiver*>::iterator i;
	for (i = mCollisionEventReceivers.begin(); i != mCollisionEventReceivers.end(); ++i)
	{
		if ((*i) == oldEventReceiver)
		{
			mCollisionEventReceivers.erase(i);
			return;
		}
	}
}

void PhysicsManager::setDefaultBodyFactory(game::ComponentFactory* factory)
{
	mDefaultBodyFactory = factory;
}

void PhysicsManager::removeDefaultBodyFactory()
{
	mDefaultBodyFactory = nullptr;
}

void PhysicsManager::setShapeFactory(ShapeFactory* factory)
{
	mShapeFactory = factory;
}

void PhysicsManager::removeShapeFactory()
{
	mShapeFactory = nullptr;
}

void PhysicsManager::setJointFactory(JointFactory* factory)
{
	mJointFactory = factory;
}

void PhysicsManager::removeJointFactory()
{
	mJointFactory = nullptr;
}

void PhysicsManager::fireCollisionStarted(Body* body1, Body* body2, const std::vector<CollisionPoint*>& points)
{
	mCollisionEvent->mBody1 = body1;
	mCollisionEvent->mBody2 = body2;
	mCollisionEvent->mCollisionPoints = points;

	// Do collision started event
	std::list<CollisionEventReceiver*>::iterator i;
	for (i = mCollisionEventReceivers.begin();i != mCollisionEventReceivers.end(); ++i)
	{
		(*i)->collisionStarted(*mCollisionEvent);
	}
}

void PhysicsManager::fireCollisionUpdate(Body* body1, Body* body2, const std::vector<CollisionPoint*>& points)
{
	mCollisionEvent->mBody1 = body1;
	mCollisionEvent->mBody2 = body2;
	mCollisionEvent->mCollisionPoints = points;

	// Do collision updated event
	std::list<CollisionEventReceiver*>::iterator i;
	for (i = mCollisionEventReceivers.begin();i != mCollisionEventReceivers.end(); ++i)
	{
		(*i)->collisionUpdate(*mCollisionEvent);
	}
}

void PhysicsManager::fireCollisionEnded(Body* body1, Body* body2)
{
	mCollisionEvent->mBody1 = body1;
	mCollisionEvent->mBody2 = body2;

	// Do collision ended event
	std::list<CollisionEventReceiver*>::iterator i;
	for (i = mCollisionEventReceivers.begin();i != mCollisionEventReceivers.end(); ++i)
	{
		(*i)->collisionEnded(*mCollisionEvent);
	}
}

void PhysicsManager::initializeImpl()
{
	if (mPhysicsDriver != nullptr)
	{
		mPhysicsDriver->setHardware(mHardware);
		mPhysicsDriver->setCollisionAccuracy(mCollisionAccuracy);
		mPhysicsDriver->setSolverAccuracy(mSolverAccuracy);
		mPhysicsDriver->setGravity(mGravity);
	}
}

void PhysicsManager::uninitializeImpl()
{
	// Remove all Bodies
	removeAllBodies();

	// Remove all Joints
	removeAllJoints();

	// Clear Materials list
	mMaterials.clear();
}

void PhysicsManager::startImpl() {}

void PhysicsManager::stopImpl() {}

void PhysicsManager::updateImpl(float elapsedTime) {}

void PhysicsManager::setSystemDriverImpl(core::SystemDriver* systemDriver)
{
	mPhysicsDriver = static_cast<PhysicsDriver*>(systemDriver);
}

void PhysicsManager::removeSystemDriverImpl()
{
	mPhysicsDriver = nullptr;
}

void PhysicsManager::registerDefaultFactoriesImpl()
{
	if (resource::ResourceManager::getInstance() != nullptr)
		resource::ResourceManager::getInstance()->registerResourceFactory(resource::RESOURCE_TYPE_BODY_DATA, mDefaultBodyDataFactory);

	if (game::GameManager::getInstance() != nullptr)
		game::GameManager::getInstance()->registerComponentFactory(game::COMPONENT_TYPE_BODY, mDefaultBodyFactory);
}

void PhysicsManager::removeDefaultFactoriesImpl()
{
	if (resource::ResourceManager::getInstance() != nullptr)
		resource::ResourceManager::getInstance()->removeResourceFactory(resource::RESOURCE_TYPE_BODY_DATA);

	if (game::GameManager::getInstance() != nullptr)
		game::GameManager::getInstance()->removeComponentFactory(game::COMPONENT_TYPE_BODY);
}

PhysicsManager* PhysicsManager::getInstance()
{
	return core::Singleton<PhysicsManager>::getInstance();
}

} // end namespace physics
