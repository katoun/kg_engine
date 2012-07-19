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

#include <physics/PhysicsDriver.h>
#include <physics/PhysicsManager.h>

namespace physics
{

PhysicsDriver::PhysicsDriver(const std::string& name): core::SystemDriver(name) {}

PhysicsDriver::~PhysicsDriver() {}

void PhysicsDriver::fireCollisionStarted(Body* body1, Body* body2, const std::vector<CollisionPoint*>& points)
{
	PhysicsManager::getInstance().fireCollisionStarted(body1, body2, points);
}

void PhysicsDriver::fireCollisionUpdate(Body* body1, Body* body2, const std::vector<CollisionPoint*>& points)
{
	PhysicsManager::getInstance().fireCollisionUpdate(body1, body2, points);
}

void PhysicsDriver::fireCollisionEnded(Body* body1, Body* body2)
{
	PhysicsManager::getInstance().fireCollisionEnded(body1, body2);
}

} // end namespace physics
