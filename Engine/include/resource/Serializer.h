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