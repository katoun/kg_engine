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

#include <Engine.h>
#include <iostream>
#include <string>

#if ENGINE_PLATFORM == PLATFORM_WIN32
#	include <windows.h>
#endif

#include <bitset>

#define MAX_SOUNDS 16

enum Movement
{
	MOVEMENT_CAMERA_MOVE_FORWARD,
	MOVEMENT_CAMERA_MOVE_BACKWARD,
	MOVEMENT_CAMERA_MOVE_LEFT,
	MOVEMENT_CAMERA_MOVE_RIGHT,
	MOVEMENT_CAMERA_MOVE_UP,
	MOVEMENT_CAMERA_MOVE_DOWN,

	MOVEMENT_OBJECT_MOVE_FORWARD,
	MOVEMENT_OBJECT_MOVE_BACKWARD,
	MOVEMENT_OBJECT_MOVE_LEFT,
	MOVEMENT_OBJECT_MOVE_RIGHT,
	MOVEMENT_OBJECT_MOVE_UP,
	MOVEMENT_OBJECT_MOVE_DOWN,

	MOVEMENT_OBJECT_ROTATE_LEFT,
	MOVEMENT_OBJECT_ROTATE_RIGHT,
	MOVEMENT_OBJECT_ROTATE_UP,
	MOVEMENT_OBJECT_ROTATE_DOWN,
	MOVEMENT_OBJECT_ROLL_LEFT,
	MOVEMENT_OBJECT_ROLL_RIGHT,

	MOVEMENT_COUNT
};

class Launcher: public engine::EngineEventReceiver, public input::KeyEventReceiver, public input::MouseEventReceiver, public physics::CollisionEventReceiver
{
public:

	Launcher():
		mEngineManager(engine::EngineManager::getInstance()),
		mInputManager(input::InputManager::getInstance()),
		mResourceManager(resource::ResourceManager::getInstance()),
		mGameManager(game::GameManager::getInstance()),
		mRenderManager(render::RenderManager::getInstance()),
		mSoundManager(sound::SoundManager::getInstance()),
		mPhysicsManager(physics::PhysicsManager::getInstance())
	{
		mEngineManager.addEngineEventReceiver(this);
		mPhysicsManager.addCollisionEventReceiver(this);

		mCursor			= NULL;

		/////////////////////
		mSelectedCameraTransform	= NULL;
		mSelectedCamera				= NULL;
		mCameras[0]					= NULL;
		mCameras[1]					= NULL;

		mSelectedNode	= NULL;

		mModels[0]		= NULL;
		mModels[1]		= NULL;

		for (unsigned char i = 0; i < MAX_SOUNDS; ++i)
			mCollisionSounds[i] = NULL;
		mCursorOverlay	= NULL;

		/////////////////////

		mCubeMeshData	= NULL;
		mSphereMeshData	= NULL;
		mPlaneBodyData	= NULL;
		mBoxBodyData	= NULL;
		mSphereBodyData	= NULL;
		mSoundData		= NULL;
		mMaterial		= NULL;
		mFont			= NULL;

		FPSMode			= false;

		moveMultiplier	= 1.0f;
		rotMultiplier	= 1.0f;
		mMoveScale		= 0.0f;
		mRotScale		= 0.0f;
		mMouseScale		= 0.0f;
	}

	~Launcher() {}

