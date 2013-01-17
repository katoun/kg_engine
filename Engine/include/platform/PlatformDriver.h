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

	virtual const std::string getCPUVendor() = 0;

	virtual const std::string getCPUName() = 0;

	virtual const std::string getCPUType() = 0;

	virtual const std::string getCPUBrand() = 0;

	virtual int getCPUFrequency() = 0;

	virtual bool checkCPUFeature(CpuFeature feature) = 0;

	virtual unsigned short int getLogicalProcessorsNum() = 0;

	virtual unsigned short int getPhysicalProcessorsNum() = 0;

	virtual unsigned int getCPUCacheSize(CacheLevel level) = 0;
};

} // end namespace platform

#endif