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
#include <core/Log.h>
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

template<> engine::EngineManager* core::Singleton<engine::EngineManager>::m_Singleton = nullptr;

namespace engine
{

EngineEvent* EngineManager::mEngineEvent = nullptr;
std::list<EngineEventReceiver*> EngineManager::mEngineEventReceivers;

EngineManager::EngineManager(): core::System("EngineManager")
{
	// Init
	mVersion = KG_VERSION;

	mVersionString = "The KG 3D Game Engine v"KG_VERSION;

	mEngineEvent = new EngineEvent();

	mLastUpdateStartTime	= 0;
	mLastUpdateEndTime		= 0;

	mLog						= new core::Log();
	mSettings					= new EngineSettings();
	mPluginManager				= new PluginManager();
	mPlatformManager			= new platform::PlatformManager();
	mInputManager				= new input::InputManager;
	mResourceManager			= new resource::ResourceManager();
	mRenderManager				= new render::RenderManager();
	mSoundManager				= new sound::SoundManager();
	mPhysicsManager				= new physics::PhysicsManager();
	mGameManager				= new game::GameManager();

	resetTimer();

#ifdef _DEBUG
	mOptionsFile = "Engine_d.xml";
#else
	mOptionsFile = "Engine.xml";
#endif
}

EngineManager::~EngineManager()
{
	SAFE_DELETE(mEngineEvent);

	SAFE_DELETE(mLog);
	SAFE_DELETE(mSettings);
	SAFE_DELETE(mPlatformManager);
	SAFE_DELETE(mInputManager);
	SAFE_DELETE(mResourceManager);
	SAFE_DELETE(mRenderManager);
	SAFE_DELETE(mSoundManager);
	SAFE_DELETE(mPhysicsManager);
	SAFE_DELETE(mGameManager);

#if defined(_DEBUG) && defined(ENGINE_MEMORY_TRACKER)
	_CrtMemDumpAllObjectsSince(nullptr);
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
	if (PluginManager::getInstance() != nullptr)
		PluginManager::getInstance()->initialize();

	engine::EngineSettings* engineOptions = engine::EngineSettings::getInstance();
	if (engine::EngineSettings::getInstance() != nullptr && render::RenderManager::getInstance() != nullptr)
	{
		//create the main window
		render::RenderWindow* pMainWindow = render::RenderManager::getInstance()->createRenderWindow(
											engine::EngineSettings::getInstance()->getWidth(),
											engine::EngineSettings::getInstance()->getHeight(),
											engine::EngineSettings::getInstance()->getBitdepth(),
											engine::EngineSettings::getInstance()->getFullscreen(),
											0,
											0,
											true,
											engine::EngineSettings::getInstance()->getMainWindowId());
		
		render::RenderManager::getInstance()->setMainWindow(pMainWindow);
	}

	// the order is important
	if (platform::PlatformManager::getInstance()	!= nullptr) platform::PlatformManager::getInstance()->initialize();
	if (resource::ResourceManager::getInstance()	!= nullptr) resource::ResourceManager::getInstance()->initialize();
	if (input::InputManager::getInstance()			!= nullptr) input::InputManager::getInstance()->initialize();

	if (render::RenderManager::getInstance()		!= nullptr) render::RenderManager::getInstance()->initialize();
	if (physics::PhysicsManager::getInstance()		!= nullptr) physics::PhysicsManager::getInstance()->initialize();
	if (sound::SoundManager::getInstance()			!= nullptr) sound::SoundManager::getInstance()->initialize();

	if (game::GameManager::getInstance()			!= nullptr) game::GameManager::getInstance()->initialize();

	fireEngineInitialized();
}

void EngineManager::uninitializeImpl()
{
	if (game::GameManager::getInstance()			!= nullptr) game::GameManager::getInstance()->uninitialize();


	if (physics::PhysicsManager::getInstance()		!= nullptr) physics::PhysicsManager::getInstance()->uninitialize();
	if (sound::SoundManager::getInstance()			!= nullptr) sound::SoundManager::getInstance()->uninitialize();
	if (render::RenderManager::getInstance()		!= nullptr) render::RenderManager::getInstance()->uninitialize();
	
	if (resource::ResourceManager::getInstance()	!= nullptr) resource::ResourceManager::getInstance()->uninitialize();
	if (input::InputManager::getInstance()			!= nullptr) input::InputManager::getInstance()->uninitialize();
	if (platform::PlatformManager::getInstance()	!= nullptr) platform::PlatformManager::getInstance()->uninitialize();

	// Unload plugins
	if (PluginManager::getInstance()				!= nullptr) PluginManager::getInstance()->uninitialize();

	//save options
	saveOptions();

	fireEngineUninitialized();
}

void EngineManager::startImpl()
{
	// the order is important
	if (platform::PlatformManager::getInstance()	!= nullptr) platform::PlatformManager::getInstance()->start();
	if (resource::ResourceManager::getInstance()	!= nullptr) resource::ResourceManager::getInstance()->start();
	if (input::InputManager::getInstance()			!= nullptr) input::InputManager::getInstance()->start();
	
	if (game::GameManager::getInstance()			!= nullptr) game::GameManager::getInstance()->start();
	if (render::RenderManager::getInstance()		!= nullptr) render::RenderManager::getInstance()->start();
	if (physics::PhysicsManager::getInstance()		!= nullptr) physics::PhysicsManager::getInstance()->start();
	if (sound::SoundManager::getInstance()			!= nullptr) sound::SoundManager::getInstance()->start();

	fireEngineStarted();
}

void EngineManager::stopImpl()
{
	fireEngineStopped();
	
	//Stopping all managers
	if (sound::SoundManager::getInstance()			!= nullptr) sound::SoundManager::getInstance()->stop();
	if (physics::PhysicsManager::getInstance()		!= nullptr) physics::PhysicsManager::getInstance()->stop();
	if (render::RenderManager::getInstance()		!= nullptr) render::RenderManager::getInstance()->stop();
	if (game::GameManager::getInstance()			!= nullptr) game::GameManager::getInstance()->stop();
	
	if (input::InputManager::getInstance()			!= nullptr) input::InputManager::getInstance()->stop();
	if (resource::ResourceManager::getInstance()	!= nullptr) resource::ResourceManager::getInstance()->stop();
	if (platform::PlatformManager::getInstance()	!= nullptr) platform::PlatformManager::getInstance()->stop();
}

void EngineManager::updateImpl(float elapsedTime)
{
	fireEngineUpdateStarted();

	// the order is important
	if (platform::PlatformManager::getInstance()	!= nullptr) platform::PlatformManager::getInstance()->update(mEngineEvent->timeSinceLastUpdate);
	if (resource::ResourceManager::getInstance()	!= nullptr) resource::ResourceManager::getInstance()->update(mEngineEvent->timeSinceLastUpdate);
	if (input::InputManager::getInstance()			!= nullptr) input::InputManager::getInstance()->update(mEngineEvent->timeSinceLastUpdate);
	
	if (game::GameManager::getInstance()			!= nullptr) game::GameManager::getInstance()->update(mEngineEvent->timeSinceLastUpdate);
	if (render::RenderManager::getInstance()		!= nullptr) render::RenderManager::getInstance()->update(mEngineEvent->timeSinceLastUpdate);
	if (physics::PhysicsManager::getInstance()		!= nullptr) physics::PhysicsManager::getInstance()->update(mEngineEvent->timeSinceLastUpdate);
	if (sound::SoundManager::getInstance()			!= nullptr) sound::SoundManager::getInstance()->update(mEngineEvent->timeSinceLastUpdate);

	fireEngineUpdateEnded();
}

void EngineManager::resetTimer()
{
#if ENGINE_PLATFORM == PLATFORM_WINDOWS
	// Get the current process core mask
	DWORD_PTR procMask;
	DWORD_PTR sysMask;
	GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

	// If procMask is 0, consider there is only one core available
	// (using 0 as procMask will cause an infinite loop below)
	if (procMask == 0)
		procMask = 1;

	// Find the lowest core that this process uses
	if( mTimerMask == 0 )
	{
		mTimerMask = 1;
		while( ( mTimerMask & procMask ) == 0 )
		{
			mTimerMask <<= 1;
		}
	}

	HANDLE thread = GetCurrentThread();

	// Set affinity to the first core
	DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

	// Get the constant frequency
	QueryPerformanceFrequency(&mFrequency);

	// Query the timer
	QueryPerformanceCounter(&mStartTime);
	mStartTick = GetTickCount();

	// Reset affinity
	SetThreadAffinityMask(thread, oldMask);

	mLastTime = 0;
#endif
}

unsigned long EngineManager::getMilliseconds()
{
#if ENGINE_PLATFORM == PLATFORM_WINDOWS
	LARGE_INTEGER curTime;

	HANDLE thread = GetCurrentThread();

	// Set affinity to the first core
	DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

	// Query the timer
	QueryPerformanceCounter(&curTime);

	// Reset affinity
	SetThreadAffinityMask(thread, oldMask);

    LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;
    
    // scale by 1000 for milliseconds
    unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);

