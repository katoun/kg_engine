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

#ifndef _INDEX_BUFFER_H_
#define _INDEX_BUFFER_H_

#include <core/Config.h>
#include <resource/Buffer.h>

namespace render
{

enum IndexType
{
	IT_16BIT,
	IT_32BIT
};

//! Class for index buffers, still abstract.
class ENGINE_PUBLIC_EXPORT IndexBuffer : public resource::Buffer
{
public:

	IndexBuffer(IndexType idxType, unsigned int numIndexes, resource::BufferUsage usage);

	virtual ~IndexBuffer();

	//! Get the type of indexes used in this buffer.
	IndexType getType();
	//! Get the number of indexes in this buffer.
	unsigned int getNumIndexes();
	//! Get the size in bytes of each index.
	unsigned int getIndexSize();

protected:
	
	IndexType mIndexType;
	unsigned int mNumIndexes;
	unsigned int mIndexSize;
};

}// end namespace render

#endif// _INDEXBUFFER_H_