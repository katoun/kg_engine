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

#include <render/PanelOverlay.h>
#include <render/VertexBuffer.h>
#include <render/IndexBuffer.h>
#include <render/Material.h>
#include <render/RenderManager.h>
#include <resource/ResourceManager.h>

namespace render
{

// vertex buffer bindings, set at compile time
#define POSITION_BINDING 0
#define TEXCOORD_BINDING 1

PanelOverlay::PanelOverlay(): Overlay()
{
	initialize();
}

PanelOverlay::PanelOverlay(const std::string& name): Overlay(name)
{
	initialize();
}

PanelOverlay::~PanelOverlay() {}

void PanelOverlay::setMaterial(const std::string& filename)
{
	std::string materialname = filename;
	mMaterial = static_cast<Material*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_RENDER_MATERIAL, materialname));
}

void PanelOverlay::setMaterial(Material* material)
{
	if (material == NULL)
		return;

	mMaterial = material;
}

void PanelOverlay::initializeImpl()
{
	//	0-----2
	//	|    /|
	//	|  /  |
	//	|/    |
	//	1-----3

	mRenderOperationType = ROT_TRIANGLE_LIST;

	mVertexData = new VertexData();
	// Basic vertex data
	mVertexData->vertexStart = 0;
	mVertexData->vertexCount = 4;

	VertexDeclaration* decl = mVertexData->vertexDeclaration;
	decl->addElement(POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION);
	decl->addElement(TEXCOORD_BINDING, 0, VET_FLOAT2, VES_TEXTURE_COORDINATES);

	// Vertex buffer #1, position
	VertexBuffer* vbuf1 = RenderManager::getInstance().createVertexBuffer(decl->getVertexSize(POSITION_BINDING), mVertexData->vertexCount, resource::BU_STATIC_WRITE_ONLY);
	// bind position
	mVertexData->vertexBufferBinding->setBinding(POSITION_BINDING, vbuf1);

	// Vertex buffer #2, texcoords
	VertexBuffer* vbuf2 = RenderManager::getInstance().createVertexBuffer(decl->getVertexSize(TEXCOORD_BINDING), mVertexData->vertexCount, resource::BU_STATIC_WRITE_ONLY);
	// bind texcoord
	mVertexData->vertexBufferBinding->setBinding(TEXCOORD_BINDING, vbuf2);

	mIndexData = new IndexData();
	// Basic index data
	mIndexData->indexStart = 0;
	mIndexData->indexCount = 6;

	// Index buffer
	IndexBuffer* ibuf = RenderManager::getInstance().createIndexBuffer(IT_32BIT, mIndexData->indexCount, resource::BU_STATIC_WRITE_ONLY);
	mIndexData->indexBuffer = ibuf;
	unsigned int* idx = (unsigned int*)(ibuf->lock(resource::BL_DISCARD));

	idx[0] = 0;
	idx[1] = 1;
	idx[2] = 2;

	idx[3] = 1;
	idx[4] = 3;
	idx[5] = 2;

	ibuf->unlock();

	mGeometryInitialized = true;
}

void PanelOverlay::updatePositionBinding()
{
	//	0-----2
	//	|    /|
	//	|  /  |
	//	|/    |
	//	1-----3
	float left, right, top, bottom;

	// Convert positions into -1, 1 coordinate space (homogenous clip space).
	left = -mWidth;
	right = mWidth;
	top = mHeight;
	bottom = -mHeight;

	// Use the furthest away depth value, since materials should have depth-check off
	// This initialized the depth buffer for any 3D objects in front
	float zValue = RenderManager::getInstance().getMaximumDepthInputValue();

	VertexBuffer* vbuf = mVertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);
	float* pos = (float*)(vbuf->lock(resource::BL_DISCARD));

	pos[ 0] = left;		//x
	pos[ 1] = top;		//y
	pos[ 2] = 0;		//z

	pos[ 3] = left;		//x
	pos[ 4] = bottom;	//y
	pos[ 5] = 0;		//z

	pos[ 6] = right;	//x
	pos[ 7] = top;		//y
	pos[ 8] = 0;		//z

	pos[ 9] = right;	//x
	pos[10] = bottom;	//y
	pos[11] = 0;		//z

	vbuf->unlock();
}

void PanelOverlay::updateTextureBinding()
{
	//	0-----2
	//	|    /|
	//	|  /  |
	//	|/    |
	//	1-----3

	VertexBuffer* vbuf = mVertexData->vertexBufferBinding->getBuffer(TEXCOORD_BINDING);
	float* tex = (float*)(vbuf->lock(resource::BL_DISCARD));

	tex[0] = mU1;	//u
	tex[1] = mV1;	//v

	tex[2] = mU1;	//u
	tex[3] = mV2;	//v

	tex[4] = mU2;	//u
	tex[5] = mV1;	//v

	tex[6] = mU2;	//u
	tex[7] = mV2;	//v

	vbuf->unlock();
}

} //namespace render
