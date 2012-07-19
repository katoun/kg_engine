/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

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

#include <resource/PixelFormat.h>
#include <core/Utils.h>

namespace resource
{

PixelFormatDescription PixelUtil::pixelFormats[PF_COUNT] =
{
	//-----------------------------------------------------------------------
	{"PF_UNKNOWN",
	/* Bytes per element */
	0,
	/* Flags */
	0,
	/* Component type and count */
	PCT_BYTE, 0,
	/* rbits, gbits, bbits, abits */
	0, 0, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_L8",
	/* Bytes per element */
	1,
	/* Flags */
	PFF_LUMINANCE | PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 1,
	/* rbits, gbits, bbits, abits */
	8, 0, 0, 0,
	/* Masks and shifts */
	0xFF, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_L16",
	/* Bytes per element */
	2,
	/* Flags */
	PFF_LUMINANCE | PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_SHORT, 1,
	/* rbits, gbits, bbits, abits */
	16, 0, 0, 0,
	/* Masks and shifts */
	0xFFFF, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_A8",
	/* Bytes per element */
	1,
	/* Flags */
	PFF_HASALPHA | PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 1,
	/* rbits, gbits, bbits, abits */
	0, 0, 0, 8,
	/* Masks and shifts */
	0, 0, 0, 0xFF, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_A4L4",
	/* Bytes per element */
	1,
	/* Flags */
	PFF_HASALPHA | PFF_LUMINANCE | PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 2,
	/* rbits, gbits, bbits, abits */
	4, 0, 0, 4,
	/* Masks and shifts */
	0x0F, 0, 0, 0xF0, 0, 0, 0, 4
	},
	//-----------------------------------------------------------------------
	{"PF_BYTE_LA",
	/* Bytes per element */
	2,
	/* Flags */
	PFF_HASALPHA | PFF_LUMINANCE,
	/* Component type and count */
	PCT_BYTE, 2,
	/* rbits, gbits, bbits, abits */
	8, 0, 0, 8,
	/* Masks and shifts */
	0,0,0,0,0,0,0,0
	},
	//-----------------------------------------------------------------------
	{"PF_R5G6B5",
	/* Bytes per element */
	2,
	/* Flags */
	PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 3,
	/* rbits, gbits, bbits, abits */
	5, 6, 5, 0,
	/* Masks and shifts */
	0xF800, 0x07E0, 0x001F, 0,
	11, 5, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_B5G6R5",
	/* Bytes per element */
	2,
	/* Flags */
	PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 3,
	/* rbits, gbits, bbits, abits */
	5, 6, 5, 0,
	/* Masks and shifts */
	0x001F, 0x07E0, 0xF800, 0,
	0, 5, 11, 0
	},
	//-----------------------------------------------------------------------
	{"PF_A4R4G4B4",
	/* Bytes per element */
	2,
	/* Flags */
	PFF_HASALPHA | PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	4, 4, 4, 4,
	/* Masks and shifts */
	0x0F00, 0x00F0, 0x000F, 0xF000,
	8, 4, 0, 12
	},
	//-----------------------------------------------------------------------
	{"PF_A1R5G5B5",
	/* Bytes per element */
	2,
	/* Flags */
	PFF_HASALPHA | PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	5, 5, 5, 1,
	/* Masks and shifts */
	0x7C00, 0x03E0, 0x001F, 0x8000,
	10, 5, 0, 15,
	},
	//-----------------------------------------------------------------------
	{"PF_R8G8B8",
	/* Bytes per element */
	3,  // 24 bit integer -- special
	/* Flags */
	PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 3,
	/* rbits, gbits, bbits, abits */
	8, 8, 8, 0,
	/* Masks and shifts */
	0xFF0000, 0x00FF00, 0x0000FF, 0,
	16, 8, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_B8G8R8",
	/* Bytes per element */
	3,  // 24 bit integer -- special
	/* Flags */
	PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 3,
	/* rbits, gbits, bbits, abits */
	8, 8, 8, 0,
	/* Masks and shifts */
	0x0000FF, 0x00FF00, 0xFF0000, 0,
	0, 8, 16, 0
	},
	//-----------------------------------------------------------------------
	{"PF_A8R8G8B8",
	/* Bytes per element */
	4,
	/* Flags */
	PFF_HASALPHA | PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	8, 8, 8, 8,
	/* Masks and shifts */
	0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000,
	16, 8, 0, 24
	},
	//-----------------------------------------------------------------------
	{"PF_A8B8G8R8",
	/* Bytes per element */
	4,
	/* Flags */
	PFF_HASALPHA | PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	8, 8, 8, 8,
	/* Masks and shifts */
	0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000,
	0, 8, 16, 24,
	},
	//-----------------------------------------------------------------------
	{"PF_B8G8R8A8",
	/* Bytes per element */
	4,
	/* Flags */
	PFF_HASALPHA | PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	8, 8, 8, 8,
	/* Masks and shifts */
	0x0000FF00, 0x00FF0000, 0xFF000000, 0x000000FF,
	8, 16, 24, 0
	},
	//-----------------------------------------------------------------------
	{"PF_A2R10G10B10",
	/* Bytes per element */
	4,
	/* Flags */
	PFF_HASALPHA | PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	10, 10, 10, 2,
	/* Masks and shifts */
	0x3FF00000, 0x000FFC00, 0x000003FF, 0xC0000000,
	20, 10, 0, 30
	},
	//-----------------------------------------------------------------------
	{"PF_A2B10G10R10",
	/* Bytes per element */
	4,
	/* Flags */
	PFF_HASALPHA | PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	10, 10, 10, 2,
	/* Masks and shifts */
	0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000,
	0, 10, 20, 30
	},
	//-----------------------------------------------------------------------
	{"PF_DXT1",
	/* Bytes per element */
	0,
	/* Flags */
	PFF_COMPRESSED | PFF_HASALPHA,
	/* Component type and count */
	PCT_BYTE, 3, // No alpha
	/* rbits, gbits, bbits, abits */
	0, 0, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_DXT2",
	/* Bytes per element */
	0,
	/* Flags */
	PFF_COMPRESSED | PFF_HASALPHA,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	0, 0, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_DXT3",
	/* Bytes per element */
	0,
	/* Flags */
	PFF_COMPRESSED | PFF_HASALPHA,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	0, 0, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_DXT4",
	/* Bytes per element */
	0,
	/* Flags */
	PFF_COMPRESSED | PFF_HASALPHA,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	0, 0, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_DXT5",
	/* Bytes per element */
	0,
	/* Flags */
	PFF_COMPRESSED | PFF_HASALPHA,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	0, 0, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_FLOAT16_RGB",
	/* Bytes per element */
	6,
	/* Flags */
	PFF_FLOAT,
	/* Component type and count */
	PCT_FLOAT16, 3,
	/* rbits, gbits, bbits, abits */
	16, 16, 16, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_FLOAT16_RGBA",
	/* Bytes per element */
	8,
	/* Flags */
	PFF_FLOAT | PFF_HASALPHA,
	/* Component type and count */
	PCT_FLOAT16, 4,
	/* rbits, gbits, bbits, abits */
	16, 16, 16, 16,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_FLOAT32_RGB",
	/* Bytes per element */
	12,
	/* Flags */
	PFF_FLOAT,
	/* Component type and count */
	PCT_FLOAT32, 3,
	/* rbits, gbits, bbits, abits */
	32, 32, 32, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_FLOAT32_RGBA",
	/* Bytes per element */
	16,
	/* Flags */
	PFF_FLOAT | PFF_HASALPHA,
	/* Component type and count */
	PCT_FLOAT32, 4,
	/* rbits, gbits, bbits, abits */
	32, 32, 32, 32,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_X8R8G8B8",
	/* Bytes per element */
	4,
	/* Flags */
	PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 3,
	/* rbits, gbits, bbits, abits */
	8, 8, 8, 0,
	/* Masks and shifts */
	0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000,
	16, 8, 0, 24
	},
	//-----------------------------------------------------------------------
	{"PF_X8B8G8R8",
	/* Bytes per element */
	4,
	/* Flags */
	PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 3,
	/* rbits, gbits, bbits, abits */
	8, 8, 8, 0,
	/* Masks and shifts */
	0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000,
	0, 8, 16, 24
	},
	//-----------------------------------------------------------------------
	{"PF_R8G8B8A8",
	/* Bytes per element */
	4,
	/* Flags */
	PFF_HASALPHA | PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	8, 8, 8, 8,
	/* Masks and shifts */
	0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF,
	24, 16, 8, 0
	},
	//-----------------------------------------------------------------------
	{"PF_DEPTH",
	/* Bytes per element */
	4,
	/* Flags */
	PFF_DEPTH,
	/* Component type and count */
	PCT_FLOAT32, 1, // ?
	/* rbits, gbits, bbits, abits */
	0, 0, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_SHORT_RGBA",
	/* Bytes per element */
	8,
	/* Flags */
	PFF_HASALPHA,
	/* Component type and count */
	PCT_SHORT, 4,
	/* rbits, gbits, bbits, abits */
	16, 16, 16, 16,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_R3G3B2",
	/* Bytes per element */
	1,
	/* Flags */
	PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_BYTE, 3,
	/* rbits, gbits, bbits, abits */
	3, 3, 2, 0,
	/* Masks and shifts */
	0xE0, 0x1C, 0x03, 0,
	5, 2, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_FLOAT16_R",
	/* Bytes per element */
	2,
	/* Flags */
	PFF_FLOAT,
	/* Component type and count */
	PCT_FLOAT16, 1,
	/* rbits, gbits, bbits, abits */
	16, 0, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_FLOAT32_R",
	/* Bytes per element */
	4,
	/* Flags */
	PFF_FLOAT,
	/* Component type and count */
	PCT_FLOAT32, 1,
	/* rbits, gbits, bbits, abits */
	32, 0, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_SHORT_GR",
	/* Bytes per element */
	4,
	/* Flags */
	PFF_NATIVEENDIAN,
	/* Component type and count */
	PCT_SHORT, 2,
	/* rbits, gbits, bbits, abits */
	16, 16, 0, 0,
	/* Masks and shifts */
	0x0000FFFF, 0xFFFF0000, 0, 0, 
	0, 16, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_FLOAT16_GR",
	/* Bytes per element */
	4,
	/* Flags */
	PFF_FLOAT,
	/* Component type and count */
	PCT_FLOAT16, 2,
	/* rbits, gbits, bbits, abits */
	16, 16, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_FLOAT32_GR",
	/* Bytes per element */
	8,
	/* Flags */
	PFF_FLOAT,
	/* Component type and count */
	PCT_FLOAT32, 2,
	/* rbits, gbits, bbits, abits */
	32, 32, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_SHORT_RGB",
	/* Bytes per element */
	6,
	/* Flags */
	0,
	/* Component type and count */
	PCT_SHORT, 3,
	/* rbits, gbits, bbits, abits */
	16, 16, 16, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_PVRTC_RGB2",
	/* Bytes per element */
	0,
	/* Flags */
	PFF_COMPRESSED,
	/* Component type and count */
	PCT_BYTE, 3,
	/* rbits, gbits, bbits, abits */
	0, 0, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_PVRTC_RGBA2",
	/* Bytes per element */
	0,
	/* Flags */
	PFF_COMPRESSED | PFF_HASALPHA,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	0, 0, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_PVRTC_RGB4",
	/* Bytes per element */
	0,
	/* Flags */
	PFF_COMPRESSED,
	/* Component type and count */
	PCT_BYTE, 3,
	/* rbits, gbits, bbits, abits */
	0, 0, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
	//-----------------------------------------------------------------------
	{"PF_PVRTC_RGBA4",
	/* Bytes per element */
	0,
	/* Flags */
	PFF_COMPRESSED | PFF_HASALPHA,
	/* Component type and count */
	PCT_BYTE, 4,
	/* rbits, gbits, bbits, abits */
	0, 0, 0, 0,
	/* Masks and shifts */
	0, 0, 0, 0, 0, 0, 0, 0
	},
};

const PixelFormatDescription& PixelUtil::getDescriptionFor(const PixelFormat format)
{
	const signed int ord = (signed int)format;
	assert(ord >= 0 && ord < PF_COUNT);

	if (ord < 0 || ord >= PF_COUNT)
		return pixelFormats[PF_UNKNOWN];

	return pixelFormats[ord];
}

unsigned int PixelUtil::getNumElemBytes(PixelFormat format)
{
	return getDescriptionFor(format).elemBytes;
}

unsigned int PixelUtil::getNumElemBits(PixelFormat format)
{
	return getDescriptionFor(format).elemBytes * 8;
}

void PixelUtil::getBitDepths(PixelFormat format, signed int rgba[4])
{
	const PixelFormatDescription& des = getDescriptionFor(format);
	rgba[0] = des.rbits;
	rgba[1] = des.gbits;
	rgba[2] = des.bbits;
	rgba[3] = des.abits;
}

void PixelUtil::getBitMasks(PixelFormat format, unsigned int rgba[4])
{
	const PixelFormatDescription& des = getDescriptionFor(format);
	rgba[0] = des.rmask;
	rgba[1] = des.gmask;
	rgba[2] = des.bmask;
	rgba[3] = des.amask;
}

unsigned int PixelUtil::getFlags(PixelFormat format)
{
	return getDescriptionFor(format).flags;
}

bool PixelUtil::hasAlpha(PixelFormat format)
{
	return ((getFlags(format) & PFF_HASALPHA) > 0) ? true : false;
}

bool PixelUtil::isFloatingPoint(PixelFormat format)
{
	return ((getFlags(format) & PFF_FLOAT) > 0) ? true : false;
}

bool PixelUtil::isCompressed(PixelFormat format)
{
	return ((getFlags(format) & PFF_COMPRESSED) > 0) ? true : false;
}

bool PixelUtil::isDepth(PixelFormat format)
{
	return ((getFlags(format) & PFF_DEPTH) > 0) ? true : false;
}

bool PixelUtil::isNativeEndian(PixelFormat format)
{
	return ((getFlags(format) & PFF_NATIVEENDIAN) > 0) ? true : false;
}

bool PixelUtil::isLuminance(PixelFormat format)
{
	return ((getFlags(format) & PFF_LUMINANCE) > 0) ? true : false;
}

void PixelUtil::unpackColor(float *r, float *g, float *b, float *a, PixelFormat pf,  const void* src)
{
	const PixelFormatDescription& des = getDescriptionFor(pf);
	if(des.flags & PFF_NATIVEENDIAN) 
	{
		// Shortcut for integer formats unpacking
		unsigned int value;
		switch(des.elemBytes)
		{
		case 1:
			value = ((unsigned char*)src)[0];
		case 2:
			value = ((unsigned short int*)src)[0];
		case 3:
			value = ((unsigned int)((unsigned char*)src)[0]) | ((unsigned int)((unsigned char*)src)[1]<<8) | ((unsigned int)((unsigned char*)src)[2]<<16);
		case 4:
			value = ((unsigned int*)src)[0];
		default:
			value = 0;
		}

		if(des.flags & PFF_LUMINANCE)
		{
			// Luminance format -- only rbits used
			*r = *g = *b = core::fixedToFloat(
				(value & des.rmask)>>des.rshift, des.rbits);
		}
		else
		{
			*r = core::fixedToFloat((value & des.rmask)>>des.rshift, des.rbits);
			*g = core::fixedToFloat((value & des.gmask)>>des.gshift, des.gbits);
			*b = core::fixedToFloat((value & des.bmask)>>des.bshift, des.bbits);
		}
		if(des.flags & PFF_HASALPHA)
		{
			*a = core::fixedToFloat((value & des.amask)>>des.ashift, des.abits);
		}
		else
		{
			*a = 1.0f; // No alpha, default a component to full
		}
	}
	else
	{
		switch(pf)
		{
		case PF_FLOAT32_R:
			*r = *g = *b = ((float*)src)[0];
			*a = 1.0f;
			break;
		case PF_FLOAT32_GR:
			*g = ((float*)src)[0];
			*r = *b = ((float*)src)[1];
			*a = 1.0f;
			break;
		case PF_FLOAT32_RGB:
			*r = ((float*)src)[0];
			*g = ((float*)src)[1];
			*b = ((float*)src)[2];
			*a = 1.0f;
			break;
		case PF_FLOAT32_RGBA:
			*r = ((float*)src)[0];
			*g = ((float*)src)[1];
			*b = ((float*)src)[2];
			*a = ((float*)src)[3];
			break;
		case PF_FLOAT16_R:
			*r = *g = *b = core::halfToFloat(((unsigned short int*)src)[0]);
			*a = 1.0f;
			break;
		case PF_FLOAT16_GR:
			*g = core::halfToFloat(((unsigned short int*)src)[0]);
			*r = *b = core::halfToFloat(((unsigned short int*)src)[1]);
			*a = 1.0f;
			break;
		case PF_FLOAT16_RGB:
			*r = core::halfToFloat(((unsigned short int*)src)[0]);
			*g = core::halfToFloat(((unsigned short int*)src)[1]);
			*b = core::halfToFloat(((unsigned short int*)src)[2]);
			*a = 1.0f;
			break;
		case PF_FLOAT16_RGBA:
			*r = core::halfToFloat(((unsigned short int*)src)[0]);
			*g = core::halfToFloat(((unsigned short int*)src)[1]);
			*b = core::halfToFloat(((unsigned short int*)src)[2]);
			*a = core::halfToFloat(((unsigned short int*)src)[3]);
			break;
		case PF_SHORT_RGB:
			*r = core::fixedToFloat(((unsigned short int*)src)[0], 16);
			*g = core::fixedToFloat(((unsigned short int*)src)[1], 16);
			*b = core::fixedToFloat(((unsigned short int*)src)[2], 16);
			*a = 1.0f;
			break;
		case PF_SHORT_RGBA:
			*r = core::fixedToFloat(((unsigned short int*)src)[0], 16);
			*g = core::fixedToFloat(((unsigned short int*)src)[1], 16);
			*b = core::fixedToFloat(((unsigned short int*)src)[2], 16);
			*a = core::fixedToFloat(((unsigned short int*)src)[3], 16);
			break;
		case PF_BYTE_LA:
			*r = *g = *b = core::fixedToFloat(((unsigned char*)src)[0], 8);
			*a = core::fixedToFloat(((unsigned char*)src)[1], 8);
			break;
		default:
			break;
		}
	}
}

void PixelUtil::packColor(const float r, const float g, const float b, const float a, const PixelFormat pf,  void* dest)
{
	// Catch-it-all here
	const PixelFormatDescription& des = getDescriptionFor(pf);
	if(des.flags & PFF_NATIVEENDIAN)
	{
		// Do the packing
		//std::cerr << dest << " " << r << " " << g <<  " " << b << " " << a << std::endl;
		const unsigned int value = ((core::floatToFixed(r, des.rbits)<<des.rshift) & des.rmask) |
			((core::floatToFixed(g, des.gbits)<<des.gshift) & des.gmask) |
			((core::floatToFixed(b, des.bbits)<<des.bshift) & des.bmask) |
			((core::floatToFixed(a, des.abits)<<des.ashift) & des.amask);
		// And write to memory
		switch(des.elemBytes) 
		{
			case 1:
				((unsigned char*)dest)[0] = (unsigned char)value;
				break;
			case 2:
				((unsigned short int*)dest)[0] = (unsigned short int)value;
				break;
			case 3:
				((unsigned char*)dest)[2] = (unsigned char)((value >> 16) & 0xFF);
				((unsigned char*)dest)[1] = (unsigned char)((value >> 8) & 0xFF);
				((unsigned char*)dest)[0] = (unsigned char)(value & 0xFF);
				break;
			case 4:
				((unsigned int*)dest)[0] = (unsigned int)value;                
				break;                
		}
	}
	else
	{
		switch(pf)
		{
		case PF_FLOAT32_R:
			((float*)dest)[0] = r;
			break;
		case PF_FLOAT32_GR:
			((float*)dest)[0] = g;
			((float*)dest)[1] = r;
			break;
		case PF_FLOAT32_RGB:
			((float*)dest)[0] = r;
			((float*)dest)[1] = g;
			((float*)dest)[2] = b;
			break;
		case PF_FLOAT32_RGBA:
			((float*)dest)[0] = r;
			((float*)dest)[1] = g;
			((float*)dest)[2] = b;
			((float*)dest)[3] = a;
			break;
		case PF_FLOAT16_R:
			((unsigned short int*)dest)[0] = core::floatToHalf(r);
			break;
		case PF_FLOAT16_GR:
			((unsigned short int*)dest)[0] = core::floatToHalf(g);
			((unsigned short int*)dest)[1] = core::floatToHalf(r);
			break;
		case PF_FLOAT16_RGB:
			((unsigned short int*)dest)[0] = core::floatToHalf(r);
			((unsigned short int*)dest)[1] = core::floatToHalf(g);
			((unsigned short int*)dest)[2] = core::floatToHalf(b);
			break;
		case PF_FLOAT16_RGBA:
			((unsigned short int*)dest)[0] = core::floatToHalf(r);
			((unsigned short int*)dest)[1] = core::floatToHalf(g);
			((unsigned short int*)dest)[2] = core::floatToHalf(b);
			((unsigned short int*)dest)[3] = core::floatToHalf(a);
			break;
		case PF_SHORT_RGB:
			((unsigned short int*)dest)[0] = core::floatToFixed(r, 16);
			((unsigned short int*)dest)[1] = core::floatToFixed(g, 16);
			((unsigned short int*)dest)[2] = core::floatToFixed(b, 16);
			break;
		case PF_SHORT_RGBA:
			((unsigned short int*)dest)[0] = core::floatToFixed(r, 16);
			((unsigned short int*)dest)[1] = core::floatToFixed(g, 16);
			((unsigned short int*)dest)[2] = core::floatToFixed(b, 16);
			((unsigned short int*)dest)[3] = core::floatToFixed(a, 16);
			break;
		case PF_BYTE_LA:
			((unsigned char*)dest)[0] = core::floatToFixed(r, 8);
			((unsigned char*)dest)[1] = core::floatToFixed(a, 8);
			break;
		default:
			break;
		}
	}
}

unsigned int PixelUtil::getMemorySize(unsigned int width, unsigned int height, unsigned int depth, PixelFormat format)
{
	if(isCompressed(format))
	{
		switch(format)
		{
			// DXT formats work by dividing the image into 4x4 blocks, then encoding each
			// 4x4 block with a certain number of bytes. DXT can only be used on 2D images.
		case PF_DXT1:
			assert(depth == 1);
			return ((width+3)/4)*((height+3)/4)*8;
		case PF_DXT2:
		case PF_DXT3:
		case PF_DXT4:
		case PF_DXT5:
			assert(depth == 1);
			return ((width+3)/4)*((height+3)/4)*16;
		}
		return ((width+3)/4)*((height+3)/4)*16;
	}
	else
	{
		return width*height*depth*getNumElemBytes(format);
	}
}

unsigned int PixelUtil::calculateSize(unsigned int mipmaps, unsigned int faces, unsigned int width, unsigned int height, unsigned int depth, PixelFormat format)
{
	unsigned int size = 0;
	for(unsigned int mip=0; mip<=mipmaps; ++mip)
	{
		size += getMemorySize(width, height, depth, format)*faces; 
		if(width!=1)	width /= 2;
		if(height!=1)	height /= 2;
		if(depth!=1)	depth /= 2;
	}
	return size;
}

}// end namespace resource
