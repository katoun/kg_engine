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

#include <render/RenderStateData.h>
#include <render/Light.h>
#include <render/Camera.h>
#include <render/Model.h>
#include <game/GameObject.h>
#include <game/ComponentDefines.h>
#include <game/Transform.h>

namespace render
{

RenderStateData::RenderStateData()
{
	mWorldMatrix = core::matrix4::IDENTITY;
	mInverseWorldMatrix = core::matrix4::IDENTITY;
	mInverseTransposeWorldMatrix = core::matrix4::IDENTITY;
	mViewMatrix = core::matrix4::IDENTITY;
	mInverseViewMatrix = core::matrix4::IDENTITY;
	mProjectionMatrix = core::matrix4::IDENTITY;
	mViewProjMatrix = core::matrix4::IDENTITY;
	mWorldViewMatrix = core::matrix4::IDENTITY;
	mInverseWorldViewMatrix = core::matrix4::IDENTITY;
	mInverseTransposeWorldViewMatrix = core::matrix4::IDENTITY;
	mWorldViewProjMatrix = core::matrix4::IDENTITY;

	mCameraPosition = core::vector3d::ORIGIN_3D;
	mCameraPositionObjectSpace = core::vector3d::ORIGIN_3D;

	mLightPosition = core::vector3d::ORIGIN_3D;
	mLightPositionObjectSpace = core::vector3d::ORIGIN_3D;
	mLightPositionViewSpace = core::vector3d::ORIGIN_3D;

	mLightDirection = core::vector3d::ORIGIN_3D;
	mLightDirectionObjectSpace = core::vector3d::ORIGIN_3D;
	mLightDirectionViewSpace = core::vector3d::ORIGIN_3D;

	mWorldMatrixDirty = true;
	mViewMatrixDirty = true;
	mProjMatrixDirty = true;

	mWorldViewMatrixDirty = true;
	mViewProjMatrixDirty = true;
	mWorldViewProjMatrixDirty = true;

	mInverseWorldMatrixDirty = true;
	mInverseViewMatrixDirty = true;

	mInverseWorldViewMatrixDirty = true;

	mInverseTransposeWorldMatrixDirty = true;
	mInverseTransposeWorldViewMatrixDirty = true;

	mCameraPositionDirty = true;
	mCameraPositionObjectSpaceDirty = true;

	mCurrentModel = nullptr;
	mCurrentCamera = nullptr;
	mCurrentLight = nullptr;
	mCurrentViewport = nullptr;

	mAmbientLightColor = Color::Black;
}

void RenderStateData::setWorldMatrix(const core::matrix4& m)
{
	mWorldMatrix = m;
	mWorldMatrixDirty = true;
}

void RenderStateData::setCurrentMaterial(Material* material)
{
	mCurrentMaterial = material;
}

void RenderStateData::setCurrentModel(Model* model)
{
	mCurrentModel = model;
	
	mWorldMatrixDirty = true;
	mViewMatrixDirty = true;
	mProjMatrixDirty = true;

	mWorldViewMatrixDirty = true;
	mViewProjMatrixDirty = true;
	mWorldViewProjMatrixDirty = true;

	mInverseWorldMatrixDirty = true;
	mInverseViewMatrixDirty = true;

	mInverseWorldViewMatrixDirty = true;

	mInverseTransposeWorldMatrixDirty = true;
	mInverseTransposeWorldViewMatrixDirty = true;
}

void RenderStateData::setCurrentCamera(Camera* cam)
{
	mCurrentCamera = cam;

	mCameraPositionDirty = true;
	mCameraPositionObjectSpaceDirty = true;

	mViewMatrixDirty = true;
	mProjMatrixDirty = true;

	mWorldViewMatrixDirty = true;
	mViewProjMatrixDirty = true;
	mWorldViewProjMatrixDirty = true;

	mInverseViewMatrixDirty = true;
	
	mInverseWorldViewMatrixDirty = true;
	
	mInverseTransposeWorldViewMatrixDirty = true;
}

void RenderStateData::setCurrentViewport(Viewport* viewport)
{
	mCurrentViewport = viewport;
}

void RenderStateData::setCurrentLight(Light* light)
{
	mCurrentLight = light;
}

Material* RenderStateData::getCurrentMaterial() const
{
	return mCurrentMaterial;
}

Model* RenderStateData::getCurrentModel() const
{
	return mCurrentModel;
}

const core::vector3d& RenderStateData::getCameraPosition()
{
	if(mCameraPositionDirty)
	{
		if (mCurrentCamera != nullptr && mCurrentCamera->getGameObject() != nullptr)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mCurrentCamera->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != nullptr)
			{
				mCameraPosition = pTransform->getAbsolutePosition();
			}
		}
		mCameraPositionDirty = false;
	}

	return mCameraPosition;
}

