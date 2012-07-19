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

#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <core/Config.h>
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

	void setResourcesFile(const std::string& resourcesFile);

	//! Creates a resource.
	Resource* createResource(const ResourceType& type, const std::string& filename);

	//! Load all resource waiting for load.
	void loadResources();
	//! Unload all resources.
	void unloadResources();

	bool loadResource(Resource* resource);
	void unloadResource(Resource* resource);
	bool reloadResource(Resource* resource);

	//! Remove a Resource from the managed resources list, calling it's unload() method.
	void removeResource(Resource *resource);
	void removeResource(const unsigned int& id);

	void removeAllResources();

	void registerResourceFactory(const ResourceType& type, ResourceFactory* factory);
	void removeResourceFactory(const ResourceType& type);

	void registerSerializer(const ResourceType& type, Serializer* serializer);
	void removeSerializer(const ResourceType& type);

	//! Adds a relative path to search for resources.
	void addPath(const std::string& path);

	//! Removes a relative path to search for resources.
	void removePath(const std::string& path);

	const std::string getPath(const std::string& filename);

	void addLoadEventReceiver(LoadEventReceiver* newEventReceiver);
	void removeLoadEventReceiver(LoadEventReceiver* oldEventReceiver);

	//! Gets the current memory usage, in bytes.
	unsigned int getMemoryUsage() const;

	static ResourceManager& getInstance();
	static ResourceManager* getInstancePtr();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void startImpl();
	void stopImpl();
	void updateImpl(float elapsedTime);

	void addFiles(const std::string& subpath, const std::string& path);

	std::string mResourcesFile;

	std::list<LoadEventReceiver*> mLoadEventReceivers;
	
	std::map<std::string, std::string> mFiles;

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