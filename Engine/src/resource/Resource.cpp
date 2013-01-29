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

#include <core/Utils.h>
#include <resource/Resource.h>
#include <resource/Serializer.h>
#include <resource/ResourceEvent.h>
#include <resource/ResourceEventReceiver.h>
#include <resource/ResourceManager.h>

#include <stdio.h>

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
	SAFE_DELETE(mResourceEvent);
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

bool Resource::save(const std::string& filename)
{
	return saveImpl(filename);
}

void Resource::updateSize()
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		std::string filePath = resource::ResourceManager::getInstance()->getDataPath() + "/" + mFilename;

		FILE* pFile = fopen(filePath.c_str(), "rb");
	
		if (pFile != nullptr)
		{
			fseek(pFile, 0, SEEK_END);
			mSize = (unsigned int)ftell(pFile);

			fclose(pFile);
		}
	}
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
	if (newEventReceiver == nullptr)
		return;

	mResourceEventReceivers.push_back(newEventReceiver);

	if (mState == RESOURCE_STATE_LOADED)
	{
		mAlreadyLoadedEventReceivers.push_back(newEventReceiver);
	}
}

void Resource::removeResourceEventReceiver(ResourceEventReceiver* oldEventReceiver)
{
	if (oldEventReceiver == nullptr)
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
	if (mSerializer != nullptr)
	{
		return mSerializer->importResource(this, mFilename);
	}

	return false;
}

void Resource::unloadImpl() {}

bool Resource::saveImpl(const std::string& filename)
{
	if (filename.empty())
		return false;

	mFilename = filename;
	
	if (mSerializer != nullptr)
	{
		return mSerializer->exportResource(this, mFilename);
	}

	return false;
}

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
