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