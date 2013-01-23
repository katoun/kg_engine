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

#include <resource/PixelFormat.h>
#include <GLTexture.h>
#include <GLPixelFormat.h>

namespace render
{

GLTexture::GLTexture(const std::string& filename, resource::Serializer* serializer):Texture(filename, serializer)
{
}

GLTexture::~GLTexture()
{
}

GLuint GLTexture::getGLID() const
{
	return mTextureID;
}

bool GLTexture::loadImpl()
{
	if (!Texture::loadImpl()) return false;

	if (!mBuffer) return false;

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

		GLenum format = resource::GLPixelUtil::getGLOriginFormat(mPixelFormat);
		GLenum internalFormat = resource::GLPixelUtil::getClosestGLInternalFormat(mPixelFormat);
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
			// Run through this process to pre-generate mipmap pyramid
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

void GLTexture::unloadImpl()
{
	if (mTextureType == TEX_TYPE_2D)
	{
		glDeleteTextures(1, &mTextureID);

		mTextureType = TEX_TYPE_2D;
	}

	Texture::unloadImpl();
}

GLenum GLTexture::getGLTextureType() const
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
