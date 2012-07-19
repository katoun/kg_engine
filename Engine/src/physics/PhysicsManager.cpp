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
#include <scene/SceneManager.h>

template<> physics::PhysicsManager& core::Singleton<physics::PhysicsManager>::ms_Singleton = physics::PhysicsManager();

namespace physics
{

CollisionEvent* PhysicsManager::mCollisionEvent = NULL;
std::list<CollisionEventReceiver*> PhysicsManager::mCollisionEventReceivers;

PhysicsManager::PhysicsManager(): core::System("PhysicsManager")
{
	mDefaultBodyDataFactory = new BodyDataFactory();

	mPhysicsDriver = NULL;

	mHardware = true;
	mCollisionAccuracy = 1.0f;
	mSolverAccuracy = 1.0f;

	mGravity = core::vector3d(0.0f, -9.81f, 0.0f);

	mCollisionEvent = new CollisionEvent();

	mBodyFactory = NULL;
	mShapeFactory = NULL;
	mJointFactory = NULL;
}

PhysicsManager::~PhysicsManager()
{
	if (mCollisionEvent != NULL)
	{
		delete mCollisionEvent;
	}
	if (mDefaultBodyDataFactory != NULL)
	{
		delete mDefaultBodyDataFactory;
	}
}

void PhysicsManager::setHardware(bool state)
{
	mHardware = state;
	
	if (mPhysicsDriver != NULL)
		mPhysicsDriver->setHardware(state);
}

void PhysicsManager::setCollisionAccuracy(float accuracy)
{
	mCollisionAccuracy = accuracy;

	if (mPhysicsDriver != NULL)
		mPhysicsDriver->setCollisionAccuracy(accuracy);
}

void PhysicsManager::setSolverAccuracy(float accuracy)
{
	mSolverAccuracy = accuracy;

	if (mPhysicsDriver != NULL)
		mPhysicsDriver->setSolverAccuracy(accuracy);
}

void PhysicsManager::setGravity(const core::vector3d& gravity)
{
	mGravity = gravity;

	if (mPhysicsDriver != NULL)
		mPhysicsDriver->setGravity(gravity);
}

Body* PhysicsManager::createBody(const std::string& bodyFilename, scene::Node* parent)
{
	if (mBodyFactory == NULL)
		return NULL;

	BodyData* newBodyData = static_cast<BodyData*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_BODY_DATA, bodyFilename));
	if (newBodyData == NULL)
		return NULL;

	Body* newBody = NULL;
	newBody = mBodyFactory->createBody(newBodyData);
	if (newBody == NULL)
		return NULL;

	mBodies[newBody->getID()] = newBody;

	scene::SceneManager::getInstance().addNode(newBody, parent);

	return newBody;
}

Body* PhysicsManager::createBody(const std::string& name, const std::string& bodyFilename, scene::Node* parent)
{
	if (mBodyFactory == NULL)
		return NULL;

	BodyData* newBodyData = static_cast<BodyData*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_BODY_DATA, bodyFilename));
	if (newBodyData == NULL)
		return NULL;

	Body* newBody = NULL;
	newBody = mBodyFactory->createBody(name, newBodyData);
	if (newBody == NULL)
		return NULL;

	mBodies[newBody->getID()] = newBody;

	scene::SceneManager::getInstance().addNode(newBody, parent);

	return newBody;
}

Body* PhysicsManager::createBody(BodyData* bodyData, scene::Node* parent)
{
	if (mBodyFactory == NULL)
		return NULL;
	if (bodyData == NULL)
		return NULL;

	Body* newBody = NULL;
	newBody = mBodyFactory->createBody(bodyData);
	if (newBody == NULL)
		return NULL;

	mBodies[newBody->getID()] = newBody;

	scene::SceneManager::getInstance().addNode(newBody, parent);

	return newBody;
}

Body* PhysicsManager::createBody(const std::string& name, BodyData* bodyData, scene::Node* parent)
{
	if (mBodyFactory == NULL)
		return NULL;
	if (bodyData == NULL)
		return NULL;

	Body* newBody = NULL;
	newBody = mBodyFactory->createBody(name, bodyData);
	if (newBody == NULL)
		return NULL;

	mBodies[newBody->getID()] = newBody;

	scene::SceneManager::getInstance().addNode(newBody, parent);

	return newBody;
}

Body* PhysicsManager::getBody(const unsigned int& id)
{
	std::map<unsigned int, Body*>::const_iterator i = mBodies.find(id);
	if (i != mBodies.end())
		return i->second;

	return NULL;
}

