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
	mWorldMatrix = glm::mat4x4(1);
	mInverseWorldMatrix = glm::mat4x4(1);
	mInverseTransposeWorldMatrix = glm::mat4x4(1);
	mViewMatrix = glm::mat4x4(1);
	mInverseViewMatrix = glm::mat4x4(1);
	mProjectionMatrix = glm::mat4x4(1);
	mViewProjMatrix = glm::mat4x4(1);
	mWorldViewMatrix = glm::mat4x4(1);
	mInverseWorldViewMatrix = glm::mat4x4(1);
	mInverseTransposeWorldViewMatrix = glm::mat4x4(1);
	mWorldViewProjMatrix = glm::mat4x4(1);

	mCameraPosition = glm::vec3(0, 0, 0);
	mCameraPositionObjectSpace = glm::vec3(0, 0, 0);

	mLightPosition = glm::vec3(0, 0, 0);
	mLightPositionObjectSpace = glm::vec3(0, 0, 0);
	mLightPositionViewSpace = glm::vec3(0, 0, 0);

	mLightDirection = glm::vec3(0, 0, 0);
	mLightDirectionObjectSpace = glm::vec3(0, 0, 0);
	mLightDirectionViewSpace = glm::vec3(0, 0, 0);

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

void RenderStateData::setWorldMatrix(const glm::mat4x4& m)
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

const glm::vec3& RenderStateData::getCameraPosition()
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

const glm::vec3& RenderStateData::getCameraPositionObjectSpace()
{
	if(mCameraPositionObjectSpaceDirty)
	{
		if (mCurrentCamera != nullptr && mCurrentCamera->getGameObject() != nullptr)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mCurrentCamera->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != nullptr)
			{
				mCameraPositionObjectSpace = pTransform->getAbsolutePosition();
				glm::vec4 pos = getInverseWorldMatrix() * glm::vec4(mCameraPositionObjectSpace, 1);
				mCameraPositionObjectSpace.x = pos.x;
				mCameraPositionObjectSpace.y = pos.y;
				mCameraPositionObjectSpace.z = pos.z;
			}
		}
		mCameraPositionObjectSpaceDirty = false;
	}

	return mCameraPositionObjectSpace;
}

const glm::vec3& RenderStateData::getCurrentLightPosition()
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

const glm::vec3& RenderStateData::getCurrentLightPositionObjectSpace()
{
	mLightPositionObjectSpace = getCurrentLightPosition();

	if (mCurrentLight && mCurrentLight->getLightType() == LIGHT_TYPE_DIRECTIONAL)
	{
		glm::mat4x4 m = glm::inverse(getInverseTransposedWorldMatrix());

		glm::vec4 pos = m * glm::vec4(mLightPositionObjectSpace, 1);
		mLightPositionObjectSpace.x = pos.x;
		mLightPositionObjectSpace.y = pos.y;
		mLightPositionObjectSpace.z = pos.z;

		mLightPositionObjectSpace = glm::normalize(mLightPositionObjectSpace);
	}
	else
	{
		glm::vec4 pos = getInverseWorldMatrix() * glm::vec4(mLightPositionObjectSpace, 1);
		mLightPositionObjectSpace.x = pos.x;
		mLightPositionObjectSpace.y = pos.y;
		mLightPositionObjectSpace.z = pos.z;
	}

	return mLightPositionObjectSpace;
}

const glm::vec3& RenderStateData::getCurrentLightPositionViewSpace()
{
	mLightPositionViewSpace = getCurrentLightPosition();

	glm::vec4 dir = getViewMatrix() * glm::vec4(mLightPositionViewSpace, 1);
	mLightPositionViewSpace.x = dir.x;
	mLightPositionViewSpace.y = dir.y;
	mLightPositionViewSpace.z = dir.z;

	return mLightPositionViewSpace;
}

const glm::vec3& RenderStateData::getCurrentLightDirection()
{
	if (mCurrentLight != nullptr)
		

	if (mCurrentLight != nullptr && mCurrentLight->getGameObject() != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mCurrentLight->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			mLightDirection = pTransform->getAbsoluteOrientation() * glm::vec3(0, 0, 1);
		}
	}

	return mLightDirection;
}

const glm::vec3& RenderStateData::getCurrentLightDirectionObjectSpace()
{
	mLightDirectionObjectSpace = getCurrentLightDirection();

	glm::vec4 dir = getInverseTransposedWorldMatrix() * glm::vec4(mLightDirectionObjectSpace, 1);
	mLightDirectionObjectSpace.x = dir.x;
	mLightDirectionObjectSpace.y = dir.y;
	mLightDirectionObjectSpace.z = dir.z;

	mLightDirectionObjectSpace = glm::normalize(mLightDirectionObjectSpace);

	return mLightDirectionObjectSpace;
}

