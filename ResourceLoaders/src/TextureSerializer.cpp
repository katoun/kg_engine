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

#include <core/Log.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <resource/ResourceManager.h>
#include <resource/PixelFormat.h>
#include <render/Texture.h>
#include <render/Color.h>
#include <TextureSerializer.h>

#include <FreeImage.h>

namespace resource
{

/**
FreeImage error handler
@param fif Format / Plugin responsible for the error 
@param message Error message
*/
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message)
{
	printf("\n*** "); 
	if(fif != FIF_UNKNOWN)
	{
		printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
	}
	printf(message);
	printf(" ***\n");
}

TextureSerializer::TextureSerializer()
{
	// Version number
	mVersion = "[TextureSerializer_v1.00]";
	FreeImage_Initialise(false);

	// initialize your own FreeImage error handler
	FreeImage_SetOutputMessage(FreeImageErrorHandler);
}

TextureSerializer::~TextureSerializer()
{
	FreeImage_DeInitialise();
}

bool TextureSerializer::importResource(Resource* dest, const std::string& filename)
{
	assert(dest != nullptr);
	if (dest == nullptr)
		return false;

	if (dest->getResourceType() != RESOURCE_TYPE_TEXTURE)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("TextureSerializer", "Unable to load texture - invalid resource pointer.", core::LOG_LEVEL_ERROR);
		return false;
	}

	if (resource::ResourceManager::getInstance() == nullptr)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("TextureSerializer", "Unable to load texture - resources data path not set.", core::LOG_LEVEL_ERROR);
		return false;
	}

	std::string filePath = resource::ResourceManager::getInstance()->getDataPath() + "/" + filename;

	FREE_IMAGE_FORMAT fi_format = FreeImage_GetFileType(filePath.c_str());
	FIBITMAP *fi_bitmap = FreeImage_Load(fi_format, filePath.c_str());

	FREE_IMAGE_TYPE fi_type = FreeImage_GetImageType(fi_bitmap);
	FREE_IMAGE_COLOR_TYPE fi_colour_type = FreeImage_GetColorType(fi_bitmap);

	if (!fi_bitmap)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("TextureSerializer", "Error decoding image.", core::LOG_LEVEL_ERROR);
		return false;
	}

	render::Texture* tex = static_cast<render::Texture*>(dest);
	assert(tex != nullptr);
	if (tex == nullptr)
		return false;

	unsigned int width = FreeImage_GetWidth(fi_bitmap);
	unsigned int height = FreeImage_GetHeight(fi_bitmap);
	unsigned int bytes = FreeImage_GetBPP(fi_bitmap);
	unsigned int depth = 1;

	PixelFormat pixelFormat = PF_UNKNOWN;

	switch(fi_type)
	{
	case FIT_BITMAP:
		// Standard image type
		// Perform any colour conversions for greyscale
		if (fi_colour_type == FIC_MINISWHITE || fi_colour_type == FIC_MINISBLACK)
		{
			FIBITMAP* newBitmap = FreeImage_ConvertToGreyscale(fi_bitmap);
			// free old bitmap and replace
			FreeImage_Unload(fi_bitmap);
			fi_bitmap = newBitmap;
			// get new formats
			bytes = FreeImage_GetBPP(fi_bitmap);
			fi_colour_type = FreeImage_GetColorType(fi_bitmap);
		}
		// Perform any colour conversions for RGB
		else if (bytes < 8 || fi_colour_type == FIC_PALETTE || fi_colour_type == FIC_CMYK)
		{
			FIBITMAP* newBitmap = FreeImage_ConvertTo24Bits(fi_bitmap);
			// free old bitmap and replace
			FreeImage_Unload(fi_bitmap);
			fi_bitmap = newBitmap;
			// get new formats
			bytes = FreeImage_GetBPP(fi_bitmap);
			fi_colour_type = FreeImage_GetColorType(fi_bitmap);
		}

		// by this stage, 8-bit is greyscale, 16/24/32 bit are RGB[A]
		switch(bytes)
		{
		case 8:
			pixelFormat = PF_L8;
			break;
		case 16:
			// Determine 555 or 565 from green mask
			// cannot be 16-bit greyscale since that's FIT_UINT16
			if(FreeImage_GetGreenMask(fi_bitmap) == FI16_565_GREEN_MASK)
				pixelFormat = PF_R5G6B5;
			else
				pixelFormat = PF_A1R5G5B5;// FreeImage doesn't support 4444 format so must be 1555
			break;
		case 24:
			// FreeImage differs per platform
			// PF_BYTE_BGR[A] for little endian (== PF_ARGB native)
			// PF_BYTE_RGB[A] for big endian (== PF_RGBA native)
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
			pixelFormat = PF_BYTE_RGB;
#else
			pixelFormat = PF_BYTE_BGR;
#endif
			break;
		case 32:
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
			pixelFormat = PF_BYTE_RGBA;
#else
			pixelFormat = PF_BYTE_BGRA;
#endif
			break;
		}
		break;

	case FIT_UINT16:
	case FIT_INT16:
		// 16-bit greyscale
		pixelFormat = PF_L16;
		break;
	case FIT_FLOAT:
		// Single-component floating point data
		pixelFormat = PF_FLOAT32_R;
		break;
	case FIT_RGB16:
		pixelFormat = PF_SHORT_RGB;
		break;
	case FIT_RGBA16:
		pixelFormat = PF_SHORT_RGBA;
		break;
	case FIT_RGBF:
		pixelFormat = PF_FLOAT32_RGB;
		break;
	case FIT_RGBAF:
		pixelFormat = PF_FLOAT32_RGBA;
		break;
	}

	unsigned char* pSrcData = FreeImage_GetBits(fi_bitmap);
	unsigned int srcPitch = FreeImage_GetPitch(fi_bitmap);

	// Final data - invert image and trim pitch at the same time
	unsigned int dstPitch = width * PixelUtil::getNumElemBytes(pixelFormat);
	unsigned int size = dstPitch * height;

	unsigned char *pBuffer = new unsigned char[size];

	unsigned char* pSrc;
	unsigned char* pDst = pBuffer;
	for (size_t y = 0; y < height; ++y)
	{
		pSrc = pSrcData + (height - y - 1) * srcPitch;
		memcpy(pDst, pSrc, dstPitch);
		pDst += dstPitch;
	}

	FreeImage_Unload(fi_bitmap);

	tex->setBuffer(pBuffer, size);

	if (pBuffer != nullptr)
		delete []pBuffer;

	tex->setWidth(width);
	tex->setHeight(height);
	tex->setDepth(depth);
	tex->setNumMipMaps(0);
	tex->setFlags(0);

	tex->setPixelSize(bytes);
	tex->setPixelFormat(pixelFormat);

	tex->hasAlpha(PixelUtil::hasAlpha(pixelFormat));

	return true;
}

bool TextureSerializer::exportResource(Resource* source, const std::string& filename)
{
	return true;
}

}// end namespace resource
