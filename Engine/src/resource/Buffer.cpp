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
