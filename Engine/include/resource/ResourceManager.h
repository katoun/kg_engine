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

#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <core/Config.h>
#include <core/Utils.h>
#include <core/System.h>
#include <core/Singleton.h>
#include <core/Math.h>

#include <string>
#include <vector>
#include <list>
#include <map>

namespace resource
{

class Serializer;
class Resource;
class ResourceFactory;
class LoadEventReceiver;
struct LoadEvent;
enum ResourceType;

//! A resource manager is responsible for managing a pool of
//! resources of a particular type. It must index them, look
//! them up, load and destroy them. It may also need to stay within
//! a defined memory budget, and temporaily unload some resources
//! if it needs to to stay within this budget.
//! \todo: Implement priorities.
class ENGINE_PUBLIC_EXPORT ResourceManager: public core::System, public core::Singleton<ResourceManager>
{
public:

	ResourceManager();
	~ResourceManager();

	//! Creates a resource.
	Resource* createResource(const ResourceType& type, const std::string& filename);

	//! Load all resource waiting for load.
	void loadResources();
	//! Unload all resources.
	void unloadResources();

	bool loadResource(Resource* resource);
	void unloadResource(Resource* resource);
	bool reloadResource(Resource* resource);
	bool saveResource(Resource* resource, const std::string& filename = "");

	//! Remove a Resource from the managed resources list, calling it's unload() method.
	void removeResource(Resource *resource);
	void removeResource(const unsigned int& id);

	void removeAllResources();

	void registerResourceFactory(const ResourceType& type, ResourceFactory* factory);
	void removeResourceFactory(const ResourceType& type);

	void registerSerializer(const ResourceType& type, Serializer* serializer);
	void removeSerializer(const ResourceType& type);

	const std::string& getDataPath();

	void addLoadEventReceiver(LoadEventReceiver* newEventReceiver);
	void removeLoadEventReceiver(LoadEventReceiver* oldEventReceiver);

	//! Gets the current memory usage, in bytes.
	unsigned int getMemoryUsage() const;

	static ResourceManager* getInstance();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void startImpl();
	void stopImpl();
	void updateImpl(float elapsedTime);

	std::list<LoadEventReceiver*> mLoadEventReceivers;

	std::string mDataPath;

	//! Central lists of resources for loading created in order of type.
	std::vector<std::list<Resource*>> mLoadResources;

	std::vector<ResourceFactory*> mResourceFactories;

	std::vector<Serializer*> mSerializers;

	//! Central list of resources - for easy memory management and lookup.
	std::map<unsigned int, Resource*> mResources;
	std::map<std::string, Resource*> mResourcesByFilename;
	
	unsigned int mMemoryUsage;		// In bytes

	unsigned int mTotalLoadSize;
	unsigned int mLoadedSize;

	LoadEvent* mLoadEvent;

	void fireLoadStarted();

	void fireLoadUpdate();

	void fireLoadEnded();
};

}// end namespace resource

#endif