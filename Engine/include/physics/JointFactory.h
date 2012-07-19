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

#ifndef _JOINT_FACTORY_H_
#define _JOINT_FACTORY_H_

#include <core/Config.h>

namespace physics
{

class Joint;
enum JointType;

class ENGINE_PUBLIC_EXPORT JointFactory
{
public:

	//! Creates a new joint.
	virtual Joint* createJoint(JointType type) = 0;
	virtual Joint* createJoint(const std::string& name, JointType type) = 0;

	//! Destroys a joint which was created by this factory.
	virtual void destroyJoint(Joint* joint) = 0;
};

} // end namespace physics

#endif