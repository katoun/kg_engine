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

#include <engine/Object.h>
#include <core/Utils.h>

namespace engine
{

unsigned int Object::msNextGeneratedObjectIndex = 0;
unsigned int Object::mIndexCounter = 0;

Object::Object()
{
	mID = mIndexCounter++;

	mName = "Object_" + core::intToString(msNextGeneratedObjectIndex++);

	mObjectType = OT_UNDEFINED;

	mChangeableName = true;

	mInitialized = false;
}

Object::Object(const std::string& name)
{
	mID = mIndexCounter++;

	mName = name;

	mObjectType = OT_UNDEFINED;

	mChangeableName = true;

	mInitialized = false;
}

Object::~Object() {}

const unsigned int& Object::getID() const
{
	return mID;
}

const ObjectType& Object::getObjectType() const
{
	return mObjectType;
}

void Object::setName(const std::string& name)
{
	if (mChangeableName) mName = name;
}

const std::string& Object::getName() const
{
	return mName;
}

void Object::initialize()
{
	initializeImpl();
	
	mInitialized = true;
}

void Object::uninitialize()
{
	uninitializeImpl();

	mInitialized = true;
}

void Object::update(float elapsedTime)
{
	updateImpl(elapsedTime);
}

bool Object::isInitialized() const
{
	return mInitialized;
}

void Object::initializeImpl() {}

void Object::uninitializeImpl() {}

void Object::updateImpl(float elapsedTime) {}

} // end namespace engine
