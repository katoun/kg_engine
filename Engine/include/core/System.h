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

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <core/Config.h>
#include <core/SystemDefines.h>
#include <core/LogDefines.h>

namespace Poco
{
class Logger;
}

namespace core
{

class Log;
class SystemDriver;

class ENGINE_PUBLIC_EXPORT System
{
public:

	System(const std::string& name);

	virtual ~System();

	//! Initialize system.
	void initialize();

	//! Uninitialize system.
	void uninitialize();

	//! Start system.
	void start();

	//! Stop system.
	void stop();

	//! Update system.
	void update(float elapsedTime);

	const SystemState& getState() const;

	bool isRunning();

	SystemDriver* getSystemDriver();
	void setSystemDriver(SystemDriver* systemDriver);
	void removeSystemDriver();

	void registerDefaultFactories();
	void removeDefaultFactories();

protected:

	virtual void initializeImpl();
	virtual void uninitializeImpl();
	virtual void startImpl();
	virtual void stopImpl();
	virtual void updateImpl(float elapsedTime);
	virtual void setSystemDriverImpl(SystemDriver* systemDriver);
	virtual void removeSystemDriverImpl();
	virtual void registerDefaultFactoriesImpl();
	virtual void removeDefaultFactoriesImpl();

	std::string mName;

	SystemState mState;

	SystemDriver* mSystemDriver;

	static Log* mLog;
};

} // end namespace core

#endif