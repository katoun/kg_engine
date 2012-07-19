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

#ifndef _PLATFORM_MANAGER_H_
#define _PLATFORM_MANAGER_H_

#include <core/Config.h>
#include <core/System.h>
#include <core/Singleton.h>
#include <core/Utils.h>
#include <platform/PlatformDefines.h>

#include <string>
#include <map>
#include <list>

namespace platform
{

class PlatformDriver;
enum CpuFeature;

//! Class which manages the platform settings the Game runs on.
//! Because the Game is designed to be platform-independent, it
//! dynamically loads a library containing all the platform-specific
//! elements.
//! This class manages that load and provides a simple interface to
//! the platform.
class ENGINE_PUBLIC_EXPORT PlatformManager: public core::System, public core::Singleton<PlatformManager>
{
public:
	
	//! Default constructor.
	PlatformManager();

	~PlatformManager();

	const std::string& getCPUVendor();

	const std::string& getCPUName();

	const std::string& getCPUType();

	const std::string& getCPUBrand();

	//! Returns the Frequency the CPU is operating at.
	int getCPUFrequency();

	//! Returns true if feature is supported by installed CPU.
	bool checkCPUFeature(CpuFeature feature);

	//! Returns the number of physical processors.
	unsigned short int getPhysicalProcessorsNum();

	//! Returns the number of logical processors per physical processors.
	unsigned short int getLogicalProcessorsNum();

	//! Returns cache size in k-bytes.
	unsigned int getCPUCacheSize(CacheLevel level);

	static PlatformManager& getInstance();
	static PlatformManager* getInstancePtr();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void startImpl();
	void stopImpl();
	void updateImpl(float elapsedTime);
	void setSystemDriverImpl(core::SystemDriver* systemDriver);
	void removeSystemDriverImpl();

	PlatformDriver* mPlatformDriver;
};

} // end namespace platform

#endif