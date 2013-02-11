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

#include <RenderView.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <game/GameManager.h>
#include <engine/EngineManager.h>
#include <render/RenderWindow.h>
#include <render/Light.h>
#include <render/Camera.h>
#include <render/Model.h>
#include <render/MeshData.h>
#include <physics/Body.h>
#include <physics/BodyData.h>
#include <resource/ResourceDefines.h>
#include <resource/ResourceManager.h>
#include <input/Cursor.h>
#include <input/Keyboard.h>
#include <input/Mouse.h>
#include <input/InputManager.h>

BEGIN_EVENT_TABLE(RenderView, wxWindow)
	EVT_SIZE(RenderView::OnSize)
	/*EVT_KEY_DOWN(RenderView::OnKeyDown)
	EVT_KEY_UP(RenderView::OnKeyUp)
	EVT_MOTION(RenderView::OnMouseMove)
	EVT_LEFT_DOWN(RenderView::OnMouseLeftDown)
	EVT_LEFT_UP(RenderView::OnMouseLeftUp)
	EVT_MIDDLE_DOWN(RenderView::OnMouseMiddleDown)
	EVT_MIDDLE_UP(RenderView::OnMouseMiddleUp)*/
END_EVENT_TABLE()

RenderView::RenderView(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style): wxWindow(parent, winid, pos, size, style)
{
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	
	moveMultiplier		= 1.0f;
	rotateMultiplier	= 1.0f;
	panMultiplier		= 1.0f;
	mMoveScale			= 0.0f;
	mRotScale			= 0.0f;
	mPanScale			= 0.0f;

	mFPSMode			= false;
	mCursor				= nullptr;

	mRenderWindow		= nullptr;

	mMainTransform		= nullptr;
	mMainCamera			= nullptr;
	mViewport			= nullptr;

	mPlaneMeshData		= nullptr;
	mPlaneBodyData		= nullptr;
	mSphereMeshData		= nullptr;
	mSphereBodyData		= nullptr;

	if (engine::EngineManager::getInstance() != nullptr)
		engine::EngineManager::getInstance()->addEngineEventReceiver(this);
}

RenderView::~RenderView()
{
	if (engine::EngineManager::getInstance() != nullptr)
		engine::EngineManager::getInstance()->removeEngineEventReceiver(this);
}

void RenderView::SetRenderWindow(render::RenderWindow* window)
{
	mRenderWindow = window;

	if (mRenderWindow != nullptr && game::GameManager::getInstance() != nullptr)
	{
		game::GameObject* pGameObject = nullptr;
		game::Transform* pTransform = nullptr;
		render::Camera* pCamera = nullptr;

		//Main Camera
		pGameObject = game::GameManager::getInstance()->createGameObject();
		pTransform = static_cast<game::Transform*>(game::GameManager::getInstance()->createComponent(game::COMPONENT_TYPE_TRANSFORM));
		pCamera = static_cast<render::Camera*>(game::GameManager::getInstance()->createComponent(game::COMPONENT_TYPE_CAMERA));
		pGameObject->attachComponent(pTransform);
		pGameObject->attachComponent(pCamera);
		pTransform->setPosition(0, 2, 20);
		pCamera->setFarClipDistance(0.0f);

		mMainTransform = pTransform;
		mMainCamera = pCamera;
		mViewport = mRenderWindow->createViewport(pCamera);
		//Main Camera
	}
}

void RenderView::engineInitialized()
{
	setupInput();

	createResources();
		
	createScene();
}
	
void RenderView::engineUninitialized()
{
}

void RenderView::engineStarted()
{
}
	
void RenderView::engineStopped()
{
}

