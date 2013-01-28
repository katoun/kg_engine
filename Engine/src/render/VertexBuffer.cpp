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
#include <core/Log.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <render/VertexBuffer.h>

namespace render
{


VertexBuffer::VertexBuffer(VertexBufferType vertexBufferType, VertexElementType vertexElementType, unsigned int numVertices, resource::BufferUsage usage)
: resource::Buffer(usage)
{
	mVertexBufferType = vertexBufferType;
	mVertexElementType = vertexElementType;
	
	mNumVertices = numVertices;
	mVertexSize = 0;

	// Calculate the size of the vertices
	switch(mVertexElementType)
	{
	case VERTEX_ELEMENT_TYPE_COLOR:
		mVertexSize =  sizeof(unsigned int);
		break;
	case VERTEX_ELEMENT_TYPE_FLOAT1:
		mVertexSize =  sizeof(float);
		break;
	case VERTEX_ELEMENT_TYPE_FLOAT2:
		mVertexSize =  sizeof(float)*2;
		break;
	case VERTEX_ELEMENT_TYPE_FLOAT3:
		mVertexSize =  sizeof(float)*3;
		break;
	case VERTEX_ELEMENT_TYPE_FLOAT4:
		mVertexSize =  sizeof(float)*4;
		break;
	case VERTEX_ELEMENT_TYPE_SHORT1:
		mVertexSize =  sizeof(signed short int);
		break;
	case VERTEX_ELEMENT_TYPE_SHORT2:
		mVertexSize =  sizeof(signed short int)*2;
		break;
	case VERTEX_ELEMENT_TYPE_SHORT3:
		mVertexSize =  sizeof(signed short int)*3;
		break;
	case VERTEX_ELEMENT_TYPE_SHORT4:
		mVertexSize =  sizeof(signed short int)*4;
		break;
	}

	// Calculate the size of the vertices
	mSizeInBytes = mVertexSize * numVertices;
}

VertexBuffer::~VertexBuffer() {}

VertexBufferType VertexBuffer::getVertexBufferType()
{
	return mVertexBufferType;
}

VertexElementType VertexBuffer::getVertexElementType()
{
	return mVertexElementType;
}

unsigned int VertexBuffer::getNumVertices()
{
	return mNumVertices;
}

unsigned int VertexBuffer::getVertexSize()
{
	return mVertexSize;
}

}// end namespace render
