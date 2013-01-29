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

#ifndef _PIXEL_FORMAT_H_
#define _PIXEL_FORMAT_H_

#include <EngineConfig.h>

namespace render
{
class Color;
}

namespace resource
{

enum PixelFormat
{
	PF_UNKNOWN = 0,				//! Unknown pixel format.
	PF_L8 = 1,					//! 8-bit pixel format, all bits luminace.
	PF_BYTE_L = PF_L8,
	PF_L16 = 2,					//! 16-bit pixel format, all bits luminace.
	PF_SHORT_L = PF_L16,
	PF_A8 = 3,					//! 8-bit pixel format, all bits alpha.
	PF_BYTE_A = PF_A8,
	PF_A4L4 = 4,				//! 8-bit pixel format, 4 bits alpha, 4 bits luminance.
	PF_BYTE_LA = 5,				//! 2 byte pixel format, 1 byte luminance, 1 byte alpha
	PF_R5G6B5 = 6,				//! 16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.
	PF_B5G6R5 = 7,				//! 16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.
	PF_R3G3B2 = 31,				//! 8-bit pixel format, 2 bits blue, 3 bits green, 3 bits red.
	PF_A4R4G4B4 = 8,			//! 16-bit pixel format, 4 bits for alpha, red, green and blue.
	PF_A1R5G5B5 = 9,			//! 16-bit pixel format, 5 bits for blue, green, red and 1 for alpha.
	PF_R8G8B8 = 10,				//! 24-bit pixel format, 8 bits for red, green and blue.
	PF_B8G8R8 = 11,				//! 24-bit pixel format, 8 bits for blue, green and red.
	PF_A8R8G8B8 = 12,			//! 32-bit pixel format, 8 bits for alpha, red, green and blue.
	PF_A8B8G8R8 = 13,			//! 32-bit pixel format, 8 bits for blue, green, red and alpha.
	PF_B8G8R8A8 = 14,			//! 32-bit pixel format, 8 bits for blue, green, red and alpha.
	PF_R8G8B8A8 = 28,			//! 32-bit pixel format, 8 bits for red, green, blue and alpha.
	PF_X8R8G8B8 = 26,			//! 32-bit pixel format, 8 bits for red, 8 bits for green, 8 bits for blue, like PF_A8R8G8B8, but alpha will get discarded
	PF_X8B8G8R8 = 27,			//! 32-bit pixel format, 8 bits for blue, 8 bits for green, 8 bits for red, like PF_A8B8G8R8, but alpha will get discarded
#if GAME_ARCHITECTURE_ENDIAN == ENDIAN_BIG
	PF_BYTE_RGB = PF_R8G8B8,	//! 3 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue
	PF_BYTE_BGR = PF_B8G8R8,	//! 3 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red
	PF_BYTE_BGRA = PF_B8G8R8A8,	//! 4 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red and one byte for alpha
	PF_BYTE_RGBA = PF_R8G8B8A8,	//! 4 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue, and one byte for alpha
#else
	PF_BYTE_RGB = PF_B8G8R8,	//! 3 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue
	PF_BYTE_BGR = PF_R8G8B8,	//! 3 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red
	PF_BYTE_BGRA = PF_A8R8G8B8,	//! 4 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red and one byte for alpha
	PF_BYTE_RGBA = PF_A8B8G8R8,	//! 4 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue, and one byte for alpha
#endif        
	PF_A2R10G10B10 = 15,		//! 32-bit pixel format, 2 bits for alpha, 10 bits for red, green and blue.
	PF_A2B10G10R10 = 16,		//! 32-bit pixel format, 10 bits for blue, green and red, 2 bits for alpha.
	PF_DXT1 = 17,				//! DDS (DirectDraw Surface) DXT1 format
	PF_DXT2 = 18,				//! DDS (DirectDraw Surface) DXT2 format
	PF_DXT3 = 19,				//! DDS (DirectDraw Surface) DXT3 format
	PF_DXT4 = 20,				//! DDS (DirectDraw Surface) DXT4 format
	PF_DXT5 = 21,				//! DDS (DirectDraw Surface) DXT5 format
	PF_FLOAT16_R = 32,			//! 16-bit pixel format, 16 bits (float) for red
	PF_FLOAT16_RGB = 22,		//! 48-bit pixel format, 16 bits (float) for red, 16 bits (float) for green, 16 bits (float) for blue
	PF_FLOAT16_RGBA = 23,		//! 64-bit pixel format, 16 bits (float) for red, 16 bits (float) for green, 16 bits (float) for blue, 16 bits (float) for alpha
	PF_FLOAT32_R = 33,			//! 16-bit pixel format, 16 bits (float) for red
	PF_FLOAT32_RGB = 24,		//! 96-bit pixel format, 32 bits (float) for red, 32 bits (float) for green, 32 bits (float) for blue
	PF_FLOAT32_RGBA = 25,		//! 128-bit pixel format, 32 bits (float) for red, 32 bits (float) for green, 32 bits (float) for blue, 32 bits (float) for alpha
	PF_FLOAT16_GR = 35,			//! 32-bit, 2-channel s10e5 floating point pixel format, 16-bit green, 16-bit red
	PF_FLOAT32_GR = 36,			//! 64-bit, 2-channel floating point pixel format, 32-bit green, 32-bit red
	PF_DEPTH = 29,				//! Depth texture format
	PF_SHORT_RGBA = 30,			//! 64-bit pixel format, 16 bits for red, green, blue and alpha
	PF_SHORT_GR = 34,			//! 32-bit pixel format, 16-bit green, 16-bit red
	PF_SHORT_RGB = 37,			//! 48-bit pixel format, 16 bits for red, green and blue
	PF_PVRTC_RGB2 = 38,			//! PVRTC (PowerVR) RGB 2 bpp
	PF_PVRTC_RGBA2 = 39,		//! PVRTC (PowerVR) RGBA 2 bpp
	PF_PVRTC_RGB4 = 40,			//! PVRTC (PowerVR) RGB 4 bpp
	PF_PVRTC_RGBA4 = 41,		//! PVRTC (PowerVR) RGBA 4 bpp
	PF_COUNT = 42				//! Number of pixel formats currently defined
};

//! Pixel component format
enum PixelComponentType
{
	PCT_BYTE,		//! Byte per component (8 bit fixed 0.0..1.0).
	PCT_SHORT,		//! Short per component (16 bit fixed 0.0..1.0).
	PCT_FLOAT16,	//! 16 bit float per component.
	PCT_FLOAT32,	//! 32 bit float per component.
	PCT_COUNT		//! Number of pixel types.
};

//! Flags defining some on/off properties of pixel formats
enum PixelFormatFlags
{	
	PFF_HASALPHA		= 0x00000001,	// This format has an alpha channel.	
	PFF_COMPRESSED		= 0x00000002,	// This format is compressed.	
	PFF_FLOAT			= 0x00000004,	// This is a floating point format.	
	PFF_DEPTH			= 0x00000008,	// This is a depth format (for depth textures).	
	PFF_NATIVEENDIAN	= 0x00000010,	// Format is in native endian.	
	PFF_LUMINANCE		= 0x00000020	//! This is an intensity format instead of a RGB one.
};

//! A record that describes a pixel format in detail.
struct PixelFormatDescription
{
	char *name;							//! Name of the format, as in the enum.
	unsigned char elemBytes;						//! Number of bytes one element (color value) takes.
	unsigned int flags;							//! Pixel format flags.
	PixelComponentType componentType;	//! Component type.
	unsigned char componentCount;					//! Component count.
	unsigned char rbits,gbits,bbits,abits;			//! Number of bits for red(or luminance), green, blue, alpha.	
	unsigned int rmask, gmask, bmask, amask;		//! Masks as used by packers/unpackers.
	unsigned char rshift, gshift, bshift, ashift;	//! Shifts as used by packers/unpackers.
};

class ENGINE_PUBLIC_EXPORT PixelUtil
{
public:

