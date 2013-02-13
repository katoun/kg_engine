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

#ifndef _RENDER_BUFFER_H_
#define _RENDER_BUFFER_H_

#include <EngineConfig.h>
#include <render/RenderBufferDefines.h>

namespace render
{

//! Abstract class defining common features of render buffers.
//! A 'render buffer' is any area of memory held inside of video ram.
class ENGINE_PUBLIC_EXPORT RenderBuffer
{
public:

	RenderBuffer(BufferUsage usage);

	virtual ~RenderBuffer();
	//! Lock the buffer for (potentially) reading / writing.
	//! \param offset: The byte offset from the start of the buffer to lock
	//! \param length: The size of the area to lock, in bytes
	//! \param options: Locking options
	//! \returns Pointer to the locked memory
	virtual void* lock(unsigned int offset, unsigned int length, BufferLocking options);

	//! Lock the entire buffer for (potentially) reading / writing.
	//! \param options: Locking options
	//! \returns Pointer to the locked memory
	void* lock(BufferLocking options);

	//! Releases the lock on this buffer. 
	//!
	//! Locking and unlocking a buffer can, in some rare circumstances such as 
	//! switching video modes whilst the buffer is locked, corrupt the 
	//! contents of a buffer. This is pretty rare, but if it occurs, 
	//! this method will throw an exception, meaning you
	//! must re-upload the data. 
	//! Note that using the 'read' and 'write' forms of updating the buffer does not
	//! suffer from this problem, so if you want to be 100% sure your
	//! data will not be lost, use the 'read' and 'write' forms instead.
	virtual void unlock();

	//! Reads data from the buffer and places it in the memory pointed to by pDest.
	//! \param offset: The byte offset from the start of the buffer to read
	//! \param length: The size of the area to read, in bytes
	//! \param pDest: The area of memory in which to place the data, must be large enough to 
	//! accommodate the data!
	virtual void readData(unsigned int offset, unsigned int length, void* pDest) = 0;
	//! Writes data to the buffer from an area of system memory; note that you must
	//! ensure that your buffer is big enough.
	//! \param offset: The byte offset from the start of the buffer to start writing
	//! \param length: The size of the data to write to, in bytes
	//! \param pSource: The source of the data to be written
	//! \param discardWholeBuffer: If true, this allows the driver to discard the entire buffer when writing,
	//! such that DMA stalls can be avoided; use if you can.
	virtual void writeData(unsigned int offset, unsigned int length, const void* pSource, bool discardWholeBuffer = false) = 0;

	//! Copy data from another buffer into this one.
	//!
	//!	Note that the source buffer must not be created with the
	//! usage BU_WRITE_ONLY otherwise this will fail. 
	//! \param srcBuffer: The buffer from which to read the copied data
	//! \param srcOffset: Offset in the source buffer at which to start reading
	//! \param dstOffset: Offset in the destination buffer to start writing
	//! \param length: Length of the data to copy, in bytes.
	//! \param discardWholeBuffer: If true, will discard the entire contents of this buffer before copying
	virtual void copyData(RenderBuffer& srcBuffer, unsigned int srcOffset, unsigned int dstOffset, unsigned int length, bool discardWholeBuffer = false);

	//! Returns the size of this buffer in bytes.
	unsigned int getSizeInBytes() const;
	//! Returns the Usage flags with which this buffer was created.
	BufferUsage getUsage() const;

	//! Returns whether or not this buffer is currently locked.
	bool isLocked() const;

protected:

	unsigned int mSizeInBytes;
	BufferUsage mUsage;
	bool mIsLocked;
	unsigned int mLockStart;
	unsigned int mLockSize;

	//! Internal implementation of lock().
	virtual void* lockImpl(unsigned int offset, unsigned int length, BufferLocking options) = 0;
	//! Internal implementation of unlock().
	virtual void unlockImpl() = 0;
};

}// end namespace render

#endif// _RENDER_BUFFER_H_