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

#include <render/MeshData.h>
#include <render/VertexIndexData.h>
#include <render/Material.h>
#include <resource/Buffer.h>
#include <resource/ResourceManager.h>
#include <platform/PlatformManager.h>
#include <core/Math.h>

namespace render
{

MeshData::MeshData(const std::string& name, resource::Serializer* serializer): resource::Resource(name, serializer)
{
	mResourceType = resource::RESOURCE_TYPE_MESH_DATA;

	mMaterial = NULL;

	mRenderOperationType = ROT_TRIANGLE_LIST;

	mVertexData = NULL;
	mIndexData = NULL;
	mVertexBufferUsage = resource::BU_STATIC_WRITE_ONLY;
	mIndexBufferUsage = resource::BU_STATIC_WRITE_ONLY;

	mAABB = core::aabox3d();
	mBoundRadius = 0.0f;
}

MeshData::~MeshData() {}

const RenderOperationType& MeshData::getRenderOperationType()
{
	return mRenderOperationType;
}

void MeshData::setNewVertexData()
{
	mVertexData = new VertexData();
}

VertexData* MeshData::getVertexData()
{
	return mVertexData;
}

void MeshData::setNewIndexData()
{
	mIndexData = new IndexData();
}

IndexData* MeshData::getIndexData()
{
	return mIndexData;
}

void MeshData::setVertexBufferPolicy(resource::BufferUsage usage)
{
	mVertexBufferUsage = usage;
}

resource::BufferUsage& MeshData::getVertexBufferUsage()
{
	return mVertexBufferUsage;
}

void MeshData::setIndexBufferPolicy(resource::BufferUsage usage)
{
	mIndexBufferUsage = usage;
}

resource::BufferUsage& MeshData::getIndexBufferUsage()
{
	return mIndexBufferUsage;
}

void MeshData::setMaterial(const std::string& filename)
{
	std::string materialname = filename;
	mMaterial = static_cast<Material*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_RENDER_MATERIAL, materialname));
}

Material* MeshData::getMaterial() const
{
	return mMaterial;
}

void MeshData::setBoundingBox(const core::aabox3d& box)
{
	mAABB = box;
}

const core::aabox3d& MeshData::getBoundingBox()
{
	return mAABB;
}

void MeshData::setBoundingSphereRadius(float radius)
{
	mBoundRadius = radius;
}

float MeshData::getBoundingSphereRadius()
{
	return mBoundRadius;
}

void MeshData::unloadImpl()
{
	if (mVertexData)
		delete mVertexData;

	if (mIndexData)
		delete mIndexData;

	mMaterial = NULL;

	mRenderOperationType = ROT_TRIANGLE_LIST;

	mVertexData = NULL;
	mIndexData = NULL;
	mVertexBufferUsage = resource::BU_STATIC_WRITE_ONLY;
	mIndexBufferUsage = resource::BU_STATIC_WRITE_ONLY;

	mAABB = core::aabox3d();
	mBoundRadius = 0.0f;
}

} //namespace render
