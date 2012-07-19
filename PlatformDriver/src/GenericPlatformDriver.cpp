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

#include <platform/PlatformDefines.h>
#include <platform/PlatformManager.h>
#include <GenericPlatformDriver.h>
#include <core/Utils.h>

namespace platform
{

GenericPlatformDriver::GenericPlatformDriver(): PlatformDriver("Generic PlatformDriver")
{
	mCPUInfo = NULL;
	mCPUVendor = core::STRING_BLANK;
	mCPUName = core::STRING_BLANK;
	mCPUType = core::STRING_BLANK;
	mCPUBrand = core::STRING_BLANK;

	mProcessorCount = getCPUCount();
	mCPUInfos = new CPUInfo[mProcessorCount];
	mProcessorCount = getMultipleCPUInfo(mCPUInfos);
	if (mProcessorCount > 0)
	{
		mCPUInfo = &mCPUInfos[0];

		mCPUVendor = mCPUInfo->getVendorName();
		mCPUName = mCPUInfo->getProcessorName();
		mCPUType = mCPUInfo->getProcessorTypeName();
		mCPUBrand = mCPUInfo->getProcessorBrandName();
	}
}

GenericPlatformDriver::~GenericPlatformDriver() {}

const std::string& GenericPlatformDriver::getCPUVendor()
{
	return mCPUVendor;
}

const std::string& GenericPlatformDriver::getCPUName()
{
	return mCPUName;
}

const std::string& GenericPlatformDriver::getCPUType()
{
	return mCPUType;
}

const std::string& GenericPlatformDriver::getCPUBrand()
{
	return mCPUBrand;
}

int GenericPlatformDriver::getCPUFrequency()
{
	if (mCPUInfo == NULL)
		return 0;

	return mCPUInfo->frequency;
}

bool GenericPlatformDriver::checkCPUFeature(CpuFeature feature)
{
	switch (feature)
	{
	case CPU_FEATURE_FPU:
		{
			if (mCPUInfo->features.fpu)
				return true;
		}
		break;
	case CPU_FEATURE_VME:
		{
			if (mCPUInfo->features.vme)
				return true;
		}
		break;
	case CPU_FEATURE_DE:
		{
			if (mCPUInfo->features.de)
				return true;
		}
		break;
	case CPU_FEATURE_PSE:
		{
			if (mCPUInfo->features.pse)
				return true;
		}
		break;
	case CPU_FEATURE_TSC:
		{
			if (mCPUInfo->features.tsc)
				return true;
		}
		break;
	case CPU_FEATURE_MSR:
		{
			if (mCPUInfo->features.msr)
				return true;
		}
		break;
	case CPU_FEATURE_PAE:
		{
			if (mCPUInfo->features.pae)
				return true;
		}
		break;
	case CPU_FEATURE_MCE:
		{
			if (mCPUInfo->features.mce)
				return true;
		}
		break;
	case CPU_FEATURE_CX8:
		{
			if (mCPUInfo->features.cx8)
				return true;
		}
		break;
	case CPU_FEATURE_APIC:
		{
			if (mCPUInfo->features.apic)
				return true;
		}
		break;
	case CPU_FEATURE_SEP:
		{
			if (mCPUInfo->features.sep)
				return true;
		}
		break;
	case CPU_FEATURE_MTRR:
		{
			if (mCPUInfo->features.mtrr)
				return true;
		}
		break;
	case CPU_FEATURE_PGE:
		{
			if (mCPUInfo->features.pge)
				return true;
		}
		break;
	case CPU_FEATURE_MCA:
		{
			if (mCPUInfo->features.mca)
				return true;
		}
		break;
	case CPU_FEATURE_CMOV:
		{
			if (mCPUInfo->features.cmov)
				return true;
		}
		break;
	case CPU_FEATURE_PAT:
		{
			if (mCPUInfo->features.pat)
				return true;
		}
		break;
	case CPU_FEATURE_PSE36:
		{
			if (mCPUInfo->features.pse36)
				return true;
		}
		break;
	case CPU_FEATURE_SERIAL:
		{
			if (mCPUInfo->features.serial)
				return true;
		}
		break;
	case CPU_FEATURE_CLFSH:
		{
			if (mCPUInfo->features.clfsh)
				return true;
		}
		break;
	case CPU_FEATURE_DS:
		{
			if (mCPUInfo->features.ds)
				return true;
		}
		break;
	case CPU_FEATURE_ACPI:
		{
			if (mCPUInfo->features.acpi)
				return true;
		}
		break;
	case CPU_FEATURE_MMX:
		{
			if (mCPUInfo->features.mmx)
				return true;
		}
		break;
	case CPU_FEATURE_FXSR:
		{
			if (mCPUInfo->features.fxsr)
				return true;
		}
		break;
	case CPU_FEATURE_SSE:
		{
			if (mCPUInfo->features.sse)
				return true;
		}
		break;
	case CPU_FEATURE_SSE2:
		{
			if (mCPUInfo->features.sse2)
				return true;
		}
		break;
	case CPU_FEATURE_SS:
		{
			if (mCPUInfo->features.ss)
				return true;
		}
		break;
	case CPU_FEATURE_HTT:
		{
			if (mCPUInfo->features.htt)
				return true;
		}
		break;
	case CPU_FEATURE_THERMAL:
		{
			if (mCPUInfo->features.thermal)
				return true;
		}
		break;
	case CPU_FEATURE_IA64:
		{
			if (mCPUInfo->features.ia64)
				return true;
		}
		break;
	case CPU_FEATURE_PBE:
		{
			if (mCPUInfo->features.pbe)
				return true;
		}
		break;

	case CPU_FEATURE_SSEFP:
		{
			if (mCPUInfo->features.ssefp)
				return true;
		}
		break;

	// Intel extended features
	case CPU_FEATURE_SSE3:
		{
			if (mCPUInfo->features.sse3)
				return true;
		}
		break;
	case CPU_FEATURE_MONITOR:
		{
			if (mCPUInfo->features.monitor)
				return true;
		}
		break;
	case CPU_FEATURE_DS_CPL:
		{
			if (mCPUInfo->features.ds_cpl)
				return true;
		}
		break;
	case CPU_FEATURE_EST:
		{
			if (mCPUInfo->features.est)
				return true;
		}
		break;
	case CPU_FEATURE_TM2:
		{
			if (mCPUInfo->features.tm2)
				return true;
		}
		break;
	case CPU_FEATURE_CNXT_ID:
		{
			if (mCPUInfo->features.cnxt_id)
				return true;
		}
		break;

	// AMD extended features
	case CPU_FEATURE_3DNOW:
		{
			if (mCPUInfo->features._3dnow)
				return true;
		}
		break;
	case CPU_FEATURE_3DNOWPLUS:
		{
			if (mCPUInfo->features._3dnowPlus)
				return true;
		}
		break;
	case CPU_FEATURE_SSEMMX:
		{
			if (mCPUInfo->features.ssemmx)
				return true;
		}
		break;
	case CPU_FEATURE_MMXPLUS:
		{
			if (mCPUInfo->features.mmxPlus)
				return true;
		}
		break;
	case CPU_FEATURE_SUPPORTSMP:
		{
			if (mCPUInfo->features.supportsMP)
				return true;
		}
		break;
	}

	return false;
}

unsigned short int GenericPlatformDriver::getPhysicalProcessorsNum()
{
	return (unsigned short int)mProcessorCount;
}

unsigned short int GenericPlatformDriver::getLogicalProcessorsNum()
{
	if (mCPUInfo == NULL)
		return 1;

	if (mCPUInfo->features.htt)
	{
		return (unsigned short int)mCPUInfo->features.logicalProcessorsPerPhysical;
	}

	return 1;
}

unsigned int GenericPlatformDriver::getCPUCacheSize(CacheLevel level)
{
	if (mCPUInfo == NULL)
		return 0;

	switch(level)
	{
	case CACHE_LEVEL_1:
		{
			if (mCPUInfo->cache.L1CacheSize != -1)
				return mCPUInfo->cache.L1CacheSize;
		}
		break;
	case CACHE_LEVEL_2:
		{
			if (mCPUInfo->cache.L2CacheSize != -1)
				return mCPUInfo->cache.L2CacheSize;
		}
		break;
	case CACHE_LEVEL_3:
		{
			if (mCPUInfo->cache.L3CacheSize != -1)
				return mCPUInfo->cache.L3CacheSize;
		}
		break;
	}
}

void GenericPlatformDriver::initializeImpl() {}

void GenericPlatformDriver::uninitializeImpl() {}

void GenericPlatformDriver::updateImpl(float elapsedTime) {}

} // end namespace platform
