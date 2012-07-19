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

#ifndef _COLLISION_POINT_H_
#define _COLLISION_POINT_H_

#include <core/Config.h>
#include <core/Vector3d.h>

namespace physics
{

//! Structure containing information about a collision point.
struct ENGINE_PUBLIC_EXPORT CollisionPoint
{
	CollisionPoint();

	//! The contact position.
	core::vector3d mCollisionPosition;

	//! The contact normal.
	core::vector3d mCollisionNormal;

	//! The distance between closest points. Negative distance indicates inter-penetrations.
	float mDistance;

	//! The impulse magnitude used to resolve the constraints on the bodies along the normal.
	float mImpulse;

	//! Impulse vector applied to the body based on friction in the direction of travel along the second body.
	core::vector3d mImpulseLateral1;

	//! Impulse vector applied to the body based on friction in the direction normal to both the contact normal and the laterl1 vector
	core::vector3d mImpulseLateral2;
};

} // end namespace physics

#endif