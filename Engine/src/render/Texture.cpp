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
#include <render/GLPixelFormat.h>

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
		SAFE_DELETE_ARRAY(mBuffer);

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

void Texture::setFlags(int flags)
{
	mFlags = flags;
}

int Texture::getFlags()
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

GLuint Texture::getGLID() const
{
	return mTextureID;
}

bool Texture::loadImpl()
{
	if (!Resource::loadImpl())
		return false;

	if (mBuffer == nullptr)
		return false;

	if (mTextureType == TEX_TYPE_2D)
	{
		// Create the GL texture
		glGenTextures(1, &mTextureID);

		glActiveTexture(GL_TEXTURE15);
		// Set texture type
		glBindTexture(getGLTextureType(), mTextureID);

		// This needs to be set otherwise the texture doesn't get rendered
		glTexParameteri(getGLTextureType(), GL_TEXTURE_MAX_LEVEL, mNumMipmaps);

		// Set some misc default parameters so NVidia won't complain, these can of course be changed later
		glTexParameteri(getGLTextureType(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(getGLTextureType(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(getGLTextureType(), GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(getGLTextureType(), GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(getGLTextureType(), GL_GENERATE_MIPMAP, GL_TRUE);

		GLenum format = GLPixelUtil::getGLOriginFormat(mPixelFormat);
		GLenum internalFormat = GLPixelUtil::getClosestGLInternalFormat(mPixelFormat);
		unsigned int width = mWidth;
		unsigned int height = mHeight;
		unsigned int depth = mDepth;

		if(resource::PixelUtil::isCompressed(mPixelFormat))
		{
			// Compressed formats
			for(unsigned int mip=0; mip<=mNumMipmaps; mip++)
			{
				unsigned int size = resource::PixelUtil::getMemorySize(width, height, depth, mPixelFormat);
				switch(mTextureType)
				{
				case TEX_TYPE_1D:
					glCompressedTexImage1D(GL_TEXTURE_1D, mip, internalFormat, width, 0, size, mBuffer);
					break;
				case TEX_TYPE_2D:
					glCompressedTexImage2D(GL_TEXTURE_2D, mip, internalFormat, width, height, 0, size, mBuffer);
					break;
				case TEX_TYPE_3D:
					glCompressedTexImage3D(GL_TEXTURE_3D, mip, internalFormat, width, height, depth, 0, size, mBuffer);
					break;
				}

				if(width>1)		width = width/2;
				if(height>1)	height = height/2;
				if(depth>1)		depth = depth/2;
			}
		}
		else
		{
			// Run through this process to pre-generate mip=map pyramid
			for(unsigned int mip=0; mip<=mNumMipmaps; mip++)
			{
				// Normal formats
				switch(mTextureType)
				{
				case TEX_TYPE_1D:
					glTexImage1D(GL_TEXTURE_1D, mip, internalFormat, width, 0, format, GL_UNSIGNED_BYTE, mBuffer);
					break;
				case TEX_TYPE_2D:
					glTexImage2D(GL_TEXTURE_2D, mip, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, mBuffer);
					break;
				case TEX_TYPE_3D:
					glTexImage3D(GL_TEXTURE_3D, mip, internalFormat, width, height, depth, 0, format, GL_UNSIGNED_BYTE, mBuffer);
					break;
				}

				if(width>1)		width = width/2;
				if(height>1)	height = height/2;
				if(depth>1)		depth = depth/2;
			}
		}

		glBindTexture(getGLTextureType(), 0);
	}

	return true;
}

void Texture::unloadImpl()
{
	if (mTextureType == TEX_TYPE_2D)
	{
		glDeleteTextures(1, &mTextureID);

		mTextureType = TEX_TYPE_2D;
	}

	SAFE_DELETE_ARRAY(mBuffer);

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

GLenum Texture::getGLTextureType() const
{
	switch(mTextureType)
	{
	case TEX_TYPE_1D:
		return GL_TEXTURE_1D;
	case TEX_TYPE_2D:
		return GL_TEXTURE_2D;
	case TEX_TYPE_3D:
		return GL_TEXTURE_3D;
	default:
		return GL_TEXTURE_2D;
	}
}


} // end namespace render
