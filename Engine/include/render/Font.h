/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

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