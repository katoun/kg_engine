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
#include <physics/CollisionEvent.h>
#include <physics/Body.h>
#include <physics/BodyFactory.h>
#include <physics/BodyData.h>
#include <physics/BodyDataFactory.h>
#include <physics/Shape.h>
#include <physics/Constraint.h>
#include <physics/Material.h>
#include <physics/MaterialFactory.h>
#include <game/GameObject.h>
#include <game/ComponentDefines.h>
#include <game/ComponentFactory.h>
#include <game/GameManager.h>

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#if defined(USE_PARALLEL_DISPATCHER) || defined(USE_PARALLEL_SOLVER)
#include <BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#include <BulletMultiThreaded/PlatformDefinitions.h>
#include "BulletMultiThreaded/Win32ThreadSupport.h"
#include "BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h"
#include "BulletMultiThreaded/btParallelConstraintSolver.h"
#endif//USE_PARALLEL_DISPATCHER || USE_PARALLEL_SOLVER

template<> physics::PhysicsManager* core::Singleton<physics::PhysicsManager>::m_Singleton = nullptr;

namespace physics
{

CollisionData::CollisionData()
{
	hashID = 0;
	body1 = nullptr;
	body2 = nullptr;
}

CollisionData::~CollisionData()
{
	std::vector<CollisionPoint*>::const_iterator i;
	for (i = collisionPoints.begin(); i != collisionPoints.end(); i++)
	{
		CollisionPoint* pCollisionPoint = (*i);
		SAFE_DELETE(pCollisionPoint);
	}
	
	collisionPoints.clear();
}

CollisionEvent* PhysicsManager::mCollisionEvent = nullptr;
std::list<CollisionEventReceiver*> PhysicsManager::mCollisionEventReceivers;

PhysicsManager::PhysicsManager(): core::System("PhysicsManager")
{
	m_fFixedTimeStep = 1.0f/60.0f;
	set_substeps = 1;
	set_pe = 1;

	mDynamicsWorld = nullptr;
	mBroadphase = nullptr;
	mDispatcher = nullptr;
	mSolver = nullptr;
	mCollisionConfiguration = nullptr;

	mLastCollisions.clear();

	mMaterialFactory = new MaterialFactory();
	mBodyDataFactory = new BodyDataFactory();

	mBodyFactory = new BodyFactory();

	mHardware = true;
	mCollisionAccuracy = 1.0f;
	mSolverAccuracy = 1.0f;

	mGravity = glm::vec3(0, -9.81f, 0);

	mCollisionEvent = new CollisionEvent();
}

PhysicsManager::~PhysicsManager()
{
	SAFE_DELETE(mCollisionEvent);
	SAFE_DELETE(mBodyDataFactory);
	SAFE_DELETE(mMaterialFactory);
	SAFE_DELETE(mBodyFactory);

	removeLastCollisions();
}

void PhysicsManager::setHardware(bool state)
{
	mHardware = state;
	
	//katoun TODO!!!
}

void PhysicsManager::setCollisionAccuracy(float accuracy)
{
	mCollisionAccuracy = accuracy;

	//katoun TODO!!!
}

void PhysicsManager::setSolverAccuracy(float accuracy)
{
	mSolverAccuracy = accuracy;

	if (mSolverAccuracy < 1.0f)
		mSolverAccuracy = 1.0f;

	if (mDynamicsWorld != nullptr)
	{
		mDynamicsWorld->getSolverInfo().m_numIterations = int(accuracy);
	}
}

void PhysicsManager::setGravity(const glm::vec3& gravity)
{
	mGravity = gravity;

	if (mDynamicsWorld != nullptr)
	{
		mDynamicsWorld->setGravity(btVector3(mGravity.x, -mGravity.y, mGravity.z));
	}
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
	Shape* pShape = nullptr;
	
	switch(type)
	{
	case SHAPE_TYPE_UNDEFINED:
		pShape = nullptr;
		break;
	case SHAPE_TYPE_PLANE:
		pShape = new PlaneShape();
		break;
	case SHAPE_TYPE_SPHERE:
		pShape = new SphereShape();
		break;
	case SHAPE_TYPE_BOX:
		pShape = new BoxShape();
		break;
	case SHAPE_TYPE_CAPSULE:
		pShape = nullptr;
		break;
	case SHAPE_TYPE_CONVEX:
		pShape = nullptr;
		break;
	case SHAPE_TYPE_MESH:
		pShape = nullptr;
		break;
	default:
		pShape = nullptr;
		break;	
	}

	return pShape;
}

Constraint* PhysicsManager::createConstraint(ConstraintType type)
{
	Constraint* pConstraint = nullptr;

	switch(type)
	{
	case CONSTRAINT_TYPE_UNDEFINED:
		pConstraint = nullptr;
		break;
	case CONSTRAINT_TYPE_SPHERICAL:
		pConstraint = new SphericalConstraint();
		break;
	case CONSTRAINT_TYPE_HINGE:
		pConstraint = new HingeConstraint();
		break;
	case CONSTRAINT_TYPE_SLIDER:
		pConstraint = new SliderConstraint();
		break;
	case CONSTRAINT_TYPE_GENERIC:
		pConstraint = new GenericConstraint();
		break;
	case CONSTRAINT_TYPE_RIGID:
		pConstraint = new RigidConstraint();
		break;
	default:
		pConstraint = nullptr;
		break;	
	}

	if (pConstraint != nullptr)
	{
		mConstraints[pConstraint->getID()] = pConstraint;
	}

	return pConstraint;
}

void PhysicsManager::removeConstraint(Constraint *joint)
{
	if (joint == nullptr)
		return;

	removeConstraint(joint->getID());
}

void PhysicsManager::removeConstraint(const unsigned int& id)
{
	std::map<unsigned int, Constraint*>::iterator i = mConstraints.find(id);
	if (i != mConstraints.end())
	{
		SAFE_DELETE(i->second);
		mConstraints.erase(i);
	}
}

void PhysicsManager::removeAllConstraints()
{
	std::map<unsigned int, Constraint*>::iterator i;
	for (i = mConstraints.begin(); i != mConstraints.end(); ++i)
	{
		SAFE_DELETE(i->second);
	}

	mConstraints.clear();
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

btDynamicsWorld* PhysicsManager::getDynamicsWorld()
{
	return mDynamicsWorld;
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
	//collision configuration contains default setup for memory, collision setup
	btDefaultCollisionConstructionInfo collisionConstructionInfo;
	collisionConstructionInfo.m_defaultMaxPersistentManifoldPoolSize = 32768;
	mCollisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration(collisionConstructionInfo);

	mBroadphase = new btDbvtBroadphase();

	//use the default collision dispatcher. For parallel processing you can use a different dispatcher (see Extras/BulletMultiThreaded)
#ifndef USE_PARALLEL_DISPATCHER
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
#else
	int maxNumOutstandingTasks = set_pe;

	btThreadSupportInterface* mThreadSupportCollision = nullptr;
	mThreadSupportCollision = new Win32ThreadSupport(Win32ThreadSupport::Win32ThreadConstructionInfo(
							"BulletCollision",	processCollisionTask, createCollisionLocalStoreMemory, maxNumOutstandingTasks));
	
	mThreadSupportCollision->startSPU();
	mDispatcher = new SpuGatheringCollisionDispatcher(mThreadSupportCollision, maxNumOutstandingTasks, mCollisionConfiguration);
#endif

#ifndef USE_PARALLEL_SOLVER
	mSolver = new btSequentialImpulseConstraintSolver;
#else
	btThreadSupportInterface* mThreadSupportSolver = nullptr;
	mThreadSupportSolver = new Win32ThreadSupport(Win32ThreadSupport::Win32ThreadConstructionInfo(
							"BulletSolver", SolverThreadFunc, SolverlsMemoryFunc, maxNumOutstandingTasks));

	mThreadSupportSolver->startSPU();
	mSolver = new btParallelConstraintSolver(mThreadSupportSolver);
	//this solver requires the contacts to be in a contiguous pool, so avoid dynamic allocation
	mDispatcher->setDispatcherFlags(btCollisionDispatcher::CD_DISABLE_CONTACTPOOL_DYNAMIC_ALLOCATION);
#endif

	mDynamicsWorld = new btSoftRigidDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);

	mDynamicsWorld->getSolverInfo().m_solverMode = SOLVER_USE_WARMSTARTING | SOLVER_USE_2_FRICTION_DIRECTIONS |
													SOLVER_RANDMIZE_ORDER | SOLVER_USE_WARMSTARTING | SOLVER_SIMD;
	mDynamicsWorld->getDispatchInfo().m_allowedCcdPenetration = btScalar(0.0001);

#ifdef USE_PARALLEL_DISPATCHER
	mDynamicsWorld->getDispatchInfo().m_enableSPU = true;
#endif

	//setHardware(mHardware);
	//setCollisionAccuracy(mCollisionAccuracy);

	if (mSolverAccuracy < 1.0f)
		mSolverAccuracy = 1.0f;
	mDynamicsWorld->getSolverInfo().m_numIterations = int(mSolverAccuracy);

	mDynamicsWorld->setGravity(btVector3(mGravity.x, -mGravity.y, mGravity.z));
}

void PhysicsManager::uninitializeImpl()
{
	// Remove all Bodies
	removeAllBodies();

	// Remove all Constraints
	removeAllConstraints();

	// Clear Materials list
	mMaterials.clear();

	SAFE_DELETE(mDynamicsWorld);
	SAFE_DELETE(mSolver);
	SAFE_DELETE(mBroadphase);
	SAFE_DELETE(mDispatcher);
	SAFE_DELETE(mCollisionConfiguration);
}

void PhysicsManager::startImpl() {}

void PhysicsManager::stopImpl() {}

void PhysicsManager::updateImpl(float elapsedTime)
{
	///step the simulation
	if (mDynamicsWorld != nullptr)
	{
		if (m_fFixedTimeStep > 0)
			mDynamicsWorld->stepSimulation(elapsedTime, set_substeps, m_fFixedTimeStep);
		else
			mDynamicsWorld->stepSimulation(elapsedTime, 0);
	}	

	if (mDispatcher == nullptr)
		return;

	// Browse all collision pairs
	std::map<unsigned long long int, CollisionData*> newCollisions;
	for (int i=0; i< mDispatcher->getNumManifolds(); i++)
	{
		btPersistentManifold* contactManifold = mDispatcher->getManifoldByIndexInternal(i);
		if (contactManifold == nullptr)
			continue;

		if (contactManifold->getBody0() == nullptr || contactManifold->getBody1() == nullptr)
			continue;

		Body* body1 = static_cast<Body*>(contactManifold->getBody0()->getUserPointer());
		Body* body2 = static_cast<Body*>(contactManifold->getBody1()->getUserPointer());

		if (body1 == nullptr || body2 == nullptr)
			continue;

		if (contactManifold->getNumContacts() == 0)
			continue;

		CollisionData* newCollisionData = new CollisionData();
		newCollisionData->body1 = body1;
		newCollisionData->body2 = body2;
		
		unsigned long long int hashID = ((unsigned long long int)body1 << sizeof(unsigned int) * 8) + (unsigned long long int)body2;

		for (int j=0; j<contactManifold->getNumContacts(); j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);

			CollisionPoint* newCollisionPoint = new CollisionPoint();

			glm::vec3 position = glm::vec3(0, 0, 0);
			glm::vec3 normal = glm::vec3(0, 1, 0);
			float distance = 0.0f;
			float impulse = 0.0f;
			glm::vec3 impulseLateral1 = glm::vec3(0, 0, 0);
			glm::vec3 impulseLateral2 = glm::vec3(0, 0, 0);

			btVector3 pos = pt.getPositionWorldOnB();
			btVector3 norm = pt.m_normalWorldOnB;

			position.x = pos.getX();
			position.y = pos.getY();
			position.z = pos.getZ();

			normal.x = norm.getX();
			normal.y = norm.getY();
			normal.z = norm.getZ();

			distance = pt.getDistance();

			newCollisionPoint->mCollisionPosition = position;
			newCollisionPoint->mCollisionNormal = normal;
			newCollisionPoint->mDistance = distance;
			newCollisionPoint->mImpulse = impulse;
			newCollisionPoint->mImpulseLateral1 = impulseLateral1;
			newCollisionPoint->mImpulseLateral2 = impulseLateral2;
			
			newCollisionData->collisionPoints.push_back(newCollisionPoint);
		}

		newCollisions[hashID] = newCollisionData;
	}

