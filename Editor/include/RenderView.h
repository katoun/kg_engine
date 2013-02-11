/*
-----------------------------------------------------------------------------
Copyright (c) 2006-2013 Catalin Alexandru Nastase

KG game engine (http://k-game.sourceforge.net) is made available under the LGPL License (http://www.gnu.org/copyleft/lgpl.html)

Under the LGPL you may use KG game engine for any purpose you wish, as long as you:
1. Release any modifications to the KG game engine source back to the community
2. Pass on the source to KG game engine with all the copyrights intact, or link back to a place where the source code can be obtained (http://k-game.sourceforge.net)
3. Make it clear where you have customised it.
The above is a precis, please do read the full license agreement.
-----------------------------------------------------------------------------
*/

#ifndef _RENDER_VIEW_H_
#define _RENDER_VIEW_H_

#include <EditorConfig.h>
#include <engine/EngineEvent.h>
#include <engine/EngineEventReceiver.h>
#include <input/KeyEvent.h>
#include <input/MouseEvent.h>
#include <input/KeyEventReceiver.h>
#include <input/MouseEventReceiver.h>

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
class MeshData;
}

namespace physics
{
class BodyData;
}

namespace input
{
class Cursor;
}

enum ViewOperation
{
	VIEW_OPERATION_ROTATE,
	VIEW_OPERATION_PAN,
	VIEW_OPERATION_MOVE_FORWARD,
	VIEW_OPERATION_MOVE_BACKWARD,
	VIEW_OPERATION_MOVE_LEFT,
	VIEW_OPERATION_MOVE_RIGHT,

	VIEW_OPERATION_COUNT
};

class RenderView: public wxWindow, public engine::EngineEventReceiver, public input::KeyEventReceiver, public input::MouseEventReceiver
{
public:

	RenderView(wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER);

	~RenderView();

	void SetRenderWindow(render::RenderWindow* window);

	void engineInitialized();
	void engineUninitialized();
	void engineStarted();
	void engineStopped();

	void engineUpdateStarted(const engine::EngineEvent& evt);
	void engineUpdateEnded(const engine::EngineEvent& evt);

	void OnSize(wxSizeEvent& event);
	/*void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnMouseMiddleDown(wxMouseEvent& event);
	void OnMouseMiddleUp(wxMouseEvent& event);*/

private:

	std::bitset<VIEW_OPERATION_COUNT> mViewOperations;
	int						mDragLastX;
	int						mDragLastY;
	float					moveMultiplier;
	float					rotateMultiplier;
	float					panMultiplier;
	float					mMoveScale;
	float					mRotScale;
	float					mPanScale;

	bool					mFPSMode;
	input::Cursor*			mCursor;

	render::RenderWindow*	mRenderWindow;

	game::Transform*		mMainTransform;
	render::Camera*			mMainCamera;
	render::Viewport*		mViewport;

	render::MeshData*		mPlaneMeshData;
	physics::BodyData*		mPlaneBodyData;
	render::MeshData*		mSphereMeshData;
	physics::BodyData*		mSphereBodyData;

	void createResources();
	void setupInput();
	void createScene();

    DECLARE_EVENT_TABLE()
};

#endif