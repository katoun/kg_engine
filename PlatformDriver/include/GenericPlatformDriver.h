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

#ifndef _GENERIC_PLATFORM_DRIVER_H_
#define _GENERIC_PLATFORM_DRIVER_H_

#include <PlatformConfig.h>
#include <platform/PlatformDriver.h>
#include <platform/PlatformDefines.h>

#include <string>

namespace platform
{

class FileSystem;

class GenericPlatformDriver : public PlatformDriver
{
public:

	GenericPlatformDriver();
	~GenericPlatformDriver();

	const std::string& getCPUVendor();

	const std::string& getCPUName();

	const std::string& getCPUType();

	const std::string& getCPUBrand();

	int getCPUFrequency();

	bool checkCPUFeature(CpuFeature feature);

	unsigned short int getPhysicalProcessorsNum();

	unsigned short int getLogicalProcessorsNum();

	unsigned int getCPUCacheSize(CacheLevel level);

protected:

	void initializeImpl();
	void uninitializeImpl();
	void updateImpl(float elapsedTime);

	CPUInfo* mCPUInfos;
	CPUInfo* mCPUInfo;
	int mProcessorCount;

	std::string mCPUVendor;
	std::string mCPUName;
	std::string mCPUType;
	std::string mCPUBrand;
};

} // end namespace platform

#endif