void RenderView::engineUpdateStarted(const engine::EngineEvent& evt)
{
	// Move about 10 units per second,
	mMoveScale = 10.0f * evt.timeSinceLastUpdate * moveMultiplier;
	// Take about 10 seconds for full rotation
	mRotScale = 36.0f * evt.timeSinceLastUpdate * rotateMultiplier;
	// Pan about 10 units per second,
	mPanScale = 10.0f * evt.timeSinceLastUpdate * panMultiplier;

	if (mMainTransform != nullptr)
	{
		if (mViewOperations[VIEW_OPERATION_MOVE_FORWARD] && !mViewOperations[VIEW_OPERATION_MOVE_BACKWARD])
			mMainTransform->translate(glm::vec3(0, 0, -mMoveScale));
		if (!mViewOperations[VIEW_OPERATION_MOVE_FORWARD] && mViewOperations[VIEW_OPERATION_MOVE_BACKWARD])
			mMainTransform->translate(glm::vec3(0, 0, mMoveScale));
			
		if (mViewOperations[VIEW_OPERATION_MOVE_LEFT] && !mViewOperations[VIEW_OPERATION_MOVE_RIGHT])
			mMainTransform->translate(glm::vec3(-mMoveScale, 0, 0));
		if (!mViewOperations[VIEW_OPERATION_MOVE_LEFT] && mViewOperations[VIEW_OPERATION_MOVE_RIGHT])
			mMainTransform->translate(glm::vec3(mMoveScale, 0, 0));
	}
}

void RenderView::engineUpdateEnded(const engine::EngineEvent& evt)
{
}

void RenderView::OnSize(wxSizeEvent& event)
{
	if (mRenderWindow != nullptr)
	{
		wxSize size = event.GetSize();
		mRenderWindow->resize(size.GetWidth(), size.GetHeight());
	}
}

void RenderView::createResources()
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		mPlaneMeshData	= static_cast<render::MeshData*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_MESH_DATA, "meshes/Plane1m.xml"));
		mPlaneBodyData	= static_cast<physics::BodyData*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_BODY_DATA, "bodies/Plane.xml"));
		mSphereMeshData	= static_cast<render::MeshData*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_MESH_DATA, "meshes/Sphere1m.xml"));
		mSphereBodyData	= static_cast<physics::BodyData*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_BODY_DATA, "bodies/Sphere.xml"));
	}
}

void RenderView::setupInput()
{
	if (input::InputManager::getInstance() != nullptr)
	{
		mCursor						= input::InputManager::getInstance()->getCursor();
		input::Keyboard* pKeyboard	= (input::Keyboard*)input::InputManager::getInstance()->createInputDevice(input::INPUT_TYPE_KEYBOARD, true);
		input::Mouse* pMouse		= (input::Mouse*)input::InputManager::getInstance()->createInputDevice(input::INPUT_TYPE_MOUSE, true);
		pKeyboard->addKeyEventReceiver(this);
		pMouse->addMouseEventReceiver(this);

		if (mFPSMode == true)
			mCursor->setAutoCenter(true);
	}
}

void RenderView::createScene()
{
	if (game::GameManager::getInstance() != nullptr)
	{
		game::GameObject* pGameObject = nullptr;
		game::Transform* pTransform = nullptr;
		render::Camera* pCamera = nullptr;
		render::Light* pLight = nullptr;
		render::Model* pModel = nullptr;
		physics::Body* pBody = nullptr;

		//Body plane
		pGameObject = game::GameManager::getInstance()->createGameObject();
		pTransform = static_cast<game::Transform*>(game::GameManager::getInstance()->createComponent(game::COMPONENT_TYPE_TRANSFORM));
		pBody = static_cast<physics::Body*>(game::GameManager::getInstance()->createComponent(game::COMPONENT_TYPE_BODY));
		pModel = static_cast<render::Model*>(game::GameManager::getInstance()->createComponent(game::COMPONENT_TYPE_MODEL));
		pGameObject->attachComponent(pTransform);
		pGameObject->attachComponent(pBody);
		pGameObject->attachComponent(pModel);
		pBody->setBodyData(mPlaneBodyData);
		pBody->setEnabled(true);
		pModel->setMeshData(mPlaneMeshData);
		//pModel->setMaterial(mMaterial);
		//Body plane

		//Body sphere
		pGameObject = game::GameManager::getInstance()->createGameObject();
		pTransform = static_cast<game::Transform*>(game::GameManager::getInstance()->createComponent(game::COMPONENT_TYPE_TRANSFORM));
		pBody = static_cast<physics::Body*>(game::GameManager::getInstance()->createComponent(game::COMPONENT_TYPE_BODY));
		pModel = static_cast<render::Model*>(game::GameManager::getInstance()->createComponent(game::COMPONENT_TYPE_MODEL));
		pGameObject->attachComponent(pTransform);
		pGameObject->attachComponent(pBody);
		pGameObject->attachComponent(pModel);
		pBody->setBodyData(mSphereBodyData);
		pBody->setEnabled(true);
		pTransform->setPosition(100, 100, 0);
		pModel->setMeshData(mSphereMeshData);
		//pModel->setMaterial(mMaterial);
		//Body sphere
	}
}

