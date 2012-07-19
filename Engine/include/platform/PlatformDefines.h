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

#ifndef _PLATFORM_DEFINES_H_
#define _PLATFORM_DEFINES_H_

namespace platform
{

//! Enumeration of supported by CPU features.
enum CpuFeature
{
	CPU_FEATURE_FPU,		//"Floating Point Unit"
	CPU_FEATURE_VME,		//"Virtual-8086 Mode Enhancement"
	CPU_FEATURE_DE,			//"Debugging Extensions"
	CPU_FEATURE_PSE,		//"Page Size Extensions"
	CPU_FEATURE_TSC,		//"Time Stamp Counter"
	CPU_FEATURE_MSR,		//"RDMSR and WRMSR Support"
	CPU_FEATURE_PAE,		//"Physical Address Extensions"
	CPU_FEATURE_MCE,		//"Machine Check Exception"
	CPU_FEATURE_CX8,		//"CMPXCHG8B Instruction"
	CPU_FEATURE_APIC,		//"APIC on Chip"
	CPU_FEATURE_SEP,		//"SYSENTER and SYSEXIT"
	CPU_FEATURE_MTRR,		//"Memory Type Range Registers"
	CPU_FEATURE_PGE,		//"PTE Global Bit"
	CPU_FEATURE_MCA,		//"Machine Check Architecture"
	CPU_FEATURE_CMOV,		//"Conditional Move/Compare Instructions"
	CPU_FEATURE_PAT,		//"Page Attribute Table"
	CPU_FEATURE_PSE36,		//"Page Size Extension"
	CPU_FEATURE_SERIAL,		//"Serial Number Available"
	CPU_FEATURE_CLFSH,		//"CLFLUSH Instruction"
	CPU_FEATURE_DS,			//"Debug Store"
	CPU_FEATURE_ACPI,		//"Thermal Monitor and Clock Control"
	CPU_FEATURE_MMX,		//"MMX Technology"
	CPU_FEATURE_FXSR,		//"FXSAVE/FXRSTOR Instructions"
	CPU_FEATURE_SSE,		//"SSE Extensions"
	CPU_FEATURE_SSE2,		//"SSE2 Extensions"
	CPU_FEATURE_SS,			//"Self Snoop"
	CPU_FEATURE_HTT,		//"Hyper-Threading Technology"
	CPU_FEATURE_THERMAL,	//"Thermal Monitor"
	CPU_FEATURE_IA64,		//"IA64 Instructions"
	CPU_FEATURE_PBE,		//"Pending Break Enable"

	CPU_FEATURE_SSEFP,		//"SSE Floating Point"

	// Intel extended features
	CPU_FEATURE_SSE3,		//"SSE3 Extensions"
	CPU_FEATURE_MONITOR,	//"MONITOR/MWAIT"
	CPU_FEATURE_DS_CPL,		//"CPL Qualified Debug Store"
	CPU_FEATURE_EST,		//"Enhanced Intel SpeedStep Technology"
	CPU_FEATURE_TM2,		//"Thermal Monitor 2"
	CPU_FEATURE_CNXT_ID,	//"L1 Context ID"

	// AMD extended features
	CPU_FEATURE_3DNOW,		//"3DNow! Instructions"
	CPU_FEATURE_3DNOWPLUS,	//"3DNow! Instructions Extensions"
	CPU_FEATURE_SSEMMX,		//"SSE MMX"
	CPU_FEATURE_MMXPLUS,	//"MMX+"
	CPU_FEATURE_SUPPORTSMP,	//"Supports Multiprocessing"
};

enum CacheLevel
{
	CACHE_LEVEL_1,
	CACHE_LEVEL_2,
	CACHE_LEVEL_3
};

} // end namespace platform

#endif