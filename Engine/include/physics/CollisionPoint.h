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