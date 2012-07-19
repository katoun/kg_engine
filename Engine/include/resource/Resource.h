/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

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