unsigned int PhysicsManager::getNumberOfBodies() const
{
	return mBodies.size();
}

void PhysicsManager::removeBody(Body *actor)
{
	if (actor == NULL)
		return;

	removeBody(actor->getID());
}

void PhysicsManager::removeBody(const unsigned int& id)
{
	std::map<unsigned int, Body*>::iterator i = mBodies.find(id);
	if (i != mBodies.end())
		mBodies.erase(i);

	scene::SceneManager::getInstance().removeNode(id);
}

void PhysicsManager::removeAllBodies()
{
	std::map<unsigned int, Body*>::const_iterator i;
	for (i = mBodies.begin(); i != mBodies.end(); ++i)
		scene::SceneManager::getInstance().removeNode(i->second->getID());

	mBodies.clear();
}

Shape* PhysicsManager::createShape(ShapeType type)
{
	if (mShapeFactory == NULL)
		return NULL;
		
	return mShapeFactory->createShape(type);

	return NULL;
}

Joint* PhysicsManager::createJoint(JointType type)
{
	if (mJointFactory == NULL)
		return NULL;

	Joint* newJoint = NULL;
	newJoint = mJointFactory->createJoint(type);

	if (newJoint == NULL)
		return NULL;

	mJoints[newJoint->getID()] = newJoint;

	return newJoint;
}

Joint* PhysicsManager::createJoint(const std::string& name, JointType type)
{
	if (mJointFactory == NULL)
		return NULL;

	Joint* newJoint = NULL;
	newJoint = mJointFactory->createJoint(name, type);

	if (newJoint == NULL)
		return NULL;

	mJoints[newJoint->getID()] = newJoint;

	return newJoint;
}

Joint* PhysicsManager::getJoint(const unsigned int& id)
{
	std::map<unsigned int, Joint*>::const_iterator i = mJoints.find(id);
	if (i != mJoints.end())
		return i->second;

	return NULL;
}

unsigned int PhysicsManager::getNumberOfJoints() const
{
	return mJoints.size();
}

void PhysicsManager::removeJoint(Joint *joint)
{
	if (joint == NULL)
		return;

	removeJoint(joint->getID());
}

void PhysicsManager::removeJoint(const unsigned int& id)
{
	std::map<unsigned int, Joint*>::iterator i = mJoints.find(id);
	if (i != mJoints.end())
	{
		delete i->second;
		mJoints.erase(i);
	}
}

void PhysicsManager::removeAllJoints()
{
	std::map<unsigned int, Joint*>::iterator i;
	for (i = mJoints.begin(); i != mJoints.end(); ++i)
	{
		delete i->second;
	}

	mJoints.clear();
}

Material* PhysicsManager::createMaterial(const std::string& materialFilename)
{
	Material* newMaterial = NULL;
	newMaterial = static_cast<Material*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_PHYSICS_MATERIAL, materialFilename));
	
	if (newMaterial == NULL)
		return NULL;
	
	mMaterials[newMaterial->getID()] = newMaterial;

	return newMaterial;
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


void PhysicsManager::setBodyFactory(BodyFactory* factory)
{
	mBodyFactory = factory;
}

void PhysicsManager::removeBodyFactory()
{
	mBodyFactory = NULL;
}

void PhysicsManager::setShapeFactory(ShapeFactory* factory)
{
	mShapeFactory = factory;
}

void PhysicsManager::removeShapeFactory()
{
	mShapeFactory = NULL;
}

void PhysicsManager::setJointFactory(JointFactory* factory)
{
	mJointFactory = factory;
}

void PhysicsManager::removeJointFactory()
{
	mJointFactory = NULL;
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
	if (mPhysicsDriver != NULL)
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
	mPhysicsDriver = NULL;
}

void PhysicsManager::registerDefaultFactoriesImpl()
{
	resource::ResourceManager::getInstance().registerResourceFactory(resource::RESOURCE_TYPE_BODY_DATA, mDefaultBodyDataFactory);
}

void PhysicsManager::removeDefaultFactoriesImpl()
{
	resource::ResourceManager::getInstance().removeResourceFactory(resource::RESOURCE_TYPE_BODY_DATA);
}

PhysicsManager& PhysicsManager::getInstance()
{
	return core::Singleton<PhysicsManager>::getInstance();
}

PhysicsManager* PhysicsManager::getInstancePtr()
{
	return core::Singleton<PhysicsManager>::getInstancePtr();
}

} // end namespace physics
