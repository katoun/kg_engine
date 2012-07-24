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

#include <game/Component.h>
#include <game/GameObject.h>
#include <core/Utils.h>

namespace game
{

unsigned int Component::mIndexCounter = 0;

Component::Component()
{
	mID = mIndexCounter++;

	mName = "Component_" + core::intToString(mID);

	mType = COMPONENT_TYPE_UNDEFINED;

	mInitialized = false;

	mGameObject = NULL;
}

Component::~Component()
{
	if (mGameObject != NULL)
	{
		mGameObject->detachComponent(this);
	}
}

const unsigned int& Component::getID() const
{
	return mID;
}

const std::string& Component::getName() const
{
	return mName;
}

const unsigned int Component::getType() const
{
	return mType;
}

void Component::initialize()
{
	initializeImpl();
	
	mInitialized = true;
}

void Component::uninitialize()
{
	uninitializeImpl();

	mInitialized = true;
}

void Component::update(float elapsedTime)
{
	updateImpl(elapsedTime);
}

bool Component::isInitialized() const
{
	return mInitialized;
}

void Component::onAttach(GameObject* gameObject)
{
	if (gameObject == NULL)
		return;

	mGameObject = gameObject;
}

void Component::onDetach()
{
	mGameObject = NULL;
}

void Component::onMessage(unsigned int messageID)
{
	onMessageImpl(messageID);
}

GameObject* Component::getGameObject()
{
	return mGameObject;
}

void Component::initializeImpl() {}

void Component::uninitializeImpl() {}

void Component::updateImpl(float elapsedTime) {}

void Component::onMessageImpl(unsigned int messageID) {}

} // end namespace game
