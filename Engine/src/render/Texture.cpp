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

#include <core/System.h>
#include <render/Texture.h>
#include <resource/PixelFormat.h>

namespace render
{

Texture::Texture(const std::string& filename, resource::Serializer* serializer): resource::Resource(filename, serializer)
{
	mResourceType = resource::RESOURCE_TYPE_TEXTURE;

	mTextureType = TEX_TYPE_2D;

	mPixelFormat = resource::PF_UNKNOWN;

	mBuffer = nullptr;
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

	mBuffer = nullptr;
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
