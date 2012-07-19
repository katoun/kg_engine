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

#include <core/System.h>
#include <render/Texture.h>
#include <resource/PixelFormat.h>

namespace render
{

Texture::Texture(const std::string& name, resource::Serializer* serializer): resource::Resource(name, serializer)
{
	mResourceType = resource::RESOURCE_TYPE_TEXTURE;

	mTextureType = TEX_TYPE_2D;

	mPixelFormat = resource::PF_UNKNOWN;

	mBuffer = NULL;
	mSize = 0;

	mWidth = 0;
	mHeight = 0;
	mDepth = 0;

	mFlags = 0;
	mPixelSize = 0;

	mNumMipmaps = 0;

	mHasAlpha = false;
}

Texture::~Texture() {}

void Texture::setTextureType(const TextureType& type)
{
	mTextureType = type;
}

TextureType Texture::getTextureType()
{
	return mTextureType;
}

void Texture::setBuffer(unsigned char* buffer, unsigned int size)
{
	if (buffer)
	{
		if (mBuffer)
			delete []mBuffer;

		mBuffer = new unsigned char[size];
		mSize = size;

		memcpy(mBuffer, buffer, size);
	}
}

void Texture::setWidth(unsigned int width)
{
	mWidth = width;
}

unsigned int Texture::getWidth()
{
	return mWidth;
}

void Texture::setHeight(unsigned int height)
{
	mHeight = height;
}

unsigned int Texture::getHeight()
{
	return mHeight;
}

void Texture::setDepth(unsigned int depth)
{
	mDepth = depth;
}

unsigned int Texture::getDepth()
{
	return mDepth;
}

void Texture::setFlags(signed int flags)
{
	mFlags = flags;
}

signed int Texture::getFlags()
{
	return mFlags;
}

void Texture::setPixelSize(unsigned char size)
{
	mPixelSize = size;
}

unsigned char Texture::getPixelSize()
{
	return mPixelSize;
}

void Texture::setNumMipMaps(unsigned int num)
{
	mNumMipmaps = num;
}

unsigned int Texture::getNumMipMaps()
{
	return mNumMipmaps;
}

void Texture::setPixelFormat(const resource::PixelFormat& format)
{
	mPixelFormat = format;
}

resource::PixelFormat Texture::getPixelFormat() const
{
	return mPixelFormat;
}

void Texture::hasAlpha(bool alpha)
{
	mHasAlpha = alpha;
}

bool Texture::hasAlpha()
{
	return mHasAlpha;
}

void Texture::unloadImpl()
{
	if (mBuffer)
		delete []mBuffer;

	mPixelFormat = resource::PF_UNKNOWN;

	mBuffer = NULL;
	mSize = 0;

	mWidth = 0;
	mHeight = 0;
	mDepth = 0;

	mFlags = 0;
	mPixelSize = 0;

	mNumMipmaps = 0;

	mHasAlpha = false;
}

} // end namespace render
