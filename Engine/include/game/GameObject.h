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

	//! Adds a child to this GameObject.
	void addChild(GameObject* child);

	void removeChild(GameObject* child);
	GameObject* removeChild(const unsigned int& id);

	void removeAllChildren();

	const std::map<unsigned int, GameObject*>& getChildren();

	void attachComponent(Component* component);

	void detachComponent(Component* component);

	Component* getComponent(unsigned int type);

	const std::map<unsigned int, Component*>& getComponents();

	void sendMessage(Component* source, unsigned int messageID);
	void sendMessage(GameObject* source, unsigned int messageID);

	void update(float elapsedTime);

protected:

	unsigned int mID;

	std::string mName;

	static unsigned int mIndexCounter;

	virtual void updateImpl(float elapsedTime);

	GameObject* mParent;

	//! Central list of children.
	std::map<unsigned int, GameObject*> mChildren;

	std::map<unsigned int, Component*> mComponents;
};

} // end namespace game

#endif