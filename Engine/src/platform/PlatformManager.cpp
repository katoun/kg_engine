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

#include <platform/PlatformDefines.h>
#include <platform/PlatformManager.h>
#include <platform/PlatformDriver.h>
#include <core/Utils.h>

template<> platform::PlatformManager& core::Singleton<platform::PlatformManager>::ms_Singleton = platform::PlatformManager();

namespace platform
{

PlatformManager::PlatformManager(): core::System("PlatformManager")
{
	mPlatformDriver = NULL;
}

PlatformManager::~PlatformManager() {}

const std::string& PlatformManager::getCPUVendor()
{
	if(mPlatformDriver == NULL)
		return core::STRING_BLANK;

	return mPlatformDriver->getCPUVendor();
}

const std::string& PlatformManager::getCPUName()
{
	if(mPlatformDriver == NULL)
		return core::STRING_BLANK;

	return mPlatformDriver->getCPUName();
}

const std::string& PlatformManager::getCPUType()
{
	if(mPlatformDriver == NULL)
		return core::STRING_BLANK;

	return mPlatformDriver->getCPUType();
}

const std::string& PlatformManager::getCPUBrand()
{
	if(mPlatformDriver == NULL)
		return core::STRING_BLANK;

	return mPlatformDriver->getCPUBrand();
}

int PlatformManager::getCPUFrequency()
{
	if(mPlatformDriver == NULL)
		return 0;

	return mPlatformDriver->getCPUFrequency();
}

bool PlatformManager::checkCPUFeature(CpuFeature feature)
{
	if(mPlatformDriver == NULL)
		return false;

	return mPlatformDriver->checkCPUFeature(feature);
}

unsigned short int PlatformManager::getPhysicalProcessorsNum()
{
	if(mPlatformDriver == NULL)
		return 0;

	return mPlatformDriver->getPhysicalProcessorsNum();
}

unsigned short int PlatformManager::getLogicalProcessorsNum()
{
	if(mPlatformDriver == NULL)
		return 0;

	return mPlatformDriver->getLogicalProcessorsNum();
}

unsigned int PlatformManager::getCPUCacheSize(CacheLevel level)
{
	if(mPlatformDriver == NULL)
		return 0;

	return mPlatformDriver->getCPUCacheSize(level);	
}

void PlatformManager::initializeImpl() {}

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
	mPlatformDriver = NULL;
}

PlatformManager& PlatformManager::getInstance()
{
	return core::Singleton<PlatformManager>::getInstance();
}

PlatformManager* PlatformManager::getInstancePtr()
{
	return core::Singleton<PlatformManager>::getInstancePtr();
}

} // end namespace platform
