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

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <core/Config.h>

namespace resource
{

//! Locking options
enum BufferLocking
{
	BL_NORMAL,		//! Normal mode, ie allows read/write and contents are preserved.    
	BL_DISCARD,		//! Discards the entire buffer while locking; Only allowed with the BU_DYNAMIC flag.   
	BL_READ_ONLY,	//! Lock the buffer for reading only. Not allowed with the BU_WRITE_ONLY flag. Mandatory on static buffers.    
	BL_NO_OVERWRITE	//! As BL_NORMAL, except the application do not overwrite any region of the buffer which has already been used in this frame.
};

//! Enum describing buffer usage; not mutually exclusive
enum BufferUsage
{
	BU_STATIC = 1,							//! Static buffer which the application rarely modifies once created.
	BU_DYNAMIC = 2,							//! Dynamic buffer which the application fairly often modifies once created.    
	BU_WRITE_ONLY = 4,						//! The application will never read the contents of the buffer back, it will only ever write data.
	BU_STATIC_WRITE_ONLY = 5,				//! Combination of BU_STATIC and BU_WRITE_ONLY.
	BU_DYNAMIC_WRITE_ONLY = 6,				//! Combination of BU_DYNAMIC and BU_WRITE_ONLY.
	BU_DISCARDABLE = 8,						//! Discardable buffer which the application will be refilling the contents of the buffer regularly (not just updating, but generating the contents from scratch)
	BU_DYNAMIC_WRITE_ONLY_DISCARDABLE = 14	//! Combination of BU_DYNAMIC, BU_WRITE_ONLY and BU_DISCARDABLE
};

//! Abstract class defining common features of buffers.
//! A 'buffer' is any area of memory held inside or outside of core system ram.
//! Can refer to video ram or other memory areas such as sound card memory, custom
//! co-processor memory etc, called 'hardware buffers'.
class ENGINE_PUBLIC_EXPORT Buffer
{
public:

	Buffer(BufferUsage usage);

	virtual ~Buffer();
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
	virtual void copyData(Buffer& srcBuffer, unsigned int srcOffset, unsigned int dstOffset, unsigned int length, bool discardWholeBuffer = false);

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

}// end namespace resource

#endif// _BUFFER_H_