	void createResources()
	{
		mFont = static_cast<render::Font*>(mResourceManager.createResource(resource::RESOURCE_TYPE_FONT, "fonts/Arial.xml"));
		mMaterial = static_cast<render::Material*>(mResourceManager.createResource(resource::RESOURCE_TYPE_RENDER_MATERIAL, "materials/ShaderDefault.xml"));
		mCubeMeshData = static_cast<render::MeshData*>(mResourceManager.createResource(resource::RESOURCE_TYPE_MESH_DATA, "meshes/Cube1m.xml"));
		mSphereMeshData = static_cast<render::MeshData*>(mResourceManager.createResource(resource::RESOURCE_TYPE_MESH_DATA, "meshes/Sphere1m.xml"));
		mPlaneBodyData = static_cast<physics::BodyData*>(mResourceManager.createResource(resource::RESOURCE_TYPE_BODY_DATA, "bodies/Plane.xml"));
		mBoxBodyData = static_cast<physics::BodyData*>(mResourceManager.createResource(resource::RESOURCE_TYPE_BODY_DATA, "bodies/Box.xml"));
		mSphereBodyData = static_cast<physics::BodyData*>(mResourceManager.createResource(resource::RESOURCE_TYPE_BODY_DATA, "bodies/Sphere.xml"));
		mSoundData = static_cast<sound::SoundData*>(mResourceManager.createResource(resource::RESOURCE_TYPE_SOUND_DATA, "sounds/collision.ogg"));
	}

	void setupInput()
	{
		mCursor = mInputManager.getCursor();

		input::Keyboard* pKeyboard = (input::Keyboard*)mInputManager.createInputDevice(input::INPUT_TYPE_KEYBOARD, true);
		input::Mouse* pMouse = (input::Mouse*)mInputManager.createInputDevice(input::INPUT_TYPE_MOUSE, true);
		pKeyboard->addKeyEventReceiver(this);
		pMouse->addMouseEventReceiver(this);

		if (FPSMode == true)
			mCursor->setAutoCenter(true);
	}

	void createScene()
	{		
		mRenderManager.setAmbientLight(render::Color(0.3f,0.3f,0.3f));	

		mWin = mRenderManager.getMainWindow();

		/////////////////////////////////////////////
		game::GameObject* pGameObject = NULL;
		game::Transform* pTransform = NULL;

		pGameObject = mGameManager.createGameObject();
		pTransform = static_cast<game::Transform*>(mGameManager.createComponent(game::COMPONENT_TYPE_TRANSFORM));
		mCameras[0] = static_cast<render::Camera*>(mGameManager.createComponent(game::COMPONENT_TYPE_CAMERA));
		sound::Listener* pListener = static_cast<sound::Listener*>(mGameManager.createComponent(game::COMPONENT_TYPE_LISTENER));
		pGameObject->attachComponent(pTransform);
		pGameObject->attachComponent(mCameras[0]);
		pGameObject->attachComponent(pListener);
		pTransform->setPosition(0, 10 * ENGINE_UNIT_M, 20 * ENGINE_UNIT_M);
		mCameras[0]->setFarClipDistance(0.0f);
		mRenderManager.addCamera(mCameras[0]);
		
		mSelectedCameraTransform	= pTransform;
		mSelectedCamera				= mCameras[0];
		mSelectedCameraTransform->setVisibleAxis(true);
		mSelectedCamera->setVisibleFrustum(true);
				
		pGameObject = mGameManager.createGameObject();
		pTransform = static_cast<game::Transform*>(mGameManager.createComponent(game::COMPONENT_TYPE_TRANSFORM));
		mCameras[1] = static_cast<render::Camera*>(mGameManager.createComponent(game::COMPONENT_TYPE_CAMERA));
		pGameObject->attachComponent(pTransform);
		pGameObject->attachComponent(mCameras[1]);
		pTransform->setPosition(1 * ENGINE_UNIT_M, 10 * ENGINE_UNIT_M, 20 * ENGINE_UNIT_M);
		mCameras[1]->setFarClipDistance(10.0f);
		mRenderManager.addCamera(mCameras[1]);

		render::Viewport* mViewport = mWin->createViewport(mCameras[0]);

		mModels[0] = mRenderManager.createModel(mCubeMeshData);
		mModels[1] = mRenderManager.createModel(mCubeMeshData);
		mModels[0]->addChild(mModels[1]);

		mSelectedNode = mModels[0];
		mModels[0]->setVisibleAxis(true);
		mModels[0]->setVisibleBoundingBox(true);
		mModels[0]->setVisibleBoundingSphere(true);

		render::Light* pLight = mRenderManager.createLight();
		pLight->setPosition(0,0, -10 * ENGINE_UNIT_M);
		pLight->setDiffuseColor(render::Color::Red);

		physics::Body* pBody = mPhysicsManager.createBody(mPlaneBodyData);
		pBody->setEnabled(true);
		
		pBody = mPhysicsManager.createBody(mSphereBodyData);
		pBody->setPosition(0, 100, 0);

		render::Model* pModel = mRenderManager.createModel(mSphereMeshData);
		pModel->setMaterial(mMaterial);
		pModel->setParent(pBody);
		pBody->setEnabled(true);

		mCursorOverlay = mRenderManager.createPanelOverlay();
		mCursorOverlay->setMaterial("materials/Cursor.xml");
		mCursorOverlay->setPosition(0.5f, 0.5f);
		mCursorOverlay->setDimension(0.1f, 0.1f);

		mSoundManager.addListener(pListener);
		mSoundManager.setActiveListener(pListener);

		for (unsigned char i = 0; i < MAX_SOUNDS; ++i)
			mCollisionSounds[i] = mSoundManager.createSound(mSoundData);
	}

