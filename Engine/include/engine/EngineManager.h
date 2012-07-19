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

#ifndef _ENGINE_MANAGER_H_
#define _ENGINE_MANAGER_H_

#include <core/Config.h>
#include <core/Singleton.h>
#include <core/System.h>

#include <Poco/Timestamp.h>

#include <string>
#include <list>
#include <map>

namespace platform
{
class PlatformManager;
}

namespace input
{
class InputManager;
}

namespace resource
{
class ResourceManager;
}

namespace scene
{
class SceneManager;
}

namespace render
{
class RenderManager;
}

namespace sound
{
class SoundManager;
}

namespace physics
{
class PhysicsManager;
}

namespace engine
{

class PluginManager;
class EngineSettings;
class EngineEventReceiver;
struct EngineEvent;

class ENGINE_PUBLIC_EXPORT EngineManager: public core::System, public core::Singleton<EngineManager>
{
public:
	//! Constructor
	EngineManager();

	//! Destructor
	~EngineManager();

	void addEngineEventReceiver(EngineEventReceiver* newEventReceiver);
	void removeEngineEventReceiver(EngineEventReceiver* oldEventReceiver);

	const std::string& getVersion() const;
	const std::string& getVersionString() const;

	void setOptionsFile(const std::string& optionsFile);

	static EngineManager& getInstance();
	static EngineManager* getInstancePtr();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void startImpl();
	void stopImpl();
	void updateImpl(float elapsedTime);
	void registerDefaultFactoriesImpl();
	void removeDefaultFactoriesImpl();

	std::string mVersion;
	std::string mVersionString;

	std::string mOptionsFile;

	static EngineEvent* mEngineEvent;
	static std::list<EngineEventReceiver*> mEngineEventReceivers;

	Poco::Timestamp mLastUpdateStartTimeStamp;
	Poco::Timestamp mLastUpdateEndTimeStamp;
	Poco::Timestamp mCurrentTimeStamp;

	void fireEngineInitialized();

	void fireEngineUninitialized();

	void fireEngineStarted();

	void fireEngineStopped();

	void fireEngineUpdateStarted();

	void fireEngineUpdateEnded();

	//! Method reads a game configuration file and instantiates all options.
	void loadOptions();
	
	//! Method saves a game configuration file with all the instantiated options.
	void saveOptions();
};

} // end namespace engine

#endif