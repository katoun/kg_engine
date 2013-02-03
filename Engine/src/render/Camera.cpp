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

#include <glm/gtc/matrix_transform.hpp>

namespace render
{

Camera::Camera(): game::Component()
{
	mType = game::COMPONENT_TYPE_CAMERA;
	
	mFixedUp = true;
	mFixedUpAxis = glm::vec3(0, 1, 0);

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
	mViewMatrix = glm::mat4x4(1);
	mProjMatrix = glm::mat4x4(1);

	mProjectionNeedsUpdate = true;
	mViewNeedsUpdate = true;
	mFrustumNeedsUpdate = true;
}

Camera::~Camera()
{
	SAFE_DELETE(mFrustum);
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

const glm::vec3& Camera::getFixedUpAxis()
{
	return mFixedUpAxis;
}

void Camera::setFixedUpAxis(bool useFixed, const glm::vec3& fixedAxis)
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

const glm::mat4x4& Camera::getProjectionMatrix()
{
	return mProjMatrix;
}

const glm::mat4x4& Camera::getViewMatrix()
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
			/*if (mFarDist != 0)
			{
				mProjMatrix = glm::perspective(mFOV, mAspect, mNearDist, mFarDist);
			}
			else
			{
				//mProjMatrix = glm::infinitePerspective(mFOV, mAspect, mNearDist);
				mProjMatrix = glm::mat4x4(1);

				float h = (float)tan(mFOV / 2);
				float w = h * mAspect;
	
				mProjMatrix[0][0] = 2.0f * mNearDist / w;
				mProjMatrix[1][0] = 0.0f;
				mProjMatrix[2][0] = 0.0f;
				mProjMatrix[3][0] = 0.0f;

				mProjMatrix[0][1] = 0.0f;
				mProjMatrix[1][1] = 2.0f * mNearDist / h;
				mProjMatrix[2][1] = 0.0f;
				mProjMatrix[3][1] = 0.0f;

				mProjMatrix[0][2] = 0.0f;
				mProjMatrix[1][2] = 0.0f;
				mProjMatrix[2][2] = -1.0f;
				mProjMatrix[3][2] = -2.0f * mNearDist;

				mProjMatrix[0][3] = 0.0f;
				mProjMatrix[1][3] = 0.0f;
				mProjMatrix[2][3] = -1.0f;
				mProjMatrix[3][3] = 0.0f;
			}*/

			mProjMatrix = glm::perspective(mFOV, mAspect, 0.1f, 100.0f);

			mProjMatrix = glm::mat4x4(1);
		}
		else if(mProjType == PROJECTION_TYPE_ORTHOGRAPHIC)
		{
			// ORTHOGRAPHIC projection

			// Get tangent of vertical FOV
			float thetaY = (mFOV / 2.0f)*core::DEGTORAD;
			float sinThetaY = core::sin(thetaY);
			float thetaX = thetaY * mAspect;
			float sinThetaX = core::sin(thetaX);

			float width = 2.0f * (sinThetaX * mNearDist);
			float height = 2.0f * (sinThetaY * mNearDist);

			mProjMatrix = glm::ortho(0.0f, width, 0.0f, height, mNearDist, mFarDist);
		}
		
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
				/*glm::vec3 pos = pTransform->getAbsolutePosition();
				glm::vec3 target = pos + (pTransform->getAbsoluteOrientation() * glm::vec3(0, 0, 1));
				glm::vec3 up = pTransform->getAbsoluteOrientation() * glm::vec3(0, 1, 0);*/

				//mViewMatrix = glm::lookAt(pos, target, up);
				/*mViewMatrix = glm::mat4x4(1);

				glm::vec3 zaxis = glm::normalize(pos - target);

				glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));

				glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));

				glm::vec3 trans;

				trans.x = - glm::dot(xaxis, pos);
				trans.y = - glm::dot(yaxis, pos);
				trans.z = - glm::dot(zaxis, pos);

				mViewMatrix[0][0] = xaxis.x;
				mViewMatrix[1][0] = xaxis.y;
				mViewMatrix[2][0] = xaxis.z;
				mViewMatrix[3][0] = trans.x;

				mViewMatrix[0][1] = yaxis.x;
				mViewMatrix[1][1] = yaxis.y;
				mViewMatrix[2][1] = yaxis.z;
				mViewMatrix[3][1] = trans.y;

				mViewMatrix[0][2] = zaxis.x;
				mViewMatrix[1][2] = zaxis.y;
				mViewMatrix[2][2] = zaxis.z;
				mViewMatrix[3][2] = trans.z;

				mViewMatrix[0][3] = 0.0f;
				mViewMatrix[1][3] = 0.0f;
				mViewMatrix[2][3] = 0.0f;
				mViewMatrix[3][3] = 1.0f;*/

				glm::vec3 pos = glm::vec3(0, 0, 50);
				glm::vec3 target = glm::vec3(0, 0, 0);
				glm::vec3 up = glm::vec3(0, 1, 0);
				mViewMatrix = glm::lookAt(pos, target, up);
			}
		}

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
