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

#include <game/Component.h>
#include <game/GameObject.h>

namespace game
{

unsigned int Component::mIndexCounter = 0;

Component::Component()
{
	mID = mIndexCounter++;

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

void Component::onParentChanged(GameObject* gameObject)
{
	onParentChangedImpl(gameObject);
}

GameObject* Component::getGameObject()
{
	return mGameObject;
}

void Component::initializeImpl() {}

void Component::uninitializeImpl() {}

void Component::updateImpl(float elapsedTime) {}

void Component::onParentChangedImpl(GameObject* gameObject) {}

} // end namespace game
