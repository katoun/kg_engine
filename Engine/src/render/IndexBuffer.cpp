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
#include <core/LogDefines.h>
#include <render/IndexBuffer.h>

namespace render
{

IndexBuffer::IndexBuffer(IndexType idxType, unsigned int numIndexes, resource::BufferUsage usage)
: resource::Buffer(usage)
{
	mIndexType = idxType;
	mNumIndexes = numIndexes;
	mIndexSize = 0;

	// Calculate the size of the indexes
	switch (mIndexType)
	{
	case IT_16BIT:
		mIndexSize = sizeof(unsigned short int);
		break;
	case IT_32BIT:
		mIndexSize = sizeof(unsigned int);
		break;
	}
	
	mSizeInBytes = mIndexSize * mNumIndexes;

	switch(usage)
	{
	case resource::BU_STATIC:
	case resource::BU_STATIC_WRITE_ONLY:
		mGLUsage = GL_STATIC_DRAW;
	case resource::BU_DYNAMIC:
	case resource::BU_DYNAMIC_WRITE_ONLY:
		mGLUsage = GL_DYNAMIC_DRAW;
	case resource::BU_DYNAMIC_WRITE_ONLY_DISCARDABLE:
		mGLUsage = GL_STREAM_DRAW;
	default:
		mGLUsage = GL_DYNAMIC_DRAW;
	};

	glGenBuffers(1, &mBufferId);

	if (!mBufferId)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLIndexBuffer", "Cannot create GL index buffer", core::LOG_LEVEL_ERROR);
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferId);

	// Initialize buffer and set usage
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mSizeInBytes, nullptr, mGLUsage);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &mBufferId);
}

IndexType IndexBuffer::getType()
{
	return mIndexType; 
}

unsigned int IndexBuffer::getNumIndexes()
{
	return mNumIndexes;
}

unsigned int IndexBuffer::getIndexSize()
{
	return mIndexSize;
}


void IndexBuffer::readData(unsigned int offset, unsigned int length, void* pDest)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferId);
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, length, pDest);
}

void IndexBuffer::writeData(unsigned int offset, unsigned int length, const void* pSource, bool discardWholeBuffer)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferId);

	if (offset == 0 && length == mSizeInBytes)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mSizeInBytes, pSource, mGLUsage);
	}
	else
	{
		if(discardWholeBuffer)
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mSizeInBytes, nullptr, mGLUsage);
		}

		// Now update the real buffer
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, length, pSource);
	}
}

GLuint IndexBuffer::getGLBufferId() const
{
	return mBufferId;
}

void* IndexBuffer::lockImpl(unsigned int offset, unsigned int length, resource::BufferLocking options)
{
	GLenum access = 0;

	if(mIsLocked)
	{		
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLIndexBuffer", "Invalid attempt to lock an index buffer that has already been locked", core::LOG_LEVEL_ERROR);
		return nullptr;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferId);

	if(options == resource::BL_DISCARD)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mSizeInBytes, nullptr, mGLUsage);

		access = (mUsage & resource::BU_DYNAMIC) ? GL_READ_WRITE : GL_WRITE_ONLY;

	}
	else if(options == resource::BL_READ_ONLY)
	{
		if(mUsage & resource::BU_WRITE_ONLY)
		{
			if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLIndexBuffer", "Invalid attempt to lock a write-only index buffer as read-only", core::LOG_LEVEL_ERROR);
			return nullptr;
		}
		access = GL_READ_ONLY;
	}
	else if(options == resource::BL_NORMAL || options == resource::BL_NO_OVERWRITE)
	{
		access = (mUsage & resource::BU_DYNAMIC) ? GL_READ_WRITE : GL_WRITE_ONLY;
	}
	else
	{	
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLIndexBuffer", "Invalid locking option set", core::LOG_LEVEL_ERROR);
		return nullptr;
	}

	void* pBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, access);

	if(pBuffer == nullptr)
	{		
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLIndexBuffer", "Index Buffer: Out of memory", core::LOG_LEVEL_ERROR);
		return nullptr;
	}

	mIsLocked = true;
	// return offset-ed
	return static_cast<void*>(static_cast<unsigned char*>(pBuffer) + offset);
}

void IndexBuffer::unlockImpl()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferId);

	if(!glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER))
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLIndexBuffer", "Buffer data corrupted, please reload", core::LOG_LEVEL_ERROR);
		return;
	}

	mIsLocked = false;
}

}// end namespace render
