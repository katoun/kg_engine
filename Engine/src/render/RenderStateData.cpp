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
	mModelViewMatrix = glm::mat4x4(1);
	mViewProjMatrix = glm::mat4x4(1);
	mModelViewProjMatrix = glm::mat4x4(1);

	mInverseModelMatrix = glm::mat4x4(1);
	mInverseViewMatrix = glm::mat4x4(1);
	mInverseProjectionMatrix = glm::mat4x4(1);

	mInverseModelViewMatrix = glm::mat4x4(1);
	mInverseViewProjectionMatrix = glm::mat4x4(1);
	mInverseModelViewProjectionMatrix = glm::mat4x4(1);
	
	mModelViewMatrixDirty = true;
	mViewProjMatrixDirty = true;
	mModelViewProjMatrixDirty = true;

	mInverseModelMatrixDirty = true;
	mInverseViewMatrixDirty = true;
	mInverseProjectionMatrixDirty = true;

	mInverseModelViewMatrixDirty = true;
	mInverseViewProjectionMatrixDirty = true;
	mInverseModelViewProjectionMatrixDirty = true;

	mCameraPosition = glm::vec3(0, 0, 0);
	mCameraPositionObjectSpace = glm::vec3(0, 0, 0);

	mCameraPositionDirty = true;
	mCameraPositionObjectSpaceDirty = true;

	mLightPosition = glm::vec3(0, 0, 0);
	mLightPositionObjectSpace = glm::vec3(0, 0, 0);
	mLightPositionViewSpace = glm::vec3(0, 0, 0);

	mLightDirection = glm::vec3(0, 0, 0);
	mLightDirectionObjectSpace = glm::vec3(0, 0, 0);
	mLightDirectionViewSpace = glm::vec3(0, 0, 0);

	mCurrentModel = nullptr;
	mCurrentCamera = nullptr;
	mCurrentLight = nullptr;
	mCurrentViewport = nullptr;

	mAmbientLightColor = Color::Black;
}

void RenderStateData::setCurrentMaterial(Material* material)
{
	mCurrentMaterial = material;
}

void RenderStateData::setCurrentModel(Model* model)
{
	mCurrentModel = model;

	mModelViewMatrixDirty = true;
	mModelViewProjMatrixDirty = true;

	mInverseModelMatrixDirty = true;

	mInverseModelViewMatrixDirty = true;
	mInverseModelViewProjectionMatrixDirty = true;
}

void RenderStateData::setCurrentCamera(Camera* cam)
{
	mCurrentCamera = cam;

	mCameraPositionDirty = true;
	mCameraPositionObjectSpaceDirty = true;

	mModelViewMatrixDirty = true;
	mViewProjMatrixDirty = true;
	mModelViewProjMatrixDirty = true;

	mInverseViewMatrixDirty = true;
	mInverseProjectionMatrixDirty = true;

	mInverseModelViewMatrixDirty = true;
	mInverseViewProjectionMatrixDirty = true;
	mInverseModelViewProjectionMatrixDirty = true;
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
				glm::vec4 pos = getInverseModelMatrix() * glm::vec4(mCameraPositionObjectSpace, 1);
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
		glm::mat4x4 m = glm::transpose(getModelMatrix());

		glm::vec4 pos = m * glm::vec4(mLightPositionObjectSpace, 1);
		mLightPositionObjectSpace.x = pos.x;
		mLightPositionObjectSpace.y = pos.y;
		mLightPositionObjectSpace.z = pos.z;

		mLightPositionObjectSpace = glm::normalize(mLightPositionObjectSpace);
	}
	else
	{
		glm::vec4 pos = getInverseModelMatrix() * glm::vec4(mLightPositionObjectSpace, 1);
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

	glm::vec4 dir = glm::transpose(getInverseModelMatrix()) * glm::vec4(mLightDirectionObjectSpace, 1);
	mLightDirectionObjectSpace.x = dir.x;
	mLightDirectionObjectSpace.y = dir.y;
	mLightDirectionObjectSpace.z = dir.z;

	mLightDirectionObjectSpace = glm::normalize(mLightDirectionObjectSpace);

	return mLightDirectionObjectSpace;
}

const glm::vec3& RenderStateData::getCurrentLightDirectionViewSpace()
{
	mLightDirectionViewSpace = getCurrentLightDirection();

	glm::vec4 dir = glm::transpose(getInverseViewMatrix()) * glm::vec4(mLightDirectionViewSpace, 1);
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

glm::mat4x4 RenderStateData::getModelMatrix()
{
	if (mCurrentModel != nullptr)
	{
		return mCurrentModel->getWorldMatrix();
	}

	return glm::mat4x4(1);
}

glm::mat4x4 RenderStateData::getViewMatrix()
{
	if (mCurrentCamera != nullptr)
	{
		return mCurrentCamera->getViewMatrix();
	}

	return glm::mat4x4(1);
}

glm::mat4x4 RenderStateData::getProjectionMatrix()
{
	if (mCurrentCamera != nullptr)
	{
		return mCurrentCamera->getProjectionMatrix();
	}

	return glm::mat4x4(1);
}

const glm::mat4x4& RenderStateData::getModelViewMatrix()
{
	if (mModelViewMatrixDirty)
	{
		mModelViewMatrix = getViewMatrix() * getModelMatrix();
		mModelViewMatrixDirty = false;
	}

	return mModelViewMatrix;
}

const glm::mat4x4& RenderStateData::getViewProjectionMatrix()
{
	return getProjectionMatrix() * getViewMatrix();
}

const glm::mat4x4& RenderStateData::getModelViewProjectionMatrix()
{
	if (mModelViewProjMatrixDirty)
	{
		mModelViewProjMatrix = getProjectionMatrix() * getModelViewMatrix();
		mModelViewProjMatrixDirty = false;
	}

	return mModelViewProjMatrix;
}

const glm::mat4x4& RenderStateData::getInverseModelMatrix()
{
	if (mInverseModelMatrixDirty)
	{
		mInverseModelMatrix = glm::inverse(getModelMatrix());
		mInverseModelMatrixDirty = false;
	}

	return mInverseModelMatrix;
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

const glm::mat4x4& RenderStateData::getInverseProjectionMatrix()
{
	if (mInverseProjectionMatrixDirty)
	{
		mInverseProjectionMatrix = glm::inverse(getProjectionMatrix());
		mInverseProjectionMatrixDirty = false;
	}

	return mInverseProjectionMatrix;
}

const glm::mat4x4& RenderStateData::getInverseModelViewMatrix()
{
	if (mInverseModelViewMatrixDirty)
	{
		mInverseModelViewMatrix = glm::inverse(getModelViewMatrix());
		mInverseModelViewMatrixDirty = false;
	}
	return mInverseModelViewMatrix;
}

const glm::mat4x4& RenderStateData::getInverseViewProjectionMatrix()
{
	if (mInverseViewProjectionMatrixDirty)
	{
		mInverseViewProjectionMatrix = glm::inverse(getViewProjectionMatrix());
		mInverseViewProjectionMatrixDirty = false;
	}
	return mInverseViewProjectionMatrix;
}

const glm::mat4x4& RenderStateData::getInverseModelViewProjectionMatrix()
{
	if (mInverseModelViewProjectionMatrixDirty)
	{
		mInverseModelViewProjectionMatrix = glm::inverse(getModelViewProjectionMatrix());
		mInverseModelViewProjectionMatrixDirty = false;
	}
	return mInverseModelViewProjectionMatrix;
}

} // end namespace render
