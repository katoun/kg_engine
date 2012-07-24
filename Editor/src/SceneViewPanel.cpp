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

#include <SceneViewPanel.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <game/GameManager.h>
#include <engine/EngineManager.h>
#include <render/RenderWindow.h>
#include <render/Camera.h>
#include <render/Viewport.h>

SceneViewPanel::SceneViewPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style): wxPanel()
{
	wxPanel::Create(parent, id, pos, size, style);

	engine::EngineManager::getInstance().addEngineEventReceiver(this);
	moveMultiplier		= 1.0f;
	rotateMultiplier	= 1.0f;
	panMultiplier		= 1.0f;
	mMoveScale			= 0.0f;
	mRotScale			= 0.0f;
	mPanScale			= 0.0f;

	mRenderWindow = NULL;

	SetBackgroundColour(wxColour(0, 0, 0));

	Connect(wxEVT_SIZE, wxSizeEventHandler(SceneViewPanel::OnSize), NULL, this);

	Connect(wxEVT_KEY_DOWN,			wxKeyEventHandler(SceneViewPanel::OnKeyDown), NULL, this);
	Connect(wxEVT_KEY_UP,			wxKeyEventHandler(SceneViewPanel::OnKeyUp), NULL, this);
	Connect(wxEVT_MOTION,			wxMouseEventHandler(SceneViewPanel::OnMouseMove), NULL, this);
	Connect(wxEVT_ENTER_WINDOW,		wxMouseEventHandler(SceneViewPanel::OnMouseEnter), NULL, this );
	Connect(wxEVT_LEAVE_WINDOW,		wxMouseEventHandler(SceneViewPanel::OnMouseLeave), NULL, this );
	Connect(wxEVT_LEFT_DOWN,		wxMouseEventHandler(SceneViewPanel::OnMouseLeftDown), NULL, this);
	Connect(wxEVT_LEFT_UP,			wxMouseEventHandler(SceneViewPanel::OnMouseLeftUp), NULL, this);
	Connect(wxEVT_RIGHT_DOWN,		wxMouseEventHandler(SceneViewPanel::OnMouseRightDown), NULL, this);
	Connect(wxEVT_RIGHT_UP,			wxMouseEventHandler(SceneViewPanel::OnMouseRightUp), NULL, this);
	Connect(wxEVT_MIDDLE_DOWN,		wxMouseEventHandler(SceneViewPanel::OnMouseMiddleDown ), NULL, this);
	Connect(wxEVT_MIDDLE_UP,		wxMouseEventHandler(SceneViewPanel::OnMouseMiddleUp), NULL, this);
	Connect(wxEVT_MOUSEWHEEL,		wxMouseEventHandler(SceneViewPanel::OnMouseWheel), NULL, this);
}

SceneViewPanel::~SceneViewPanel()
{
	engine::EngineManager::getInstance().removeEngineEventReceiver(this);

	Disconnect(wxEVT_SIZE, wxSizeEventHandler(SceneViewPanel::OnSize), NULL, this);

	Disconnect(wxEVT_KEY_DOWN,		wxKeyEventHandler(SceneViewPanel::OnKeyDown), NULL, this);
	Disconnect(wxEVT_KEY_UP,		wxKeyEventHandler(SceneViewPanel::OnKeyUp), NULL, this);
	Disconnect(wxEVT_MOTION,		wxMouseEventHandler(SceneViewPanel::OnMouseMove), NULL, this);
	Disconnect(wxEVT_ENTER_WINDOW,	wxMouseEventHandler(SceneViewPanel::OnMouseEnter), NULL, this );
	Disconnect(wxEVT_LEAVE_WINDOW,	wxMouseEventHandler(SceneViewPanel::OnMouseLeave), NULL, this );
	Disconnect(wxEVT_LEFT_DOWN,		wxMouseEventHandler(SceneViewPanel::OnMouseLeftDown), NULL, this);
	Disconnect(wxEVT_LEFT_UP,		wxMouseEventHandler(SceneViewPanel::OnMouseLeftUp), NULL, this);
	Disconnect(wxEVT_RIGHT_DOWN,	wxMouseEventHandler(SceneViewPanel::OnMouseRightDown), NULL, this);
	Disconnect(wxEVT_RIGHT_UP,		wxMouseEventHandler(SceneViewPanel::OnMouseRightUp), NULL, this);
	Disconnect(wxEVT_MIDDLE_DOWN,	wxMouseEventHandler(SceneViewPanel::OnMouseMiddleDown ), NULL, this);
	Disconnect(wxEVT_MIDDLE_UP,		wxMouseEventHandler(SceneViewPanel::OnMouseMiddleUp), NULL, this);
	Disconnect(wxEVT_MOUSEWHEEL,	wxMouseEventHandler(SceneViewPanel::OnMouseWheel), NULL, this);
}

