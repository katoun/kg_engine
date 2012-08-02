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
#include <game/GameManager.h>
#include <game/Scene.h>
#include <game/GameObject.h>
#include <game/Component.h>
#include <game/ComponentFactory.h>
#include <game/TransformFactory.h>
#include <game/SceneFactory.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>

template<> game::GameManager& core::Singleton<game::GameManager>::ms_Singleton = game::GameManager();

namespace game
{

GameManager::GameManager(): core::System("GameManager")
{
	mNewSceneName = "NewScene";

	mDefaultSceneFactory = new SceneFactory();

	mDefaultTransformFactory = new TransformFactory();
}

GameManager::~GameManager()
{
	if (mDefaultSceneFactory != NULL)
	{
		delete mDefaultSceneFactory;
	}
	if (mDefaultTransformFactory != NULL)
	{
		delete mDefaultTransformFactory;
	}
}

Scene* GameManager::getScene()
{
	return mCurrentScene;
}

void GameManager::createScene()
{
	Scene* pScene = static_cast<Scene*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_SCENE, mNewSceneName));
	if (pScene == NULL)
		return;

	removeScene();

	// Remove all Components
	removeAllComponents();

	// Remove all GameObjects
	removeAllGameObjects();

	mCurrentScene = pScene;
}

void GameManager::openScene(const std::string& filename)
{
	Scene* pScene = static_cast<Scene*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_SCENE, filename));
	if (pScene == NULL)
		return;

	removeScene();

	// Remove all Components
	removeAllComponents();

	// Remove all GameObjects
	removeAllGameObjects();

	mCurrentScene = pScene;
}

void GameManager::saveScene()
{
	resource::ResourceManager::getInstance().saveResource(mCurrentScene);
}

void GameManager::saveAsScene(const std::string& filename)
{
	resource::ResourceManager::getInstance().saveResource(mCurrentScene, filename);
}

void GameManager::removeScene()
{
	if (mCurrentScene != NULL)
	{
		resource::ResourceManager::getInstance().removeResource(mCurrentScene->getID());
	}
}

GameObject* GameManager::createGameObject(GameObject* parent)
{
	GameObject* pGameObject = new GameObject();
	if (pGameObject == NULL)
		return NULL;

	mGameObjects[pGameObject->getID()] = pGameObject;

	if (parent != NULL)
	{
		pGameObject->setParent(parent);
	}

	/*if (mCurrentScene != NULL)
	{
		mCurrentScene->addGameObject(pGameObject);
	}*/

	return pGameObject;
}

GameObject* GameManager::createGameObject(const std::string& name, GameObject* parent)
{
	GameObject* pGameObject = new GameObject(name);
	if (pGameObject == NULL)
		return NULL;

	mGameObjects[pGameObject->getID()] = pGameObject;

	if (parent != NULL)
	{
		pGameObject->setParent(parent);
	}

	/*if (mCurrentScene != NULL)
	{
		mCurrentScene->addGameObject(pGameObject);
	}*/

	return pGameObject;
}

void GameManager::removeGameObject(GameObject* gameObject)
{
	if (gameObject == NULL)
		return;

	removeGameObject(gameObject->getID());
}

void GameManager::removeGameObject(const unsigned int& id)
{
	std::map<unsigned int, GameObject*>::iterator i = mGameObjects.find(id);
	if (i != mGameObjects.end())
	{
		GameObject* pGameObject = i->second;
		if (pGameObject != NULL)
		{
			/*if (mCurrentScene != NULL)
			{
				mCurrentScene->removeGameObject(pGameObject);
			}
			else*/
			{
				delete pGameObject;
			}
			i->second = NULL;
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
		if (pGameObject != NULL)
		{
			/*if (mCurrentScene != NULL)
			{
				mCurrentScene->removeGameObject(pGameObject);
			}
			else*/
			{
				delete pGameObject;
			}
			i->second = NULL;
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
		if (pComponentFactory != NULL)
		{
			Component* pComponent = pComponentFactory->createComponent();
			if (pComponent != NULL)
			{
				mComponents[pComponent->getID()] = pComponent;

				return pComponent;
			}
		}
	}

	return NULL;
}

void GameManager::removeComponent(Component* component)
{
	if (component == NULL)
		return;

	removeComponent(component->getID());
}

void GameManager::removeComponent(const unsigned int& id)
{
	std::map<unsigned int, Component*>::iterator i = mComponents.find(id);
	if (i != mComponents.end())
	{
		Component* pComponent = i->second;
		if (pComponent != NULL)
		{
			pComponent->onDetach();
			
			std::map<unsigned int, ComponentFactory*>::iterator j = mComponentFactories.find(pComponent->getType());
			if (j != mComponentFactories.end())
			{
				ComponentFactory* pComponentFactory = j->second;
				if (pComponentFactory != NULL)
				{
					pComponentFactory->destroyComponent(pComponent);
				}
			}
			else
			{
				delete pComponent;
			}
			i->second = NULL;

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
		if (pComponent != NULL)
		{
			std::map<unsigned int, ComponentFactory*>::iterator j = mComponentFactories.find(pComponent->getType());
			if (j != mComponentFactories.end())
			{
				ComponentFactory* pComponentFactory = j->second;
				if (pComponentFactory != NULL)
				{
					pComponentFactory->destroyComponent(pComponent);
				}
			}
			else
			{
				delete pComponent;
			}
			i->second = NULL;
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

void GameManager::initializeImpl() {}

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

		if (pGameObject != NULL)
		{
			pGameObject->update(elapsedTime);
		}
	}
}

void GameManager::registerDefaultFactoriesImpl()
{
	resource::ResourceManager::getInstance().registerResourceFactory(resource::RESOURCE_TYPE_SCENE, mDefaultSceneFactory);

	registerComponentFactory(COMPONENT_TYPE_TRANSFORM, mDefaultTransformFactory);
}

void GameManager::removeDefaultFactoriesImpl()
{
	resource::ResourceManager::getInstance().removeResourceFactory(resource::RESOURCE_TYPE_SCENE);

	removeComponentFactory(COMPONENT_TYPE_TRANSFORM);
}

GameManager& GameManager::getInstance()
{
	return core::Singleton<GameManager>::getInstance();
}

GameManager* GameManager::getInstancePtr()
{
	return core::Singleton<GameManager>::getInstancePtr();
}

} // end namespace game
