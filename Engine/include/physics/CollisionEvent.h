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

#ifndef _COLLISION_EVENT_H_
#define _COLLISION_EVENT_H_

#include <core/Config.h>
#include <physics/CollisionPoint.h>

#include <vector>

namespace physics
{

class Body;

//! Structure containing information about a collision event.
struct ENGINE_PUBLIC_EXPORT CollisionEvent
{
	CollisionEvent();

	//! A body involved in the collision.
	Body* mBody1;

	//! Another body involved in the collision.
	Body* mBody2;

	std::vector<CollisionPoint*> mCollisionPoints;
};

} // end namespace physics

#endif