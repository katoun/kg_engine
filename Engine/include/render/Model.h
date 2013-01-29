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

#ifndef _MODEL_H_
#define _MODEL_H_

#include <EngineConfig.h>
#include <core/Matrix4.h>
#include <core/Aabox3d.h>
#include <core/Sphere3d.h>
#include <render/Color.h>
#include <game/Component.h>
#include <resource/ResourceEventReceiver.h>

#include <string>

namespace resource
{
struct ResourceEvent;
}

namespace render
{

class VertexBuffer;
class IndexBuffer;
class Material;
class MeshData;
class MeshProperties;
enum RenderOperationType;
enum VertexBufferType;

//! Representation of a model in the render world.
class ENGINE_PUBLIC_EXPORT Model: public game::Component, public resource::ResourceEventReceiver
{
public:

	Model();
	~Model();

	//! Sets the mesh data this mesh will use.
	void setMeshData(const std::string& filename);
	void setMeshData(MeshData* meshData);

	MeshData* getMeshData() const;

	//! Sets the name of the material which this mesh will use.
	void setMaterial(const std::string& filename);
	void setMaterial(Material* material);
	
	//! Gets the material which this mesh uses.
	Material* getMaterial() const;

	bool getVisibleBoundingBox();
	void setVisibleBoundingBox(bool visible);

	bool getVisibleBoundingSphere();
	void setVisibleBoundingSphere(bool visible);

	const core::matrix4& getWorldMatrix();

	const core::aabox3d& getBoundingBox();

	const core::sphere3d& getBoundingSphere();

	const RenderOperationType& getRenderOperationType();

	VertexBuffer* getVertexBuffer(VertexBufferType type);

	IndexBuffer* getIndexBuffer();

	void resourceLoaded(const resource::ResourceEvent& evt);
	void resourceUnloaded(const resource::ResourceEvent& evt);

protected:

	void updateImpl(float elapsedTime);
	void onMessageImpl(unsigned int messageID);

	MeshData* mMeshData;

	Material* mMaterial;

	bool mVisibleBoundingBox;
	bool mVisibleBoundingSphere;

	// world matrix
	core::matrix4 mWorldMatrix;

	//! Bounding box that 'contains' this object.
	core::aabox3d mBoundingBox;

	//! Bounding sphere that 'contains' this object.
	core::sphere3d mBoundingSphere;

	//! The type of rendering operation.
	RenderOperationType mRenderOperationType;

	bool mModelNeedsUpdate;

	void updateWorldTransform();
};

} //namespace render

#endif