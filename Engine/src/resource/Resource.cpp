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

#include <core/Utils.h>
#include <resource/Resource.h>
#include <resource/Serializer.h>
#include <resource/ResourceEvent.h>
#include <resource/ResourceEventReceiver.h>
#include <resource/ResourceManager.h>

#include <Poco/File.h>

namespace resource
{

unsigned int Resource::mIndexCounter = 0;

Resource::Resource(const std::string& filename, Serializer* serializer)
{
	mID = mIndexCounter++;

	mResourceType = RESOURCE_TYPE_UNDEFINED;

	mFilename = filename;

	mSerializer = serializer;

	mState = RESOURCE_STATE_UNLOADED;
	mSize = 0;

	mResourceEvent = new ResourceEvent();
}

Resource::~Resource()
{
	delete mResourceEvent;
}

const unsigned int& Resource::getID() const
{
	return mID;
}

const ResourceType& Resource::getResourceType() const
{ 
	return mResourceType;
}

const std::string& Resource::getFilename() const
{
	return mFilename;
}

const ResourceState& Resource::getState() const
{
	return mState;
}

unsigned int Resource::getSize()
{
	return mSize;
}

bool Resource::load()
{
	if (mState == RESOURCE_STATE_LOADED) return true;

	mState = RESOURCE_STATE_LOADING;

	if (!loadImpl())
	{
		unloadImpl();
		mState = RESOURCE_STATE_UNLOADED;
		return false;
	}

	mState = RESOURCE_STATE_LOADED;

	fireLoaded();

	return true;
}

void Resource::unload()
{
	if (mState == RESOURCE_STATE_UNLOADED)
		return;

	mState = RESOURCE_STATE_UNLOADING;
	
	unloadImpl();

	mState = RESOURCE_STATE_UNLOADED;

	fireUnloaded();
}

bool Resource::reload()
{
	unload();
	return load();
}

void Resource::updateSize()
{
	std::string filePath = ResourceManager::getInstance().getPath(mFilename);
	
	if (filePath == core::STRING_BLANK)
		return;
	
	Poco::File file(filePath);
	if (!file.exists())
		return;
	
	mSize = (unsigned int)file.getSize();
}

void Resource::checkAlreadyLoaded()
{
	//! if there were ResourceEventReceivers added when the resource was already loaded then send them the loaded event
	if (mAlreadyLoadedEventReceivers.size() != 0)
	{
		mResourceEvent->source = this;
		std::list<ResourceEventReceiver*>::iterator i;
		for (i = mAlreadyLoadedEventReceivers.begin(); i != mAlreadyLoadedEventReceivers.end(); ++i)
		{
			(*i)->resourceLoaded(*mResourceEvent);
		}

		mAlreadyLoadedEventReceivers.clear();
	}
}

void Resource::addResourceEventReceiver(ResourceEventReceiver* newEventReceiver)
{
	if (newEventReceiver == NULL)
		return;

	mResourceEventReceivers.push_back(newEventReceiver);

	if (mState == RESOURCE_STATE_LOADED)
	{
		mAlreadyLoadedEventReceivers.push_back(newEventReceiver);
	}
}

void Resource::removeResourceEventReceiver(ResourceEventReceiver* oldEventReceiver)
{
	if (oldEventReceiver == NULL)
		return;

	std::list<ResourceEventReceiver*>::iterator i;
	for (i = mResourceEventReceivers.begin(); i != mResourceEventReceivers.end(); ++i)
	{
		if ((*i) == oldEventReceiver)
		{
			mResourceEventReceivers.erase(i);
			return;
		}
	}
}

bool Resource::loadImpl()
{
	if (mSerializer != NULL)
	{
		return mSerializer->importResource(this, mFilename);
	}

	return false;
}

void Resource::unloadImpl() {}

void Resource::fireLoaded()
{
	// Do loaded event
	mResourceEvent->source = this;

	// Tell all listeners
	std::list<ResourceEventReceiver*>::const_iterator i;
	for (i = mResourceEventReceivers.begin(); i != mResourceEventReceivers.end(); ++i)
	{
		(*i)->resourceLoaded(*mResourceEvent);
	}
}

void Resource::fireUnloaded()
{
	// Do unloaded event
	mResourceEvent->source = this;

	// Tell all listeners
	std::list<ResourceEventReceiver*>::const_iterator i;
	for (i = mResourceEventReceivers.begin(); i != mResourceEventReceivers.end(); ++i)
	{
		(*i)->resourceUnloaded(*mResourceEvent);
	}
}

}// end namespace resource
