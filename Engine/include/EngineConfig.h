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

#ifndef _ENGINE_CONFIG_H_
#define _ENGINE_CONFIG_H_

//////////////////////////////////////////////////////////////////////////
// Init Section
#define PLATFORM_WINDOWS 1
#define PLATFORM_LINUX 2
#define PLATFORM_APPLE 3

#define COMPILER_MSVC 1
#define COMPILER_GNUC 2
#define COMPILER_BORL 3
#define COMPILER_WINSCW 4
#define COMPILER_GCCE 5

#define COMPILER_DEBUG 1
#define COMPILER_RELEASE 2

#define ARCHITECTURE_32 1
#define ARCHITECTURE_64 2

#define ENDIAN_LITTLE 1
#define ENDIAN_BIG 2

// Init Section
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Platform Section
#if defined(__APPLE_CC__)
#	define ENGINE_PLATFORM PLATFORM_APPLE

#elif defined(__WIN32__) || defined(_WIN32)
#	define ENGINE_PLATFORM PLATFORM_WINDOWS

#elif !defined(WIN32) && !defined(_XBOX) && !defined(OS2) && !defined(MACOSX)
#	define ENGINE_PLATFORM PLATFORM_LINUX

#else
#	pragma error "No known OS. Abort! Abort!"

#endif
// Platform Section
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Compiler Section
#if defined( __GCCE__ )
#   define ENGINE_COMPILER COMPILER_GCCE
#   define ENGINE_COMPILER_VERSION _MSC_VER
//#	include <staticlibinit_gcce.h> // This is a GCCE toolchain workaround needed when compiling with GCCE 
#elif defined( __WINSCW__ )
#   define ENGINE_COMPILER COMPILER_WINSCW
#   define ENGINE_COMPILER_VERSION _MSC_VER
#elif defined(_MSC_VER)
#	define ENGINE_COMPILER COMPILER_MSVC
#	define ENGINE_COMPILER_VERSION _MSC_VER

#elif defined(__GNUC__)
#	define ENGINE_COMPILER COMPILER_GNUC
#	define ENGINE_COMPILER_VERSION (((__GNUC__)*100) + (__GNUC_MINOR__*10) + __GNUC_PATCHLEVEL__)

#elif defined(__BORLANDC__)
#	define ENGINE_COMPILER COMPILER_BORL
#	define ENGINE_COMPILER_VERSION __BCPLUSPLUS__
#   define __FUNCTION__ __FUNC__
#else
#	pragma error "No known compiler. Abort! Abort!"

#endif

#if ENGINE_PLATFORM == PLATFORM_WINDOWS
// Win32 compilers use _DEBUG for specifying debug builds.
#	ifdef _DEBUG
#		define ENGINE_COMPILER_MODE COMPILER_DEBUG
#	else
#		define ENGINE_COMPILER_MODE COMPILER_RELEASE
#	endif
#elif ENGINE_PLATFORM == PLATFORM_LINUX || ENGINE_PLATFORM == PLATFORM_APPLE
// Unlike the Win32 compilers, Linux compilers seem to use DEBUG for when
// specifying a debug build.
#	ifdef DEBUG
#		define ENGINE_COMPILER_MODE COMPILER_DEBUG
#	else
#		define ENGINE_COMPILER_MODE COMPILER_RELEASE
#	endif
#endif

// Compiler Section
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Architecture Type Section
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#	define ENGINE_ARCHITECTURE_TYPE ARCHITECTURE_64
#else
#	define ENGINE_ARCHITECTURE_TYPE ARCHITECTURE_32
#endif

#if defined(__sparc__) || defined(__sun__)
#	define ENGINE_ARCHITECTURE_ENDIAN ENDIAN_BIG
#else
#	define ENGINE_ARCHITECTURE_ENDIAN ENDIAN_LITTLE
#endif
// Architecture Type Section
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Unicode Section
#if ENGINE_PLATFORM == PLATFORM_WINDOWS
// Disable unicode support on MingW at the moment, poorly supported in stdlibc++
// STLPORT fixes this though so allow if found
// MinGW C++ Toolkit supports unicode and sets the define __MINGW32_TOOLKIT_UNICODE__ in _mingw.h
#	if defined( __MINGW32__ ) && !defined(_STLPORT_VERSION)
#		include<_mingw.h>
#		if defined(__MINGW32_TOOLBOX_UNICODE__)
#			define ENGINE_UNICODE_SUPPORT 1
#		else
#			define ENGINE_UNICODE_SUPPORT 0
#		endif
#	else
#		define ENGINE_UNICODE_SUPPORT 1
#	endif

#elif ENGINE_PLATFORM == PLATFORM_LINUX || ENGINE_PLATFORM == PLATFORM_APPLE
// Always enable unicode support for the moment
// Perhaps disable in old versions of gcc if necessary
#	define ENGINE_UNICODE_SUPPORT 1