	// Check for added contacts
	if (newCollisions.size() != 0)
	{
		std::map<unsigned long long int, CollisionData*>::const_iterator i;
		for(i = newCollisions.begin(); i != newCollisions.end(); ++i)
		{
			CollisionData* colData = i->second;
			std::map<unsigned long long int, CollisionData*>::const_iterator j = mLastCollisions.find(i->first);
			if(j == mLastCollisions.end())
				fireCollisionStarted(colData->body1, colData->body2, colData->collisionPoints);
			else
				fireCollisionUpdate(colData->body1, colData->body2, colData->collisionPoints);
		}
	}

	// Check for removed contacts
	if (mLastCollisions.size() != 0)
	{
		std::map<unsigned long long int, CollisionData*>::const_iterator i;
		for(i = mLastCollisions.begin(); i != mLastCollisions.end(); ++i)
		{
			CollisionData* colData = i->second;
			std::map<unsigned long long int, CollisionData*>::const_iterator j = newCollisions.find(i->first);
			if(j == newCollisions.end())
				fireCollisionEnded(colData->body1, colData->body2);
		}
	}

	removeLastCollisions();
	
	std::map<unsigned long long int, CollisionData*>::const_iterator i;
	for(i = newCollisions.begin(); i != newCollisions.end(); ++i)
	{
		mLastCollisions[i->first] = i->second;
	}
	newCollisions.clear();
}

