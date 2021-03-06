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

#include <core/System.h>
#include <core/SystemDriver.h>
#include <core/Log.h>

namespace core
{

System::System(const std::string& name)
{
	mName = name;

	mState = SYSTEM_STATE_UNINITIALIZED;

	mSystemDriver = nullptr;

	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Create");
}

System::~System()
{
	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Destroy");
}

void System::initialize()
{
	if (mState == SYSTEM_STATE_INITIALIZED)
		return;

	mState = SYSTEM_STATE_INITIALIZING;

	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Initializing");

	if (mSystemDriver != nullptr)
		mSystemDriver->initialize();

	registerDefaultFactories();
	initializeImpl();

	mState = SYSTEM_STATE_INITIALIZED;

	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Initialized");
}

void System::uninitialize()
{
	if (mState == SYSTEM_STATE_UNINITIALIZED)
		return;

	mState = SYSTEM_STATE_UNINITIALIZING;

	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Uninitializing");

	if (mSystemDriver != nullptr)
		mSystemDriver->uninitialize();

	uninitializeImpl();
	removeDefaultFactories();

	mState = SYSTEM_STATE_UNINITIALIZED;

	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Uninitialized");
}

void System::start()
{
	if (mState == SYSTEM_STATE_STARTED)
		return;

	mState = SYSTEM_STATE_STARTING;

	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Starting");

	if (mSystemDriver != nullptr)
		mSystemDriver->start();

	startImpl();

	mState = SYSTEM_STATE_STARTED;

	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Started");
}

void System::stop()
{
	if (mState == SYSTEM_STATE_STOPED)
		return;

	if (mState != SYSTEM_STATE_UPDATING)
	{
		mState = SYSTEM_STATE_STOPING;

		if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Stoping");

		if (mSystemDriver != nullptr)
			mSystemDriver->stop();

		stopImpl();

		mState = SYSTEM_STATE_STOPED;

		if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Stoped");
	}
	else
	{
		mState = SYSTEM_STATE_STOPING;

		if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Stoping");
	}
}

void System::update(float elapsedTime)
{
	if (mState != SYSTEM_STATE_STARTED && mState != SYSTEM_STATE_UPDATED)
		return;

	mState = SYSTEM_STATE_UPDATING;

	if (mSystemDriver != nullptr)
		mSystemDriver->update(elapsedTime);

	updateImpl(elapsedTime);

	if (mState == SYSTEM_STATE_STOPING)
	{
		if (mSystemDriver != nullptr)
			mSystemDriver->stop();

		stopImpl();

		mState = SYSTEM_STATE_STOPED;

		if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Stoped");
	}
	else
	{
		mState = SYSTEM_STATE_UPDATED;
	}
}

const SystemState& System::getState() const
{
	return mState;
}

bool System::isRunning()
{
	return mState == SYSTEM_STATE_STARTED || mState == SYSTEM_STATE_UPDATING || mState == SYSTEM_STATE_UPDATED;
}

SystemDriver* System::getSystemDriver()
{
	return mSystemDriver;
}

void System::setSystemDriver(SystemDriver* systemDriver)
{
	mSystemDriver = systemDriver;
	setSystemDriverImpl(systemDriver);
}

void System::removeSystemDriver()
{
	mSystemDriver = nullptr;
	removeSystemDriverImpl();
}

void System::registerDefaultFactories()
{
	registerDefaultFactoriesImpl();
}

void System::removeDefaultFactories()
{
	removeDefaultFactoriesImpl();
}

void System::initializeImpl() {}

void System::uninitializeImpl() {}

void System::startImpl() {}

void System::stopImpl() {}

void System::updateImpl(float elapsedTime) {}

void System::setSystemDriverImpl(SystemDriver* systemDriver) {}

void System::removeSystemDriverImpl() {}

void System::registerDefaultFactoriesImpl() {}
void System::removeDefaultFactoriesImpl() {}

} // end namespace engine
