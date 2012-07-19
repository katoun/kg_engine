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

	//! Creates a body to be managed by physics manager.
	Body* createBody(const std::string& bodyFilename, scene::Node* parent = NULL);
	Body* createBody(const std::string& name, const std::string& bodyFilename, scene::Node* parent = NULL);

	//! Creates a body to be managed by physics manager.
	Body* createBody(BodyData* bodyData, scene::Node* parent = NULL);
	Body* createBody(const std::string& name, BodyData* bodyData, scene::Node* parent = NULL);

	//! Retrieves a pointer to a body by id.
	Body* getBody(const unsigned int& id);

	//! Retrieves the total number of created bodies.
	unsigned int getNumberOfBodies() const;

	//! Removes a body.
	void removeBody(Body *actor);
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

	void setShapeFactory(ShapeFactory* factory);
	void removeShapeFactory();

	void setJointFactory(JointFactory* factory);
	void removeJointFactory();

	static PhysicsManager& getInstance();
	static PhysicsManager* getInstancePtr();

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
	
	PhysicsDriver* mPhysicsDriver;

	static CollisionEvent* mCollisionEvent;
	static std::list<CollisionEventReceiver*> mCollisionEventReceivers;

	//! Central list of bodies - for easy memory management and lookup.
	std::map<unsigned int, Body*> mBodies;

	//! Central list of joints - for easy memory management and lookup.
	std::map<unsigned int, Joint*> mJoints;

	//! Central list of materials - for easy memory management and lookup.
	std::map<unsigned int, Material*> mMaterials;

	BodyFactory* mBodyFactory;
	ShapeFactory* mShapeFactory;
	JointFactory* mJointFactory;

	bool mHardware;
	float mCollisionAccuracy;
	float mSolverAccuracy;

	core::vector3d mGravity;
};

} // end namespace physics

#endif