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

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <core/Config.h>
#include <render/TextureDefines.h>
#include <resource/Resource.h>

#include <string>

namespace resource
{
class Serializer;
enum PixelFormat;
}

namespace render
{

//! Abstract class representing a Texture resource.
//!
//! The actual concrete subclass which will exist for a texture
//! is dependent on the rendering system in use (Direct3D, OpenGL etc).
class ENGINE_PUBLIC_EXPORT Texture : public resource::Resource
{
public:

	Texture(const std::string& filename, resource::Serializer* serializer);
	virtual ~Texture();

	//! Sets the type of texture,
	void setTextureType(const TextureType& type);

	//! Gets the type of texture.
	TextureType getTextureType();

	//! Sets the buffer of the texture.
	void setBuffer(unsigned char* buffer, unsigned int size);

	//! Sets the width of the texture.
	void setWidth(unsigned int width);

	//! Returns the width of the texture.
	unsigned int getWidth();

	//! Sets the height of the texture.
	void setHeight(unsigned int height);

	//! Returns the height of the texture.
	unsigned int getHeight();

	//! Sets the depth of the texture.
	void setDepth(unsigned int depth);

	//! Returns the depth of the texture.
	unsigned int getDepth();

	//! Sets the flags of the texture.
	void setFlags(int flags);

	//! Returns the flags of the texture.
	int getFlags();

	//! Sets the pixel size of the texture.
	void setPixelSize(unsigned char size);

	//! Returns the pixel size of the texture.
	unsigned char getPixelSize();

	//! Sets the number of mipmaps to be used for this texture.
	void setNumMipMaps(unsigned int num);

	//! Gets the number of mipmaps to be used for this texture.
	unsigned int getNumMipMaps();

	//! Sets the pixel format for the texture surface.
	void setPixelFormat(const resource::PixelFormat& format);

	//! Returns the pixel format for the texture surface.
	resource::PixelFormat getPixelFormat() const;

	//! Sets if the texture has an alpha layer.
	void hasAlpha(bool alpha);

	//! Returns true if the texture has an alpha layer.
	bool hasAlpha();

protected:

	virtual void unloadImpl();

	TextureType mTextureType;

	resource::PixelFormat mPixelFormat;

	unsigned char* mBuffer;
	unsigned int mSize;

	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mDepth;

	int mFlags;			//! Image specific flags
	unsigned char mPixelSize;		//! The number of bytes per pixel

	unsigned int mNumMipmaps;

	bool mHasAlpha;
};

} // end namespace render

#endif