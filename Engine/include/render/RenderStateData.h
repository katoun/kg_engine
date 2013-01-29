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
#include <core/Vector2d.h>
#include <core/Vector3d.h>
#include <core/Vector4d.h>
#include <core/Matrix4.h>

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

	void setWorldMatrix(const core::matrix4& m);
	void setCurrentMaterial(Material* material);
	void setCurrentModel(Model* model);
	void setCurrentCamera(Camera* cam);
	void setCurrentViewport(Viewport* viewport);
	void setCurrentLight(Light* light);

	Material* getCurrentMaterial() const;
	Model* getCurrentModel() const;

	const core::vector3d& getCameraPosition();
	const core::vector3d& getCameraPositionObjectSpace();

	const core::vector3d& getCurrentLightPosition();
	const core::vector3d& getCurrentLightPositionObjectSpace();
	const core::vector3d& getCurrentLightPositionViewSpace();
	const core::vector3d& getCurrentLightDirection();
	const core::vector3d& getCurrentLightDirectionObjectSpace();
	const core::vector3d& getCurrentLightDirectionViewSpace();

	void setAmbientLightColor(const Color& ambient);
	const Color& getAmbientLightColour() const;

	const Color& getCurrentLightDiffuseColour() const;
	const Color& getCurrentLightSpecularColour() const;
	core::vector4d getCurrentLightAttenuation() const;

	const core::matrix4& getWorldMatrix();
	const core::matrix4& getViewMatrix();
	const core::matrix4& getProjectionMatrix();

	const core::matrix4& getWorldViewMatrix();
	const core::matrix4& getViewProjectionMatrix();
	const core::matrix4& getWorldViewProjMatrix();

	const core::matrix4& getInverseWorldMatrix();
	const core::matrix4& getInverseViewMatrix();
	core::matrix4 getInverseProjectionMatrix();

	const core::matrix4& getInverseWorldViewMatrix();
	core::matrix4 getInverseViewProjectionMatrix();
	core::matrix4 getInverseWorldViewProjMatrix();

	core::matrix4 getTransposedWorldMatrix();
	core::matrix4 getTransposedViewMatrix();
	core::matrix4 getTransposedProjectionMatrix();

	core::matrix4 getTransposedWorldViewMatrix();
	core::matrix4 getTransposedViewProjectionMatrix();
	core::matrix4 getTransposedWorldViewProjMatrix();

	const core::matrix4& getInverseTransposedWorldMatrix();
	core::matrix4 getInverseTransposedViewMatrix();
	core::matrix4 getInverseTransposedProjectionMatrix();

	const core::matrix4& getInverseTransposedWorldViewMatrix();
	core::matrix4 getInverseTransposedViewProjectionMatrix();
	core::matrix4 getInverseTransposedWorldViewProjMatrix();

protected:

	core::matrix4 mWorldMatrix;
	core::matrix4 mViewMatrix;
	core::matrix4 mProjectionMatrix;

	core::matrix4 mWorldViewMatrix;
	core::matrix4 mViewProjMatrix;
	core::matrix4 mWorldViewProjMatrix;
	
	core::matrix4 mInverseWorldMatrix;
	core::matrix4 mInverseViewMatrix;

	core::matrix4 mInverseWorldViewMatrix;
	
	core::matrix4 mInverseTransposeWorldMatrix;
	
	core::matrix4 mInverseTransposeWorldViewMatrix;

	core::vector3d mCameraPosition;
	core::vector3d mCameraPositionObjectSpace;

	core::vector3d mLightPosition;
	core::vector3d mLightPositionObjectSpace;
	core::vector3d mLightPositionViewSpace;

	core::vector3d mLightDirection;
	core::vector3d mLightDirectionObjectSpace;
	core::vector3d mLightDirectionViewSpace;

	Color mAmbientLightColor;
	Color mFogColor;

	bool mWorldMatrixDirty;
	bool mViewMatrixDirty;
	bool mProjMatrixDirty;

	bool mWorldViewMatrixDirty;
	bool mViewProjMatrixDirty;
	bool mWorldViewProjMatrixDirty;

	bool mInverseWorldMatrixDirty;
	bool mInverseViewMatrixDirty;
	
	bool mInverseWorldViewMatrixDirty;

	bool mInverseTransposeWorldMatrixDirty;
	bool mInverseTransposeWorldViewMatrixDirty;

	bool mCameraPositionDirty;
	bool mCameraPositionObjectSpaceDirty;

	Material* mCurrentMaterial;
	Model* mCurrentModel;
	Camera* mCurrentCamera;
	Light* mCurrentLight;
	Viewport* mCurrentViewport;
	
	std::list<Light*> mLights;
};

} // end namespace render

#endif