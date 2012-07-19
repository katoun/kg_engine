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

#ifndef _VERTEX_INDEX_DATA_H_
#define _VERTEX_INDEX_DATA_H_

#include <core/Config.h>

namespace render
{

class VertexDeclaration;
class VertexBufferBinding;
class IndexBuffer;

//! Summary class collecting together vertex source information.
class ENGINE_PUBLIC_EXPORT VertexData
{
public:

	VertexData();
	~VertexData();

	//! Declaration of the vertex to be used in this operation. 
	VertexDeclaration* vertexDeclaration;
	//! The vertex buffer bindings to be used. 
	VertexBufferBinding* vertexBufferBinding;
	//! The base vertex index to start from, if using un indexed geometry.
	unsigned int vertexStart;
	//! The number of vertices used in this operation.
	unsigned int vertexCount;
};

//! Summary class collecting together index data source information.
class ENGINE_PUBLIC_EXPORT IndexData
{
public:

	IndexData();
	~IndexData();

	//! pointer to the IndexBuffer to use.
	IndexBuffer* indexBuffer;

	//! index in the buffer to start from for this operation.
	unsigned int indexStart;

	//! The number of indexes to use from the buffer.
	unsigned int indexCount;
};

}// end namespace render

#endif// _VERTEXINDEXDATA_H_