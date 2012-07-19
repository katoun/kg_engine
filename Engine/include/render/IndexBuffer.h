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