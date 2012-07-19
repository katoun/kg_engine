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
					glCompressedTexImage1DARB(GL_TEXTURE_1D, mip, internalFormat, width, 0, size, mBuffer);
					break;
				case TEX_TYPE_2D:
					glCompressedTexImage2DARB(GL_TEXTURE_2D, mip, internalFormat, width, height, 0, size, mBuffer);
					break;
				case TEX_TYPE_3D:
					glCompressedTexImage3DARB(GL_TEXTURE_3D, mip, internalFormat, width, height, depth, 0, size, mBuffer);
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
