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

#include <sound/Listener.h>
#include <core/Vector3d.h>
#include <core/Utils.h>

namespace sound
{

unsigned int Listener::msNextGeneratedListenerIndex = 0;

Listener::Listener(): scene::Node("Listener_" + core::intToString(msNextGeneratedListenerIndex++))
{
	mNodeType = scene::NT_LISTENER;

	mVelocity = core::vector3d::ORIGIN_3D;
}

Listener::Listener(const std::string& name): scene::Node(name)
{
	mNodeType = scene::NT_LISTENER;

	mVelocity = core::vector3d::ORIGIN_3D;
}

Listener::~Listener() {}

void Listener::setVelocity(float x, float y, float z)
{
	mVelocity.X = x;
	mVelocity.Y = y;
	mVelocity.Z = z;
	mModifiedAbsoluteTransform = true;
}

void Listener::setVelocity(const core::vector3d& vec)
{
	mVelocity = vec;
	mModifiedAbsoluteTransform = true;
}

const core::vector3d& Listener::getVelocity() const
{
	return mVelocity;
}

} // end namespace sound
