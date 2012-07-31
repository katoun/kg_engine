/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2012 Catalin Alexandru Nastase

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

#include <SceneView.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <game/GameManager.h>
#include <engine/EngineManager.h>
#include <render/RenderWindow.h>
#include <render/Camera.h>
#include <render/Viewport.h>

SceneViewWidget::SceneViewWidget(QWidget *parent): QWidget(parent, Qt::MSWindowsOwnDC)
{
	moveMultiplier		= 1.0f;
	rotateMultiplier	= 1.0f;
	panMultiplier		= 1.0f;
	mMoveScale			= 0.0f;
	mRotScale			= 0.0f;
	mPanScale			= 0.0f;

	mRenderWindow = NULL;

	engine::EngineManager::getInstance().addEngineEventReceiver(this);

	setAttribute(Qt::WA_NoBackground);

	setFocusPolicy(Qt::WheelFocus);
	setAcceptDrops(true);
    setMouseTracking(true);
}

SceneViewWidget::~SceneViewWidget()
{
	engine::EngineManager::getInstance().removeEngineEventReceiver(this);

	destroy();
}

QPaintEngine* SceneViewWidget::paintEngine() const
{
    // We don't want another paint engine to get in the way for our Ogre based paint engine.
    // So we return nothing.
    return NULL;
}

void SceneViewWidget::SetRenderWindow(render::RenderWindow* window)
{
	mRenderWindow = window;

	if (mRenderWindow != NULL)
	{
		mGameObject = game::GameManager::getInstance().createGameObject();
		mTransform = static_cast<game::Transform*>(game::GameManager::getInstance().createComponent(game::COMPONENT_TYPE_TRANSFORM));
		mCamera = static_cast<render::Camera*>(game::GameManager::getInstance().createComponent(game::COMPONENT_TYPE_CAMERA));
		mGameObject->attachComponent(mTransform);
		mGameObject->attachComponent(mCamera);
		mTransform->setPosition(0, 10 * ENGINE_UNIT_M, 20 * ENGINE_UNIT_M);
		mCamera->setFarClipDistance(0.0f);

		mViewport = mRenderWindow->createViewport(mCamera);
	}
}

void SceneViewWidget::engineUpdateStarted(const engine::EngineEvent& evt)
{
	// Move about 10 units per second,
	mMoveScale = 10.0f * evt.timeSinceLastUpdate * moveMultiplier;
	// Take about 10 seconds for full rotation
	mRotScale = 36.0f * evt.timeSinceLastUpdate * rotateMultiplier;
	// Pan about 10 units per second,
	mPanScale = 10.0f * evt.timeSinceLastUpdate * panMultiplier;

	if (mTransform != NULL)
	{
		if (mViewOperations[VIEW_OPERATION_MOVE_FORWARD] && !mViewOperations[VIEW_OPERATION_MOVE_BACKWARD])
			mTransform->translate(core::vector3d(0, 0, -mMoveScale));
		if (!mViewOperations[VIEW_OPERATION_MOVE_FORWARD] && mViewOperations[VIEW_OPERATION_MOVE_BACKWARD])
			mTransform->translate(core::vector3d(0, 0, mMoveScale));
			
		if (mViewOperations[VIEW_OPERATION_MOVE_LEFT] && !mViewOperations[VIEW_OPERATION_MOVE_RIGHT])
			mTransform->translate(core::vector3d(-mMoveScale, 0, 0));
		if (!mViewOperations[VIEW_OPERATION_MOVE_LEFT] && mViewOperations[VIEW_OPERATION_MOVE_RIGHT])
			mTransform->translate(core::vector3d(mMoveScale, 0, 0));
	}
}

void SceneViewWidget::resizeEvent(QResizeEvent* evt)
{
	if (mRenderWindow != NULL)
	{
		mRenderWindow->resize(width(), height());
	}
}

void SceneViewWidget::focusOutEvent(QFocusEvent *evt)
{
	//TODO!!!

	 evt->setAccepted(true);
}

void SceneViewWidget::focusInEvent(QFocusEvent *evt)
{
	//TODO!!!

	 evt->setAccepted(true);
}

