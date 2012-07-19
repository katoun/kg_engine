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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <core/Config.h>

// Export Section
#if ENGINE_PLATFORM == PLATFORM_WIN32
// If we're not including this from a client build, specify that the stuff
// should get exported. Otherwise, import it.
#	if defined(MINGW) || defined(__MINGW32__)
// Linux compilers don't have symbol import/export directives.
#		define RESOURCE_PUBLIC_EXPORT
#		define RESOURCE_TEMPLATE_EXPORT
#		define RESOURCE_PRIVATE_EXPORT
#	else
#		ifdef RESOURCELOADERS_DLL
#			define RESOURCE_PUBLIC_EXPORT			__declspec(dllexport)
#			define RESOURCE_TEMPLATE_EXPORT			__declspec(dllexport)
#		else
#			define RESOURCE_PUBLIC_EXPORT			__declspec(dllimport)
#			define RESOURCE_TEMPLATE_EXPORT
#		endif
#		define RESOURCE_PRIVATE_EXPORT
#	endif
#elif ENGINE_PLATFORM == PLATFORM_LINUX || ENGINE_PLATFORM == PLATFORM_APPLE
// Enable GCC 4.0 symbol visibility
#	if ENGINE_COMPILER_VERSION >= 400
#		define RESOURCE_PUBLIC_EXPORT			__attribute__ ((visibility("default")))
#		define RESOURCE_TEMPLATE_EXPORT			__attribute__ ((visibility("default")))
#		define RESOURCE_PRIVATE_EXPORT			__attribute__ ((visibility("hidden")))
#	else
#		define RESOURCE_PUBLIC_EXPORT
#		define RESOURCE_TEMPLATE_EXPORT
#		define RESOURCE_PRIVATE_EXPORT
#	endif
#else
#	define RESOURCE_PUBLIC_EXPORT
#	define RESOURCE_TEMPLATE_EXPORT
#	define RESOURCE_PRIVATE_EXPORT
#endif
// Export Section

#endif