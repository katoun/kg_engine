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