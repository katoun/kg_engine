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

#include <game/GameObject.h>
#include <game/Component.h>
#include <game/MessageDefines.h>
#include <game/GameManager.h>
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

			game::GameManager::getInstance().removeComponent(pComponent);
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
	sendMessage(this, MESSAGE_PARENT_CHANGED);
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
	sendMessage(this, MESSAGE_PARENT_CHANGED);
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

const std::map<unsigned int, Component*>& GameObject::getComponents()
{
	return mComponents;
}

void GameObject::sendMessage(Component* source, unsigned int messageID)
{
	//Send to components first
	std::map<unsigned int, Component*>::iterator i;
	for (i = mComponents.begin(); i != mComponents.end(); ++i)
	{
		Component* pComponent = i->second;

		if (pComponent != NULL && pComponent != source)
		{
			pComponent->onMessage(messageID);
		}
	}

	//Sent to childer second
	std::map<unsigned int, GameObject*>::iterator j;
	for (j = mChildren.begin(); j != mChildren.end(); ++j)
	{
		GameObject* pGameObject = j->second;
		if (pGameObject != NULL)
		{
			pGameObject->sendMessage(source, messageID);
		}
	}
}

void GameObject::sendMessage(GameObject* source, unsigned int messageID)
{
	//Send to components first
	std::map<unsigned int, Component*>::iterator i;
	for (i = mComponents.begin(); i != mComponents.end(); ++i)
	{
		Component* pComponent = i->second;

		if (pComponent != NULL)
		{
			pComponent->onMessage(messageID);
		}
	}

	//Sent to childer second
	std::map<unsigned int, GameObject*>::iterator j;
	for (j = mChildren.begin(); j != mChildren.end(); ++j)
	{
		GameObject* pGameObject = j->second;
		if (pGameObject != NULL && pGameObject != source)
		{
			pGameObject->sendMessage(source, messageID);
		}
	}
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
