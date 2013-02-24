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

#ifndef _SHADER_PARAM_DATA_H_
#define _SHADER_PARAM_DATA_H_

#include <EngineConfig.h>
#include <render/Color.h>

#include <glm/glm.hpp>

namespace render
{

class Light;
class Camera;
class Model;
class Material;
class Viewport;

class ENGINE_PUBLIC_EXPORT RenderStateData
{
public:

	RenderStateData();

	void setCurrentMaterial(Material* material);
	void setCurrentModel(Model* model);
	void setCurrentCamera(Camera* cam);
	void setCurrentViewport(Viewport* viewport);
	void setCurrentLight(Light* light);

	Material* getCurrentMaterial();
	Model* getCurrentModel();

	glm::mat4x4 getModelMatrix();
	glm::mat4x4 getViewMatrix();
	glm::mat4x4 getProjectionMatrix();

	const glm::mat4x4& getModelViewMatrix();
	const glm::mat4x4& getViewProjectionMatrix();
	const glm::mat4x4& getModelViewProjectionMatrix();

	Color getMaterialAmbientColour();
	Color getMaterialDiffuseColour();
	Color getMaterialSpecularColour();
	float getMaterialShininess();

	glm::vec3 getLightPosition();
	Color getLightAmbientColour();
	Color getLightDiffuseColour();
	Color getLightSpecularColour();
	glm::vec4 getLightAttenuation();

	glm::vec3 getCameraPosition();

protected:

	glm::mat4x4 mModelViewMatrix;
	glm::mat4x4 mViewProjMatrix;
	glm::mat4x4 mModelViewProjMatrix;

	bool mModelViewMatrixDirty;
	bool mViewProjMatrixDirty;
	bool mModelViewProjMatrixDirty;

	Material* mCurrentMaterial;
	Model* mCurrentModel;
	Camera* mCurrentCamera;
	Light* mCurrentLight;
	Viewport* mCurrentViewport;
	
	std::list<Light*> mLights;//TODO!!!
};

} // end namespace render

#endif