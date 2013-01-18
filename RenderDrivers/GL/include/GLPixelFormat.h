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

#ifndef _GL_PIXEL_FORMAT_H_
#define _GL_PIXEL_FORMAT_H_

#include <GLConfig.h>
#include <resource/PixelFormat.h>

namespace resource
{

class GLPixelUtil
{
public:

	//!* Takes the pixel format and returns the appropriate GL one.
	static GLenum getGLOriginFormat(PixelFormat mFormat);
	
	//!	Takes the pixel format and returns the type that must be provided to GL as internal format.
	static GLenum getGLInternalFormat(PixelFormat mFormat, bool hwGamma = false);
	
	//!	Takes the pixel format and returns the type that must be provided to GL as internal format.
	//!If no match exists, returns the closest match.
	static GLenum getClosestGLInternalFormat(PixelFormat mFormat, bool hwGamma = false);
};

}// end namespace resource


#endif// _GL_PIXEL_FORMAT_H_