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

#include <core/Log.h>
#include <resource/ResourceManager.h>
#include <resource/Serializer.h>
#include <resource/Resource.h>
#include <resource/ResourceFactory.h>
#include <resource/LoadEvent.h>
#include <resource/LoadEventReceiver.h>
#include <platform/PlatformManager.h>
#include <engine/EngineSettings.h>

#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>

template<> resource::ResourceManager& core::Singleton<resource::ResourceManager>::ms_Singleton = resource::ResourceManager();

namespace resource
{

ResourceManager::ResourceManager(): core::System("ResourceManager")
{
	mLoadResources.resize(RESOURCE_TYPE_COUNT);
	mResourceFactories.resize(RESOURCE_TYPE_COUNT);
	mSerializers.resize(RESOURCE_TYPE_COUNT);
	
	for (unsigned int i = RESOURCE_TYPE_UNDEFINED; i < RESOURCE_TYPE_COUNT; ++i)
	{
		mLoadResources[i] = std::list<Resource*>();
		mResourceFactories[i] = NULL;
		mSerializers[i] = NULL;
	}

	mDataPath = core::STRING_BLANK;
	
	mMemoryUsage = 0;

	mTotalLoadSize = 0;
	mLoadedSize = 0;

	mLoadEvent = new LoadEvent();
}

ResourceManager::~ResourceManager()
{
	delete mLoadEvent;

	// Update memory usage
	mMemoryUsage = 0;
}

Resource* ResourceManager::createResource(const ResourceType& type, const std::string& filename)
{
	if (filename == core::STRING_BLANK)
		return NULL;

	std::string filePath = Poco::Path(filename).toString();
	std::map<std::string, Resource*>::iterator i = mResourcesByFilename.find(filePath);
	if (i != mResourcesByFilename.end())
	{
		return i->second;
	}
	else
	{
		ResourceFactory* resourceFactory = mResourceFactories[(unsigned int)(type)];
		Serializer* serializer = mSerializers[(unsigned int)(type)];

		if (resourceFactory == NULL)
			return NULL;
		
		Resource* newResource = resourceFactory->createResource(filePath, serializer);
		if (newResource == NULL)
			return NULL;

		mResources[newResource->getID()] = newResource;
		mLoadResources[(unsigned int)(type)].push_back(newResource);
		mResourcesByFilename[filePath] = newResource;

		std::string message = "Resource: " + newResource->getFilename() + " id: " + core::intToString(newResource->getID()) + " created.";
		core::Log::getInstance().logMessage("ResourceManager", message);

		return newResource;
	}

	return NULL;
}

void ResourceManager::loadResources()
{
	mTotalLoadSize = 0;
	for (unsigned int i = RESOURCE_TYPE_UNDEFINED; i < RESOURCE_TYPE_COUNT; ++i)
	{
		std::list<Resource*>::iterator j;
		for (j = mLoadResources[i].begin(); j != mLoadResources[i].end(); ++j)
		{
			Resource* resource = (*j);

			assert(resource != NULL);
			if (resource == NULL)
				continue;

			resource->updateSize();

			mTotalLoadSize += resource->getSize();
		}
	}
	mLoadedSize = 0;

	fireLoadStarted();
	
	for (unsigned int i = RESOURCE_TYPE_UNDEFINED; i < RESOURCE_TYPE_COUNT; ++i)
	{
		std::list<Resource*>::iterator j;
		for (j = mLoadResources[i].begin(); j != mLoadResources[i].end(); ++j)
			loadResource(*j);
		
		mLoadResources[i].clear();
	}	
	
	fireLoadEnded();

	mTotalLoadSize = 0;
	mLoadedSize = 0;
	
	core::Log::getInstance().logMessage("ResourceManager", "Resources loaded.");
}

void ResourceManager::unloadResources()
{
	mTotalLoadSize = mMemoryUsage;
	mLoadedSize = 0;
	
	fireLoadStarted();

	std::map<unsigned int, Resource*>::iterator i;
	for (i = mResources.begin(); i != mResources.end(); ++i)
		unloadResource(i->second);

	fireLoadEnded();

	mTotalLoadSize = 0;
	mLoadedSize = 0;

	core::Log::getInstance().logMessage("ResourceManager", "Resources unloaded.");
}

bool ResourceManager::loadResource(Resource* resource)
{
	assert(resource != NULL);
	if (resource == NULL)
		return false;

	if (!resource->load())
		return false;

	// Update memory usage
	mMemoryUsage += resource->getSize();//in bytes
	// Update loaded size;
	mLoadedSize += resource->getSize();//in bytes

	fireLoadUpdate();

	std::string message = "Resource: " + resource->getFilename() + " id: " + core::intToString(resource->getID()) + " loaded.";
	core::Log::getInstance().logMessage("ResourceManager", message);

	return true;
}

void ResourceManager::unloadResource(Resource* resource)
{
	assert(resource != NULL);
	if (resource == NULL)
		return;

	resource->unload();

	// Update memory usage
	mMemoryUsage -= resource->getSize();//in bytes
	// Update loaded size;
	mLoadedSize -= resource->getSize();//in bytes

	fireLoadUpdate();

	std::string message = "Resource: " + resource->getFilename() + " id: " + core::intToString(resource->getID()) + " unloaded.";
	core::Log::getInstance().logMessage("ResourceManager", message);
}

bool ResourceManager::reloadResource(Resource* resource)
{
	assert(resource != NULL);
	if (resource == NULL)
		return false;

	if (!resource->reload())
		return false;

	fireLoadUpdate();

	std::string message = "Resource: " + resource->getFilename() + " id: " + core::intToString(resource->getID()) + " reloaded.";
	core::Log::getInstance().logMessage("ResourceManager", message);

	return true;
}

bool ResourceManager::saveResource(Resource* resource, const std::string& filename)
{
	assert(resource != NULL);
	if (resource == NULL)
		return false;

	if (filename != core::STRING_BLANK)
	{
		std::map<std::string, Resource*>::iterator i = mResourcesByFilename.find(filename);
		if (i != mResourcesByFilename.end())
		{
			mResourcesByFilename.erase(i);
			mResourcesByFilename[filename] = resource;
		}
	}

	if (!resource->save(filename))
		return false;

	std::string message = "Resource: " + resource->getFilename() + " id: " + core::intToString(resource->getID()) + " saved.";
	core::Log::getInstance().logMessage("ResourceManager", message);

	return true;
}

void ResourceManager::removeResource(Resource *resource)
{
	if (resource == NULL)
		return;

	removeResource(resource->getID());
}

void ResourceManager::removeResource(const unsigned int& id)
{
	std::map<unsigned int, Resource*>::iterator i = mResources.find(id);
	if (i != mResources.end())
	{
		Resource* resource = i->second;

		assert(resource != NULL);
		if (resource == NULL)
			return;

		// Remove entry in map
		mResources.erase(i);

		std::map<std::string, Resource*>::iterator j = mResourcesByFilename.find(resource->getFilename());
		if (j != mResourcesByFilename.end())
		{
			mResourcesByFilename.erase(j);
		}

		if (resource->getState() != RESOURCE_STATE_LOADED)
		{
			std::list<Resource*>::iterator k;
			for (k = mLoadResources[(unsigned int)(resource->getResourceType())].begin(); k != mLoadResources[(unsigned int)(resource->getResourceType())].end(); ++k)
			{
				if ((*k)->getID() == id)
				{
					mLoadResources[(unsigned int)(resource->getResourceType())].erase(k);
					break;
				}
			}
		}

		unloadResource(resource);

		ResourceFactory* resourceFactory = mResourceFactories[(unsigned int)(resource->getResourceType())];

		if (resourceFactory)
			resourceFactory->destroyResource(resource);
		else
			delete resource;
	}
}

void ResourceManager::removeAllResources()
{
	std::map<unsigned int, Resource*>::iterator i;
	for (i = mResources.begin(); i != mResources.end(); ++i)
	{
		Resource* resource = i->second;

		assert(resource != NULL);
		if (resource == NULL)
			continue;

		unloadResource(resource);

		ResourceFactory* resourceFactory = mResourceFactories[(unsigned int)(resource->getResourceType())];

		if (resourceFactory != NULL)
			resourceFactory->destroyResource(resource);
		else
			delete resource;
	}

	mResources.clear();
	mResourcesByFilename.clear();
}

void ResourceManager::registerResourceFactory(const ResourceType& type, ResourceFactory* factory)
{
	mResourceFactories[(unsigned int)type] = factory;
}

void ResourceManager::removeResourceFactory(const ResourceType& type)
{
	mResourceFactories[(unsigned int)type] = NULL;
}

void ResourceManager::registerSerializer(const ResourceType& type, Serializer* serializer)
{
	mSerializers[(unsigned int)(type)] = serializer;
}

void ResourceManager::removeSerializer(const ResourceType& type)
{
	mSerializers[(unsigned int)(type)] = NULL;
}

const std::string& ResourceManager::getDataPath()
{
	return mDataPath;
}

void ResourceManager::addLoadEventReceiver(LoadEventReceiver* newEventReceiver)
{
	mLoadEventReceivers.push_back(newEventReceiver);
}

void ResourceManager::removeLoadEventReceiver(LoadEventReceiver* oldEventReceiver)
{
	std::list<LoadEventReceiver*>::iterator i;
	for (i = mLoadEventReceivers.begin(); i != mLoadEventReceivers.end(); ++i)
	{
		if ((*i) == oldEventReceiver)
		{
			mLoadEventReceivers.erase(i);
			return;
		}
	}
}

void ResourceManager::fireLoadStarted()
{	
	// Do load start event
	mLoadEvent->totalSize = mTotalLoadSize;
	mLoadEvent->loadedSize = mLoadedSize;

	// Tell all listeners
	std::list<LoadEventReceiver*>::const_iterator i;
	for (i = mLoadEventReceivers.begin();i != mLoadEventReceivers.end(); ++i)
		(*i)->loadStarted(*mLoadEvent);
}

void ResourceManager::fireLoadUpdate()
{		
	// Do load ended event
	mLoadEvent->totalSize = mTotalLoadSize;
	mLoadEvent->loadedSize = mLoadedSize;

	std::list<LoadEventReceiver*>::const_iterator i;
	for (i = mLoadEventReceivers.begin();i != mLoadEventReceivers.end(); ++i)
		(*i)->loadUpdate(*mLoadEvent);
}

void ResourceManager::fireLoadEnded()
{		
	// Do load ended event
	mLoadEvent->totalSize = mTotalLoadSize;
	mLoadEvent->loadedSize = mLoadedSize;

	std::list<LoadEventReceiver*>::const_iterator i;
	for (i = mLoadEventReceivers.begin();i != mLoadEventReceivers.end(); ++i)
		(*i)->loadEnded(*mLoadEvent);
}

unsigned int ResourceManager::getMemoryUsage() const
{ 
	return mMemoryUsage; 
}

void ResourceManager::initializeImpl()
{
	std::string dataPath = engine::EngineSettings::getInstance().getDataPath();

	mDataPath = Poco::Path(dataPath).toString();
}

void ResourceManager::uninitializeImpl()
{
	// Remove all Resources
	removeAllResources();

	mResourcesByFilename.clear();

	mLoadResources.clear();

	mLoadEventReceivers.clear();
}

void ResourceManager::startImpl()
{
	loadResources();
}

void ResourceManager::stopImpl()
{
	unloadResources();
}

void ResourceManager::updateImpl(float elapsedTime)
{
	std::map<unsigned int, Resource*>::iterator i;
	for (i = mResources.begin(); i != mResources.end(); ++i)
	{
		Resource* resource = i->second;
		if (resource != NULL)
			resource->checkAlreadyLoaded();
	}
}

ResourceManager& ResourceManager::getInstance()
{
	return core::Singleton<ResourceManager>::getInstance();
}

ResourceManager* ResourceManager::getInstancePtr()
{
	return core::Singleton<ResourceManager>::getInstancePtr();
}

}// end namespace resource