/*
void RenderView::OnKeyDown(wxKeyEvent& event)
{
	if (event.GetUnicodeKey() == 'w')
	{
		mViewOperations.set(VIEW_OPERATION_MOVE_FORWARD);
	}
	if (event.GetUnicodeKey() == 's')
	{
		mViewOperations.set(VIEW_OPERATION_MOVE_BACKWARD);
	}
	if (event.GetUnicodeKey() == 'a')
	{
		mViewOperations.set(VIEW_OPERATION_MOVE_LEFT);
	}
	if (event.GetUnicodeKey() == 'd')
	{
		mViewOperations.set(VIEW_OPERATION_MOVE_RIGHT);
	}
}

void RenderView::OnKeyUp(wxKeyEvent& event)
{
	if (event.GetUnicodeKey() == 'w')
	{
		mViewOperations.reset(VIEW_OPERATION_MOVE_FORWARD);
	}
	if (event.GetUnicodeKey() == 's')
	{
		mViewOperations.reset(VIEW_OPERATION_MOVE_BACKWARD);
	}
	if (event.GetUnicodeKey() == 'a')
	{
		mViewOperations.reset(VIEW_OPERATION_MOVE_LEFT);
	}
	if (event.GetUnicodeKey() == 'd')
	{
		mViewOperations.reset(VIEW_OPERATION_MOVE_RIGHT);
	}
}

void RenderView::OnMouseMove(wxMouseEvent& event)
{
	if (mMainTransform == NULL || mMainCamera == NULL)
		return;

	wxPoint pos = event.GetPosition();
	float DeltaX = (pos.x - mDragLastX);
	float DeltaY = (pos.y - mDragLastY);

	if (mViewOperations[VIEW_OPERATION_ROTATE])
	{
		mMainTransform->rotateX(DeltaY * mRotScale);

		if (mMainCamera->getFixedUp())
			mMainTransform->rotate(DeltaX * mRotScale, mMainCamera->getFixedUpAxis(), game::TRANSFORM_WORLD_SPACE);
		else
			mMainTransform->rotateY(DeltaX * mRotScale);
	}

	if (mViewOperations[VIEW_OPERATION_PAN])
	{
		mMainTransform->translate(glm::vec3(0, DeltaY * mPanScale, 0));
		mMainTransform->translate(glm::vec3(-DeltaX * mPanScale, 0, 0));
	}

	mDragLastX = pos.x;
	mDragLastY = pos.y;
}

void RenderView::OnMouseLeftDown(wxMouseEvent& event)
{
	mViewOperations.set(VIEW_OPERATION_ROTATE);

	wxPoint pos = event.GetPosition();
	mDragLastX = pos.x;
	mDragLastY = pos.y;
}

void RenderView::OnMouseLeftUp(wxMouseEvent& event)
{
	mViewOperations.reset(VIEW_OPERATION_ROTATE);
}

void RenderView::OnMouseMiddleDown(wxMouseEvent& event)
{
	mViewOperations.set(VIEW_OPERATION_PAN);

	wxPoint pos = event.GetPosition();
	mDragLastX = pos.x;
	mDragLastY = pos.y;
}

void RenderView::OnMouseMiddleUp(wxMouseEvent& event)
{
	mViewOperations.reset(VIEW_OPERATION_PAN);
}*/