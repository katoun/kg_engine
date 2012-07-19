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

#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <core/Config.h>
#include <resource/ResourceDefines.h>

#include <string>

namespace resource
{

class Serializer;
class ResourceManager;
struct ResourceEvent;
class ResourceEventReceiver;

//! Abstract class reprensenting a loadable resource (e.g. textures, sounds etc)
//!
//! Resources are generally passive constructs, handled through the
//! ResourceManager abstract class for the appropriate subclass.
//! The main thing is that Resources can be loaded or unloaded by the
//! ResourceManager to stay within a defined memory budget. Therefore,
//! all Resources must be able to load, unload (whilst retainin enough
//! info about themselves to be reloaded later), and state how big
//! they are.
class ENGINE_PUBLIC_EXPORT Resource
{
	friend class Serializer;
	friend class ResourceManager;

public:
    
	Resource(const std::string& filename, Serializer* serializer = NULL);
	virtual ~Resource();

	//! Returns the id of the resource.
	const unsigned int& getID() const;

	//! Gets resource type.
	const ResourceType& getResourceType() const;

	const std::string& getFilename() const;

	const ResourceState& getState() const;

	//! Retrieves info about the size of the resource.
	unsigned int getSize();

	bool load();

	void unload();

	bool reload();

	void updateSize();

	void checkAlreadyLoaded();

	void addResourceEventReceiver(ResourceEventReceiver* newEventReceiver);
	void removeResourceEventReceiver(ResourceEventReceiver* oldEventReceiver);

protected:

	unsigned int mID;

	static unsigned int mIndexCounter;

	ResourceType mResourceType;

	virtual bool loadImpl();
	virtual void unloadImpl();

	std::string mFilename;
	Serializer* mSerializer;
	ResourceState mState;
	unsigned int mSize;

	ResourceEvent* mResourceEvent;
	std::list<ResourceEventReceiver*> mResourceEventReceivers;
	std::list<ResourceEventReceiver*> mAlreadyLoadedEventReceivers;

	//!  internal method for firing events.
	void fireLoaded();
	//!  internal method for firing events.
	void fireUnloaded();
};

}// end namespace resource

#endif
