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

#include <core/Log.h>
#include <core/LogDefines.h>
#include <GLVertexBuffer.h>
#include <GLRenderDriver.h>

namespace render
{

GLVertexBuffer::GLVertexBuffer(unsigned int vertexSize, unsigned int numVertices, resource::BufferUsage usage)
: VertexBuffer(vertexSize, numVertices, usage)
{
	glGenBuffersARB(1, &mBufferId);

	if (!mBufferId)
	{
		core::Log::getInstance().logMessage("GLVertexBuffer", "Cannot create GL vertex buffer", core::LOG_LEVEL_ERROR);
		return;
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);

	// Initialize mapped buffer and set usage
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, mSizeInBytes, NULL, GLRenderDriver::getGLUsage(usage));
}

GLVertexBuffer::~GLVertexBuffer() 
{
	glDeleteBuffersARB(1, &mBufferId);
}

void GLVertexBuffer::readData(unsigned int offset, unsigned int length, void* pDest)
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);
	glGetBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, length, pDest);
}

void GLVertexBuffer::writeData(unsigned int offset, unsigned int length, const void* pSource, bool discardWholeBuffer)
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);

	if (offset == 0 && length == mSizeInBytes)
	{
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, mSizeInBytes, pSource, GLRenderDriver::getGLUsage(mUsage));
	}
	else
	{
		if(discardWholeBuffer)
		{
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, mSizeInBytes, NULL, GLRenderDriver::getGLUsage(mUsage));
		}

		// Now update the real buffer
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, length, pSource);
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
		core::Log::getInstance().logMessage("GLVertexBuffer", "Invalid attempt to lock an index buffer that has already been locked", core::LOG_LEVEL_ERROR);
		return NULL;
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);

	if(options == resource::BL_DISCARD)
	{
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, mSizeInBytes, NULL, GLRenderDriver::getGLUsage(mUsage));

		access = (mUsage & resource::BU_DYNAMIC) ? GL_READ_WRITE_ARB : GL_WRITE_ONLY_ARB;

	}
	else if(options == resource::BL_READ_ONLY)
	{
		if(mUsage & resource::BU_WRITE_ONLY)
		{
			core::Log::getInstance().logMessage("GLVertexBuffer", "Invalid attempt to lock a write-only vertex buffer as read-only", core::LOG_LEVEL_ERROR);
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
		core::Log::getInstance().logMessage("GLVertexBuffer", "Invalid locking option set", core::LOG_LEVEL_ERROR);
		return NULL;
	}

	void* pBuffer = glMapBufferARB(GL_ARRAY_BUFFER_ARB, access);

	if(pBuffer == NULL)
	{		
		core::Log::getInstance().logMessage("GLVertexBuffer", "Vertex Buffer: Out of memory", core::LOG_LEVEL_ERROR);
		return NULL;
	}

	mIsLocked = true;
	// return offset-ed
	return static_cast<void*>(static_cast<unsigned char*>(pBuffer) + offset);
}

void GLVertexBuffer::unlockImpl()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);

	if(!glUnmapBufferARB(GL_ARRAY_BUFFER_ARB))
	{
		core::Log::getInstance().logMessage("GLVertexBuffer", "Buffer data corrupted, please reload", core::LOG_LEVEL_ERROR);
		return;
	}

	mIsLocked = false;
}

}// end namespace render