void SceneViewWidget::keyPressEvent(QKeyEvent *evt)
{
	if (evt->key() == Qt::Key_W)
	{
		mViewOperations.set(VIEW_OPERATION_MOVE_FORWARD);
	}
	if (evt->key() == Qt::Key_S)
	{
		mViewOperations.set(VIEW_OPERATION_MOVE_BACKWARD);
	}
	if (evt->key() == Qt::Key_A)
	{
		mViewOperations.set(VIEW_OPERATION_MOVE_LEFT);
	}
	if (evt->key() == Qt::Key_D)
	{
		mViewOperations.set(VIEW_OPERATION_MOVE_RIGHT);
	}
}

void SceneViewWidget::keyReleaseEvent(QKeyEvent *evt)
{
	if (evt->key() == Qt::Key_W)
	{
		mViewOperations.reset(VIEW_OPERATION_MOVE_FORWARD);
	}
	if (evt->key() == Qt::Key_S)
	{
		mViewOperations.reset(VIEW_OPERATION_MOVE_BACKWARD);
	}
	if (evt->key() == Qt::Key_A)
	{
		mViewOperations.reset(VIEW_OPERATION_MOVE_LEFT);
	}
	if (evt->key() == Qt::Key_D)
	{
		mViewOperations.reset(VIEW_OPERATION_MOVE_RIGHT);
	}
}

void SceneViewWidget::mouseMoveEvent(QMouseEvent *evt)
{
	if (mTransform == NULL || mCamera == NULL)
		return;

	float DeltaX = (evt->x() - mDragLastX);
	float DeltaY = (evt->y() - mDragLastY);

	if (mViewOperations[VIEW_OPERATION_ROTATE])
	{
		mTransform->rotateX(DeltaY * mRotScale);

		if (mCamera->getFixedUp())
			mTransform->rotate(DeltaX * mRotScale, mCamera->getFixedUpAxis(), game::TRANSFORM_SPACE_WORLD);
		else
			mTransform->rotateY(DeltaX * mRotScale);
	}

	if (mViewOperations[VIEW_OPERATION_PAN])
	{
		mTransform->translate(core::vector3d(0, DeltaY * mPanScale, 0));
		mTransform->translate(core::vector3d(-DeltaX * mPanScale, 0, 0));
	}

	mDragLastX = evt->x();
	mDragLastY = evt->y();
}

void SceneViewWidget::mousePressEvent(QMouseEvent *evt)
{
	if(evt->button() == Qt::LeftButton)
	{
		mViewOperations.set(VIEW_OPERATION_ROTATE);

		mDragLastX = evt->x();
		mDragLastY = evt->y();
	}

	if(evt->button() == Qt::MidButton)
	{
		mViewOperations.set(VIEW_OPERATION_PAN);

		mDragLastX = evt->x();
		mDragLastY = evt->y();
	}
}

void SceneViewWidget::mouseReleaseEvent(QMouseEvent *evt)
{
	if(evt->button() == Qt::LeftButton)
	{
		mViewOperations.reset(VIEW_OPERATION_ROTATE);
	}

	if(evt->button() == Qt::MidButton)
	{
		mViewOperations.reset(VIEW_OPERATION_PAN);
	}
}

void SceneViewWidget::wheelEvent(QWheelEvent *evt)
{
	if (mTransform != NULL && !mViewOperations[VIEW_OPERATION_PAN])
	{
		mTransform->translate(core::vector3d(0, 0, (float)evt->delta()));
	}
}

void SceneViewWidget::leaveEvent(QEvent *evt)
{
	mViewOperations.reset(VIEW_OPERATION_MOVE_FORWARD);
	mViewOperations.reset(VIEW_OPERATION_MOVE_BACKWARD);
	mViewOperations.reset(VIEW_OPERATION_MOVE_LEFT);
	mViewOperations.reset(VIEW_OPERATION_MOVE_RIGHT);
	
	mViewOperations.reset(VIEW_OPERATION_ROTATE);
	mViewOperations.reset(VIEW_OPERATION_PAN);
}