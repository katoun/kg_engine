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

#include <platform/PlatformDefines.h>
#include <platform/PlatformManager.h>
#include <platform/PlatformDriver.h>
#include <core/Utils.h>

template<> platform::PlatformManager* core::Singleton<platform::PlatformManager>::m_Singleton = nullptr;

namespace platform
{

PlatformManager::PlatformManager(): core::System("PlatformManager")
{
	mPlatformDriver = nullptr;

	mCPUVendor	= "";
	mCPUName	= "";
	mCPUType	= "";
	mCPUBrand	= "";
}

PlatformManager::~PlatformManager() {}

const std::string& PlatformManager::getCPUVendor()
{
	return mCPUVendor;
}

const std::string& PlatformManager::getCPUName()
{
	return mCPUName;
}

const std::string& PlatformManager::getCPUType()
{
	return mCPUType;
}

const std::string& PlatformManager::getCPUBrand()
{
	return mCPUBrand;
}

int PlatformManager::getCPUFrequency()
{
	if(mPlatformDriver == nullptr)
		return 0;

	return mPlatformDriver->getCPUFrequency();
}

bool PlatformManager::checkCPUFeature(CpuFeature feature)
{
	if(mPlatformDriver == nullptr)
		return false;

	return mPlatformDriver->checkCPUFeature(feature);
}

unsigned short int PlatformManager::getPhysicalProcessorsNum()
{
	if(mPlatformDriver == nullptr)
		return 0;

	return mPlatformDriver->getPhysicalProcessorsNum();
}

unsigned short int PlatformManager::getLogicalProcessorsNum()
{
	if(mPlatformDriver == nullptr)
		return 0;

	return mPlatformDriver->getLogicalProcessorsNum();
}

unsigned int PlatformManager::getCPUCacheSize(CacheLevel level)
{
	if(mPlatformDriver == nullptr)
		return 0;

	return mPlatformDriver->getCPUCacheSize(level);	
}

void PlatformManager::initializeImpl()
{
	if (mPlatformDriver != nullptr)
	{
		mCPUVendor	= mPlatformDriver->getCPUVendor();
		mCPUName	= mPlatformDriver->getCPUName();
		mCPUType	= mPlatformDriver->getCPUType();
		mCPUBrand	= mPlatformDriver->getCPUBrand();
	}
}

void PlatformManager::uninitializeImpl() {}

void PlatformManager::startImpl() {}

void PlatformManager::stopImpl() {}

void PlatformManager::updateImpl(float elapsedTime) {}

void PlatformManager::setSystemDriverImpl(core::SystemDriver* systemDriver)
{
	mPlatformDriver = static_cast<PlatformDriver*>(systemDriver);
}

void PlatformManager::removeSystemDriverImpl()
{
	mPlatformDriver = nullptr;
}

PlatformManager* PlatformManager::getInstance()
{
	return core::Singleton<PlatformManager>::getInstance();
}

} // end namespace platform
