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

#include <Config.h>
#include <resource/ResourceManager.h>
#include <resource/ResourceDefines.h>
#include <FontSerializer.h>
#include <TextureSerializer.h>
#include <MaterialSerializer.h>
#include <MeshSerializer.h>
#include <BodySerializer.h>

namespace resource
{

Serializer* pFontSerializer = NULL;
Serializer* pTextureSerializer = NULL;
Serializer* pMaterialSerializer = NULL;
Serializer* pMeshSerializer = NULL;
Serializer* pBodySerializer = NULL;

extern "C" void RESOURCE_PUBLIC_EXPORT loadPlugin()
{	
	pFontSerializer = new FontSerializer();
	pTextureSerializer = new TextureSerializer();
	pMaterialSerializer = new MaterialSerializer();
	pMeshSerializer = new MeshSerializer();
	pBodySerializer = new BodySerializer();

	ResourceManager::getInstance().registerSerializer(RESOURCE_TYPE_FONT, pFontSerializer);
	ResourceManager::getInstance().registerSerializer(RESOURCE_TYPE_TEXTURE, pTextureSerializer);
	ResourceManager::getInstance().registerSerializer(RESOURCE_TYPE_RENDER_MATERIAL, pMaterialSerializer);
	ResourceManager::getInstance().registerSerializer(RESOURCE_TYPE_PHYSICS_MATERIAL, pMaterialSerializer);
	ResourceManager::getInstance().registerSerializer(RESOURCE_TYPE_MESH_DATA, pMeshSerializer);
	ResourceManager::getInstance().registerSerializer(RESOURCE_TYPE_BODY_DATA, pBodySerializer);
}

/// Destroys
extern "C" void RESOURCE_PUBLIC_EXPORT unloadPlugin()
{
	ResourceManager::getInstance().removeSerializer(RESOURCE_TYPE_FONT);
	ResourceManager::getInstance().removeSerializer(RESOURCE_TYPE_TEXTURE);
	ResourceManager::getInstance().removeSerializer(RESOURCE_TYPE_RENDER_MATERIAL);
	ResourceManager::getInstance().removeSerializer(RESOURCE_TYPE_PHYSICS_MATERIAL);
	ResourceManager::getInstance().removeSerializer(RESOURCE_TYPE_MESH_DATA);
	ResourceManager::getInstance().removeSerializer(RESOURCE_TYPE_BODY_DATA);

	if (pFontSerializer != NULL)
		delete pFontSerializer;
	if (pTextureSerializer != NULL)
		delete pTextureSerializer;
	if (pMaterialSerializer != NULL)
		delete pMaterialSerializer;
	if (pMeshSerializer != NULL)
		delete pMeshSerializer;
	if (pBodySerializer != NULL)
		delete pBodySerializer;
}

} // end namespace resource
