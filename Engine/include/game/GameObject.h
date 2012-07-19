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

#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <core/Config.h>

#include <string>
#include <map>

namespace game
{

class Component;

class ENGINE_PUBLIC_EXPORT GameObject
{
public:

	GameObject();
	GameObject(const std::string& name);

	~GameObject();

	//! Returns the id of the game object.
	const unsigned int& getID() const;

	const std::string& getName() const;
	void setName(const std::string& name);

	//! Gets this GameObject's parent.
	GameObject* getParent();

	//! Sets this GameObject's parent.
	void setParent(GameObject* parent);

	std::map<unsigned int, GameObject*>& getChildren();

	//! Adds a child to this GameObject.
	void addChild(GameObject* child);

	void removeChild(GameObject* child);
	GameObject* removeChild(const unsigned int& id);

	void removeAllChildren();

	void attachComponent(Component* component);

	void detachComponent(Component* component);

	Component* getComponent(unsigned int type);

	void update(float elapsedTime);

protected:

	unsigned int mID;

	std::string mName;

	static unsigned int mIndexCounter;

	GameObject* mParent;

	//! Central list of children.
	std::map<unsigned int, GameObject*> mChildren;

	std::map<unsigned int, Component*> mComponents;

	virtual void updateImpl(float elapsedTime);
};

} // end namespace game

#endif