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

#include <game/GameObject.h>
#include <game/Component.h>
#include <core/Utils.h>

namespace game
{

unsigned int GameObject::mIndexCounter = 0;

GameObject::GameObject()
{
	mID = mIndexCounter++;

	mName = "GameObject_" + core::intToString(mID);

	mParent = NULL;
}

GameObject::GameObject(const std::string& name)
{
	mID = mIndexCounter++;

	mName = name;

	mParent = NULL;
}

GameObject::~GameObject()
{
	removeAllChildren();

	if (mParent != NULL)
		mParent->removeChild(this);

	std::map<unsigned int, Component*>::iterator i;
	for (i = mComponents.begin(); i != mComponents.end(); ++i)
	{
		Component* pComponent = i->second;

		if (pComponent != NULL)
		{
			i->second = NULL;
			pComponent->onDetach();
		}
	}

	mComponents.clear();
}

const unsigned int& GameObject::getID() const
{
	return mID;
}

const std::string& GameObject::getName() const
{
	return mName;
}

void GameObject::setName(const std::string& name)
{
	mName = name;
}

GameObject* GameObject::getParent()
{
	return mParent;
}

void GameObject::setParent(GameObject* parent)
{
	if (parent == NULL)
		return;

	if (mParent != NULL)
		mParent->removeChild(this);

	mParent = parent;
	mParent->mChildren[mID] = this;
	//notify components that parent game object has changed!!!
	std::map<unsigned int, Component*>::iterator i;
	for (i = mComponents.begin(); i != mComponents.end(); ++i)
	{
		Component* pComponent = i->second;

		if (pComponent != NULL)
		{
			pComponent->onParentChanged(parent);
		}
	}
}

std::map<unsigned int, GameObject*>& GameObject::getChildren()
{
	return mChildren;
}

void GameObject::addChild(GameObject* child)
{
	if (child == NULL)
		return;

	if (child->mParent != NULL)
		child->mParent->removeChild(child);

	mChildren[child->getID()] = child;
	child->mParent = this;
	
	//notify child components that parent game object has changed!!!
	std::map<unsigned int, Component*>::iterator i;
	for (i = mComponents.begin(); i != mComponents.end(); ++i)
	{
		Component* pComponent = i->second;

		if (pComponent != NULL)
		{
			pComponent->onParentChanged(this);
		}
	}
}

void GameObject::removeChild(GameObject* child)
{
	if (child == NULL)
		return;

	removeChild(child->getID());
}

GameObject* GameObject::removeChild(const unsigned int& id)
{
	std::map<unsigned int, GameObject*>::iterator i = mChildren.find(id);
	if (i != mChildren.end())
	{
		GameObject* pGameObject = i->second;
		if (pGameObject != NULL)
		{
			pGameObject->mParent = NULL;
			mChildren.erase(i);

			return pGameObject;
		}
	}

	return NULL;
}

void GameObject::removeAllChildren()
{
	std::map<unsigned int, GameObject*>::iterator i;
	for (i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		GameObject* pGameObject = i->second;
		if (pGameObject != NULL)
		{
			pGameObject->mParent = NULL;
		}
	}
	
	mChildren.clear();
}

void GameObject::attachComponent(Component* component)
{
	if (component == NULL)
		return;

	std::map<unsigned int, Component*>::iterator i = mComponents.find(component->getType());
	if (i == mComponents.end())
	{
		mComponents[component->getType()] = component;
		component->onAttach(this);
	}
}

void GameObject::detachComponent(Component* component)
{
	if (component == NULL)
		return;

	std::map<unsigned int, Component*>::iterator i = mComponents.find(component->getType());
	if (i != mComponents.end())
	{
		mComponents.erase(i);
		component->onDetach();
	}
}

Component* GameObject::getComponent(unsigned int type)
{
	std::map<unsigned int, Component*>::iterator i = mComponents.find(type);
	if (i != mComponents.end())
	{
		return i->second;
	}

	return NULL;
}

void GameObject::update(float elapsedTime)
{
	updateImpl(elapsedTime);
}

void GameObject::updateImpl(float elapsedTime)
{
	std::map<unsigned int, Component*>::iterator i;
	for (i = mComponents.begin(); i != mComponents.end(); ++i)
	{
		Component* pComponent = i->second;

		if (pComponent != NULL)
		{
			pComponent->update(elapsedTime);
		}
	}
}

} // end namespace game
