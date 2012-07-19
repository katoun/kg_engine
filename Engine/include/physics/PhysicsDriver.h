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

#ifndef _PHYSICS_DRIVER_H_
#define _PHYSICS_DRIVER_H_

#include <core/Config.h>
#include <core/SystemDriver.h>

namespace core
{
class vector3d;
class quaternion;
}

namespace physics
{

class Body;
class BodyData;
class Shape;
class Material;
class Joint;
struct CollisionPoint;
enum ShapeType;
enum JointType;

//! Defines the functionality of a 3D Physics API
//!
//! The PhysicsSystem class provides a base interface
//! which abstracts the general functionality of the physics API
//! e.g. ODE or PhysX. Whilst a few of the general
//! methods have implementations, most of this class is
//! abstract, requiring a subclass based on a specific API
//! to be constructed to provide the full functionality.
class ENGINE_PUBLIC_EXPORT PhysicsDriver: public core::SystemDriver
{
public:

	// Default Constructor
	PhysicsDriver(const std::string& name);

	// Destructor
	virtual ~PhysicsDriver();

	virtual void setHardware(bool state) = 0;

	virtual void setCollisionAccuracy(float accuracy) = 0;
	virtual void setSolverAccuracy(float accuracy) = 0;

	virtual void setGravity(const core::vector3d& gravity) = 0;

protected:

	static void fireCollisionStarted(Body* body1, Body* body2, const std::vector<CollisionPoint*>& points);

	static void fireCollisionUpdate(Body* body1, Body* body2, const std::vector<CollisionPoint*>& points);

	static void fireCollisionEnded(Body* body1, Body* body2);
};

} // end namespace physics

#endif