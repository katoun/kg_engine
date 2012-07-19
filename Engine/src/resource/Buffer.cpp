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

#include <resource/Buffer.h>

namespace resource
{

Buffer::Buffer(BufferUsage usage)
{
	mUsage = usage;
	mIsLocked = false;
}

Buffer::~Buffer() {}

void* Buffer::lock(unsigned int offset, unsigned int length, BufferLocking options)
{
	assert((!isLocked()) && "Cannot lock this buffer, it is already locked!");

	void* ret = lockImpl(offset, length, options);
	mIsLocked = true;
	mLockStart = offset;
	mLockSize = length;
	return ret;
}

void* Buffer::lock(BufferLocking options)
{
	return this->lock(0, mSizeInBytes, options);
}

void Buffer::unlock()
{
	assert((isLocked()) && "Cannot unlock this buffer, it is not locked!");

	// Otherwise, unlock the real one
	unlockImpl();
	mIsLocked = false;
}

void Buffer::copyData(Buffer& srcBuffer, unsigned int srcOffset, unsigned int dstOffset, unsigned int length, bool discardWholeBuffer)
{
	const void *srcData = srcBuffer.lock(srcOffset, length, BL_READ_ONLY);
	this->writeData(dstOffset, length, srcData, discardWholeBuffer);
	srcBuffer.unlock();
}

unsigned int Buffer::getSizeInBytes() const
{ 
	return mSizeInBytes;
}

BufferUsage Buffer::getUsage() const
{
	return mUsage;
}

bool Buffer::isLocked() const
{
	return mIsLocked;
}

}// end namespace resource
