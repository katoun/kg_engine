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
#include <render/RenderWindow.h>
#include <render/Camera.h>
#include <render/Viewport.h>

SceneViewPanel::SceneViewPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style): wxPanel()
{
	wxPanel::Create(parent, id, pos, size, style);

	mRenderWindow = NULL;

	SetBackgroundColour(wxColour(0, 0, 0));

	Connect(wxEVT_SIZE, wxSizeEventHandler(SceneViewPanel::OnSize), NULL, this);
}

SceneViewPanel::~SceneViewPanel()
{
	Disconnect(wxEVT_SIZE, wxSizeEventHandler(SceneViewPanel::OnSize), NULL, this);
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

void SceneViewPanel::OnKeyDown(wxKeyEvent &evt)
{
}

void SceneViewPanel::OnKeyUp(wxKeyEvent &evt)
{
}

void SceneViewPanel::OnMouseMove(wxMouseEvent &evt)
{
}

void SceneViewPanel::OnMouseLeave(wxMouseEvent &evt)
{
}

void SceneViewPanel::OnMouseLeftDown(wxMouseEvent &evt)
{
}

void SceneViewPanel::OnMouseLeftUp(wxMouseEvent &evt)
{
}

void SceneViewPanel::OnMouseRightDown(wxMouseEvent &evt)
{
}

void SceneViewPanel::OnMouseMiddleDown(wxMouseEvent &evt)
{
}

void SceneViewPanel::OnMouseMiddleUp(wxMouseEvent &evt)
{
}

void SceneViewPanel::OnMouseRightUp(wxMouseEvent &evt)
{
}

void SceneViewPanel::OnMouseWheel(wxMouseEvent &evt)
{
}


