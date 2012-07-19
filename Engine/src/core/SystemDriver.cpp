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

#include <core/SystemDriver.h>
#include <core/Log.h>

namespace core
{

SystemDriver::SystemDriver(const std::string& name)
{
	mName = name;

	Log::getInstance().logMessage(mName, "Create");
}

SystemDriver::~SystemDriver()
{
	Log::getInstance().logMessage(mName, "Destroy");
}

void SystemDriver::initialize()
{
	Log::getInstance().logMessage(mName, "Initialize");

	initializeImpl();

	Log::getInstance().logMessage(mName, "Initialized");
}

void SystemDriver::uninitialize()
{
	Log::getInstance().logMessage(mName, "Uninitialize");

	uninitializeImpl();

	Log::getInstance().logMessage(mName, "Uninitialized");
}

void SystemDriver::start()
{
	Log::getInstance().logMessage(mName, "Starting");

	startImpl();

	Log::getInstance().logMessage(mName, "Started");
}

void SystemDriver::stop()
{
	Log::getInstance().logMessage(mName, "Stoping");

	stopImpl();

	Log::getInstance().logMessage(mName, "Stoped");
}

void SystemDriver::update(float elapsedTime)
{
	updateImpl(elapsedTime);
}

void SystemDriver::initializeImpl() {}

void SystemDriver::uninitializeImpl() {}

void SystemDriver::startImpl() {}

void SystemDriver::stopImpl() {}

void SystemDriver::updateImpl(float elapsedTime) {}

} // end namespace engine
