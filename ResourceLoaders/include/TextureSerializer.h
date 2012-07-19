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

#ifndef _TEXTURE_SERIALIZER_H_
#define _TEXTURE_SERIALIZER_H_

#include <Config.h>
#include <resource/Serializer.h>

#include <string>

namespace render
{
class Texture;
class Color;
}

namespace resource
{

// Forward declarations for loading DDS data.
enum PixelFormat;
class DataStream;

//! Class for serialising texture data to/from a texture file.
class TextureSerializer: public Serializer
{
public:

	TextureSerializer();
	virtual ~TextureSerializer();

	//! Imports a Texture from a texture file.
	bool importResource(Resource* dest, const std::string& filename);

	//! Exports a texture to the file specified.
	bool exportResource(Resource* source, const std::string& filename);
};

}// end namespace resource

#endif