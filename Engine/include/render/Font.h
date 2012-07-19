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

#ifndef _FONT_H_
#define _FONT_H_

#include <core/Config.h>
#include <resource/Resource.h>

#include <string>

namespace resource
{
class Serializer;
}

namespace render
{

class Material;

// Allow 8-bit ASCII 
// Only chars 33+ are any use though
#define GAME_NUM_CHARS (256 - 32)
// How to look up chars
#define GAME_CHAR_INDEX(c) c - 33

enum FontType
{
	FT_TRUETYPE,	//! Generated from a truetype (.ttf) font
	FT_IMAGE,		//! Loaded from an image created by an artist
	FT_COUNT
};

//! Class representing a font in the system.
//!
//! This class is simply a way of getting a font texture into the render system and
//! to easily retrieve the texture coordinates required to accurately render them.
class ENGINE_PUBLIC_EXPORT Font: public resource::Resource
{
public:

	Font(const std::string& name, resource::Serializer* serializer);
	~Font();

	//! Sets the material for the font.
	void setMaterial(const std::string& filename);
	void setMaterial(Material* material);

	Material* getMaterial() const;

	void setCharTexCoords(char id, float u1, float v1, float u2, float v2);

	void getCharTexCoords(char id, float& u1, float& v1, float& u2, float& v2) const;
	
	float getCharAspectRatio(char id) const;

protected:

	Material* mMaterial;

	//! Start u coords
	float mTexCoordsU1[GAME_NUM_CHARS];
	//! End u coords
	float mTexCoordsU2[GAME_NUM_CHARS];
	//! Start v coords
	float mTexCoordsV1[GAME_NUM_CHARS];
	//! End v coords
	float mTexCoordsV2[GAME_NUM_CHARS];

	//! Aspect ratio between x and y (width / height)
	float mAspectRatio[GAME_NUM_CHARS];
};

} //namespace render

#endif