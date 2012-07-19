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