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

#include <GLPixelFormat.h>

namespace resource
{

GLenum GLPixelUtil::getGLOriginFormat(PixelFormat mFormat)
{
	switch(mFormat)
	{
	case resource::PF_A8:
		return GL_ALPHA;
	case resource::PF_L8:
		return GL_LUMINANCE;
	case resource::PF_L16:
		return GL_LUMINANCE;
	case resource::PF_BYTE_LA:
		return GL_LUMINANCE_ALPHA;
	case resource::PF_R3G3B2:
		return GL_RGB;
	case resource::PF_A1R5G5B5:
		return GL_BGRA;
	case resource::PF_R5G6B5:
		return GL_RGB;
	case resource::PF_B5G6R5:
		return GL_BGR;
	case resource::PF_A4R4G4B4:
		return GL_BGRA;
#if ENGINE_ARCHITECTURE_ENDIAN == ENDIAN_BIG
		// Formats are in native endian, so R8G8B8 on little endian is
		// BGR, on big endian it is RGB.
	case resource::PF_R8G8B8:
		return GL_RGB;
	case resource::PF_B8G8R8:
		return GL_BGR;
#else
	case resource::PF_R8G8B8:
		return GL_BGR;
	case resource::PF_B8G8R8:
		return GL_RGB;
#endif
	case resource::PF_X8R8G8B8:
	case resource::PF_A8R8G8B8:
		return GL_BGRA;
	case resource::PF_X8B8G8R8:
	case resource::PF_A8B8G8R8:
		return GL_RGBA;
	case resource::PF_B8G8R8A8:
		return GL_BGRA;
	case resource::PF_R8G8B8A8:
		return GL_RGBA;
	case resource::PF_A2R10G10B10:
		return GL_BGRA;
	case resource::PF_A2B10G10R10:
		return GL_RGBA;
	case resource::PF_FLOAT16_R:
		return GL_LUMINANCE;
	case resource::PF_FLOAT16_GR:
		return GL_LUMINANCE_ALPHA;
	case resource::PF_FLOAT16_RGB:
		return GL_RGB;
	case resource::PF_FLOAT16_RGBA:
		return GL_RGBA;
	case resource::PF_FLOAT32_R:
		return GL_LUMINANCE;
	case resource::PF_FLOAT32_GR:
		return GL_LUMINANCE_ALPHA;
	case resource::PF_FLOAT32_RGB:
		return GL_RGB;
	case resource::PF_FLOAT32_RGBA:
		return GL_RGBA;
	case resource::PF_SHORT_RGBA:
		return GL_RGBA;
	case resource::PF_SHORT_RGB:
		return GL_RGB;
	case resource::PF_SHORT_GR:
		return GL_LUMINANCE_ALPHA;
	case resource::PF_DXT1:
		return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	case resource::PF_DXT3:
		return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
	case resource::PF_DXT5:
		return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
	default:
		return GL_NONE;
	}
}

GLenum GLPixelUtil::getGLInternalFormat(PixelFormat mFormat, bool hwGamma)
{
	switch(mFormat)
	{
	case resource::PF_L8:
		return GL_LUMINANCE8;
	case resource::PF_L16:
		return GL_LUMINANCE16;
	case resource::PF_A8:
		return GL_ALPHA8;
	case resource::PF_A4L4:
		return GL_LUMINANCE4_ALPHA4;
	case resource::PF_BYTE_LA:
		return GL_LUMINANCE8_ALPHA8;
	case resource::PF_R3G3B2:
		return GL_R3_G3_B2;
	case resource::PF_A1R5G5B5:
		return GL_RGB5_A1;
	case resource::PF_R5G6B5:
	case resource::PF_B5G6R5:
		return GL_RGB5;
	case resource::PF_A4R4G4B4:
		return GL_RGBA4;
	case resource::PF_R8G8B8:
	case resource::PF_B8G8R8:
	case resource::PF_X8B8G8R8:
	case resource::PF_X8R8G8B8:
		if (hwGamma)
			return GL_SRGB8;
		else
			return GL_RGB8;
	case resource::PF_A8R8G8B8:
	case resource::PF_B8G8R8A8:
		if (hwGamma)
			return GL_SRGB8_ALPHA8;
		else
			return GL_RGBA8;
	case resource::PF_A2R10G10B10:
	case resource::PF_A2B10G10R10:
		return GL_RGB10_A2;
	case resource::PF_FLOAT16_R:
		return GL_LUMINANCE16F_ARB;
	case resource::PF_FLOAT16_RGB:
		return GL_RGB16F_ARB;
	case resource::PF_FLOAT16_GR: 
		return GL_LUMINANCE_ALPHA16F_ARB;
	case resource::PF_FLOAT16_RGBA:
		return GL_RGBA16F_ARB;
	case resource::PF_FLOAT32_R:
		return GL_LUMINANCE32F_ARB;
	case resource::PF_FLOAT32_GR:
		return GL_LUMINANCE_ALPHA32F_ARB;
	case resource::PF_FLOAT32_RGB:
		return GL_RGB32F_ARB;
	case resource::PF_FLOAT32_RGBA:
		return GL_RGBA32F_ARB;
	case resource::PF_SHORT_RGBA:
		return GL_RGBA16;
	case resource::PF_SHORT_RGB:
		return GL_RGB16;
	case resource::PF_SHORT_GR:
		return GL_LUMINANCE16_ALPHA16;
	case resource::PF_DXT1:
		if (hwGamma)
			return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
		else
			return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	case resource::PF_DXT3:
		if (hwGamma)
			return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
		else
			return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
	case resource::PF_DXT5:
		if (hwGamma)
			return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
		else
			return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
	default:
		return GL_NONE;
	}
}

GLenum GLPixelUtil::getClosestGLInternalFormat(PixelFormat mFormat, bool hwGamma)
{
	GLenum format = getGLInternalFormat(mFormat, hwGamma);
	if(format == GL_NONE)
	{
		if (hwGamma)
			return GL_SRGB8;
		else
			return GL_RGBA8;
	}
	else
		return format;
}

}// end namespace resource
