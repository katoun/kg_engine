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

#ifndef _SHAPE_DEFINES_H_
#define _SHAPE_DEFINES_H_

namespace physics
{

enum ShapeType
{
	SHAPE_TYPE_UNDEFINED,
	SHAPE_TYPE_PLANE,
	SHAPE_TYPE_SPHERE,
	SHAPE_TYPE_BOX,	
	SHAPE_TYPE_CAPSULE,
	SHAPE_TYPE_CONVEX,
	SHAPE_TYPE_MESH,
	SHAPE_TYPE_COUNT
};

} // end namespace physics

#endif
