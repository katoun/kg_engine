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

#ifndef _RESOURCE_FACTORY_H_
#define _RESOURCE_FACTORY_H_

#include <core/Config.h>

namespace resource
{

class Resource;
class Serializer;

class ENGINE_PUBLIC_EXPORT ResourceFactory
{
public:

	//! Creates a new resource object.
	virtual Resource* createResource(const std::string& filename, Serializer* serializer) = 0;

	//! Destroys a resource object which was created by this factory.
	virtual void destroyResource(Resource* resource) = 0;
};

} // end namespace resource

#endif