#endif
// Unicode Section
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Export Section
#if ENGINE_PLATFORM == PLATFORM_WINDOWS
// If we're not including this from a client build, specify that the stuff
// should get exported. Otherwise, import it.
#	if defined(MINGW) || defined(__MINGW32__)
// Linux compilers don't have symbol import/export directives.
#		define ENGINE_PUBLIC_EXPORT
#		define ENGINE_TEMPLATE_EXPORT
#		define ENGINE_PRIVATE_EXPORT
#	else
#		ifdef GAME_ENGINE_DLL
#			define ENGINE_PUBLIC_EXPORT			__declspec(dllexport)
#			define ENGINE_TEMPLATE_EXPORT		__declspec(dllexport)
#		else
#			define ENGINE_PUBLIC_EXPORT			__declspec(dllimport)
#			define ENGINE_TEMPLATE_EXPORT
#		endif
#		define ENGINE_PRIVATE_EXPORT
#	endif
#elif ENGINE_PLATFORM == PLATFORM_LINUX || ENGINE_PLATFORM == PLATFORM_APPLE
// Enable GCC 4.0 symbol visibility
#	if ENGINE_COMPILER_VERSION >= 400
#		define ENGINE_PUBLIC_EXPORT				__attribute__ ((visibility("default")))
#		define ENGINE_TEMPLATE_EXPORT			__attribute__ ((visibility("default")))
#		define ENGINE_PRIVATE_EXPORT			__attribute__ ((visibility("hidden")))
#	else
#		define ENGINE_PUBLIC_EXPORT
#		define ENGINE_TEMPLATE_EXPORT
#		define ENGINE_PRIVATE_EXPORT
#	endif
#else
#	define ENGINE_PUBLIC_EXPORT
#	define ENGINE_TEMPLATE_EXPORT
#	define ENGINE_PRIVATE_EXPORT
#endif
// Export Section
//////////////////////////////////////////////////////////////////////////

#if ENGINE_PLATFORM == PLATFORM_WINDOWS
#	if ENGINE_COMPILER == COMPILER_MSVC
#		define ENGINE_CALLBACK				__stdcall
#		define ENGINE_CALLCONV				__cdecl
#	else
#		define ENGINE_CALLBACK
#		define ENGINE_CALLCONV
#	endif
#elif ENGINE_PLATFORM == PLATFORM_LINUX || ENGINE_PLATFORM == PLATFORM_APPLE
#	define ENGINE_CALLBACK
#	define ENGINE_CALLCONV
#else
#	define ENGINE_CALLBACK
#	define ENGINE_CALLCONV
#endif

#if ENGINE_PLATFORM == PLATFORM_WINDOWS
#	if ENGINE_COMPILER == COMPILER_MSVC
#		define	snprintf	_snprintf
#		if !defined(_CRT_SECURE_NO_DEPRECATE)
#			define _CRT_SECURE_NO_DEPRECATE
#		endif
#	endif
#endif

#if (ENGINE_COMPILER == COMPILER_GNUC) && !defined(STLPORT)
#	if ENGINE_COMPILER_VERSION >= 430
#		include <tr1/unordered_map>
#		include <tr1/unordered_set>
#	else
#		include <ext/hash_map>
#		include <ext/hash_set>
#	endif
#else
#	if (ENGINE_COMPILER == COMPILER_MSVC) && !defined(STLPORT) && ENGINE_COMPILER_VERSION >= 1600 // VC++ 10.0
#		include <unordered_map>
#		include <unordered_set>
#	else
#		include <hash_set>
#		include <hash_map>
#	endif
#endif

#if ENGINE_PLATFORM == COMPILER_GNUC && ENGINE_COMPILER_VERSION >= 310 && !defined(STLPORT)
#	if ENGINE_COMPILER_VERSION >= 430
#		define hashmap ::std::tr1::unordered_map
#		define hashset ::std::tr1::unordered_set
#	else
#		define hashmap ::__gnu_cxx::hash_map
#		define hashset ::__gnu_cxx::hash_set
#	endif
#else
#   if ENGINE_COMPILER == COMPILER_MSVC
#       if ENGINE_COMPILER_VERSION >= 1600 // VC++ 10.0
#			define hashmap ::std::tr1::unordered_map
#			define hashset ::std::tr1::unordered_set
#		elif ENGINE_COMPILER_VERSION > 1300 && !defined(_STLP_MSVC)
#			define hashmap ::stdext::hash_map
#			define hashset ::stdext::hash_set
#		else
#			define hashmap ::std::hash_map
#			define hashset ::std::hash_set
#		endif
#	else
#		define hashmap ::std::hash_map
#		define hashset ::std::hash_set
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// Warnings Section
#if ENGINE_COMPILER == COMPILER_MSVC
// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#	pragma warning (disable : 4786)

// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#	pragma warning (disable : 4503)

// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#	pragma warning (disable : 4251)

#endif
// Warnings Section
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Memory Tracking Section
#define ENGINE_MEMORY_TRACKER

#if defined(_DEBUG) && defined(ENGINE_MEMORY_TRACKER)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // _DEBUG

// Memory Tracking Section
//////////////////////////////////////////////////////////////////////////

#include <wchar.h>
#include <assert.h>

#ifdef _DEBUG
#	include <iostream>
#endif

//////////////////////////////////////////////////////////////////////////
// Config Section

//! KG Version
#define KG_VERSION "0.4.0"

#define ENGINE_UNIT_M 1

#define MAX_PATH_SIZE 255

#define SAFE_DELETE(object_ptr) if(object_ptr != nullptr) { delete object_ptr; object_ptr = nullptr; }
#define SAFE_DELETE_ARRAY(object_ptr) if(object_ptr != nullptr) { delete []object_ptr; object_ptr = nullptr; }

// Config Section
//////////////////////////////////////////////////////////////////////////


#endif // _CONFIG_H_