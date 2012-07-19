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

#ifndef _GL_CONFIG_H_
#define _GL_CONFIG_H_

#include <core/Config.h>

#if ENGINE_PLATFORM == PLATFORM_WIN32
#	include <windows.h>			// Header File For Windows
#	include <gl\glew.h>			// Header File For The Glew Library
#	include <gl\gl.h>			// Header File For The OpenGL32 Library
#	include <gl\glu.h>			// Header File For The GLu32 Library
#elif ENGINE_PLATFORM == PLATFORM_LINUX
#	include <gl\glew.h>
#	include <GL/gl.h>
#	include <GL/glu.h>
#elif ENGINE_PLATFORM == PLATFORM_APPLE
#	include <gl\glew.h>
#	include <OpenGL/gl.h>
#	define GL_EXT_texture_env_combine 1
#	include <OpenGL/glu.h>
#	include <OpenGL/glext.h>
#endif

#include <Cg/cg.h>
#include <Cg/cgGL.h>

// Export Section
#if ENGINE_PLATFORM == PLATFORM_WIN32
// If we're not including this from a client build, specify that the stuff
// should get exported. Otherwise, import it.
#	if defined(MINGW) || defined(__MINGW32__)
// Linux compilers don't have symbol import/export directives.
#		define GL_PUBLIC_EXPORT
#		define GL_TEMPLATE_EXPORT
#		define GL_PRIVATE_EXPORT
#	else
#		ifdef RENDERSYSTEM_GL_DLL
#			define GL_PUBLIC_EXPORT			__declspec(dllexport)
#			define GL_TEMPLATE_EXPORT			__declspec(dllexport)
#		else
#			define GL_PUBLIC_EXPORT			__declspec(dllimport)
#			define GL_TEMPLATE_EXPORT
#		endif
#		define GL_PRIVATE_EXPORT
#	endif
#elif ENGINE_PLATFORM == PLATFORM_LINUX || ENGINE_PLATFORM == PLATFORM_APPLE
// Enable GCC 4.0 symbol visibility
#	if ENGINE_COMPILER_VERSION >= 400
#		define GL_PUBLIC_EXPORT			__attribute__ ((visibility("default")))
#		define GL_TEMPLATE_EXPORT			__attribute__ ((visibility("default")))
#		define GL_PRIVATE_EXPORT			__attribute__ ((visibility("hidden")))
#	else
#		define GL_PUBLIC_EXPORT
#		define GL_TEMPLATE_EXPORT
#		define GL_PRIVATE_EXPORT
#	endif
#else
#	define GL_PUBLIC_EXPORT
#	define GL_TEMPLATE_EXPORT
#	define GL_PRIVATE_EXPORT
#endif
// Export Section

#endif