const core::vector3d& RenderStateData::getCameraPositionObjectSpace()
{
	if(mCameraPositionObjectSpaceDirty)
	{
		if (mCurrentCamera != nullptr && mCurrentCamera->getGameObject() != nullptr)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mCurrentCamera->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != nullptr)
			{
				mCameraPositionObjectSpace = pTransform->getAbsolutePosition();
				getInverseWorldMatrix().transformVector(mCameraPositionObjectSpace);
			}
		}
		mCameraPositionObjectSpaceDirty = false;
	}

	return mCameraPositionObjectSpace;
}

const core::vector3d& RenderStateData::getCurrentLightPosition()
{
	if (mCurrentLight != nullptr && mCurrentLight->getGameObject() != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mCurrentLight->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			mLightPosition = pTransform->getAbsolutePosition();
		}
	}

	return mLightPosition;
}

const core::vector3d& RenderStateData::getCurrentLightPositionObjectSpace()
{
	mLightPositionObjectSpace = getCurrentLightPosition();

	if (mCurrentLight && mCurrentLight->getLightType() == LIGHT_TYPE_DIRECTIONAL)
	{
		core::matrix4 m = getInverseTransposedWorldMatrix().getInverse();
			
		m.transformVector(mLightPositionObjectSpace);
		mLightPositionObjectSpace.normalize();
	}
	else
	{
		getInverseWorldMatrix().transformVector(mLightPositionObjectSpace);
	}

	return mLightPositionObjectSpace;
}

const core::vector3d& RenderStateData::getCurrentLightPositionViewSpace()
{
	mLightPositionViewSpace = getCurrentLightPosition();

	getViewMatrix().transformVector(mLightPositionViewSpace);

	return mLightPositionViewSpace;
}

const core::vector3d& RenderStateData::getCurrentLightDirection()
{
	if (mCurrentLight != nullptr)
		

	if (mCurrentLight != nullptr && mCurrentLight->getGameObject() != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mCurrentLight->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			mLightDirection = pTransform->getAbsoluteOrientation() * core::vector3d::NEGATIVE_UNIT_Z;
		}
	}

	return mLightDirection;
}

const core::vector3d& RenderStateData::getCurrentLightDirectionObjectSpace()
{
	mLightDirectionObjectSpace = getCurrentLightDirection();

	getInverseTransposedWorldMatrix().transformVector(mLightDirectionObjectSpace);
	mLightDirectionObjectSpace.normalize();

	return mLightDirectionObjectSpace;
}

const core::vector3d& RenderStateData::getCurrentLightDirectionViewSpace()
{
	mLightDirectionViewSpace = getCurrentLightDirection();

	getInverseTransposedViewMatrix().transformVector(mLightDirectionViewSpace);
	mLightDirectionViewSpace.normalize();

	return mLightDirectionViewSpace;
}

void RenderStateData::setAmbientLightColor(const Color& ambient)
{
	mAmbientLightColor = ambient;
}

const Color& RenderStateData::getAmbientLightColour() const
{
	return mAmbientLightColor;
}

const Color& RenderStateData::getCurrentLightDiffuseColour() const
{
	if (mCurrentLight) return mCurrentLight->getDiffuseColor();

	return Color::White;
}

const Color& RenderStateData::getCurrentLightSpecularColour() const
{
	if (mCurrentLight) return mCurrentLight->getSpecularColor();

	return Color::Black;
}

core::vector4d RenderStateData::getCurrentLightAttenuation() const
{
	core::vector4d lightAttenuation(1000.0f, 1.0f, 0.0f, 0.0f);

	if (mCurrentLight)
	{
		core::vector4d lightAttenuation;
		lightAttenuation.X = mCurrentLight->getAttenuationRange();
		lightAttenuation.Y = mCurrentLight->getAttenuationConstant();
		lightAttenuation.Z = mCurrentLight->getAttenuationLinear();
		lightAttenuation.W = mCurrentLight->getAttenuationQuadric();
	}

	return lightAttenuation;
}

const core::matrix4& RenderStateData::getWorldMatrix()
{
	if (mWorldMatrixDirty)
	{
		if (mCurrentModel != nullptr)
		{
			mWorldMatrix = mCurrentModel->getWorldMatrix();
		}
		mWorldMatrixDirty = false;
	}

	return mWorldMatrix;
}

const core::matrix4& RenderStateData::getViewMatrix()
{
	 if (mViewMatrixDirty)
	 {
		if (mCurrentCamera != nullptr)
		{
			mViewMatrix = mCurrentCamera->getViewMatrix();
		}
		mViewMatrixDirty = false;
	 }

	return mViewMatrix;
}