    // detect and compensate for performance counter leaps
    // (surprisingly common, see Microsoft KB: Q274323)
    unsigned long check = GetTickCount() - mStartTick;
    signed long msecOff = (signed long)(newTicks - check);
    if (msecOff < -100 || msecOff > 100)
    {
        // We must keep the timer running forward :)
        LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
        mStartTime.QuadPart += adjust;
        newTime -= adjust;

        // Re-calculate milliseconds
        newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
    }

    // Record last time for adjust
    mLastTime = newTime;

    return newTicks;
#endif
}

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
	unsigned long now = getMilliseconds();

	mEngineEvent->timeSinceLastUpdate = 0.0f;
	if (now != 0 && mLastUpdateStartTime != 0)
	{
		unsigned long diff = now - mLastUpdateStartTime;
		mEngineEvent->timeSinceLastUpdate = (float)(diff) / 1000.0f;
	}
	mLastUpdateStartTime = now;

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
	unsigned long now = getMilliseconds();

	mEngineEvent->timeSinceLastUpdate = 0.0f;
	if (now != 0 && mLastUpdateEndTime != 0)
	{
		unsigned long diff = now - mLastUpdateEndTime;
		mEngineEvent->timeSinceLastUpdate = (float)(diff) / 1000.0f;
	}
	mLastUpdateEndTime = now;

	// Tell all listeners
	std::list<EngineEventReceiver*>::const_iterator i;
	for (i = mEngineEventReceivers.begin(); i != mEngineEventReceivers.end(); ++i)
	{
		(*i)->engineUpdateEnded(*mEngineEvent);
	}
}

void EngineManager::loadOptions()
{
	if (EngineSettings::getInstance() != nullptr)
		EngineSettings::getInstance()->loadOptions(mOptionsFile);
}

void EngineManager::saveOptions()
{
	if (EngineSettings::getInstance() != nullptr)
		EngineSettings::getInstance()->saveOptions(mOptionsFile);
}

EngineManager* EngineManager::getInstance()
{
	return core::Singleton<EngineManager>::getInstance();
}

} // end namespace engine
