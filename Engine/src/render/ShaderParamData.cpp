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

#include <render/ShaderParamData.h>
#include <render/Light.h>
#include <render/Camera.h>
#include <render/Renderable.h>
#include <game/GameObject.h>
#include <game/ComponentDefines.h>
#include <game/Transform.h>

namespace render
{

ShaderParamData::ShaderParamData()
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

	mCurrentRenderable = NULL;
	mCurrentCamera = NULL;
	mCurrentLight = NULL;
	mCurrentViewport = NULL;

	mAmbientLightColor = Color::Black;
}

void ShaderParamData::setWorldMatrices(const core::matrix4& m)
{
	mWorldMatrix = m;
	mWorldMatrixDirty = true;
}

void ShaderParamData::setCurrentRenderable(Renderable* rend)
{
	mCurrentRenderable = rend;
	
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

void ShaderParamData::setCurrentCamera(Camera* cam)
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

void ShaderParamData::setCurrentViewport(Viewport* viewport)
{
	mCurrentViewport = viewport;
}

void ShaderParamData::setCurrentLight(Light* light)
{
	mCurrentLight = light;
}

const core::vector3d& ShaderParamData::getCameraPosition()
{
	if(mCameraPositionDirty)
	{
		if (mCurrentCamera != NULL && mCurrentCamera->getGameObject() != NULL)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mCurrentCamera->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != NULL)
			{
				mCameraPosition = pTransform->getAbsolutePosition();
			}
		}
		mCameraPositionDirty = false;
	}

	return mCameraPosition;
}

const core::vector3d& ShaderParamData::getCameraPositionObjectSpace()
{
	if(mCameraPositionObjectSpaceDirty)
	{
		if (mCurrentCamera != NULL && mCurrentCamera->getGameObject() != NULL)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mCurrentCamera->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != NULL)
			{
				mCameraPositionObjectSpace = pTransform->getAbsolutePosition();
				getInverseWorldMatrix().transformVector(mCameraPositionObjectSpace);
			}
		}
		mCameraPositionObjectSpaceDirty = false;
	}

	return mCameraPositionObjectSpace;
}

const core::vector3d& ShaderParamData::getCurrentLightPosition()
{
	if (mCurrentLight) mLightPosition = mCurrentLight->getAbsolutePosition();

	return mLightPosition;
}

const core::vector3d& ShaderParamData::getCurrentLightPositionObjectSpace()
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

const core::vector3d& ShaderParamData::getCurrentLightPositionViewSpace()
{
	mLightPositionViewSpace = getCurrentLightPosition();

	getViewMatrix().transformVector(mLightPositionViewSpace);

	return mLightPositionViewSpace;
}

const core::vector3d& ShaderParamData::getCurrentLightDirection()
{
	if (mCurrentLight != NULL)
		mLightDirection = mCurrentLight->getAbsoluteOrientation() * core::vector3d::NEGATIVE_UNIT_Z;

	return mLightDirection;
}

const core::vector3d& ShaderParamData::getCurrentLightDirectionObjectSpace()
{
	mLightDirectionObjectSpace = getCurrentLightDirection();

	getInverseTransposedWorldMatrix().transformVector(mLightDirectionObjectSpace);
	mLightDirectionObjectSpace.normalize();

	return mLightDirectionObjectSpace;
}

const core::vector3d& ShaderParamData::getCurrentLightDirectionViewSpace()
{
	mLightDirectionViewSpace = getCurrentLightDirection();

	getInverseTransposedViewMatrix().transformVector(mLightDirectionViewSpace);
	mLightDirectionViewSpace.normalize();

	return mLightDirectionViewSpace;
}

void ShaderParamData::setAmbientLightColor(const Color& ambient)
{
	mAmbientLightColor = ambient;
}

const Color& ShaderParamData::getAmbientLightColour() const
{
	return mAmbientLightColor;
}

const Color& ShaderParamData::getCurrentLightDiffuseColour() const
{
	if (mCurrentLight) return mCurrentLight->getDiffuseColor();

	return Color::White;
}

const Color& ShaderParamData::getCurrentLightSpecularColour() const
{
	if (mCurrentLight) return mCurrentLight->getSpecularColor();

	return Color::Black;
}

core::vector4d ShaderParamData::getCurrentLightAttenuation() const
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

const core::matrix4& ShaderParamData::getWorldMatrix()
{
	if (mWorldMatrixDirty)
	{
		if (mCurrentRenderable)	mWorldMatrix = mCurrentRenderable->getWorldMatrix();
		mWorldMatrixDirty = false;
	}

	return mWorldMatrix;
}

const core::matrix4& ShaderParamData::getViewMatrix()
{
	 if (mViewMatrixDirty)
	 {
		if (mCurrentCamera) mViewMatrix = mCurrentCamera->getViewMatrix();
		mViewMatrixDirty = false;
	 }

	return mViewMatrix;
}