	static const PixelFormatDescription& getDescriptionFor(const PixelFormat format);
	//! Returns the size in bytes of an element of the given pixel format.
	static unsigned int getNumElemBytes(PixelFormat format);

	//! Returns the size in bits of an element of the given pixel format.
	static unsigned int getNumElemBits(PixelFormat format);

	//! Gives the number of bits (RGBA) for a format.
	static void getBitDepths(PixelFormat format, int rgba[4]);

	//! Gives the masks for the R, G, B and A component.
	static void getBitMasks(PixelFormat format, unsigned int rgba[4]);

	//! Returns the property flags for this pixel format.
	static unsigned int getFlags(PixelFormat format);
	//! Determine if the format has an alpha component.
	static bool hasAlpha(PixelFormat format);
	//! Determine if the format is floating point.
	static bool isFloatingPoint(PixelFormat format);
	//! Determine if the format is compressed.
	static bool isCompressed(PixelFormat format);
	//! Determine if the format is a depth format.
	static bool isDepth(PixelFormat format);
	//! Determine if the format is in native endian format.
	static bool isNativeEndian(PixelFormat format);
	//! Determine if the format is a luminance format.
	static bool isLuminance(PixelFormat format);

	static void unpackColor(float *r, float *g, float *b, float *a, PixelFormat pf,  const void* src);
	static void packColor(const float r, const float g, const float b, const float a, const PixelFormat pf,  void* dest);

	//! Returns the size in memory of a region with the given extents and pixel
	static unsigned int getMemorySize(unsigned int width, unsigned int height, unsigned int depth, PixelFormat format);

	// Calculate size in bytes from the number of mipmaps, faces and the dimensions
	static unsigned int calculateSize(unsigned int mipmaps, unsigned int faces, unsigned int width, unsigned int height, unsigned int depth, PixelFormat format);

private:

	static PixelFormatDescription pixelFormats[PF_COUNT];
};

}// end namespace resource

#endif// _PIXELFORMAT_H_