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

#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include <core/Config.h>
#include <core/Matrix4.h>
#include <core/Aabox3d.h>
#include <core/Sphere3d.h>
#include <render/RenderableDefines.h>
#include <render/VertexIndexData.h>
#include <scene/Node.h>

#include <string>

namespace render
{

class Material;

//! Representation of a renderable in the render world.
class ENGINE_PUBLIC_EXPORT Renderable: public scene::Node
{
public:

	Renderable();
	Renderable(const std::string& name);

	virtual ~Renderable();

	//! Gets renderable type.
	const RenderableType& getRenderableType() const;

	bool getVisibleBoundingBox();
	void setVisibleBoundingBox(bool visible);

	bool getVisibleBoundingSphere();
	void setVisibleBoundingSphere(bool visible);

	virtual Material* getMaterial() const;

	const core::matrix4& getWorldMatrix();

	const core::aabox3d& getBoundingBox();

	const core::sphere3d& getBoundingSphere();

	const RenderOperationType& getRenderOperationType();

	VertexData* getVertexData();

	IndexData* getIndexData();

protected:

	// Incremented count for next Index
	static unsigned int msNextGeneratedRenderableIndex;

	RenderableType mRenderableType;

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

	//! Vertex source data.
	VertexData* mVertexData;

	//! Index data - only valid if useIndexes is true.
	IndexData* mIndexData;
};

} //namespace render

#endif