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
