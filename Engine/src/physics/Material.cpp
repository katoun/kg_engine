/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

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
