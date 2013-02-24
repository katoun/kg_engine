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
#include <render/Material.h>
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
	
	mModelViewMatrixDirty = true;
	mViewProjMatrixDirty = true;
	mModelViewProjMatrixDirty = true;

	mCurrentModel = nullptr;
	mCurrentCamera = nullptr;
	mCurrentLight = nullptr;
	mCurrentViewport = nullptr;
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
}

void RenderStateData::setCurrentCamera(Camera* cam)
{
	mCurrentCamera = cam;

	mModelViewMatrixDirty = true;
	mViewProjMatrixDirty = true;
	mModelViewProjMatrixDirty = true;
}

void RenderStateData::setCurrentViewport(Viewport* viewport)
{
	mCurrentViewport = viewport;
}

void RenderStateData::setCurrentLight(Light* light)
{
	mCurrentLight = light;
}

Material* RenderStateData::getCurrentMaterial()
{
	return mCurrentMaterial;
}

Model* RenderStateData::getCurrentModel()
{
	return mCurrentModel;
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

glm::vec4 RenderStateData::getMaterialAmbientColour()
{
	if (mCurrentMaterial == nullptr)
		return glm::vec4(1, 1, 1, 1);

	return mCurrentMaterial->getAmbientColor();
}

glm::vec4 RenderStateData::getMaterialDiffuseColour()
{
	if (mCurrentMaterial == nullptr)
		return glm::vec4(1, 1, 1, 1);

	return mCurrentMaterial->getDiffuseColor();
}

glm::vec4 RenderStateData::getMaterialSpecularColour()
{
	if (mCurrentMaterial == nullptr)
		return glm::vec4(0, 0, 0, 1);

	return mCurrentMaterial->getSpecularColor();
}

float RenderStateData::getMaterialShininess()
{
	if (mCurrentMaterial == nullptr)
		return 0;

	return mCurrentMaterial->getShininess();
}

glm::vec3 RenderStateData::getLightPosition()
{
	if (mCurrentLight != nullptr && mCurrentLight->getGameObject() != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mCurrentLight->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			return pTransform->getAbsolutePosition();
		}
	}

	return glm::vec3(0);
}

glm::vec4 RenderStateData::getLightAmbientColour()
{
	if (mCurrentLight == nullptr)
		return glm::vec4(1, 1, 1, 1);

	return mCurrentLight->getAmbientColor();
}

glm::vec4 RenderStateData::getLightDiffuseColour()
{
	if (mCurrentLight == nullptr)
		return glm::vec4(1, 1, 1, 1);

	return mCurrentLight->getDiffuseColor();
}

glm::vec4 RenderStateData::getLightSpecularColour()
{
	if (mCurrentLight == nullptr)
		return glm::vec4(0, 0, 0, 1);

	return mCurrentLight->getSpecularColor();
}

glm::vec4 RenderStateData::getLightAttenuation()
{
	glm::vec4 lightAttenuation(1000, 1, 0, 0);

	if (mCurrentLight != nullptr)
	{
		lightAttenuation.x = mCurrentLight->getAttenuationRange();
		lightAttenuation.y = mCurrentLight->getAttenuationConstant();
		lightAttenuation.z = mCurrentLight->getAttenuationLinear();
		lightAttenuation.w = mCurrentLight->getAttenuationQuadric();
	}

	return lightAttenuation;
}

glm::vec3 RenderStateData::getCameraPosition()
{
	if (mCurrentCamera != nullptr && mCurrentCamera->getGameObject() != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mCurrentCamera->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			return pTransform->getAbsolutePosition();
		}
	}

	return glm::vec3(0);
}

} // end namespace render
