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

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <core/Config.h>
#include <engine/ObjectDefines.h>
#include <core/Singleton.h>

#include <string>

namespace engine
{

class ENGINE_PUBLIC_EXPORT Object
{
public:

	Object();
	Object(const std::string& name);

	~Object();

	//! Returns the id of the object.
	const unsigned int& getID() const;

	//! Gets object type.
	const ObjectType& getObjectType() const;

	void setName(const std::string& name);
	const std::string& getName() const;

	//! Initialize object.
	void initialize();

	//! Uninitialize object.
	void uninitialize();

	void update(float elapsedTime);

	//! Returns true if the Object has been initialized.
	bool isInitialized() const;

protected:

	unsigned int mID;

	bool mChangeableName;
	std::string mName;

	ObjectType mObjectType;

	//! Incremented count for next object index
	static unsigned int msNextGeneratedObjectIndex;

	static unsigned int mIndexCounter;

	virtual void initializeImpl();
	virtual void uninitializeImpl();
	virtual void updateImpl(float elapsedTime);

	bool mInitialized;
};

} // end namespace engine

#endif