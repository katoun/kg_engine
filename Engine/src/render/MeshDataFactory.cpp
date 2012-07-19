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

#include <render/MeshDataFactory.h>
#include <render/MeshData.h>

namespace render
{

resource::Resource* MeshDataFactory::createResource(const std::string& filename, resource::Serializer* serializer)
{
	return new MeshData(filename, serializer);
}

void MeshDataFactory::destroyResource(resource::Resource* resource)
{
	MeshData* meshData = static_cast<MeshData*>(resource);

	assert(meshData != NULL);
	if (meshData != NULL) delete meshData;
}


} // end namespace render
