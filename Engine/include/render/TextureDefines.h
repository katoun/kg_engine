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

#ifndef _TEXTURE_DEFINES_H_
#define _TEXTURE_DEFINES_H_

namespace render
{

//! Enum identifying the texture type
enum TextureType
{
	TEX_TYPE_1D = 1,		//! 1D texture, used in combination with 1D texture coordinates
	TEX_TYPE_2D = 2,		//! 2D texture, used in combination with 2D texture coordinates (default)
	TEX_TYPE_3D = 3,		//! 3D texture, used in combination with 3D texture coordinates
};

enum ImageFlags
{
	IF_COMPRESSED = 0x00000001,
	IF_CUBEMAP    = 0x00000002,
	IF_3D_TEXTURE = 0x00000004
};

} // end namespace render

#endif