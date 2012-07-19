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

#include <physics/Material.h>

namespace physics
{

Material::Material(const std::string& name, resource::Serializer* serializer): resource::Resource(name, serializer)
{
	mResourceType = resource::RESOURCE_TYPE_PHYSICS_MATERIAL;

	mRestitution = 0.0f;
	mStaticFriction = 0.0f;
	mDynamicFriction = 0.0f;
}

Material::~Material() {}

void Material::setStaticFriction(float staticFriction)
{
	mStaticFriction = staticFriction;
}

float Material::getStaticFriction() const
{
	return mStaticFriction;
}

void Material::setDynamicFriction(float dynamicFriction)
{
	mDynamicFriction = dynamicFriction;
}

float Material::getDynamicFriction() const
{
	return mDynamicFriction;
}

void Material::setRestitution(float restitution)
{
	mRestitution = restitution;
}

float Material::getRestitution() const
{
	return mRestitution;
}

void Material::unloadImpl()
{
	mRestitution = 0.0f;
	mStaticFriction = 0.0f;
	mDynamicFriction = 0.0f;
}

} //namespace physics
