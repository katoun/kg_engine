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

VertexBuffer::VertexBuffer(VertexBufferType vertexBufferType, VertexElementType vertexElementType, unsigned int numVertices, BufferUsage usage)
{
	mVertexBufferType = vertexBufferType;
	mVertexElementType = vertexElementType;
	
	mNumVertices = numVertices;
	mVertexSize = 0;

	mIsLocked = false;

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

	switch(usage)
	{
	case BU_STATIC:
	case BU_STATIC_WRITE_ONLY:
		mGLUsage = GL_STATIC_DRAW;
	case BU_DYNAMIC:
	case BU_DYNAMIC_WRITE_ONLY:
		mGLUsage = GL_DYNAMIC_DRAW;
	case BU_DYNAMIC_WRITE_ONLY_DISCARDABLE:
		mGLUsage = GL_STREAM_DRAW;
	default:
		mGLUsage = GL_DYNAMIC_DRAW;
	};

	// Calculate the size of the vertices
	mSizeInBytes = mVertexSize * numVertices;

	glGenBuffers(1, &mBufferId);

	if (!mBufferId)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLVertexBuffer", "Cannot create GL vertex buffer", core::LOG_LEVEL_ERROR);
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);

	// Initialize mapped buffer and set usage
	glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, nullptr, mGLUsage);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &mBufferId);
}

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

void VertexBuffer::readData(unsigned int offset, unsigned int length, void* pDest)
{
	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);
	glGetBufferSubData(GL_ARRAY_BUFFER, offset, length, pDest);
}

void VertexBuffer::writeData(unsigned int offset, unsigned int length, const void* pSource, bool discardWholeBuffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);

	if (offset == 0 && length == mSizeInBytes)
	{
		glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, pSource, mGLUsage);
	}
	else
	{
		if(discardWholeBuffer)
		{
			glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, nullptr, mGLUsage);
		}

		// Now update the real buffer
		glBufferSubData(GL_ARRAY_BUFFER, offset, length, pSource);
	}
}

void* VertexBuffer::lock(BufferLocking options)
{
	return lock(0, mSizeInBytes, options);
}

void* VertexBuffer::lock(unsigned int offset, unsigned int length, BufferLocking options)
{
	GLenum access = 0;

	if(mIsLocked)
	{		
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLVertexBuffer", "Invalid attempt to lock an index buffer that has already been locked", core::LOG_LEVEL_ERROR);
		return nullptr;
	}

	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);

	if(options == BL_DISCARD)
	{
		glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, nullptr, mGLUsage);

		access = (mUsage & BU_DYNAMIC) ? GL_READ_WRITE : GL_WRITE_ONLY;

	}
	else if(options == BL_READ_ONLY)
	{
		if(mUsage & BU_WRITE_ONLY)
		{
			if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLVertexBuffer", "Invalid attempt to lock a write-only vertex buffer as read-only", core::LOG_LEVEL_ERROR);
			return nullptr;
		}
		access = GL_READ_ONLY;
	}
	else if(options == BL_NORMAL || options == BL_NO_OVERWRITE)
	{
		access = (mUsage & BU_DYNAMIC) ? GL_READ_WRITE : GL_WRITE_ONLY;
	}
	else
	{	
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLVertexBuffer", "Invalid locking option set", core::LOG_LEVEL_ERROR);
		return nullptr;
	}

	void* pBuffer = glMapBuffer(GL_ARRAY_BUFFER, access);

	if(pBuffer == nullptr)
	{		
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLVertexBuffer", "Vertex Buffer: Out of memory", core::LOG_LEVEL_ERROR);
		return nullptr;
	}

	mIsLocked = true;
	// return offset-ed
	return static_cast<void*>(static_cast<unsigned char*>(pBuffer) + offset);
}

void VertexBuffer::unlock()
{
	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);

	if(!glUnmapBuffer(GL_ARRAY_BUFFER))
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLVertexBuffer", "Buffer data corrupted, please reload", core::LOG_LEVEL_ERROR);
		return;
	}

	mIsLocked = false;
}

GLuint VertexBuffer::getGLBufferId() const
{
	return mBufferId;
}

}// end namespace render
