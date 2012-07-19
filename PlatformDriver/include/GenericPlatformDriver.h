/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

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