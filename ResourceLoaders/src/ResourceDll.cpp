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

#include <Config.h>
#include <resource/ResourceManager.h>
#include <resource/ResourceDefines.h>
#include <FontSerializer.h>
#include <TextureSerializer.h>
#include <MaterialSerializer.h>
#include <MeshSerializer.h>
#include <BodySerializer.h>
#include <SceneSerializer.h>

namespace resource
{

Serializer* pFontSerializer = nullptr;
Serializer* pTextureSerializer = nullptr;
Serializer* pMaterialSerializer = nullptr;
Serializer* pMeshSerializer = nullptr;
Serializer* pBodySerializer = nullptr;
Serializer* pSceneSerializer = nullptr;

extern "C" void RESOURCE_PUBLIC_EXPORT loadPlugin()
{	
	pFontSerializer = new FontSerializer();
	pTextureSerializer = new TextureSerializer();
	pMaterialSerializer = new MaterialSerializer();
	pMeshSerializer = new MeshSerializer();
	pBodySerializer = new BodySerializer();
	pSceneSerializer = new SceneSerializer();

	if (ResourceManager::getInstance() != nullptr)
	{
		ResourceManager::getInstance()->registerSerializer(RESOURCE_TYPE_FONT, pFontSerializer);
		ResourceManager::getInstance()->registerSerializer(RESOURCE_TYPE_TEXTURE, pTextureSerializer);
		ResourceManager::getInstance()->registerSerializer(RESOURCE_TYPE_RENDER_MATERIAL, pMaterialSerializer);
		ResourceManager::getInstance()->registerSerializer(RESOURCE_TYPE_PHYSICS_MATERIAL, pMaterialSerializer);
		ResourceManager::getInstance()->registerSerializer(RESOURCE_TYPE_MESH_DATA, pMeshSerializer);
		ResourceManager::getInstance()->registerSerializer(RESOURCE_TYPE_BODY_DATA, pBodySerializer);
		ResourceManager::getInstance()->registerSerializer(RESOURCE_TYPE_SCENE, pSceneSerializer);
	}
}

/// Destroys
extern "C" void RESOURCE_PUBLIC_EXPORT unloadPlugin()
{
	if (ResourceManager::getInstance() != nullptr)
	{
		ResourceManager::getInstance()->removeSerializer(RESOURCE_TYPE_FONT);
		ResourceManager::getInstance()->removeSerializer(RESOURCE_TYPE_TEXTURE);
		ResourceManager::getInstance()->removeSerializer(RESOURCE_TYPE_RENDER_MATERIAL);
		ResourceManager::getInstance()->removeSerializer(RESOURCE_TYPE_PHYSICS_MATERIAL);
		ResourceManager::getInstance()->removeSerializer(RESOURCE_TYPE_MESH_DATA);
		ResourceManager::getInstance()->removeSerializer(RESOURCE_TYPE_BODY_DATA);
		ResourceManager::getInstance()->removeSerializer(RESOURCE_TYPE_SCENE);
	}

	SAFE_DELETE(pFontSerializer);
	SAFE_DELETE(pTextureSerializer);
	SAFE_DELETE(pMaterialSerializer);
	SAFE_DELETE(pMeshSerializer);
	SAFE_DELETE(pBodySerializer);
	SAFE_DELETE(pSceneSerializer);
}

} // end namespace resource
