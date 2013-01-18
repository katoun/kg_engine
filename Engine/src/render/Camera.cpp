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

#include <render/Camera.h>
#include <render/CameraDefines.h>
#include <render/Frustum.h>
#include <render/RenderManager.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <game/MessageDefines.h>
#include <core/Math.h>
#include <core/Utils.h>

namespace render
{

Camera::Camera(): game::Component()
{
	mType = game::COMPONENT_TYPE_CAMERA;
	
	mFixedUp = true;
	mFixedUpAxis = core::vector3d::UNIT_Y;

	// Reasonable defaults to camera params
	mFOV = 45.0f;
	mAspect = 1.33333333333333f;
	mNearDist = 1.0f;
	mFarDist = 1000.0f;

	mFrustum = new Frustum();

	mProjType = PROJECTION_TYPE_PERSPECTIVE;
	mSceneDetail = SCENE_DETAIL_LEVEL_TEXTURED;

	mVisibleFrustum = false;

	// Init matrices
	mViewMatrix = core::matrix4::ZERO;
	mProjMatrix = core::matrix4::ZERO;
	mProjMatrixRS = core::matrix4::ZERO;

	mProjectionNeedsUpdate = true;
	mViewNeedsUpdate = true;
	mFrustumNeedsUpdate = true;
}

Camera::~Camera()
{
	if (mFrustum != nullptr)
	{
		delete mFrustum;
	}
}

void Camera::setProjectionType(ProjectionType pt)
{
	mProjType = pt;
	mProjectionNeedsUpdate = true;
}

ProjectionType Camera::getProjectionType()
{
	return mProjType;
}

void Camera::setDetailLevel(SceneDetailLevel sd)
{
	mSceneDetail = sd;
}

SceneDetailLevel Camera::getDetailLevel()
{
	return mSceneDetail;
}

bool Camera::getVisibleFrustum()
{
	return mVisibleFrustum;
}

void Camera::setVisibleFrustum(bool visible)
{
	mVisibleFrustum = visible;
}

bool Camera::getFixedUp()
{
	return mFixedUp;
}

const core::vector3d& Camera::getFixedUpAxis()
{
	return mFixedUpAxis;
}

void Camera::setFixedUpAxis(bool useFixed, const core::vector3d& fixedAxis)
{
	mFixedUp = useFixed;
	mFixedUpAxis = fixedAxis;
}

void Camera::setFOV(float fov)
{
	mFOV = fov;
	mProjectionNeedsUpdate = true;
}

float Camera::getFOV()
{
	return mFOV;
}

void Camera::setNearClipDistance(float nearPlane)
{
	mNearDist = nearPlane;
	mProjectionNeedsUpdate = true;
}

float Camera::getNearClipDistance()
{
	return mNearDist;
}

void Camera::setFarClipDistance(float farPlane)
{
	mFarDist = farPlane;
}

float Camera::getFarClipDistance()
{
	return mFarDist;
}

void Camera::setAspectRatio(float r)
{
	mAspect = r;
	mProjectionNeedsUpdate = true;
}

float Camera::getAspectRatio()
{
	return mAspect;
}

const core::matrix4& Camera::getProjectionMatrixRS()
{
	return mProjMatrixRS;
}

const core::matrix4& Camera::getProjectionMatrix()
{
	return mProjMatrix;
}

const core::matrix4& Camera::getViewMatrix()
{
	return mViewMatrix;
}

Frustum* Camera::getFrustum() const
{
	return mFrustum;
}

void Camera::updateImpl(float elapsedTime)
{
	updateProjection();
	updateView();
	updateFrustum();
}

void Camera::onMessageImpl(unsigned int messageID)
{
	if (messageID == game::MESSAGE_TRANSFORM_NEEDS_UPDATE)
	{
		mViewNeedsUpdate = true;
	}
}

void Camera::updateProjection()
{
	if(mProjectionNeedsUpdate)
	{
		if(mProjType == PROJECTION_TYPE_PERSPECTIVE)
		{
			// PERSPECTIVE transform
			if (mFarDist != 0)
				mProjMatrix.buildProjectionMatrixPerspectiveFov(mFOV, mAspect, mNearDist, mFarDist);
			else
				mProjMatrix.buildProjectionMatrixInfinitePerspectiveFov(mFOV, mAspect, mNearDist);
		}
		else if(mProjType == PROJECTION_TYPE_ORTHOGRAPHIC)
		{
			// ORTHOGRAPHIC projection

			//----------------------------
			// Matrix elements
			//----------------------------

			// [ 2/wV	0		0		0		]
			// [ 0		2/hV	0		0		]
			// [ 0		0		1/(n-f)	n/(n-f)	]
			// [ 0		0		0		1		]

			// Get tangent of vertical FOV
			float thetaY = (mFOV / 2.0f)*core::DEGTORAD;
			float sinThetaY = core::sin(thetaY);
			float thetaX = thetaY * mAspect;
			float sinThetaX = core::sin(thetaX);

			float width = 2.0f * (sinThetaX * mNearDist);
			float height = 2.0f * (sinThetaY * mNearDist);

			mProjMatrix.buildProjectionMatrixOrtho(width, height, mNearDist, mFarDist);
		}
		
		if (RenderManager::getInstance() != nullptr)
			RenderManager::getInstance()->convertProjectionMatrix(mProjMatrix, mProjMatrixRS);

		mProjectionNeedsUpdate = false;
		mFrustumNeedsUpdate = true;
	}
}

void Camera::updateView()
{
	if(mViewNeedsUpdate)
	{
		// ----------------------
		// Update the view matrix
		// ----------------------

		if (mGameObject != nullptr)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != nullptr)
			{
				core::vector3d pos = pTransform->getAbsolutePosition();
				core::vector3d target = pos + (pTransform->getAbsoluteOrientation() * core::vector3d::NEGATIVE_UNIT_Z);
				core::vector3d up = pTransform->getAbsoluteOrientation() * core::vector3d::UNIT_Y;

				mViewMatrix.buildViewMatrix(pos, target, up);
			}
		}
		
#ifdef _DEBUG
		//std::cout<<"Pos: "<<pos<<std::endl;
		//std::cout<<"Right: "<<(mOrientation * core::vector3d::UNIT_X)<<std::endl;
		//std::cout<<"Target: "<<(target)<<std::endl;
		//std::cout<<"Up: "<<(up)<<std::endl;
#endif

		mViewNeedsUpdate = false;
		mFrustumNeedsUpdate = true;
	}
}

void Camera::updateFrustum()
{
	if (mFrustumNeedsUpdate)
	{
		mFrustum->buildViewFrustum(mProjMatrix, mViewMatrix, mProjType, mFOV, mAspect, mNearDist, mFarDist);

		mFrustumNeedsUpdate = false;
	}
}

} //namespace render
