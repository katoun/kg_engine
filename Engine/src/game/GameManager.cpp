/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2013 Catalin Alexandru Nastase

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
#include <game/GameManager.h>
#include <game/Scene.h>
#include <game/GameObject.h>
#include <game/Component.h>
#include <game/ComponentFactory.h>
#include <game/TransformFactory.h>
#include <game/SceneFactory.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>

template<> game::GameManager* core::Singleton<game::GameManager>::m_Singleton = nullptr;

namespace game
{

GameManager::GameManager(): core::System("GameManager")
{
	mNewSceneName				= "NewScene";
	mCurrentScene				= nullptr;

	mSceneFactory		= new SceneFactory();

	mDefaultTransformFactory	= new TransformFactory();
}

GameManager::~GameManager()
{
	SAFE_DELETE(mSceneFactory);
	SAFE_DELETE(mDefaultTransformFactory);
}

Scene* GameManager::getCurrentScene()
{
	return mCurrentScene;
}

void GameManager::openScene(const std::string& filename)
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		Scene* pScene = static_cast<Scene*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_SCENE, filename));
		if (pScene == nullptr)
			return;

		removeScene();

		// Remove all Components
		removeAllComponents();

		// Remove all GameObjects
		removeAllGameObjects();

		mCurrentScene = pScene;
	}
}

void GameManager::saveScene()
{
	if (resource::ResourceManager::getInstance() != nullptr)
		resource::ResourceManager::getInstance()->saveResource(mCurrentScene);
}

void GameManager::saveAsScene(const std::string& filename)
{
	if (resource::ResourceManager::getInstance() != nullptr)
		resource::ResourceManager::getInstance()->saveResource(mCurrentScene, filename);
}

void GameManager::removeScene()
{
	if (mCurrentScene != nullptr && resource::ResourceManager::getInstance() != nullptr)
	{
		resource::ResourceManager::getInstance()->removeResource(mCurrentScene->getID());
	}
}

GameObject* GameManager::createGameObject(GameObject* parent)
{
	GameObject* pGameObject = new GameObject();
	if (pGameObject == nullptr)
		return nullptr;

	mGameObjects[pGameObject->getID()] = pGameObject;

	if (parent != nullptr)
	{
		pGameObject->setParent(parent);
	}

	/*if (mCurrentScene != nullptr)
	{
		mCurrentScene->addGameObject(pGameObject);
	}*/

	return pGameObject;
}

GameObject* GameManager::createGameObject(const std::string& name, GameObject* parent)
{
	GameObject* pGameObject = new GameObject(name);
	if (pGameObject == nullptr)
		return nullptr;

	mGameObjects[pGameObject->getID()] = pGameObject;

	if (parent != nullptr)
	{
		pGameObject->setParent(parent);
	}

	/*if (mCurrentScene != nullptr)
	{
		mCurrentScene->addGameObject(pGameObject);
	}*/

	return pGameObject;
}

void GameManager::removeGameObject(GameObject* gameObject)
{
	if (gameObject == nullptr)
		return;

	removeGameObject(gameObject->getID());
}

void GameManager::removeGameObject(const unsigned int& id)
{
	std::map<unsigned int, GameObject*>::iterator i = mGameObjects.find(id);
	if (i != mGameObjects.end())
	{
		GameObject* pGameObject = i->second;
		if (pGameObject != nullptr)
		{
			/*if (mCurrentScene != nullptr)
			{
				mCurrentScene->removeGameObject(pGameObject);
			}
			else*/
			{
				SAFE_DELETE(pGameObject);
			}
			i->second = nullptr;
		}
		mGameObjects.erase(i);
	}
}

void GameManager::removeAllGameObjects()
{
	std::map<unsigned int, GameObject*>::iterator i;
	for (i = mGameObjects.begin(); i != mGameObjects.end(); ++i)
	{
		GameObject* pGameObject = i->second;
		if (pGameObject != nullptr)
		{
			/*if (mCurrentScene != nullptr)
			{
				mCurrentScene->removeGameObject(pGameObject);
			}
			else*/
			{
				SAFE_DELETE(pGameObject);
			}
			i->second = nullptr;
		}
	}

	mGameObjects.clear();
}

const std::map<unsigned int, GameObject*>& GameManager::getGameObjects()
{
	return mGameObjects;
}

