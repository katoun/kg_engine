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

#ifndef _SCENE_VIEW_WIDGET_H_
#define _SCENE_VIEW_WIDGET_H_

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
	VIEW_OPERATION_ROTATE,
	VIEW_OPERATION_PAN,
	VIEW_OPERATION_MOVE_FORWARD,
	VIEW_OPERATION_MOVE_BACKWARD,
	VIEW_OPERATION_MOVE_LEFT,
	VIEW_OPERATION_MOVE_RIGHT,

	VIEW_OPERATION_COUNT
};

class SceneViewWidget : public QWidget, public engine::EngineEventReceiver
{
	Q_OBJECT

public:

	SceneViewWidget(QWidget *parent = NULL);
	~SceneViewWidget();

	QPaintEngine* paintEngine() const; // Turn off QTs paint engine for the SceneView widget.

	void SetRenderWindow(render::RenderWindow* window);

	void engineUpdateStarted(const engine::EngineEvent& evt);

protected:

	void resizeEvent(QResizeEvent* evt);
	void focusOutEvent(QFocusEvent *evt);
	void focusInEvent(QFocusEvent *evt);

	void keyPressEvent(QKeyEvent *evt);
	void keyReleaseEvent(QKeyEvent *evt);
	void mouseMoveEvent(QMouseEvent *evt);
	void mousePressEvent(QMouseEvent *evt);
	void mouseReleaseEvent(QMouseEvent *evt);
	void wheelEvent(QWheelEvent *evt);

	void leaveEvent(QEvent *evt);

	std::bitset<VIEW_OPERATION_COUNT> mViewOperations;
	int mDragLastX;
	int mDragLastY;
	float moveMultiplier;
	float rotateMultiplier;
	float panMultiplier;
	float mMoveScale;
	float mRotScale;
	float mPanScale;

	render::RenderWindow* mRenderWindow;

	game::GameObject* mGameObject;
	game::Transform* mTransform;
	render::Camera* mCamera;
	render::Viewport* mViewport;
};

#endif