const core::matrix4& ShaderParamData::getProjectionMatrix()
{
	if (mProjMatrixDirty)
	{
		if (mCurrentCamera) mProjectionMatrix = mCurrentCamera->getProjectionMatrixRS();
		mProjMatrixDirty = false;
	}
	
	return mProjectionMatrix;
}

const core::matrix4& ShaderParamData::getWorldViewMatrix()
{
	if (mWorldViewMatrixDirty)
	{
		mWorldViewMatrix = getViewMatrix() * getWorldMatrix();
		mWorldViewMatrixDirty = false;
	}

	return mWorldViewMatrix;
}

const core::matrix4& ShaderParamData::getViewProjectionMatrix()
{
	if (mProjMatrixDirty)
	{
		mViewProjMatrix = getProjectionMatrix() * getViewMatrix();
		mProjMatrixDirty = false;
	}
	
	return mViewProjMatrix;
}

const core::matrix4& ShaderParamData::getWorldViewProjMatrix()
{
	if (mWorldViewProjMatrixDirty)
	{
		mWorldViewProjMatrix = getProjectionMatrix() * getWorldViewMatrix();
		mWorldViewProjMatrixDirty = false;
	}

	return mWorldViewProjMatrix;
}

const core::matrix4& ShaderParamData::getInverseWorldMatrix()
{
	if (mInverseWorldMatrixDirty)
	{
		mInverseWorldMatrix = getWorldMatrix().getInverse();
		mInverseWorldMatrixDirty = false;
	}

	return mInverseWorldMatrix;
}

const core::matrix4& ShaderParamData::getInverseViewMatrix()
{
	if (mInverseViewMatrixDirty)
	{
		mInverseViewMatrix = getViewMatrix().getInverse();
		mInverseViewMatrixDirty = false;
	}
	return mInverseViewMatrix;
}

core::matrix4 ShaderParamData::getInverseProjectionMatrix()
{
	return getProjectionMatrix().getInverse();
}

const core::matrix4& ShaderParamData::getInverseWorldViewMatrix()
{
	if (mInverseWorldViewMatrixDirty)
	{
		mInverseWorldViewMatrix = getWorldViewMatrix().getInverse();
		mInverseWorldViewMatrixDirty = false;
	}
	return mInverseWorldViewMatrix;
}

core::matrix4 ShaderParamData::getInverseViewProjectionMatrix()
{
	return getViewProjectionMatrix().getInverse();
}

core::matrix4 ShaderParamData::getInverseWorldViewProjMatrix()
{
	return getWorldViewProjMatrix().getInverse();
}

core::matrix4 ShaderParamData::getTransposedWorldMatrix()
{
	return getWorldMatrix().getTransposed();
}

core::matrix4 ShaderParamData::getTransposedViewMatrix()
{
	return getViewMatrix().getTransposed();
}

core::matrix4 ShaderParamData::getTransposedProjectionMatrix()
{
	return getProjectionMatrix().getTransposed();
}

core::matrix4 ShaderParamData::getTransposedWorldViewMatrix()
{
	return getWorldViewMatrix().getTransposed();
}

core::matrix4 ShaderParamData::getTransposedViewProjectionMatrix()
{
	return getViewProjectionMatrix().getTransposed();
}

core::matrix4 ShaderParamData::getTransposedWorldViewProjMatrix()
{
	return getWorldViewProjMatrix().getTransposed();
}

const core::matrix4& ShaderParamData::getInverseTransposedWorldMatrix()
{
	if (mInverseTransposeWorldMatrixDirty)
	{
		mInverseTransposeWorldMatrix = getInverseWorldMatrix().getTransposed();
		mInverseTransposeWorldMatrixDirty = false;
	}

	return mInverseTransposeWorldMatrix;
}

core::matrix4 ShaderParamData::getInverseTransposedViewMatrix()
{
	return getInverseViewMatrix().getTransposed();
}

core::matrix4 ShaderParamData::getInverseTransposedProjectionMatrix()
{
	return getInverseProjectionMatrix().getTransposed();
}

const core::matrix4& ShaderParamData::getInverseTransposedWorldViewMatrix()
{
	if (mInverseTransposeWorldViewMatrixDirty)
	{
		mInverseTransposeWorldViewMatrix = getInverseWorldViewMatrix().getTransposed();
		mInverseTransposeWorldViewMatrixDirty = false;
	}

	return mInverseTransposeWorldViewMatrix;
}

core::matrix4 ShaderParamData::getInverseTransposedViewProjectionMatrix()
{
	return getInverseViewProjectionMatrix().getTransposed();
}

core::matrix4 ShaderParamData::getInverseTransposedWorldViewProjMatrix()
{
	return getInverseWorldViewProjMatrix().getTransposed();
}

} // end namespace render
