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

#include <render/Overlay.h>
#include <render/Material.h>
#include <render/VertexIndexData.h>
#include <render/RenderManager.h>
#include <resource/ResourceManager.h>
#include <core/Utils.h>

namespace render
{

unsigned int Overlay::msNextGeneratedOverlayIndex = 0;

Overlay::Overlay(): Renderable("Overlay_" + core::intToString(msNextGeneratedOverlayIndex++))
{
	mRenderableType = RENDERABLE_TYPE_OVERLAY;

	mIsVisible = true;
	mMaterial = NULL;

	mWidth = 1.0f;
	mHeight = 1.0f;

	mMetricsMode = MM_RELATIVE;

	mU1 = 0.0f;
	mV1 = 0.0f;
	mU2 = 1.0f;
	mV2 = 1.0f;

	mGeometryInitialized = false;
	mGeomPositionsOutOfDate = true;
	mGeomUVsOutOfDate = true;

	// Init matrix
	mWorldMatrix = core::matrix4::IDENTITY;

	mModifiedWorldTransform = true;
}

Overlay::Overlay(const std::string& name): Renderable(name)
{
	mRenderableType = RENDERABLE_TYPE_OVERLAY;

	mIsVisible = true;
	mMaterial = NULL;

	mWidth = 1.0f;
	mHeight = 1.0f;

	mMetricsMode = MM_RELATIVE;

	mU1 = 0.0f;
	mV1 = 0.0f;
	mU2 = 1.0f;
	mV2 = 1.0f;

	mGeometryInitialized = false;
	mGeomPositionsOutOfDate = true;
	mGeomUVsOutOfDate = true;

	// Init matrix
	mWorldMatrix = core::matrix4::IDENTITY;

	mModifiedWorldTransform = true;
}

Overlay::~Overlay()
{
	if (mVertexData != NULL) delete mVertexData;
	if (mIndexData != NULL)	delete mIndexData;
}

void Overlay::setVisible(bool enabled)
{
	mIsVisible = enabled;
}

bool Overlay::isVisible()
{
	return mIsVisible;
}

void Overlay::setPosition(float x, float y)
{
	mPosition.X = x;
	mPosition.Y = y;
	
	mModifiedAbsoluteTransform = true;
}

void Overlay::setDimension(float width, float height)
{
	mWidth = width;
	mHeight = height;
	
	mGeomPositionsOutOfDate = true;
}

float Overlay::getWidth()
{
	return mWidth;
}

float Overlay::getHeight()
{
	return mHeight;
}


void Overlay::setMetricsMode(MetricsMode mm)
{
	mMetricsMode = mm;
}

MetricsMode Overlay::getMetricsMode() const
{
	return mMetricsMode;
}

Material* Overlay::getMaterial() const
{
	return mMaterial;
}

void Overlay::updateImpl(float elapsedTime)
{
	mModifiedWorldTransform = mModifiedAbsoluteTransform;

	Node::updateImpl(elapsedTime);

	updateWorldTransform();

	// Tell self to update own position geometry
	if (mGeometryInitialized && mGeomPositionsOutOfDate)
		updatePositionBinding();
		
	// Tell self to update own texture geometry
	if (mGeometryInitialized && mGeomUVsOutOfDate)
		updateTextureBinding();

	mGeomPositionsOutOfDate = false;
	mGeomUVsOutOfDate = false;

	mModifiedWorldTransform = false;
}

void Overlay::updateWorldTransform()
{
	if(mModifiedWorldTransform)
	{
		updateTransformImpl();

		mWorldMatrix = core::matrix4::IDENTITY;
		// Ordering:
		//    1. Scale
		//    2. Rotate
		//    3. Translate

		// Own scale is applied before rotation
		core::matrix4 rot = mAbsoluteOrientation.toRotationMatrix();
		core::matrix4 scale;
		scale.setScale(mAbsoluteScale);
		mWorldMatrix = rot * scale;
		mWorldMatrix.setTranslation(mAbsolutePosition);

#ifdef _DEBUG
		//std::cout<<"Pos: "<<mPosition<<std::endl;
		//std::cout<<"Orientation: "<<mOrientation<<std::endl;
		//std::cout<<"Scale: "<<mScale<<std::endl;
#endif
	}
}

} //namespace render
