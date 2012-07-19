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

#ifndef _LISTENER_H_
#define _LISTENER_H_

#include <core/Config.h>
#include <scene/Node.h>

namespace core
{
class vector3d;
}

namespace sound
{
//!  Defines a listener in the sound world.
//!  Author: Kat'Oun
//!  version: 1.0
class ENGINE_PUBLIC_EXPORT Listener: public scene::Node
{
public:

	Listener();
	Listener(const std::string& name);

	virtual ~Listener();

	void setVelocity(float x, float y, float z);
	void setVelocity(const core::vector3d& vec);
	const core::vector3d& getVelocity() const;

protected:

	// Incremented count for next index
	static unsigned int msNextGeneratedListenerIndex;

	core::vector3d mVelocity;
};

} // end namespace sound

#endif