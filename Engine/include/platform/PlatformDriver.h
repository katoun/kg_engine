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

#ifndef _PLATFORM_DRIVER_H_
#define _PLATFORM_DRIVER_H_

#include <core/Config.h>
#include <core/SystemDriver.h>
#include <platform/PlatformDefines.h>

namespace platform
{

enum CpuTechnology;

//! Defines the functionality of a platform API
//! The PlatformSystem class provides a base interface
//! which abstracts the general functionality of the platform API
//! e.g. Windows or Linux. Most of this class is
//! abstract, requiring a subclass based on a specific API
//! to be constructed to provide the full functionality.
//! Author: Kat'Oun
//! version: 1.0
class ENGINE_PUBLIC_EXPORT PlatformDriver: public core::SystemDriver
{
public:

	//! Default Constructor
	PlatformDriver(const std::string& name);

	//! Destructor
	virtual ~PlatformDriver();

	virtual const std::string& getCPUVendor() = 0;

	virtual const std::string& getCPUName() = 0;

	virtual const std::string& getCPUType() = 0;

	virtual const std::string& getCPUBrand() = 0;

	virtual int getCPUFrequency() = 0;

	virtual bool checkCPUFeature(CpuFeature feature) = 0;

	virtual unsigned short int getLogicalProcessorsNum() = 0;

	virtual unsigned short int getPhysicalProcessorsNum() = 0;

	virtual unsigned int getCPUCacheSize(CacheLevel level) = 0;
};

} // end namespace platform

#endif