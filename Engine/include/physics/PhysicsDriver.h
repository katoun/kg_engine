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

#ifndef _PHYSICS_DRIVER_H_
#define _PHYSICS_DRIVER_H_

#include <EngineConfig.h>
#include <core/SystemDriver.h>

#include <glm/glm.hpp>

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

	virtual void setGravity(const glm::vec3& gravity) = 0;

protected:

	static void fireCollisionStarted(Body* body1, Body* body2, const std::vector<CollisionPoint*>& points);

	static void fireCollisionUpdate(Body* body1, Body* body2, const std::vector<CollisionPoint*>& points);

	static void fireCollisionEnded(Body* body1, Body* body2);
};

} // end namespace physics

#endif