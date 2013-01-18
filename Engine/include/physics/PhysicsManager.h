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

#ifndef _PHYSICS_MANAGER_H_
#define _PHYSICS_MANAGER_H_

#include <core/Config.h>
#include <core/System.h>
#include <core/Singleton.h>
#include <core/Vector3d.h>
#include <core/Quaternion.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>
#include <physics/CollisionEventReceiver.h>

#include <string>
#include <map>

namespace scene
{
class Node;
}

namespace game
{
class ComponentFactory;
}

namespace physics
{

class Body;
class BodyFactory;
class BodyData;
class BodyDataFactory;
class Shape;
class ShapeFactory;
class Joint;
class JointFactory;
class Material;
class PhysicsDriver;
struct CollisionPoint;
struct CollisionEvent;
enum BodyType;
enum ShapeType;
enum JointType;

//! Physics Manager.
//! 
//! This class handles everything related to physics.
//! \author: Kat'Oun
//! \version: 1.0
class ENGINE_PUBLIC_EXPORT PhysicsManager: public core::System, public core::Singleton<PhysicsManager>
{
	friend class PhysicsDriver;

public:

	// Default Constructor
	PhysicsManager();

	~PhysicsManager();

	void setHardware(bool state);

	void setCollisionAccuracy(float accuracy);
	void setSolverAccuracy(float accuracy);

	void setGravity(const core::vector3d& gravity);

	//!  Adds a boy to be managed by this physics manager.
	void addBody(Body* body);

	//! Removes a body.
	void removeBody(Body* body);
	//! Removes a body.
	void removeBody(const unsigned int& id);
	//! Removes (and destroys) all bodies.
	void removeAllBodies();

	Shape* createShape(ShapeType type);

	//! Creates a joint to be managed by physics manager.
	Joint* createJoint(JointType type);
	Joint* createJoint(const std::string& name, JointType type);

	//! Retrieves a pointer to a joint by id.
	Joint* getJoint(const unsigned int& id);

	//! Retrieves the total number of created joints.
	unsigned int getNumberOfJoints() const;

	//! Removes a joint.
	void removeJoint(Joint *joint);
	//! Removes a joint.
	void removeJoint(const unsigned int& id);
	//! Removes (and destroys) all bodies.
	void removeAllJoints();

	Material* createMaterial(const std::string& materialFilename);

	void addCollisionEventReceiver(CollisionEventReceiver* newEventReceiver);
	void removeCollisionEventReceiver(CollisionEventReceiver* oldEventReceiver);

	void setBodyFactory(BodyFactory* factory);
	void removeBodyFactory();

	void setDefaultBodyFactory(game::ComponentFactory* factory);
	void removeDefaultBodyFactory();

	void setShapeFactory(ShapeFactory* factory);
	void removeShapeFactory();

	void setJointFactory(JointFactory* factory);
	void removeJointFactory();

	static PhysicsManager* getInstance();

private:

	static void fireCollisionStarted(Body* body1, Body* body2, const std::vector<CollisionPoint*>& points);

	static void fireCollisionUpdate(Body* body1, Body* body2, const std::vector<CollisionPoint*>& points);

	static void fireCollisionEnded(Body* body1, Body* body2);

protected:

	void initializeImpl();
	void uninitializeImpl();
	void startImpl();
	void stopImpl();
	void updateImpl(float elapsedTime);
	void setSystemDriverImpl(core::SystemDriver* systemDriver);
	void removeSystemDriverImpl();
	void registerDefaultFactoriesImpl();
	void removeDefaultFactoriesImpl();

	BodyDataFactory* mDefaultBodyDataFactory;
	game::ComponentFactory* mDefaultBodyFactory;
	
	PhysicsDriver* mPhysicsDriver;

	static CollisionEvent* mCollisionEvent;
	static std::list<CollisionEventReceiver*> mCollisionEventReceivers;

	//! Central list of bodies - for easy memory management and lookup.
	std::map<unsigned int, Body*> mBodies;

	//! Central list of joints - for easy memory management and lookup.
	std::map<unsigned int, Joint*> mJoints;

	//! Central list of materials - for easy memory management and lookup.
	std::map<unsigned int, Material*> mMaterials;

	ShapeFactory* mShapeFactory;
	JointFactory* mJointFactory;

	bool mHardware;
	float mCollisionAccuracy;
	float mSolverAccuracy;

	core::vector3d mGravity;
};

} // end namespace physics

#endif