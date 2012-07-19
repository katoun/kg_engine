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

#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <core/Config.h>
#include <core/Singleton.h>
#include <core/System.h>

#include <string>
#include <map>

namespace game
{

class GameObject;
class Component;
class ComponentFactory;
class TransformFactory;

class ENGINE_PUBLIC_EXPORT GameManager: public core::System, public core::Singleton<GameManager>
{
public:
	//! Constructor
	GameManager();

	//! Destructor
	~GameManager();

	//! Creates a game object.
	GameObject* createGameObject();
	GameObject* createGameObject(const std::string& name);

	//! Remove a game object from the managed list.
	void removeGameObject(GameObject* gameObject);
	void removeGameObject(const unsigned int& id);

	void removeAllGameObjects();

	//! Creates a component.
	Component* createComponent(unsigned int type);

	//! Remove a Compoent from the managed list.
	void removeComponent(Component* component);
	void removeComponent(const unsigned int& id);

	void removeAllComponents();

	void registerComponentFactory(unsigned int type, ComponentFactory* factory);
	void removeComponentFactory(unsigned int type);

	static GameManager& getInstance();
	static GameManager* getInstancePtr();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void startImpl();
	void stopImpl();
	void updateImpl(float elapsedTime);
	void registerDefaultFactoriesImpl();
	void removeDefaultFactoriesImpl();

	std::map<unsigned int, GameObject*> mGameObjects;
	std::map<unsigned int, Component*> mComponents;
	std::map<unsigned int, ComponentFactory*> mComponentFactories;

	TransformFactory* mDefaultTransformFactory;
};

} // end namespace engine

#endif