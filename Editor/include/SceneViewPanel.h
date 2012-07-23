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

#ifndef _SCENE_VIEW_PANEL_H_
#define _SCENE_VIEW_PANEL_H_

#include <EditorConfig.h>
#include <engine/EngineEvent.h>
#include <engine/EngineEventReceiver.h>

#include <bitset>

namespace game
{
class GameObject;
class Transform;
}

namespace render
{
class RenderWindow;
class Camera;
class Viewport;
}

enum ViewOperation
{
	VIEW_OPERATION_NONE,
	VIEW_OPERATION_ROTATE,
	VIEW_OPERATION_PAN,
	VIEW_OPERATION_MOVE_FORWARD,
	VIEW_OPERATION_MOVE_BACKWARD,
	VIEW_OPERATION_MOVE_LEFT,
	VIEW_OPERATION_MOVE_RIGHT,

	VIEW_OPERATION_COUNT
};

class SceneViewPanel : public wxPanel, public engine::EngineEventReceiver
{
public:

	SceneViewPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSUNKEN_BORDER | wxTAB_TRAVERSAL);

	~SceneViewPanel();

	void OnSize(wxSizeEvent& InEvent);

	void SetRenderWindow(render::RenderWindow* window);

	void engineUpdateStarted(const engine::EngineEvent& evt);

protected:

	void OnKeyDown(wxKeyEvent &evt);	
	void OnKeyUp(wxKeyEvent &evt);
	void OnMouseEnter(wxMouseEvent &evt);
	void OnMouseLeave(wxMouseEvent &evt);
	void OnMouseMove(wxMouseEvent &evt);
	void OnMouseLeftDown(wxMouseEvent &evt);
	void OnMouseLeftUp(wxMouseEvent &evt);
	void OnMouseRightDown(wxMouseEvent &evt);
	void OnMouseRightUp(wxMouseEvent &evt);
	void OnMouseMiddleDown(wxMouseEvent &evt);
	void OnMouseMiddleUp(wxMouseEvent &evt);
	void OnMouseWheel(wxMouseEvent &evt);

	std::bitset<VIEW_OPERATION_COUNT> mViewOperations;
	wxPoint mDragStartPoint;
	float moveMultiplier;
	float rotMultiplier;
	float mMoveScale;
	float mRotScale;

	render::RenderWindow* mRenderWindow;

	game::GameObject* mGameObject;
	game::Transform* mTransform;
	render::Camera* mCamera;
	render::Viewport* mViewport;
};

#endif