const core::matrix4& RenderStateData::getProjectionMatrix()
{
	if (mProjMatrixDirty)
	{
		if (mCurrentCamera != nullptr)
		{
			mProjectionMatrix = mCurrentCamera->getProjectionMatrix();
		}
		mProjMatrixDirty = false;
	}
	
	return mProjectionMatrix;
}

const core::matrix4& RenderStateData::getWorldViewMatrix()
{
	if (mWorldViewMatrixDirty)
	{
		mWorldViewMatrix = getViewMatrix() * getWorldMatrix();
		mWorldViewMatrixDirty = false;
	}

	return mWorldViewMatrix;
}

const core::matrix4& RenderStateData::getViewProjectionMatrix()
{
	if (mProjMatrixDirty)
	{
		mViewProjMatrix = getProjectionMatrix() * getViewMatrix();
		mProjMatrixDirty = false;
	}
	
	return mViewProjMatrix;
}

const core::matrix4& RenderStateData::getWorldViewProjMatrix()
{
	if (mWorldViewProjMatrixDirty)
	{
		mWorldViewProjMatrix = getProjectionMatrix() * getWorldViewMatrix();
		mWorldViewProjMatrixDirty = false;
	}

	return mWorldViewProjMatrix;
}

const core::matrix4& RenderStateData::getInverseWorldMatrix()
{
	if (mInverseWorldMatrixDirty)
	{
		mInverseWorldMatrix = getWorldMatrix().getInverse();
		mInverseWorldMatrixDirty = false;
	}

	return mInverseWorldMatrix;
}

const core::matrix4& RenderStateData::getInverseViewMatrix()
{
	if (mInverseViewMatrixDirty)
	{
		mInverseViewMatrix = getViewMatrix().getInverse();
		mInverseViewMatrixDirty = false;
	}
	return mInverseViewMatrix;
}

core::matrix4 RenderStateData::getInverseProjectionMatrix()
{
	return getProjectionMatrix().getInverse();
}

const core::matrix4& RenderStateData::getInverseWorldViewMatrix()
{
	if (mInverseWorldViewMatrixDirty)
	{
		mInverseWorldViewMatrix = getWorldViewMatrix().getInverse();
		mInverseWorldViewMatrixDirty = false;
	}
	return mInverseWorldViewMatrix;
}

core::matrix4 RenderStateData::getInverseViewProjectionMatrix()
{
	return getViewProjectionMatrix().getInverse();
}

core::matrix4 RenderStateData::getInverseWorldViewProjMatrix()
{
	return getWorldViewProjMatrix().getInverse();
}

core::matrix4 RenderStateData::getTransposedWorldMatrix()
{
	return getWorldMatrix().getTransposed();
}

core::matrix4 RenderStateData::getTransposedViewMatrix()
{
	return getViewMatrix().getTransposed();
}

core::matrix4 RenderStateData::getTransposedProjectionMatrix()
{
	return getProjectionMatrix().getTransposed();
}

core::matrix4 RenderStateData::getTransposedWorldViewMatrix()
{
	return getWorldViewMatrix().getTransposed();
}

core::matrix4 RenderStateData::getTransposedViewProjectionMatrix()
{
	return getViewProjectionMatrix().getTransposed();
}

core::matrix4 RenderStateData::getTransposedWorldViewProjMatrix()
{
	return getWorldViewProjMatrix().getTransposed();
}

const core::matrix4& RenderStateData::getInverseTransposedWorldMatrix()
{
	if (mInverseTransposeWorldMatrixDirty)
	{
		mInverseTransposeWorldMatrix = getInverseWorldMatrix().getTransposed();
		mInverseTransposeWorldMatrixDirty = false;
	}

	return mInverseTransposeWorldMatrix;
}

core::matrix4 RenderStateData::getInverseTransposedViewMatrix()
{
	return getInverseViewMatrix().getTransposed();
}

core::matrix4 RenderStateData::getInverseTransposedProjectionMatrix()
{
	return getInverseProjectionMatrix().getTransposed();
}

const core::matrix4& RenderStateData::getInverseTransposedWorldViewMatrix()
{
	if (mInverseTransposeWorldViewMatrixDirty)
	{
		mInverseTransposeWorldViewMatrix = getInverseWorldViewMatrix().getTransposed();
		mInverseTransposeWorldViewMatrixDirty = false;
	}

	return mInverseTransposeWorldViewMatrix;
}

core::matrix4 RenderStateData::getInverseTransposedViewProjectionMatrix()
{
	return getInverseViewProjectionMatrix().getTransposed();
}

core::matrix4 RenderStateData::getInverseTransposedWorldViewProjMatrix()
{
	return getInverseWorldViewProjMatrix().getTransposed();
}

} // end namespace render
