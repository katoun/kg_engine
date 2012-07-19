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

#ifndef _SPHERE_3D_H_
#define _SPHERE_3D_H_

#include <core/Config.h>
#include <core/Vector3d.h>

namespace core
{

//! Sphere
class ENGINE_PUBLIC_EXPORT sphere3d
{
public:

	// Constructors

	sphere3d();
	sphere3d(const vector3d& center, const float radius);
	sphere3d(const sphere3d& other);

	// operators

	sphere3d& operator=(const sphere3d& other);

	bool operator==(const sphere3d& other) const;
	bool operator!=(const sphere3d& other) const;

	// functions

	inline void set(const vector3d center, const float radius);
	inline void set(const sphere3d& s);

	vector3d Center;
	float Radius;
};

} // end namespace core

#endif