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

	Texture(const std::string& name, resource::Serializer* serializer);
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
	void setFlags(signed int flags);

	//! Returns the flags of the texture.
	signed int getFlags();

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

	signed int mFlags;			//! Image specific flags
	unsigned char mPixelSize;		//! The number of bytes per pixel

	unsigned int mNumMipmaps;

	bool mHasAlpha;
};

} // end namespace render

#endif