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

#ifndef _SHADER_PARAM_DATA_H_
#define _SHADER_PARAM_DATA_H_

#include <core/Config.h>
#include <render/Color.h>
#include <core/Vector2d.h>
#include <core/Vector3d.h>
#include <core/Vector4d.h>
#include <core/Matrix4.h>

namespace render
{

class Light;
class Camera;
class Renderable;
class Viewport;

class ENGINE_PUBLIC_EXPORT ShaderParamData
{
public:

	ShaderParamData();

	void setWorldMatrices(const core::matrix4& m);
	void setCurrentRenderable(Renderable* rend);
	void setCurrentCamera(Camera* cam);
	void setCurrentViewport(Viewport* viewport);
	void setCurrentLight(Light* light);

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

	Renderable* mCurrentRenderable;
	Camera* mCurrentCamera;
	Light* mCurrentLight;
	Viewport* mCurrentViewport;
	
	std::list<Light*> mLights;
};

} // end namespace render

#endif