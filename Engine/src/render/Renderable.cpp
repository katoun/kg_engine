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