void PhysicsManager::registerFactoriesImpl()
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		resource::ResourceManager::getInstance()->registerResourceFactory(resource::RESOURCE_TYPE_BODY_DATA, mBodyDataFactory);
		resource::ResourceManager::getInstance()->registerResourceFactory(resource::RESOURCE_TYPE_PHYSICS_MATERIAL, mMaterialFactory);
	}

	if (game::GameManager::getInstance() != nullptr)
		game::GameManager::getInstance()->registerComponentFactory(game::COMPONENT_TYPE_BODY, mBodyFactory);
}

void PhysicsManager::removeFactoriesImpl()
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		resource::ResourceManager::getInstance()->removeResourceFactory(resource::RESOURCE_TYPE_BODY_DATA);
		resource::ResourceManager::getInstance()->removeResourceFactory(resource::RESOURCE_TYPE_PHYSICS_MATERIAL);
	}

	if (game::GameManager::getInstance() != nullptr)
		game::GameManager::getInstance()->removeComponentFactory(game::COMPONENT_TYPE_BODY);
}

void PhysicsManager::removeLastCollisions()
{
	std::map<unsigned long long int, CollisionData*>::const_iterator i;
	for (i = mLastCollisions.begin(); i != mLastCollisions.end(); ++i)
	{
		CollisionData* pCollisionData = i->second;
		SAFE_DELETE(pCollisionData);
	}

	mLastCollisions.clear();
}

PhysicsManager* PhysicsManager::getInstance()
{
	return core::Singleton<PhysicsManager>::getInstance();
}

} // end namespace physics
