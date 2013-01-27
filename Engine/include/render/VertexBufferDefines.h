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

#ifndef _VERTEX_BUFFER_DEFINES_H_
#define _VERTEX_BUFFER_DEFINES_H_

namespace render
{

//! Vertex element semantics, used to identify the meaning of vertex buffer contents.
enum VertexElementSemantic
{
	VERTEX_ELEMENT_SEMANTIC_POSITION,				//! Position, 3 reals per vertex
	VERTEX_ELEMENT_SEMANTIC_NORMAL,					//! Normal, 3 reals per vertex
	VERTEX_ELEMENT_SEMANTIC_TEXTURE_COORDINATES,	//! Texture coordinates
	VERTEX_ELEMENT_SEMANTIC_COUNT
};

//! Vertex element type, used to identify the base types of the vertex contents.
enum VertexElementType
{
	VERTEX_ELEMENT_TYPE_FLOAT1,
	VERTEX_ELEMENT_TYPE_FLOAT2,
	VERTEX_ELEMENT_TYPE_FLOAT3,
	VERTEX_ELEMENT_TYPE_FLOAT4,
	VERTEX_ELEMENT_TYPE_COLOR,
	VERTEX_ELEMENT_TYPE_SHORT1,
	VERTEX_ELEMENT_TYPE_SHORT2,
	VERTEX_ELEMENT_TYPE_SHORT3,
	VERTEX_ELEMENT_TYPE_SHORT4
};

}// end namespace render

#endif// _VERTEX_BUFFER_DEFINES_H_