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

#include <render/Renderable.h>
#include <core/Utils.h>

namespace render
{

unsigned int Renderable::msNextGeneratedRenderableIndex = 0;

Renderable::Renderable(): scene::Node(mName = "Renderable_" + core::intToString(msNextGeneratedRenderableIndex++))
{
	mNodeType = scene::NT_RENDERABLE;

	mRenderableType = RENDERABLE_TYPE_UNDEFINED;

	mVisibleBoundingBox = false;
	mVisibleBoundingSphere = false;

	mWorldMatrix = core::matrix4::IDENTITY;

	mRenderOperationType = ROT_TRIANGLE_LIST;
	mVertexData = NULL;
	mIndexData = NULL;
}

Renderable::Renderable(const std::string& name): scene::Node(name)
{
	mNodeType = scene::NT_RENDERABLE;

	mRenderableType = RENDERABLE_TYPE_UNDEFINED;

	mVisibleBoundingBox = false;
	mVisibleBoundingSphere = false;

	mWorldMatrix = core::matrix4::IDENTITY;

	mRenderOperationType = ROT_TRIANGLE_LIST;
	mVertexData = NULL;
	mIndexData = NULL;
}

Renderable::~Renderable() {}

const RenderableType& Renderable::getRenderableType() const
{
	return mRenderableType;
}

bool Renderable::getVisibleBoundingBox()
{
	return mVisibleBoundingBox;
}

void Renderable::setVisibleBoundingBox(bool visible)
{
	mVisibleBoundingBox = visible;
}

bool Renderable::getVisibleBoundingSphere()
{
	return mVisibleBoundingSphere;
}

void Renderable::setVisibleBoundingSphere(bool visible)
{
	mVisibleBoundingSphere = visible;
}

Material* Renderable::getMaterial() const
{
	return NULL;
}

const core::matrix4& Renderable::getWorldMatrix()
{
	return mWorldMatrix;
}

const core::aabox3d& Renderable::getBoundingBox()
{
	return mBoundingBox;
}

const core::sphere3d& Renderable::getBoundingSphere()
{
	return mBoundingSphere;
}

const RenderOperationType& Renderable::getRenderOperationType()
{
	return mRenderOperationType;
}

VertexData* Renderable::getVertexData()
{
	return mVertexData;
}

IndexData* Renderable::getIndexData()
{
	return mIndexData;
}

} //namespace render
