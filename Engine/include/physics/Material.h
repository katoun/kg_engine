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

#ifndef _PHYSICSMATERIAL_H_
#define _PHYSICSMATERIAL_H_

#include <core/Config.h>
#include <resource/Resource.h>

#include <string>

namespace resource
{
class Serializer;
}

namespace physics
{

//! Class encapsulates properties of a body.
class ENGINE_PUBLIC_EXPORT Material : public resource::Resource
{
public:

	Material(const std::string& name, resource::Serializer* serializer);
	virtual ~Material();

	//! Sets the restitution of this material.
	virtual void setRestitution(float restitution);

	//! Gets the restitution of this material.
	float getRestitution() const;

	//! Sets the static friction of this material.
	virtual void setStaticFriction(float staticFriction);
	
	//! Gets the static friction of this material.
	float getStaticFriction() const;
	
	//! Sets the dynamic friction of this material.
	virtual void setDynamicFriction(float dynamicFriction);

	//! Gets the dynamic friction of this material.
	float getDynamicFriction() const;

protected:

	void unloadImpl();

	float mRestitution;
	float mStaticFriction;
	float mDynamicFriction;
};

} //namespace physics

#endif