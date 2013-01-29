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

#ifndef _GL_CONFIG_H_
#define _GL_CONFIG_H_

#include <EngineConfig.h>

#if ENGINE_PLATFORM == PLATFORM_WINDOWS
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

// Export Section
#if ENGINE_PLATFORM == PLATFORM_WINDOWS
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