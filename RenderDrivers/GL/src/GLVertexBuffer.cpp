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
#include <GLVertexBuffer.h>
#include <GLRenderDriver.h>

namespace render
{

GLVertexBuffer::GLVertexBuffer(VertexBufferType vertexBufferType, VertexElementType vertexElementType, unsigned int numVertices, resource::BufferUsage usage)
: VertexBuffer(vertexBufferType, vertexElementType, numVertices, usage)
{
	glGenBuffers(1, &mBufferId);

	if (!mBufferId)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLVertexBuffer", "Cannot create GL vertex buffer", core::LOG_LEVEL_ERROR);
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);

	// Initialize mapped buffer and set usage
	glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, nullptr, GLRenderDriver::getGLUsage(usage));
}

GLVertexBuffer::~GLVertexBuffer() 
{
	glDeleteBuffers(1, &mBufferId);
}

void GLVertexBuffer::readData(unsigned int offset, unsigned int length, void* pDest)
{
	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);
	glGetBufferSubData(GL_ARRAY_BUFFER, offset, length, pDest);
}

void GLVertexBuffer::writeData(unsigned int offset, unsigned int length, const void* pSource, bool discardWholeBuffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);

	if (offset == 0 && length == mSizeInBytes)
	{
		glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, pSource, GLRenderDriver::getGLUsage(mUsage));
	}
	else
	{
		if(discardWholeBuffer)
		{
			glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, nullptr, GLRenderDriver::getGLUsage(mUsage));
		}

		// Now update the real buffer
		glBufferSubData(GL_ARRAY_BUFFER, offset, length, pSource);
	}
}

GLuint GLVertexBuffer::getGLBufferId() const
{
	return mBufferId;
}

void* GLVertexBuffer::lockImpl(unsigned int offset, unsigned int length, resource::BufferLocking options)
{
	GLenum access = 0;

	if(mIsLocked)
	{		
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLVertexBuffer", "Invalid attempt to lock an index buffer that has already been locked", core::LOG_LEVEL_ERROR);
		return nullptr;
	}

	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);

	if(options == resource::BL_DISCARD)
	{
		glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, nullptr, GLRenderDriver::getGLUsage(mUsage));

		access = (mUsage & resource::BU_DYNAMIC) ? GL_READ_WRITE : GL_WRITE_ONLY;

	}
	else if(options == resource::BL_READ_ONLY)
	{
		if(mUsage & resource::BU_WRITE_ONLY)
		{
			if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLVertexBuffer", "Invalid attempt to lock a write-only vertex buffer as read-only", core::LOG_LEVEL_ERROR);
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

void GLVertexBuffer::unlockImpl()
{
	glBindBuffer(GL_ARRAY_BUFFER, mBufferId);

	if(!glUnmapBuffer(GL_ARRAY_BUFFER))
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("GLVertexBuffer", "Buffer data corrupted, please reload", core::LOG_LEVEL_ERROR);
		return;
	}

	mIsLocked = false;
}

}// end namespace render
