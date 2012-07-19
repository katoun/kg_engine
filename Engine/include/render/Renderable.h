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