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

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <core/Config.h>
#include <game/ComponentDefines.h>

#include <string>

namespace game
{

class GameObject;

class ENGINE_PUBLIC_EXPORT Component
{
public:

	Component();
	~Component();

	//! Returns the id of the game object.
	const unsigned int& getID() const;

	const std::string& getName() const;

	//! Gets object type.
	const unsigned int getType() const;

	//! Initialize object.
	void initialize();

	//! Uninitialize object.
	void uninitialize();

	void update(float elapsedTime);

	//! Returns true if the Component has been initialized.
	bool isInitialized() const;

	void onAttach(GameObject* gameObject);

	void onDetach();

	void onMessage(unsigned int messageID);

	GameObject* getGameObject();

protected:

	unsigned int mID;

	std::string mName;

	static unsigned int mIndexCounter;

	unsigned int mType;

	virtual void initializeImpl();
	virtual void uninitializeImpl();
	virtual void updateImpl(float elapsedTime);
	virtual void onMessageImpl(unsigned int messageID);

	bool mInitialized;

	GameObject* mGameObject;
};

} // end namespace game

#endif