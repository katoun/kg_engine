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

	void onParentChanged(GameObject* gameObject);

	GameObject* getGameObject();

protected:

	unsigned int mID;

	static unsigned int mIndexCounter;

	unsigned int mType;

	virtual void initializeImpl();
	virtual void uninitializeImpl();
	virtual void updateImpl(float elapsedTime);
	virtual void onParentChangedImpl(GameObject* gameObject);

	bool mInitialized;

	GameObject* mGameObject;
};

} // end namespace game

#endif