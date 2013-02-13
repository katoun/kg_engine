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

#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <EngineConfig.h>
#include <core/Singleton.h>
#include <core/System.h>
#include <resource/ResourceEventReceiver.h>

#include <string>
#include <map>

namespace game
{

class Scene;
class GameObject;
class Component;
class ComponentFactory;
class TransformFactory;
class SceneFactory;

class ENGINE_PUBLIC_EXPORT GameManager: public core::System, public core::Singleton<GameManager>
{
public:
	//! Constructor
	GameManager();

	//! Destructor
	~GameManager();

	//! Gets the current scene.
	Scene* getCurrentScene();
	
	//! Opens a scene to be managed by this game manager as the current scene.
	void openScene(const std::string& filename);

	//! Saves the current scene.
	void saveScene();

	//! Saves the current scene as a new scene.
	void saveAsScene(const std::string& filename);

	//! Removes the current scene.
	void removeScene();

	//! Creates a game object.
	GameObject* createGameObject(GameObject* parent = nullptr);
	GameObject* createGameObject(const std::string& name, GameObject* parent = nullptr);

	//! Remove a game object from the managed list.
	void removeGameObject(GameObject* gameObject);
	void removeGameObject(const unsigned int& id);

	void removeAllGameObjects();

	const std::map<unsigned int, GameObject*>& getGameObjects();

	//! Creates a component.
	Component* createComponent(unsigned int type);

	//! Remove a Compoent from the managed list.
	void removeComponent(Component* component);
	void removeComponent(const unsigned int& id);

	void removeAllComponents();

	void registerComponentFactory(unsigned int type, ComponentFactory* factory);
	void removeComponentFactory(unsigned int type);

	static GameManager* getInstance();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void startImpl();
	void stopImpl();
	void updateImpl(float elapsedTime);
	void registerFactoriesImpl();
	void removeFactoriesImpl();

	std::string mNewSceneName;
	Scene* mCurrentScene;

	std::map<unsigned int, GameObject*> mGameObjects;
	std::map<unsigned int, Component*> mComponents;
	std::map<unsigned int, ComponentFactory*> mComponentFactories;

	SceneFactory* mSceneFactory;

	TransformFactory* mDefaultTransformFactory;
};

} // end namespace engine

#endif