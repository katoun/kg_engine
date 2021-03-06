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

#include <core/SystemDriver.h>
#include <core/Log.h>

namespace core
{

SystemDriver::SystemDriver(const std::string& name)
{
	mName = name;

	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Create");
}

SystemDriver::~SystemDriver()
{
	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Destroy");
}

void SystemDriver::initialize()
{
	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Initialize");

	initializeImpl();

	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Initialized");
}

void SystemDriver::uninitialize()
{
	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Uninitialize");

	uninitializeImpl();

	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Uninitialized");
}

void SystemDriver::start()
{
	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Starting");

	startImpl();

	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Started");
}

void SystemDriver::stop()
{
	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Stoping");

	stopImpl();

	if (Log::getInstance() != nullptr) Log::getInstance()->logMessage(mName, "Stoped");
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
