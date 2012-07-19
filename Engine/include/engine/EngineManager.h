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