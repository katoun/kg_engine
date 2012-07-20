/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

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