const glm::vec3& RenderStateData::getCurrentLightDirectionViewSpace()
{
	mLightDirectionViewSpace = getCurrentLightDirection();

	glm::vec4 dir = getInverseTransposedViewMatrix() * glm::vec4(mLightDirectionViewSpace, 1);
	mLightDirectionViewSpace.x = dir.x;
	mLightDirectionViewSpace.y = dir.y;
	mLightDirectionViewSpace.z = dir.z;

	mLightDirectionViewSpace = glm::normalize(mLightDirectionViewSpace);

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

glm::vec4 RenderStateData::getCurrentLightAttenuation() const
{
	glm::vec4 lightAttenuation(1000.0f, 1.0f, 0.0f, 0.0f);

	if (mCurrentLight != nullptr)
	{
		lightAttenuation.x = mCurrentLight->getAttenuationRange();
		lightAttenuation.y = mCurrentLight->getAttenuationConstant();
		lightAttenuation.z = mCurrentLight->getAttenuationLinear();
		lightAttenuation.w = mCurrentLight->getAttenuationQuadric();
	}

	return lightAttenuation;
}

const glm::mat4x4& RenderStateData::getWorldMatrix()
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

const glm::mat4x4& RenderStateData::getViewMatrix()
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

const glm::mat4x4& RenderStateData::getProjectionMatrix()
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

const glm::mat4x4& RenderStateData::getWorldViewMatrix()
{
	if (mWorldViewMatrixDirty)
	{
		mWorldViewMatrix = getViewMatrix() * getWorldMatrix();
		mWorldViewMatrixDirty = false;
	}

	return mWorldViewMatrix;
}

const glm::mat4x4& RenderStateData::getViewProjectionMatrix()
{
	if (mProjMatrixDirty)
	{
		mViewProjMatrix = getProjectionMatrix() * getViewMatrix();
		mProjMatrixDirty = false;
	}
	
	return mViewProjMatrix;
}

const glm::mat4x4& RenderStateData::getWorldViewProjMatrix()
{
	if (mWorldViewProjMatrixDirty)
	{
		mWorldViewProjMatrix = getProjectionMatrix() * getWorldViewMatrix();
		mWorldViewProjMatrixDirty = false;
	}

	return mWorldViewProjMatrix;
}

const glm::mat4x4& RenderStateData::getInverseWorldMatrix()
{
	if (mInverseWorldMatrixDirty)
	{
		mInverseWorldMatrix = glm::inverse(getWorldMatrix());
		mInverseWorldMatrixDirty = false;
	}

	return mInverseWorldMatrix;
}

const glm::mat4x4& RenderStateData::getInverseViewMatrix()
{
	if (mInverseViewMatrixDirty)
	{
		mInverseViewMatrix = glm::inverse(getViewMatrix());
		mInverseViewMatrixDirty = false;
	}
	return mInverseViewMatrix;
}

glm::mat4x4 RenderStateData::getInverseProjectionMatrix()
{
	return glm::inverse(getProjectionMatrix());
}

const glm::mat4x4& RenderStateData::getInverseWorldViewMatrix()
{
	if (mInverseWorldViewMatrixDirty)
	{
		mInverseWorldViewMatrix = glm::inverse(getWorldViewMatrix());
		mInverseWorldViewMatrixDirty = false;
	}
	return mInverseWorldViewMatrix;
}

glm::mat4x4 RenderStateData::getInverseViewProjectionMatrix()
{
	return glm::inverse(getViewProjectionMatrix());
}

glm::mat4x4 RenderStateData::getInverseWorldViewProjMatrix()
{
	return glm::inverse(getWorldViewProjMatrix());
}

glm::mat4x4 RenderStateData::getTransposedWorldMatrix()
{
	return glm::transpose(getWorldMatrix());
}

glm::mat4x4 RenderStateData::getTransposedViewMatrix()
{
	return glm::transpose(getViewMatrix());
}

glm::mat4x4 RenderStateData::getTransposedProjectionMatrix()
{
	return glm::transpose(getProjectionMatrix());
}

glm::mat4x4 RenderStateData::getTransposedWorldViewMatrix()
{
	return glm::transpose(getWorldViewMatrix());
}

glm::mat4x4 RenderStateData::getTransposedViewProjectionMatrix()
{
	return glm::transpose(getViewProjectionMatrix());
}

glm::mat4x4 RenderStateData::getTransposedWorldViewProjMatrix()
{
	return glm::transpose(getWorldViewProjMatrix());
}

const glm::mat4x4& RenderStateData::getInverseTransposedWorldMatrix()
{
	if (mInverseTransposeWorldMatrixDirty)
	{
		mInverseTransposeWorldMatrix = glm::transpose(getInverseWorldMatrix());
		mInverseTransposeWorldMatrixDirty = false;
	}

	return mInverseTransposeWorldMatrix;
}

glm::mat4x4 RenderStateData::getInverseTransposedViewMatrix()
{
	return glm::transpose(getInverseViewMatrix());
}

glm::mat4x4 RenderStateData::getInverseTransposedProjectionMatrix()
{
	return glm::transpose(getInverseProjectionMatrix());
}

const glm::mat4x4& RenderStateData::getInverseTransposedWorldViewMatrix()
{
	if (mInverseTransposeWorldViewMatrixDirty)
	{
		mInverseTransposeWorldViewMatrix = glm::transpose(getInverseWorldViewMatrix());
		mInverseTransposeWorldViewMatrixDirty = false;
	}

	return mInverseTransposeWorldViewMatrix;
}

glm::mat4x4 RenderStateData::getInverseTransposedViewProjectionMatrix()
{
	return glm::transpose(getInverseViewProjectionMatrix());
}

glm::mat4x4 RenderStateData::getInverseTransposedWorldViewProjMatrix()
{
	return glm::transpose(getInverseWorldViewProjMatrix());
}

} // end namespace render