void SceneViewPanel::OnSize(wxSizeEvent& InEvent)
{
	if (mRenderWindow == NULL)
		return;

	wxRect rc = GetClientRect();
	mRenderWindow->resize(rc.GetWidth(), rc.GetHeight());
}

void SceneViewPanel::SetRenderWindow(render::RenderWindow* window)
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

void SceneViewPanel::engineUpdateStarted(const engine::EngineEvent& evt)
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

void SceneViewPanel::OnKeyDown(wxKeyEvent &evt)
{
	if (evt.GetKeyCode() == (char)'W')
		mViewOperations.set(VIEW_OPERATION_MOVE_FORWARD);
	if (evt.GetKeyCode() == (char)'S')
		mViewOperations.set(VIEW_OPERATION_MOVE_BACKWARD);
	if (evt.GetKeyCode() == (char)'A')
		mViewOperations.set(VIEW_OPERATION_MOVE_LEFT);
	if (evt.GetKeyCode() == (char)'D')
		mViewOperations.set(VIEW_OPERATION_MOVE_RIGHT);
}

void SceneViewPanel::OnKeyUp(wxKeyEvent &evt)
{
	if (evt.GetKeyCode() == (char)'W')
		mViewOperations.reset(VIEW_OPERATION_MOVE_FORWARD);
	if (evt.GetKeyCode() == (char)'S')
		mViewOperations.reset(VIEW_OPERATION_MOVE_BACKWARD);
	if (evt.GetKeyCode() == (char)'A')
		mViewOperations.reset(VIEW_OPERATION_MOVE_LEFT);
	if (evt.GetKeyCode() == (char)'D')
		mViewOperations.reset(VIEW_OPERATION_MOVE_RIGHT);
}

void SceneViewPanel::OnMouseEnter(wxMouseEvent &evt)
{
	SetFocus();
}

void SceneViewPanel::OnMouseLeave(wxMouseEvent &evt)
{
	mViewOperations.reset(VIEW_OPERATION_MOVE_FORWARD);
	mViewOperations.reset(VIEW_OPERATION_MOVE_BACKWARD);
	mViewOperations.reset(VIEW_OPERATION_MOVE_LEFT);
	mViewOperations.reset(VIEW_OPERATION_MOVE_RIGHT);
	
	mViewOperations.reset(VIEW_OPERATION_ROTATE);
	mViewOperations.reset(VIEW_OPERATION_PAN);
}

void SceneViewPanel::OnMouseMove(wxMouseEvent &evt)
{
	if (mTransform == NULL || mCamera == NULL)
		return;
	
	float DeltaX = (evt.GetPosition().x - mDragLastPoint.x);
	float DeltaY = (evt.GetPosition().y - mDragLastPoint.y);

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

	mDragLastPoint = evt.GetPosition();
}

void SceneViewPanel::OnMouseLeftDown(wxMouseEvent &evt)
{
	//Do select object here!!!
}

void SceneViewPanel::OnMouseLeftUp(wxMouseEvent &evt)
{
}

void SceneViewPanel::OnMouseRightDown(wxMouseEvent &evt)
{
	mViewOperations.set(VIEW_OPERATION_ROTATE);

	mDragLastPoint = evt.GetPosition();
}

void SceneViewPanel::OnMouseRightUp(wxMouseEvent &evt)
{
	mViewOperations.reset(VIEW_OPERATION_ROTATE);
}

void SceneViewPanel::OnMouseMiddleDown(wxMouseEvent &evt)
{
	mViewOperations.set(VIEW_OPERATION_PAN);

	mDragLastPoint = evt.GetPosition();
}

void SceneViewPanel::OnMouseMiddleUp(wxMouseEvent &evt)
{
	mViewOperations.reset(VIEW_OPERATION_PAN);
}

void SceneViewPanel::OnMouseWheel(wxMouseEvent &evt)
{
	mTransform->translate(core::vector3d(0, 0, -(float)evt.GetWheelRotation()));
}


