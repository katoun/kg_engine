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

#ifndef _BULLET_PHYSICS_DRIVER_H_
#define _BULLET_PHYSICS_DRIVER_H_

#include <BulletConfig.h>
#include <core/Singleton.h>
#include <physics/PhysicsDriver.h>

#include <vector>
#include <map>

namespace resource
{
// Forward definition of references
class Resource;
class Serializer;
}

namespace physics
{

// Forward definition of references
struct CollisionPoint;

struct CollisionData
{
	CollisionData();

	~CollisionData();

	unsigned long long int hashID;

	Body* body1;
	Body* body2;

	std::vector<CollisionPoint*> collisionPoints;
};

class BulletPhysicsDriver : public PhysicsDriver, public core::Singleton<BulletPhysicsDriver>
{
public:
	// Default constructor / destructor
	BulletPhysicsDriver();
	~BulletPhysicsDriver();

	void setHardware(bool state);

	void setCollisionAccuracy(float accuracy);
	void setSolverAccuracy(float accuracy);

	void setGravity(const core::vector3d& gravity);

	btDynamicsWorld* getDynamicsWorld();

	std::vector<short> mCollisionMasks;

	static BulletPhysicsDriver& getInstance();
	static BulletPhysicsDriver* getInstancePtr();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void updateImpl(float elapsedTime);

	float m_fFixedTimeStep;
	int set_substeps;
	int set_pe;

	btDynamicsWorld*					mDynamicsWorld;
	btBroadphaseInterface*				mBroadphase;
	btCollisionDispatcher*				mDispatcher;
	btConstraintSolver*					mSolver;
	btDefaultCollisionConfiguration*	mCollisionConfiguration;

	std::map<unsigned long long int, CollisionData*>	mLastCollisions;

	void removeAllCollisions();
};

} // end namespace game

#endif