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

#ifndef _PLATFORM_CONFIG_H_
#define _PLATFORM_CONFIG_H_

#include <core/Config.h>

#include <CPUInfo.h>

// Export Section
#if ENGINE_PLATFORM == PLATFORM_WIN32
// If we're not including this from a client build, specify that the stuff
// should get exported. Otherwise, import it.
#	if defined(MINGW) || defined(__MINGW32__)
// Linux compilers don't have symbol import/export directives.
#		define PLATFORM_PUBLIC_EXPORT
#		define PLATFORM_TEMPLATE_EXPORT
#		define PLATFORM_PRIVATE_EXPORT
#	else
#		ifdef PLATFORMSYSTEM_DLL
#			define PLATFORM_PUBLIC_EXPORT			__declspec(dllexport)
#			define PLATFORM_TEMPLATE_EXPORT			__declspec(dllexport)
#		else
#			define PLATFORM_PUBLIC_EXPORT			__declspec(dllimport)
#			define PLATFORM_TEMPLATE_EXPORT
#		endif
#		define PLATFORM_PRIVATE_EXPORT
#	endif
#elif ENGINE_PLATFORM == PLATFORM_LINUX || ENGINE_PLATFORM == PLATFORM_APPLE
// Enable GCC 4.0 symbol visibility
#	if ENGINE_COMPILER_VERSION >= 400
#		define PLATFORM_PUBLIC_EXPORT			__attribute__ ((visibility("default")))
#		define PLATFORM_TEMPLATE_EXPORT			__attribute__ ((visibility("default")))
#		define PLATFORM_PRIVATE_EXPORT			__attribute__ ((visibility("hidden")))
#	else
#		define PLATFORM_PUBLIC_EXPORT
#		define PLATFORM_TEMPLATE_EXPORT
#		define PLATFORM_PRIVATE_EXPORT
#	endif
#else
#	define PLATFORM_PUBLIC_EXPORT
#	define PLATFORM_TEMPLATE_EXPORT
#	define PLATFORM_PRIVATE_EXPORT
#endif
// Export Section

#endif