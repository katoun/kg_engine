/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2013 Catalin Alexandru Nastase

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

#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_

#include <core/Config.h>
#include <core/Aabox3d.h>
#include <resource/Resource.h>
#include <render/Color.h>
#include <render/RenderDefines.h>
#include <render/VertexBufferDefines.h>

#include <string>
#include <map>

namespace resource
{
class Serializer;
enum BufferUsage;
}

namespace render
{

class Model;
class Material;
class VertexBuffer;
class IndexBuffer;

//! Defines a mesh resource.
class ENGINE_PUBLIC_EXPORT MeshData: public resource::Resource
{
public:

	MeshData(const std::string& name, resource::Serializer* serializer);
	~MeshData();

	//! Gets render operation type.
	const RenderOperationType& getRenderOperationType();

	//! Gets the vertex buffer by type for this Mesh.
	VertexBuffer* getVertexBuffer(VertexBufferType type);

	//! Sets new vertex buffer by type for this Mesh.
	void setVertexBuffer(VertexBufferType type, VertexBuffer* buffer);

	//! Gets the index buffer for this Mesh.
	IndexBuffer* getIndexBuffer();

	//! Sets new index buffer for this Mesh.
	void setIndexBuffer(IndexBuffer* buffer);

	//! Sets the policy for the vertex buffers to be used when loading this Mesh.
	//! You can define the approach to a Mesh by changing the default parameters to 
	//! MeshManager::load if you wish; this means the Mesh is loaded with those options
	//! the first time.
	//! \param usage: The usage flags, which by default are resource::BU_STATIC_WRITE_ONLY.
	void setVertexBufferPolicy(resource::BufferUsage usage);

	//! Gets the vertex buffer usage for this Mesh.
	resource::BufferUsage& getVertexBufferUsage();

	//! Sets the policy for the index buffers to be used when loading this Mesh.
	void setIndexBufferPolicy(resource::BufferUsage usage);

	//! Gets the index buffer usage for this Mesh.
	resource::BufferUsage& getIndexBufferUsage();

	//! Sets the name of the material which this mesh will use.
	void setMaterial(const std::string& filename);

	//! Sets the material which this mesh will use.
	Material* getMaterial() const;

	//! Sets the axis-aligned bounding box for this mesh.
	void setBoundingBox(const core::aabox3d& box);

	//! Gets the axis-aligned bounding box for this mesh.
	const core::aabox3d& getBoundingBox();

	//! Sets the radius of the bounding sphere surrounding this mesh.
	void setBoundingSphereRadius(float radius);

	//! Gets the radius of the bounding sphere surrounding this mesh.
	float getBoundingSphereRadius();

private:

	void unloadImpl();

	//! The material this mesh uses.
	Material* mMaterial;

	//! The render operation type used to render this mesh.
	RenderOperationType mRenderOperationType;

	resource::BufferUsage mVertexBufferUsage;
	resource::BufferUsage mIndexBufferUsage;

	//! Dedicated vertex data.
	std::map<VertexBufferType, VertexBuffer*> mVertexBuffers;

	//! Face index buffer.
	IndexBuffer* mIndexBuffer;

	//! Local bounding box volume.
	core::aabox3d mAABB;

	//! Local bounding sphere radius (centered on object).
	float mBoundRadius;
};

} //namespace render

#endif