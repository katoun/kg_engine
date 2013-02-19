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

#define GL_PARAMETER_NAME_BUFFERSIZE 255

#endif