Component* GameManager::createComponent(unsigned int type)
{
	std::map<unsigned int, ComponentFactory*>::iterator i = mComponentFactories.find(type);
	if (i != mComponentFactories.end())
	{
		ComponentFactory* pComponentFactory = i->second;
		if (pComponentFactory != nullptr)
		{
			Component* pComponent = pComponentFactory->createComponent();
			if (pComponent != nullptr)
			{
				mComponents[pComponent->getID()] = pComponent;

				return pComponent;
			}
		}
	}

	return nullptr;
}

void GameManager::removeComponent(Component* component)
{
	if (component == nullptr)
		return;

	removeComponent(component->getID());
}

void GameManager::removeComponent(const unsigned int& id)
{
	std::map<unsigned int, Component*>::iterator i = mComponents.find(id);
	if (i != mComponents.end())
	{
		Component* pComponent = i->second;
		if (pComponent != nullptr)
		{
			pComponent->onDetach();
			
			std::map<unsigned int, ComponentFactory*>::iterator j = mComponentFactories.find(pComponent->getType());
			if (j != mComponentFactories.end())
			{
				ComponentFactory* pComponentFactory = j->second;
				if (pComponentFactory != nullptr)
				{
					pComponentFactory->destroyComponent(pComponent);
				}
			}
			else
			{
				SAFE_DELETE(pComponent);
			}
			i->second = nullptr;

			mComponents.erase(i);
		}
	}
}

void GameManager::removeAllComponents()
{
	std::map<unsigned int, Component*>::iterator i;
	for (i = mComponents.begin(); i != mComponents.end(); ++i)
	{
		Component* pComponent = i->second;
		if (pComponent != nullptr)
		{
			std::map<unsigned int, ComponentFactory*>::iterator j = mComponentFactories.find(pComponent->getType());
			if (j != mComponentFactories.end())
			{
				ComponentFactory* pComponentFactory = j->second;
				if (pComponentFactory != nullptr)
				{
					pComponentFactory->destroyComponent(pComponent);
				}
			}
			else
			{
				SAFE_DELETE(pComponent);
			}
			i->second = nullptr;
		}
	}

	mComponents.clear();
}

void GameManager::registerComponentFactory(unsigned int type, ComponentFactory* factory)
{
	std::map<unsigned int, ComponentFactory*>::const_iterator i = mComponentFactories.find(type);
	if (i == mComponentFactories.end())
	{
		mComponentFactories[type] = factory;
	}
}

void GameManager::removeComponentFactory(unsigned int type)
{
	std::map<unsigned int, ComponentFactory*>::iterator i = mComponentFactories.find(type);
	if (i != mComponentFactories.end())
	{
		mComponentFactories.erase(i);
	}
}

void GameManager::initializeImpl()
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		Scene* pScene = static_cast<Scene*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_SCENE, mNewSceneName));
		if (pScene == nullptr)
			return;

		mCurrentScene = pScene;
	}
}

void GameManager::uninitializeImpl()
{
	// Remove all Components
	removeAllComponents();

	// Remove all GameObjects
	removeAllGameObjects();
}

void GameManager::startImpl() {}

void GameManager::stopImpl() {}

void GameManager::updateImpl(float elapsedTime)
{
	// update game objects
	std::map<unsigned int, GameObject*>::iterator i;
	for (i = mGameObjects.begin(); i != mGameObjects.end(); ++i)
	{
		GameObject* pGameObject = i->second;

		if (pGameObject != nullptr)
		{
			pGameObject->update(elapsedTime);
		}
	}
}

void GameManager::registerFactoriesImpl()
{
	if (resource::ResourceManager::getInstance() != nullptr)
		resource::ResourceManager::getInstance()->registerResourceFactory(resource::RESOURCE_TYPE_SCENE, mSceneFactory);

	registerComponentFactory(COMPONENT_TYPE_TRANSFORM, mDefaultTransformFactory);
}

void GameManager::removeFactoriesImpl()
{
	if (resource::ResourceManager::getInstance() != nullptr)
		resource::ResourceManager::getInstance()->removeResourceFactory(resource::RESOURCE_TYPE_SCENE);

	removeComponentFactory(COMPONENT_TYPE_TRANSFORM);
}

GameManager* GameManager::getInstance()
{
	return core::Singleton<GameManager>::getInstance();
}

} // end namespace game
