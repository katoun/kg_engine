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
