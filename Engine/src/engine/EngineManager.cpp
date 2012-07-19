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
#include <engine/EngineManager.h>
#include <engine/EngineSettings.h>
#include <engine/EngineEvent.h>
#include <engine/EngineEventReceiver.h>
#include <engine/Object.h>
#include <engine/PluginManager.h>
#include <game/GameManager.h>
#include <platform/PlatformManager.h>
#include <input/InputManager.h>
#include <resource/ResourceManager.h>
#include <render/RenderManager.h>
#include <sound/SoundManager.h>
#include <physics/PhysicsManager.h>
#include <scene/SceneManager.h>

template<> engine::EngineManager& core::Singleton<engine::EngineManager>::ms_Singleton = engine::EngineManager();

namespace engine
{

EngineEvent* EngineManager::mEngineEvent = NULL;
std::list<EngineEventReceiver*> EngineManager::mEngineEventReceivers;

EngineManager::EngineManager(): core::System("EngineManager")
{
	// Init
	mVersion = KG_VERSION;

	mVersionString = "The KG 3D Game Engine v"KG_VERSION;

	mEngineEvent = new EngineEvent();

	mLastUpdateStartTimeStamp.update();
	mLastUpdateEndTimeStamp.update();
	mCurrentTimeStamp.update();

#ifdef _DEBUG
	mOptionsFile = "Engine_d.xml";
#else
	mOptionsFile = "Engine.xml";
#endif
}

EngineManager::~EngineManager()
{
	if (mEngineEvent != NULL)
		delete mEngineEvent;

#if defined(_DEBUG) && defined(ENGINE_MEMORY_TRACKER)
	_CrtMemDumpAllObjectsSince(NULL);
#endif // _DEBUG
}

void EngineManager::addEngineEventReceiver(EngineEventReceiver* newEventReceiver)
{
	mEngineEventReceivers.push_back(newEventReceiver);
}

void EngineManager::removeEngineEventReceiver(EngineEventReceiver* oldEventReceiver)
{
	std::list<EngineEventReceiver*>::iterator i;
	for (i = mEngineEventReceivers.begin(); i != mEngineEventReceivers.end(); ++i)
	{
		if ((*i) == oldEventReceiver)
		{
			mEngineEventReceivers.erase(i);
			return;
		}
	}
}

const std::string& EngineManager::getVersion() const
{
	return mVersion;
}

const std::string& EngineManager::getVersionString() const
{
	return mVersionString;
}

void EngineManager::setOptionsFile(const std::string& optionsFile)
{
	mOptionsFile = optionsFile;	
}

void EngineManager::initializeImpl()
{
	loadOptions();

	// Load plugins
	PluginManager::getInstance().initialize();

	// the order is important
	platform::PlatformManager::getInstance().initialize();
	resource::ResourceManager::getInstance().initialize();
	render::RenderManager::getInstance().initialize();
	input::InputManager::getInstance().initialize();

	physics::PhysicsManager::getInstance().initialize();
	sound::SoundManager::getInstance().initialize();
	scene::SceneManager::getInstance().initialize();

	game::GameManager::getInstance().initialize();

	fireEngineInitialized();
}

void EngineManager::uninitializeImpl()
{
	game::GameManager::getInstance().uninitialize();

	physics::PhysicsManager::getInstance().uninitialize();
	sound::SoundManager::getInstance().uninitialize();
	render::RenderManager::getInstance().uninitialize();
	scene::SceneManager::getInstance().uninitialize();
	
	resource::ResourceManager::getInstance().uninitialize();
	input::InputManager::getInstance().uninitialize();
	platform::PlatformManager::getInstance().uninitialize();

	// Unload plugins
	PluginManager::getInstance().uninitialize();

	//save options
	saveOptions();

	fireEngineUninitialized();
}

void EngineManager::startImpl()
{
	// the order is important
	platform::PlatformManager::getInstance().start();
	resource::ResourceManager::getInstance().start();
	input::InputManager::getInstance().start();
	
	game::GameManager::getInstance().start();
	scene::SceneManager::getInstance().start();
	render::RenderManager::getInstance().start();
	physics::PhysicsManager::getInstance().start();
	sound::SoundManager::getInstance().start();

	fireEngineStarted();
}

void EngineManager::stopImpl()
{
	fireEngineStopped();
	
	//Stopping all managers
	sound::SoundManager::getInstance().stop();
	physics::PhysicsManager::getInstance().stop();	
	render::RenderManager::getInstance().stop();
	scene::SceneManager::getInstance().stop();
	game::GameManager::getInstance().stop();
	
	input::InputManager::getInstance().stop();
	resource::ResourceManager::getInstance().stop();
	platform::PlatformManager::getInstance().stop();
}

void EngineManager::updateImpl(float elapsedTime)
{
	fireEngineUpdateStarted();

	// the order is important
	platform::PlatformManager::getInstance().update(mEngineEvent->timeSinceLastUpdate);
	resource::ResourceManager::getInstance().update(mEngineEvent->timeSinceLastUpdate);
	input::InputManager::getInstance().update(mEngineEvent->timeSinceLastUpdate);
	
	game::GameManager::getInstance().update(mEngineEvent->timeSinceLastUpdate);
	scene::SceneManager::getInstance().update(mEngineEvent->timeSinceLastUpdate);
	render::RenderManager::getInstance().update(mEngineEvent->timeSinceLastUpdate);
	physics::PhysicsManager::getInstance().update(mEngineEvent->timeSinceLastUpdate);
	sound::SoundManager::getInstance().update(mEngineEvent->timeSinceLastUpdate);

	fireEngineUpdateEnded();
}

void EngineManager::registerDefaultFactoriesImpl() {}

void EngineManager::removeDefaultFactoriesImpl() {}

void EngineManager::fireEngineInitialized()
{
	// Tell all listeners
	std::list<EngineEventReceiver*>::const_iterator i;
	for (i = mEngineEventReceivers.begin(); i != mEngineEventReceivers.end(); ++i)
	{
		(*i)->engineInitialized();
	}
}

void EngineManager::fireEngineUninitialized()
{
	// Tell all listeners
	std::list<EngineEventReceiver*>::const_iterator i;
	for (i = mEngineEventReceivers.begin(); i != mEngineEventReceivers.end(); ++i)
	{
		(*i)->engineUninitialized();
	}
}

void EngineManager::fireEngineStarted()
{
	// Tell all listeners
	std::list<EngineEventReceiver*>::const_iterator i;
	for (i = mEngineEventReceivers.begin(); i != mEngineEventReceivers.end(); ++i)
	{
		(*i)->engineStarted();
	}
}

void EngineManager::fireEngineStopped()
{
	// Tell all listeners
	std::list<EngineEventReceiver*>::const_iterator i;
	for (i = mEngineEventReceivers.begin(); i != mEngineEventReceivers.end(); ++i)
	{
		(*i)->engineStopped();
	}
}

void EngineManager::fireEngineUpdateStarted()
{
	// Do frame start event
	mCurrentTimeStamp.update();
		
	mEngineEvent->timeSinceLastUpdate = 0.0f;
	if (mCurrentTimeStamp != NULL && mLastUpdateStartTimeStamp != NULL)
	{
		Poco::Timestamp::TimeDiff diff = mCurrentTimeStamp - mLastUpdateStartTimeStamp;
		mEngineEvent->timeSinceLastUpdate = (float)(diff * 1.0f / Poco::Timestamp::resolution());
	}
	mLastUpdateStartTimeStamp.swap(mCurrentTimeStamp);

	// Tell all listeners
	std::list<EngineEventReceiver*>::const_iterator i;
	for (i = mEngineEventReceivers.begin(); i != mEngineEventReceivers.end(); ++i)
	{
		(*i)->engineUpdateStarted(*mEngineEvent);
	}
}

void EngineManager::fireEngineUpdateEnded()
{
	// Do frame start event
	mCurrentTimeStamp.update();

	mEngineEvent->timeSinceLastUpdate = 0.0f;
	if (mCurrentTimeStamp != NULL && mLastUpdateEndTimeStamp != NULL)
	{
		Poco::Timestamp::TimeDiff diff = mCurrentTimeStamp - mLastUpdateEndTimeStamp;
		mEngineEvent->timeSinceLastUpdate = (float)(diff * 1.0f / Poco::Timestamp::resolution());
	}
	mLastUpdateEndTimeStamp.swap(mCurrentTimeStamp);

	// Tell all listeners
	std::list<EngineEventReceiver*>::const_iterator i;
	for (i = mEngineEventReceivers.begin(); i != mEngineEventReceivers.end(); ++i)
	{
		(*i)->engineUpdateEnded(*mEngineEvent);
	}
}

void EngineManager::loadOptions()
{
	EngineSettings::getInstance().loadOptions(mOptionsFile);
}

void EngineManager::saveOptions()
{
	EngineSettings::getInstance().saveOptions(mOptionsFile);
}

EngineManager& EngineManager::getInstance()
{
	return core::Singleton<EngineManager>::getInstance();
}

EngineManager* EngineManager::getInstancePtr()
{
	return core::Singleton<EngineManager>::getInstancePtr();
}

} // end namespace engine
