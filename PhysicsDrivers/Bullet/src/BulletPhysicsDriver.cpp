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

#include <physics/CollisionPoint.h>
#include <physics/Material.h>
#include <physics/PhysicsManager.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>
#include <BulletPhysicsDriver.h>
#include <BulletBody.h>
#include <BulletShape.h>
#include <BulletJoint.h>

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

#if GAME_PLATFORM == PLATFORM_WINDOWS
#include <windows.h>
#endif

template<> physics::BulletPhysicsDriver* core::Singleton<physics::BulletPhysicsDriver>::m_Singleton = nullptr;

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

BulletPhysicsDriver::BulletPhysicsDriver(): PhysicsDriver("Bullet PhysicsDriver")
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
}

BulletPhysicsDriver::~BulletPhysicsDriver()
{
	removeAllCollisions();
}

void BulletPhysicsDriver::setHardware(bool state)
{
	//katoun TODO
}

void BulletPhysicsDriver::setCollisionAccuracy(float accuracy)
{
	//katoun TODO
}

void BulletPhysicsDriver::setSolverAccuracy(float accuracy)
{
	if (mDynamicsWorld != nullptr)
	{
		if (accuracy < 1.0f)
			accuracy = 1.0f;
		mDynamicsWorld->getSolverInfo().m_numIterations = int(accuracy);
	}
}

void BulletPhysicsDriver::setGravity(const glm::vec3& gravity)
{
	if (mDynamicsWorld != nullptr)
	{
		mDynamicsWorld->setGravity(btVector3(gravity.x, -gravity.y, gravity.z));
	}
}

btDynamicsWorld* BulletPhysicsDriver::getDynamicsWorld()
{
	return mDynamicsWorld;
}

void BulletPhysicsDriver::initializeImpl()
{
	PhysicsDriver::initializeImpl();

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

	mDynamicsWorld->setGravity(btVector3(0.0f, 9.81f, 0.0f));

	mDynamicsWorld->getSolverInfo().m_solverMode = SOLVER_USE_WARMSTARTING | SOLVER_USE_2_FRICTION_DIRECTIONS |
													SOLVER_RANDMIZE_ORDER | SOLVER_USE_WARMSTARTING | SOLVER_SIMD;
	mDynamicsWorld->getDispatchInfo().m_allowedCcdPenetration = btScalar(0.0001);

#ifdef USE_PARALLEL_DISPATCHER
	mDynamicsWorld->getDispatchInfo().m_enableSPU = true;
#endif
}

void BulletPhysicsDriver::uninitializeImpl()
{
	SAFE_DELETE(mDynamicsWorld);
	SAFE_DELETE(mSolver);
	SAFE_DELETE(mBroadphase);
	SAFE_DELETE(mDispatcher);
	SAFE_DELETE(mCollisionConfiguration);
}

void BulletPhysicsDriver::updateImpl(float elapsedTime)
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

	removeAllCollisions();
	
	std::map<unsigned long long int, CollisionData*>::const_iterator i;
	for(i = newCollisions.begin(); i != newCollisions.end(); ++i)
	{
		mLastCollisions[i->first] = i->second;
	}
	newCollisions.clear();
}

void BulletPhysicsDriver::removeAllCollisions()
{
	std::map<unsigned long long int, CollisionData*>::const_iterator i;
	for (i = mLastCollisions.begin(); i != mLastCollisions.end(); ++i)
	{
		CollisionData* pCollisionData = i->second;
		SAFE_DELETE(pCollisionData);
	}

	mLastCollisions.clear();
}

BulletPhysicsDriver* BulletPhysicsDriver::getInstance()
{
	return core::Singleton<BulletPhysicsDriver>::getInstance();
}

} // end namespace game