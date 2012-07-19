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

#include <render/IndexBuffer.h>

namespace render
{

IndexBuffer::IndexBuffer(IndexType idxType, unsigned int numIndexes, resource::BufferUsage usage)
: resource::Buffer(usage)
{
	mIndexType = idxType;
	mNumIndexes = numIndexes;

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
}

IndexBuffer::~IndexBuffer() {}

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

}// end namespace render