	void switchCamera()
	{
		if (mSelectedCamera == mCameras[0])
		{
			mSelectedCameraTransform->setVisibleAxis(false);
			mCameras[0]->setVisibleFrustum(false);

			mSelectedCamera = mCameras[1];
			mSelectedCameraTransform = static_cast<game::Transform*>(mSelectedCamera->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));

			mSelectedCameraTransform->setVisibleAxis(true);
			mCameras[1]->setVisibleFrustum(true);
		}
		else if (mSelectedCamera == mCameras[1])
		{
			mSelectedCameraTransform->setVisibleAxis(false);
			mCameras[1]->setVisibleFrustum(false);

			mSelectedCamera = mCameras[0];
			mSelectedCameraTransform = static_cast<game::Transform*>(mSelectedCamera->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));

			mSelectedCameraTransform->setVisibleAxis(true);
			mCameras[1]->setVisibleFrustum(true);
		}
	}

	void switchObject()
	{
		if (mSelectedNode == mModels[0])
		{
			mModels[0]->setVisibleAxis(false);
			mModels[0]->setVisibleBoundingBox(false);
			mModels[0]->setVisibleBoundingSphere(false);

			mSelectedNode = mModels[1];

			mModels[1]->setVisibleAxis(true);
			mModels[1]->setVisibleBoundingBox(true);
			mModels[1]->setVisibleBoundingSphere(true);
		}
		else if (mSelectedNode == mModels[1])
		{
			mModels[1]->setVisibleAxis(false);
			mModels[1]->setVisibleBoundingBox(false);
			mModels[1]->setVisibleBoundingSphere(false);

			mSelectedNode = mModels[0];

			mModels[0]->setVisibleAxis(true);
			mModels[0]->setVisibleBoundingBox(true);
			mModels[0]->setVisibleBoundingSphere(true);
		}
	}

	void engineInitialized()
	{
		setupInput();

		createResources();
		
		createScene();
	}
	
	void engineUninitialized()
	{
	}

	void engineStarted()
	{
	}
	
	void engineStopped()
	{
	}
	
	void engineUpdateStarted(const engine::EngineEvent& evt)
	{
		char tmp[255];
		sprintf_s(tmp,"OpenGL FPS: %.2f", mWin->getAverageFPS());

		mWin->setCaption(tmp);

		// Move about 100 units per second,
		mMoveScale = 100.0f * evt.timeSinceLastUpdate * moveMultiplier;
		// Take about 10 seconds for full rotation
		mRotScale = 36.0f * evt.timeSinceLastUpdate * rotMultiplier;
		mMouseScale = 10.0f * evt.timeSinceLastUpdate * rotMultiplier;

		if (mSelectedCameraTransform != NULL)
		{
			if (mMovements[MOVEMENT_CAMERA_MOVE_FORWARD] && !mMovements[MOVEMENT_CAMERA_MOVE_BACKWARD])
				mSelectedCameraTransform->translate(core::vector3d(0, 0, -mMoveScale));
			if (!mMovements[MOVEMENT_CAMERA_MOVE_FORWARD] && mMovements[MOVEMENT_CAMERA_MOVE_BACKWARD])
				mSelectedCameraTransform->translate(core::vector3d(0, 0, mMoveScale));
			
			if (mMovements[MOVEMENT_CAMERA_MOVE_LEFT] && !mMovements[MOVEMENT_CAMERA_MOVE_RIGHT])
				mSelectedCameraTransform->translate(core::vector3d(-mMoveScale, 0, 0));
			if (!mMovements[MOVEMENT_CAMERA_MOVE_LEFT] && mMovements[MOVEMENT_CAMERA_MOVE_RIGHT])
				mSelectedCameraTransform->translate(core::vector3d(mMoveScale, 0, 0));

			if (mMovements[MOVEMENT_CAMERA_MOVE_UP] && !mMovements[MOVEMENT_CAMERA_MOVE_DOWN])
				mSelectedCameraTransform->translate(core::vector3d(0, mMoveScale, 0));
			if (!mMovements[MOVEMENT_CAMERA_MOVE_UP] && mMovements[MOVEMENT_CAMERA_MOVE_DOWN])
				mSelectedCameraTransform->translate(core::vector3d(0, -mMoveScale, 0));
		}
		
		if (mSelectedNode != NULL)
		{
			if (mMovements[MOVEMENT_OBJECT_MOVE_FORWARD] && !mMovements[MOVEMENT_OBJECT_MOVE_BACKWARD])
				mSelectedNode->translate(core::vector3d(0, 0, -mMoveScale));
			if (!mMovements[MOVEMENT_OBJECT_MOVE_FORWARD] && mMovements[MOVEMENT_OBJECT_MOVE_BACKWARD])
				mSelectedNode->translate(core::vector3d(0, 0, mMoveScale));
			
			if (mMovements[MOVEMENT_OBJECT_MOVE_LEFT] && !mMovements[MOVEMENT_OBJECT_MOVE_RIGHT])
				mSelectedNode->translate(core::vector3d(-mMoveScale, 0, 0));
			if (!mMovements[MOVEMENT_OBJECT_MOVE_LEFT] && mMovements[MOVEMENT_OBJECT_MOVE_RIGHT])
				mSelectedNode->translate(core::vector3d(mMoveScale, 0, 0));

			if (mMovements[MOVEMENT_OBJECT_MOVE_UP] && !mMovements[MOVEMENT_OBJECT_MOVE_DOWN])
				mSelectedNode->translate(core::vector3d(0, mMoveScale, 0));
			if (!mMovements[MOVEMENT_OBJECT_MOVE_UP] && mMovements[MOVEMENT_OBJECT_MOVE_DOWN])
				mSelectedNode->translate(core::vector3d(0, -mMoveScale, 0));

			if (mMovements[MOVEMENT_OBJECT_ROTATE_LEFT] && !mMovements[MOVEMENT_OBJECT_ROTATE_RIGHT])
				mSelectedNode->rotateY(mRotScale);
			if (!mMovements[MOVEMENT_OBJECT_ROTATE_LEFT] && mMovements[MOVEMENT_OBJECT_ROTATE_RIGHT])
				mSelectedNode->rotateY(-mRotScale);

			if (mMovements[MOVEMENT_OBJECT_ROTATE_UP] && !mMovements[MOVEMENT_OBJECT_ROTATE_DOWN])
				mSelectedNode->rotateX(mRotScale);
			if (!mMovements[MOVEMENT_OBJECT_ROTATE_UP] && mMovements[MOVEMENT_OBJECT_ROTATE_DOWN])
				mSelectedNode->rotateX(-mRotScale);

			if (mMovements[MOVEMENT_OBJECT_ROLL_LEFT] && !mMovements[MOVEMENT_OBJECT_ROLL_RIGHT])
				mSelectedNode->rotateZ(mRotScale);
			if (!mMovements[MOVEMENT_OBJECT_ROLL_LEFT] && mMovements[MOVEMENT_OBJECT_ROLL_RIGHT])
				mSelectedNode->rotateZ(-mRotScale);
		}
	}
	
	void engineUpdateEnded(const engine::EngineEvent& evt)
	{
	}

	void collisionStarted(const physics::CollisionEvent& evt)
	{
		for (unsigned char i=0; i<MAX_SOUNDS; ++i)
		{
			if (!mCollisionSounds[i]->isPlaying())
			{
				mCollisionSounds[i]->setPosition(evt.mCollisionPoints[0]->mCollisionPosition);
				mCollisionSounds[i]->play();
				break;
			}
		}
	}

	void collisionUpdate(const physics::CollisionEvent& evt)
	{
	}

	void collisionEnded(const physics::CollisionEvent& evt)
	{
	}

	void keyPressed(input::KeyEvent& event)
	{
		switch(event.getKey())
		{
		case input::KEY_ESCAPE:
			mEngineManager.stop();
			break;

		case input::KEY_P:
			if (mWin->isFullScreen() == true)
				mWin->setFullscreen(false, mWin->getWidth(), mWin->getHeight());
			else
				mWin->setFullscreen(true, mWin->getWidth(), mWin->getHeight());
			break;

		case input::KEY_W:
			mMovements.set(MOVEMENT_CAMERA_MOVE_FORWARD);
			break;
		case input::KEY_S:
			mMovements.set(MOVEMENT_CAMERA_MOVE_BACKWARD);
			break;

		case input::KEY_A:
			mMovements.set(MOVEMENT_CAMERA_MOVE_LEFT);
			break;
		case input::KEY_D:
			mMovements.set(MOVEMENT_CAMERA_MOVE_RIGHT);
			break;

		case input::KEY_E:
			mMovements.set(MOVEMENT_CAMERA_MOVE_UP);
			break;
		case input::KEY_Q:
			mMovements.set(MOVEMENT_CAMERA_MOVE_DOWN);
			break;

		/////////////////////////////////////////////////////

		case input::KEY_UP:
			mMovements.set(MOVEMENT_OBJECT_MOVE_FORWARD);
			break;
		case input::KEY_DOWN:
			mMovements.set(MOVEMENT_OBJECT_MOVE_BACKWARD);
			break;

		case input::KEY_LEFT:
			mMovements.set(MOVEMENT_OBJECT_MOVE_LEFT);
			break;
		case input::KEY_RIGHT:
			mMovements.set(MOVEMENT_OBJECT_MOVE_RIGHT);
			break;

		case input::KEY_PGUP:
			mMovements.set(MOVEMENT_OBJECT_MOVE_UP);
			break;
		case input::KEY_PGDOWN:
			mMovements.set(MOVEMENT_OBJECT_MOVE_DOWN);
			break;

		case input::KEY_HOME:
			mMovements.set(MOVEMENT_OBJECT_ROTATE_UP);
			break;
		case input::KEY_END:
			mMovements.set(MOVEMENT_OBJECT_ROTATE_DOWN);
			break;

		case input::KEY_INSERT:
			mMovements.set(MOVEMENT_OBJECT_ROTATE_LEFT);
			break;
		case input::KEY_DELETE:
			mMovements.set(MOVEMENT_OBJECT_ROTATE_RIGHT);
			break;

		/////////////////////////////////////////////////////
		
		case input::KEY_NUMPAD0:
			//switch selected camera
			switchCamera();
			break;
		case input::KEY_NUMPAD1:
			//switch selected object
			switchObject();
			break;

		/////////////////////////////////////////////////////

		case input::KEY_NUMPADADD:
			if (event.isModifier(input::KEY_MODIFIER_SHIFT))
				rotMultiplier += 1.0f;
			else
				moveMultiplier += 1.0f;
			break;
		case input::KEY_NUMPADSUBTRACT:
			if (event.isModifier(input::KEY_MODIFIER_SHIFT))
			{
				rotMultiplier -= 1.0f;
				if (rotMultiplier < 0.0f)
					rotMultiplier = 0.0f;
			}
			else
			{
				moveMultiplier -= 1.0f;
				if (moveMultiplier < 0.0f)
					moveMultiplier = 0.0f;
			}
			break;
		case input::KEY_SPACE:
			{
				if (mCameras[0] != NULL && mCameras[0]->getGameObject() != NULL)
				{
					game::Transform* pTransform = static_cast<game::Transform*>(mCameras[0]->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
					if (pTransform != NULL)
					{
						physics::Body* pBody = mPhysicsManager.createBody(mBoxBodyData);
						pBody->setPosition(pTransform->getAbsolutePosition());
						pBody->setOrientation(pTransform->getAbsoluteOrientation());

						render::Model* pModel = mRenderManager.createModel(mCubeMeshData);
						pModel->setMaterial(mMaterial);
						pModel->setParent(pBody);

						pBody->setEnabled(true);
						pBody->applyLinearImpulse(core::vector3d::NEGATIVE_UNIT_Z * 100.0f * ENGINE_UNIT_M);
					}
				}
			}
			break;
		}
	}

	void keyReleased(input::KeyEvent& event)
	{
		switch(event.getKey())
		{
		case input::KEY_W:
			mMovements.reset(MOVEMENT_CAMERA_MOVE_FORWARD);
			break;
		case input::KEY_S:
			mMovements.reset(MOVEMENT_CAMERA_MOVE_BACKWARD);
			break;

		case input::KEY_A:
			mMovements.reset(MOVEMENT_CAMERA_MOVE_LEFT);
			break;
		case input::KEY_D:
			mMovements.reset(MOVEMENT_CAMERA_MOVE_RIGHT);
			break;

		case input::KEY_E:
			mMovements.reset(MOVEMENT_CAMERA_MOVE_UP);
			break;
		case input::KEY_Q:
			mMovements.reset(MOVEMENT_CAMERA_MOVE_DOWN);
			break;

		/////////////////////////////////////////////////////

		case input::KEY_UP:
			mMovements.reset(MOVEMENT_OBJECT_MOVE_FORWARD);
			break;
		case input::KEY_DOWN:
			mMovements.reset(MOVEMENT_OBJECT_MOVE_BACKWARD);
			break;

		case input::KEY_LEFT:
			mMovements.reset(MOVEMENT_OBJECT_MOVE_LEFT);
			break;
		case input::KEY_RIGHT:
			mMovements.reset(MOVEMENT_OBJECT_MOVE_RIGHT);
			break;

		case input::KEY_PGUP:
			mMovements.reset(MOVEMENT_OBJECT_MOVE_UP);
			break;
		case input::KEY_PGDOWN:
			mMovements.reset(MOVEMENT_OBJECT_MOVE_DOWN);
			break;

		case input::KEY_HOME:
			mMovements.reset(MOVEMENT_OBJECT_ROTATE_UP);
			break;
		case input::KEY_END:
			mMovements.reset(MOVEMENT_OBJECT_ROTATE_DOWN);
			break;

		case input::KEY_INSERT:
			mMovements.reset(MOVEMENT_OBJECT_ROTATE_LEFT);
			break;
		case input::KEY_DELETE:
			mMovements.reset(MOVEMENT_OBJECT_ROTATE_RIGHT);
			break;

		/////////////////////////////////////////////////////
		}
	}

	void mousePressed(input::MouseEvent& event)
	{
	}

	void mouseReleased(input::MouseEvent& event)
	{
		switch(event.getButton())
		{
		case input::MOUSE_RIGHT_BUTTON:
			if (FPSMode == false)
			{
				FPSMode = true;
				if (mCursor != NULL)
				{
					mCursor->setAutoCenter(true);
					mCursor->setPosition(core::position2d::ORIGIN_2D);
				}
				if (mCursorOverlay != NULL)
				{
					mCursorOverlay->setPosition(0.0f, 0.0f);
					mCursorOverlay->setVisible(false);
				}
			}
			else
			{
				FPSMode = false;
				if (mCursor != NULL)
				{
					mCursor->setAutoCenter(false);
					mCursor->setPosition(core::position2d::ORIGIN_2D);
				}
				if (mCursorOverlay != NULL)
				{
					mCursorOverlay->setPosition(0.0f, 0.0f);
					mCursorOverlay->setVisible(true);
				}
			}
			break;
		}
	}

	void mouseMoved(input::MouseEvent& event)
	{
		core::position2d pos = mCursor->getPosition();
		core::position2d apos = mCursor->getAbsolutePosition();

		unsigned int windowW = mWin->getWidth();
		unsigned int windowH = mWin->getHeight();

		if (pos.X < 0)
			pos.X = 0;
		if ((unsigned int)pos.X > windowW)
			pos.X = windowW;
		if (pos.Y < 0)
			pos.Y = 0;
		if ((unsigned int)pos.Y > windowH)
			pos.Y = windowH;

		float cursorPosX = (float)pos.X / windowW;
		float cursorPosY = (float)pos.Y / windowH;

		if (mCursorOverlay != NULL)
			mCursorOverlay->setPosition(cursorPosX, cursorPosY);

		signed int moveX = event.getAxisX();
		signed int moveY = event.getAxisY();

		if (FPSMode && mSelectedCameraTransform != NULL && mSelectedCamera != NULL)
		{
			if(moveY != 0)
				mSelectedCameraTransform->rotateX(-moveY * mMouseScale);
			if(moveX != 0)
			{
				if (mSelectedCamera->getFixedUp())
					mSelectedCameraTransform->rotate(-moveX * mMouseScale, mSelectedCamera->getFixedUpAxis(), game::TRANSFORM_SPACE_WORLD);
				else
					mSelectedCameraTransform->rotateY(-moveX * mMouseScale);
			}
		}
	}

	void run()
	{
		mEngineManager.initialize();
		mEngineManager.start();
		while (mEngineManager.isRunning())
		{
#if ENGINE_PLATFORM == PLATFORM_WIN32
			MSG  msg;
			while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
#endif
			mEngineManager.update(0);
		}

		mEngineManager.uninitialize();
	}

private:

	engine::EngineManager& mEngineManager;
	input::InputManager& mInputManager;
	resource::ResourceManager& mResourceManager;
	game::GameManager& mGameManager;
	render::RenderManager& mRenderManager;
	sound::SoundManager& mSoundManager;
	physics::PhysicsManager& mPhysicsManager;

	render::RenderWindow* mWin;

	input::Cursor* mCursor;

	/////////////////////////////////
	game::Transform* mSelectedCameraTransform;
	render::Camera* mSelectedCamera;
	render::Camera* mCameras[2];
	
	scene::Node* mSelectedNode;

	render::Model* mModels[2];
	
	sound::Sound* mCollisionSounds[MAX_SOUNDS];
	///////////////////////////////////

	render::MeshData* mCubeMeshData;
	render::MeshData* mSphereMeshData;
	physics::BodyData* mPlaneBodyData;
	physics::BodyData* mBoxBodyData;
	physics::BodyData* mSphereBodyData;
	sound::SoundData* mSoundData;
	render::Material* mMaterial;
	render::Font* mFont;

	render::PanelOverlay* mCursorOverlay;

	bool FPSMode;

	std::bitset<MOVEMENT_COUNT> mMovements;

	float moveMultiplier;
	float rotMultiplier;
	float mMoveScale;
	float mRotScale;
	float mMouseScale;
};

int main(int argc, char **argv)
{
	Launcher launcher;

	launcher.run();

	return 0;
}