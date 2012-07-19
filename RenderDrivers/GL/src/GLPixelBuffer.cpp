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

#include <GLPixelBuffer.h>
#include <GLRenderDriver.h>
#include <engine/LogManager.h>

namespace render
{

GLPixelBuffer::GLPixelBuffer(unsigned int width, unsigned int height, unsigned int depth, resource::PixelFormat format, resource::BufferUsage usage)
: PixelBuffer(width, height, depth, format, usage)
{
	glGenBuffersARB(1, &mBufferId);

	if (!mBufferId)
	{
		engine::LogManager::getInstance()->logMessage("GLPixelBuffer", "Cannot create GL pixel buffer", engine::LL_ERROR);
		return;
	}

	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,mBufferId);

	// Initialize buffer and set usage
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, mSizeInBytes, NULL, GLRenderDriver::getGLUsage(usage));
}

GLPixelBuffer::~GLPixelBuffer() 
{
	glDeleteBuffersARB(1, &mBufferId);
}

void GLPixelBuffer::readData(unsigned int offset, unsigned int length, void* pDest)
{
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mBufferId);
	glGetBufferSubDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, offset, length, pDest);
}

void GLPixelBuffer::writeData(unsigned int offset, unsigned int length, const void* pSource, bool discardWholeBuffer)
{
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mBufferId);

	if (offset == 0 && length == mSizeInBytes)
	{
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, mSizeInBytes, pSource, GLRenderDriver::getGLUsage(mUsage));
	}
	else
	{
		if(discardWholeBuffer)
		{
			glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, mSizeInBytes, NULL, GLRenderDriver::getGLUsage(mUsage));
		}

		// Now update the real buffer
		glBufferSubDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, offset, length, pSource);
	}
}

GLuint GLPixelBuffer::getGLBufferId() const
{
	return mBufferId;
}

void* GLPixelBuffer::lockImpl(unsigned int offset, unsigned int length, resource::BufferLocking options)
{
	GLenum access = 0;

	if(mIsLocked)
	{		
		engine::LogManager::getInstance()->logMessage("GLPixelBuffer", "Invalid attempt to lock a pixel buffer that has already been locked", engine::LL_ERROR);
		return NULL;
	}

	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mBufferId);

	if(options == resource::BL_DISCARD)
	{
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, mSizeInBytes, NULL, GLRenderDriver::getGLUsage(mUsage));

		access = (mUsage & resource::BU_DYNAMIC) ? GL_READ_WRITE_ARB : GL_WRITE_ONLY_ARB;

	}
	else if(options == resource::BL_READ_ONLY)
	{
		if(mUsage & resource::BU_WRITE_ONLY)
		{
			engine::LogManager::getInstance()->logMessage("GLPixelBuffer", "Invalid attempt to lock a write-only pixel buffer as read-only", engine::LL_ERROR);
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
		engine::LogManager::getInstance()->logMessage("GLPixelBuffer", "Invalid locking option set", engine::LL_ERROR);
		return NULL;
	}

	void* pBuffer = glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, access);

	if(pBuffer == NULL)
	{		
		engine::LogManager::getInstance()->logMessage("GLPixelBuffer", "Pixel Buffer: Out of memory", engine::LL_ERROR);
		return NULL;
	}

	mIsLocked = true;
	// return offset-ed
	return static_cast<void*>(static_cast<unsigned char*>(pBuffer) + offset);
}

void GLPixelBuffer::unlockImpl()
{
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mBufferId);

	if(!glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB))
	{
		engine::LogManager::getInstance()->logMessage("GLIndexBuffer", "Buffer data corrupted, please reload", engine::LL_ERROR);
		return;
	}

	mIsLocked = false;
}

}// end namespace render
