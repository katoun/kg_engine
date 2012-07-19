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
