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

#include <core/Log.h>
#include <core/LogDefines.h>
#include <GLIndexBuffer.h>
#include <GLRenderDriver.h>

namespace render
{

GLIndexBuffer::GLIndexBuffer(IndexType idxType, unsigned int numIndexes, resource::BufferUsage usage)
: IndexBuffer(idxType, numIndexes, usage)
{
	glGenBuffersARB(1, &mBufferId);

	if (!mBufferId)
	{
		core::Log::getInstance().logMessage("GLIndexBuffer", "Cannot create GL index buffer", core::LOG_LEVEL_ERROR);
		return;
	}

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferId);

	// Initialize buffer and set usage
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mSizeInBytes, NULL, GLRenderDriver::getGLUsage(usage));
}

GLIndexBuffer::~GLIndexBuffer() 
{
	glDeleteBuffersARB(1, &mBufferId);
}

void GLIndexBuffer::readData(unsigned int offset, unsigned int length, void* pDest)
{
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferId);
	glGetBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, offset, length, pDest);
}

void GLIndexBuffer::writeData(unsigned int offset, unsigned int length, const void* pSource, bool discardWholeBuffer)
{
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferId);

	if (offset == 0 && length == mSizeInBytes)
	{
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mSizeInBytes, pSource, GLRenderDriver::getGLUsage(mUsage));
	}
	else
	{
		if(discardWholeBuffer)
		{
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mSizeInBytes, NULL, GLRenderDriver::getGLUsage(mUsage));
		}

		// Now update the real buffer
		glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, offset, length, pSource);
	}
}

GLuint GLIndexBuffer::getGLBufferId() const
{
	return mBufferId;
}

void* GLIndexBuffer::lockImpl(unsigned int offset, unsigned int length, resource::BufferLocking options)
{
	GLenum access = 0;

	if(mIsLocked)
	{		
		core::Log::getInstance().logMessage("GLIndexBuffer", "Invalid attempt to lock an index buffer that has already been locked", core::LOG_LEVEL_ERROR);
		return NULL;
	}

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferId);

	if(options == resource::BL_DISCARD)
	{
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mSizeInBytes, NULL, GLRenderDriver::getGLUsage(mUsage));

		access = (mUsage & resource::BU_DYNAMIC) ? GL_READ_WRITE_ARB : GL_WRITE_ONLY_ARB;

	}
	else if(options == resource::BL_READ_ONLY)
	{
		if(mUsage & resource::BU_WRITE_ONLY)
		{
			core::Log::getInstance().logMessage("GLIndexBuffer", "Invalid attempt to lock a write-only index buffer as read-only", core::LOG_LEVEL_ERROR);
			return NULL;
		}
		access = GL_READ_ONLY_ARB;
	}
	else if(options == resource::BL_NORMAL || options == resource::BL_NO_OVERWRITE)
	{
		access = (mUsage & resource::BU_DYNAMIC) ? GL_READ_WRITE_ARB : GL_WRITE_ONLY_ARB;
	}
	else
	{	
		core::Log::getInstance().logMessage("GLIndexBuffer", "Invalid locking option set", core::LOG_LEVEL_ERROR);
		return NULL;
	}

	void* pBuffer = glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, access);

	if(pBuffer == NULL)
	{		
		core::Log::getInstance().logMessage("GLIndexBuffer", "Index Buffer: Out of memory", core::LOG_LEVEL_ERROR);
		return NULL;
	}

	mIsLocked = true;
	// return offset-ed
	return static_cast<void*>(static_cast<unsigned char*>(pBuffer) + offset);
}

void GLIndexBuffer::unlockImpl()
{
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferId);

	if(!glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB))
	{
		core::Log::getInstance().logMessage("GLIndexBuffer", "Buffer data corrupted, please reload", core::LOG_LEVEL_ERROR);
		return;
	}

	mIsLocked = false;
}

}// end namespace render
