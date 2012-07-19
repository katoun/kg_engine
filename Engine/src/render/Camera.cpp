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

#include <render/Camera.h>
#include <render/CameraDefines.h>
#include <render/Frustum.h>
#include <render/RenderManager.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <core/Math.h>
#include <core/Utils.h>

namespace render
{

Camera::Camera(): game::Component()
{
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

	mModifiedProjection = true;
	mModifiedView = true;
}

Camera::~Camera()
{
	if (mFrustum != NULL)
	{
		delete mFrustum;
	}
}

void Camera::setProjectionType(ProjectionType pt)
{
	mProjType = pt;
	mModifiedProjection = true;
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
	mModifiedProjection = true;
}

float Camera::getFOV()
{
	return mFOV;
}

void Camera::setNearClipDistance(float nearPlane)
{
	mNearDist = nearPlane;
	mModifiedProjection = true;
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
	mModifiedProjection = true;
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
	if (mGameObject != NULL)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != NULL)
		{
			mModifiedView = pTransform->isAbsoluteTransformModified();
		}
	}

	updateProjection();
	updateView();
	updateFrustum();
}

void Camera::updateProjection()
{
	if(mModifiedProjection)
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
		
		RenderManager::getInstance().convertProjectionMatrix(mProjMatrix, mProjMatrixRS);

		mModifiedProjection = false;
	}
}

void Camera::updateView()
{
	if(mModifiedView)
	{
		// ----------------------
		// Update the view matrix
		// ----------------------

		if (mGameObject != NULL)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != NULL)
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

		mModifiedView = false;
	}
}

void Camera::updateFrustum()
{
	mFrustum->buildViewFrustum(mProjMatrix, mViewMatrix, mProjType, mFOV, mAspect, mNearDist, mFarDist);
}

} //namespace render
