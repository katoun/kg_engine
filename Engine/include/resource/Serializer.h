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

#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include <core/Config.h>

#include <string>

namespace core
{
class vector3d;
class quaternion;
}

namespace render
{
class Color;
}

namespace resource
{

class Resource;
enum ResourceType;

//! Chunk overhead = ID + size
const unsigned int CHUNK_OVERHEAD_SIZE = sizeof(unsigned short int) + sizeof(unsigned int);
const unsigned short int HEADER_CHUNK_ID = 0x1000;

//! Generic class for serialising data to / from binary chunk-based files.
//!
//! This class provides a number of useful methods for exporting / importing data
//! from chunk-oriented binary files (e.g. .mesh).
class ENGINE_PUBLIC_EXPORT Serializer
{
public:

	Serializer();
	virtual ~Serializer();

	//! Imports a resource from the file specified.
	virtual bool importResource(Resource* dest, const std::string& filename) = 0;

	//! Exports a resource to the file specified. 
	virtual bool exportResource(Resource* source, const std::string& filename) = 0;

protected:

	ResourceType mResourceType;

	unsigned int mCurrentChunkLen;

	std::string mVersion;
};

inline core::vector3d ENGINE_PUBLIC_EXPORT parseVector3d(std::string& params);
inline core::quaternion ENGINE_PUBLIC_EXPORT parseQuaternion(std::string& params);
inline render::Color ENGINE_PUBLIC_EXPORT parseColor(std::string& params);

}// end namespace resource

#endif