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