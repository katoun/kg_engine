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

#include <Engine.h>
#include <iostream>
#include <string>

#if ENGINE_PLATFORM == PLATFORM_WINDOWS
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

struct CollisionSoundsInfo
{
	sound::Sound* mSound;
	game::Transform* mTransform;

	CollisionSoundsInfo()
	{
		mSound = nullptr;
		mTransform = nullptr;
	}
};

struct CameraInfo
{
	game::GameObject* mGameObject;
	render::Camera* mCamera;
	game::Transform* mTransform;

	CameraInfo()
	{
		mGameObject = nullptr;
		mCamera = nullptr;
		mTransform = nullptr;
	}
};

struct ModelInfo
{
	game::GameObject* mGameObject;
	render::Model* mModel;
	game::Transform* mTransform;

	ModelInfo()
	{
		mGameObject = nullptr;
		mModel = nullptr;
		mTransform = nullptr;
	}
};

class Launcher: public engine::EngineEventReceiver, public input::KeyEventReceiver, public input::MouseEventReceiver, public physics::CollisionEventReceiver
{
public:

	Launcher()
	{
		mEngineManager		= new engine::EngineManager();

		mInputManager		= input::InputManager::getInstance();
		mResourceManager	= resource::ResourceManager::getInstance();
		mGameManager		= game::GameManager::getInstance();
		mRenderManager		= render::RenderManager::getInstance();
		mSoundManager		= sound::SoundManager::getInstance();
		mPhysicsManager		= physics::PhysicsManager::getInstance();

		if (mEngineManager != nullptr) mEngineManager->addEngineEventReceiver(this);
		if (mPhysicsManager != nullptr) mPhysicsManager->addCollisionEventReceiver(this);

		mCursor			= nullptr;

		/////////////////////
		mSelectedCamera				= 0;
		mSelectedModel				= 0;

		/////////////////////

		mCubeMeshData	= nullptr;
		mSphereMeshData	= nullptr;
		mPlaneBodyData	= nullptr;
		mBoxBodyData	= nullptr;
		mSphereBodyData	= nullptr;
		mSoundData		= nullptr;
		mMaterial		= nullptr;
		mFont			= nullptr;

		FPSMode			= false;

		moveMultiplier	= 1.0f;
		rotMultiplier	= 1.0f;
		mMoveScale		= 0.0f;
		mRotScale		= 0.0f;
		mMouseScale		= 0.0f;
	}

	~Launcher()
	{
		SAFE_DELETE(mEngineManager);
	}

	void createResources()
	{
		if (mResourceManager != nullptr)
		{
			mFont			= static_cast<render::Font*>(mResourceManager->createResource(resource::RESOURCE_TYPE_FONT, "fonts/Arial.xml"));
			mMaterial		= static_cast<render::Material*>(mResourceManager->createResource(resource::RESOURCE_TYPE_RENDER_MATERIAL, "materials/ShaderDefault.xml"));
			mPlaneMeshData	= static_cast<render::MeshData*>(mResourceManager->createResource(resource::RESOURCE_TYPE_MESH_DATA, "meshes/Plane100m.xml"));
			mCubeMeshData	= static_cast<render::MeshData*>(mResourceManager->createResource(resource::RESOURCE_TYPE_MESH_DATA, "meshes/Cube1m.xml"));
			mSphereMeshData	= static_cast<render::MeshData*>(mResourceManager->createResource(resource::RESOURCE_TYPE_MESH_DATA, "meshes/Sphere1m.xml"));
			mPlaneBodyData	= static_cast<physics::BodyData*>(mResourceManager->createResource(resource::RESOURCE_TYPE_BODY_DATA, "bodies/Plane.xml"));
			mBoxBodyData	= static_cast<physics::BodyData*>(mResourceManager->createResource(resource::RESOURCE_TYPE_BODY_DATA, "bodies/Box.xml"));
			mSphereBodyData	= static_cast<physics::BodyData*>(mResourceManager->createResource(resource::RESOURCE_TYPE_BODY_DATA, "bodies/Sphere.xml"));
			mSoundData		= static_cast<sound::SoundData*>(mResourceManager->createResource(resource::RESOURCE_TYPE_SOUND_DATA, "sounds/collision.ogg"));
		}
	}

	void setupInput()
	{
		if (mInputManager != nullptr)
		{
			mCursor						= mInputManager->getCursor();
			input::Keyboard* pKeyboard	= (input::Keyboard*)mInputManager->createInputDevice(input::INPUT_TYPE_KEYBOARD, true);
			input::Mouse* pMouse		= (input::Mouse*)mInputManager->createInputDevice(input::INPUT_TYPE_MOUSE, true);
			pKeyboard->addKeyEventReceiver(this);
			pMouse->addMouseEventReceiver(this);

			if (FPSMode == true)
				mCursor->setAutoCenter(true);
		}
	}

	void createScene()
	{	
		mWin = mRenderManager->getMainWindow();

		/////////////////////////////////////////////
		mGameManager->openScene("scenes/DefaultScene.xml");

		game::GameObject* pGameObject = nullptr;
		game::Transform* pTransform = nullptr;
		render::Camera* pCamera = nullptr;
		render::Light* pLight = nullptr;
		render::Model* pModel = nullptr;
		physics::Body* pBody = nullptr;

		//Camera 0
		pGameObject = mGameManager->createGameObject();
		pTransform = static_cast<game::Transform*>(mGameManager->createComponent(game::COMPONENT_TYPE_TRANSFORM));
		pCamera = static_cast<render::Camera*>(mGameManager->createComponent(game::COMPONENT_TYPE_CAMERA));
		sound::Listener* pListener = static_cast<sound::Listener*>(mGameManager->createComponent(game::COMPONENT_TYPE_LISTENER));
		pGameObject->attachComponent(pTransform);
		pGameObject->attachComponent(pCamera);
		pGameObject->attachComponent(pListener);
		pTransform->setPosition(0, 2 * ENGINE_UNIT_M, 20 * ENGINE_UNIT_M);
		pTransform->setVisibleAxis(true);
		pCamera->setFarClipDistance(0.0f);
		pCamera->setVisibleFrustum(true);

		mCameras[0].mGameObject = pGameObject;
		mCameras[0].mTransform = pTransform;
		mCameras[0].mCamera = pCamera;
		//Camera 0

		//Camera 1
		pGameObject = mGameManager->createGameObject();
		pTransform = static_cast<game::Transform*>(mGameManager->createComponent(game::COMPONENT_TYPE_TRANSFORM));
		pCamera = static_cast<render::Camera*>(mGameManager->createComponent(game::COMPONENT_TYPE_CAMERA));
		pGameObject->attachComponent(pTransform);
		pGameObject->attachComponent(pCamera);
		pTransform->setPosition(1 * ENGINE_UNIT_M, 10 * ENGINE_UNIT_M, 20 * ENGINE_UNIT_M);
		pCamera->setFarClipDistance(10.0f);

		mCameras[1].mGameObject = pGameObject;
		mCameras[1].mTransform = pTransform;
		mCameras[1].mCamera = pCamera;
		//Camera 1

		render::Viewport* mViewport = mWin->createViewport(mCameras[0].mCamera);

		//Model 0
		pGameObject = mGameManager->createGameObject();
		pTransform = static_cast<game::Transform*>(mGameManager->createComponent(game::COMPONENT_TYPE_TRANSFORM));
		pModel = static_cast<render::Model*>(mGameManager->createComponent(game::COMPONENT_TYPE_MODEL));
		pGameObject->attachComponent(pTransform);
		pGameObject->attachComponent(pModel);
		pTransform->setVisibleAxis(true);
		pModel->setMeshData(mCubeMeshData);
		pModel->setVisibleBoundingBox(true);
		pModel->setVisibleBoundingSphere(true);

		mModels[0].mGameObject = pGameObject;
		mModels[0].mTransform = pTransform;
		mModels[0].mModel = pModel;
		//Model 0

		mSelectedModel = 0;

		//Model 1
		pGameObject = mGameManager->createGameObject();
		pTransform = static_cast<game::Transform*>(mGameManager->createComponent(game::COMPONENT_TYPE_TRANSFORM));
		pModel = static_cast<render::Model*>(mGameManager->createComponent(game::COMPONENT_TYPE_MODEL));
		pGameObject->attachComponent(pTransform);
		pGameObject->attachComponent(pModel);
		pModel->setMeshData(mCubeMeshData);

		mModels[1].mGameObject = pGameObject;
		mModels[1].mTransform = pTransform;
		mModels[1].mModel = pModel;
		
		mModels[0].mGameObject->addChild(pGameObject);
		//Model 1

		//Light 0
		pGameObject = mGameManager->createGameObject();
		pTransform = static_cast<game::Transform*>(mGameManager->createComponent(game::COMPONENT_TYPE_TRANSFORM));
		pLight = static_cast<render::Light*>(mGameManager->createComponent(game::COMPONENT_TYPE_LIGHT));
		pGameObject->attachComponent(pTransform);
		pGameObject->attachComponent(pLight);
		pTransform->setPosition(0,0, -10 * ENGINE_UNIT_M);
		pTransform->setVisibleAxis(true);
		pLight->setDiffuseColor(render::Color::Red);
		//Light 0

		//Body plane
		pGameObject = mGameManager->createGameObject();
		pTransform = static_cast<game::Transform*>(mGameManager->createComponent(game::COMPONENT_TYPE_TRANSFORM));
		pBody = static_cast<physics::Body*>(mGameManager->createComponent(game::COMPONENT_TYPE_BODY));
		pModel = static_cast<render::Model*>(mGameManager->createComponent(game::COMPONENT_TYPE_MODEL));
		pGameObject->attachComponent(pTransform);
		pGameObject->attachComponent(pBody);
		pGameObject->attachComponent(pModel);
		//pTransform->setScale(100, 0, 100);
		pBody->setBodyData(mPlaneBodyData);
		pBody->setEnabled(true);
		pModel->setMeshData(mPlaneMeshData);
		pModel->setMaterial(mMaterial);
		//Body plane

		//Body sphere
		pGameObject = mGameManager->createGameObject();
		pTransform = static_cast<game::Transform*>(mGameManager->createComponent(game::COMPONENT_TYPE_TRANSFORM));
		pBody = static_cast<physics::Body*>(mGameManager->createComponent(game::COMPONENT_TYPE_BODY));
		pModel = static_cast<render::Model*>(mGameManager->createComponent(game::COMPONENT_TYPE_MODEL));
		pGameObject->attachComponent(pTransform);
		pGameObject->attachComponent(pBody);
		pGameObject->attachComponent(pModel);
		pBody->setBodyData(mSphereBodyData);
		pBody->setEnabled(true);
		pTransform->setPosition(0, 100, 0);
		pModel->setMeshData(mSphereMeshData);
		pModel->setMaterial(mMaterial);
		//Body sphere

		mSoundManager->setActiveListener(pListener);

		for (unsigned char i = 0; i < MAX_SOUNDS; ++i)
		{
			pGameObject = mGameManager->createGameObject();
			pTransform = static_cast<game::Transform*>(mGameManager->createComponent(game::COMPONENT_TYPE_TRANSFORM));
			sound::Sound* pSound = static_cast<sound::Sound*>(mGameManager->createComponent(game::COMPONENT_TYPE_SOUND));
			pGameObject->attachComponent(pTransform);
			pGameObject->attachComponent(pSound);
			pSound->setSoundData(mSoundData);
			
			mCollisionSounds[i].mSound = pSound;
			mCollisionSounds[i].mTransform = pTransform;
		}

		/////////////////////////////////////////////

		//game::GameManager::getInstance().openScene("scenes/DefaultScene.xml");
		//game::GameManager::getInstance().createScene();
	}

	void switchCamera()
	{
		if (mSelectedCamera == 0)
		{
			mCameras[0].mTransform->setVisibleAxis(false);
			mCameras[0].mCamera->setVisibleFrustum(false);

			mSelectedCamera = 1;

			mCameras[1].mTransform->setVisibleAxis(true);
			mCameras[1].mCamera->setVisibleFrustum(true);
		}
		else if (mSelectedCamera == 1)
		{
			mCameras[1].mTransform->setVisibleAxis(false);
			mCameras[1].mCamera->setVisibleFrustum(false);

			mSelectedCamera = 0;

			mCameras[0].mTransform->setVisibleAxis(true);
			mCameras[0].mCamera->setVisibleFrustum(true);
		}
	}

	void switchObject()
	{
		if (mSelectedModel == 0)
		{
			mModels[0].mTransform->setVisibleAxis(false);
			mModels[0].mModel->setVisibleBoundingBox(false);
			mModels[0].mModel->setVisibleBoundingSphere(false);

			mSelectedModel = 1;

			mModels[1].mTransform->setVisibleAxis(true);
			mModels[1].mModel->setVisibleBoundingBox(true);
			mModels[1].mModel->setVisibleBoundingSphere(true);
		}
		else if (mSelectedModel == 1)
		{
			mModels[1].mTransform->setVisibleAxis(false);
			mModels[1].mModel->setVisibleBoundingBox(false);
			mModels[1].mModel->setVisibleBoundingSphere(false);

			mSelectedModel = 0;

			mModels[0].mTransform->setVisibleAxis(true);
			mModels[0].mModel->setVisibleBoundingBox(true);
			mModels[0].mModel->setVisibleBoundingSphere(true);
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

		if (mCameras[mSelectedCamera].mTransform != nullptr)
		{
			if (mMovements[MOVEMENT_CAMERA_MOVE_FORWARD] && !mMovements[MOVEMENT_CAMERA_MOVE_BACKWARD])
				mCameras[mSelectedCamera].mTransform->translate(core::vector3d(0, 0, -mMoveScale));
			if (!mMovements[MOVEMENT_CAMERA_MOVE_FORWARD] && mMovements[MOVEMENT_CAMERA_MOVE_BACKWARD])
				mCameras[mSelectedCamera].mTransform->translate(core::vector3d(0, 0, mMoveScale));
			
			if (mMovements[MOVEMENT_CAMERA_MOVE_LEFT] && !mMovements[MOVEMENT_CAMERA_MOVE_RIGHT])
				mCameras[mSelectedCamera].mTransform->translate(core::vector3d(-mMoveScale, 0, 0));
			if (!mMovements[MOVEMENT_CAMERA_MOVE_LEFT] && mMovements[MOVEMENT_CAMERA_MOVE_RIGHT])
				mCameras[mSelectedCamera].mTransform->translate(core::vector3d(mMoveScale, 0, 0));

			if (mMovements[MOVEMENT_CAMERA_MOVE_UP] && !mMovements[MOVEMENT_CAMERA_MOVE_DOWN])
				mCameras[mSelectedCamera].mTransform->translate(core::vector3d(0, mMoveScale, 0));
			if (!mMovements[MOVEMENT_CAMERA_MOVE_UP] && mMovements[MOVEMENT_CAMERA_MOVE_DOWN])
				mCameras[mSelectedCamera].mTransform->translate(core::vector3d(0, -mMoveScale, 0));
		}
		
		if (mModels[mSelectedModel].mTransform != nullptr)
		{
			if (mMovements[MOVEMENT_OBJECT_MOVE_FORWARD] && !mMovements[MOVEMENT_OBJECT_MOVE_BACKWARD])
				mModels[mSelectedModel].mTransform->translate(core::vector3d(0, 0, -mMoveScale));
			if (!mMovements[MOVEMENT_OBJECT_MOVE_FORWARD] && mMovements[MOVEMENT_OBJECT_MOVE_BACKWARD])
				mModels[mSelectedModel].mTransform->translate(core::vector3d(0, 0, mMoveScale));
			
			if (mMovements[MOVEMENT_OBJECT_MOVE_LEFT] && !mMovements[MOVEMENT_OBJECT_MOVE_RIGHT])
				mModels[mSelectedModel].mTransform->translate(core::vector3d(-mMoveScale, 0, 0));
			if (!mMovements[MOVEMENT_OBJECT_MOVE_LEFT] && mMovements[MOVEMENT_OBJECT_MOVE_RIGHT])
				mModels[mSelectedModel].mTransform->translate(core::vector3d(mMoveScale, 0, 0));

			if (mMovements[MOVEMENT_OBJECT_MOVE_UP] && !mMovements[MOVEMENT_OBJECT_MOVE_DOWN])
				mModels[mSelectedModel].mTransform->translate(core::vector3d(0, mMoveScale, 0));
			if (!mMovements[MOVEMENT_OBJECT_MOVE_UP] && mMovements[MOVEMENT_OBJECT_MOVE_DOWN])
				mModels[mSelectedModel].mTransform->translate(core::vector3d(0, -mMoveScale, 0));

			if (mMovements[MOVEMENT_OBJECT_ROTATE_LEFT] && !mMovements[MOVEMENT_OBJECT_ROTATE_RIGHT])
				mModels[mSelectedModel].mTransform->rotateY(mRotScale);
			if (!mMovements[MOVEMENT_OBJECT_ROTATE_LEFT] && mMovements[MOVEMENT_OBJECT_ROTATE_RIGHT])
				mModels[mSelectedModel].mTransform->rotateY(-mRotScale);

			if (mMovements[MOVEMENT_OBJECT_ROTATE_UP] && !mMovements[MOVEMENT_OBJECT_ROTATE_DOWN])
				mModels[mSelectedModel].mTransform->rotateX(mRotScale);
			if (!mMovements[MOVEMENT_OBJECT_ROTATE_UP] && mMovements[MOVEMENT_OBJECT_ROTATE_DOWN])
				mModels[mSelectedModel].mTransform->rotateX(-mRotScale);

			if (mMovements[MOVEMENT_OBJECT_ROLL_LEFT] && !mMovements[MOVEMENT_OBJECT_ROLL_RIGHT])
				mModels[mSelectedModel].mTransform->rotateZ(mRotScale);
			if (!mMovements[MOVEMENT_OBJECT_ROLL_LEFT] && mMovements[MOVEMENT_OBJECT_ROLL_RIGHT])
				mModels[mSelectedModel].mTransform->rotateZ(-mRotScale);
		}
	}
	
	void engineUpdateEnded(const engine::EngineEvent& evt)
	{
	}

	void collisionStarted(const physics::CollisionEvent& evt)
	{
		for (unsigned char i=0; i<MAX_SOUNDS; ++i)
		{
			if (mCollisionSounds[i].mSound != nullptr && mCollisionSounds[i].mTransform != nullptr && !mCollisionSounds[i].mSound->isPlaying())
			{
				mCollisionSounds[i].mTransform->setPosition(evt.mCollisionPoints[0]->mCollisionPosition);
				mCollisionSounds[i].mSound->play();
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
			mEngineManager->stop();
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
				if (mCameras[mSelectedCamera].mTransform != nullptr)
				{
					//Body box
					game::GameObject* pGameObject = mGameManager->createGameObject();
					game::Transform* pTransform = static_cast<game::Transform*>(mGameManager->createComponent(game::COMPONENT_TYPE_TRANSFORM));
					physics::Body* pBody = static_cast<physics::Body*>(mGameManager->createComponent(game::COMPONENT_TYPE_BODY));
					render::Model* pModel = static_cast<render::Model*>(mGameManager->createComponent(game::COMPONENT_TYPE_MODEL));
					pGameObject->attachComponent(pTransform);
					pGameObject->attachComponent(pBody);
					pGameObject->attachComponent(pModel);
					pTransform->setPosition(mCameras[mSelectedCamera].mTransform->getAbsolutePosition());
					pTransform->setOrientation(mCameras[mSelectedCamera].mTransform->getAbsoluteOrientation());
					pBody->setBodyData(mBoxBodyData);
					pBody->setEnabled(true);
					pBody->applyLinearImpulse(core::vector3d::NEGATIVE_UNIT_Z * 100.0f * ENGINE_UNIT_M);
					pModel->setMeshData(mCubeMeshData);
					pModel->setMaterial(mMaterial);
					//Body sphere
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
				if (mCursor != nullptr)
				{
					mCursor->setAutoCenter(true);
					mCursor->setPosition(core::position2d::ORIGIN_2D);
				}
			}
			else
			{
				FPSMode = false;
				if (mCursor != nullptr)
				{
					mCursor->setAutoCenter(false);
					mCursor->setPosition(core::position2d::ORIGIN_2D);
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

		if (pos.x < 0)
			pos.x = 0;
		if ((unsigned int)pos.x > windowW)
			pos.x = windowW;
		if (pos.y < 0)
			pos.y = 0;
		if ((unsigned int)pos.y > windowH)
			pos.y = windowH;

		float cursorPosX = (float)pos.x / windowW;
		float cursorPosY = (float)pos.y / windowH;

		int moveX = event.getAxisX();
		int moveY = event.getAxisY();

		if (FPSMode && mCameras[mSelectedCamera].mTransform != nullptr && mCameras[mSelectedCamera].mCamera != nullptr)
		{
			if(moveY != 0)
				mCameras[mSelectedCamera].mTransform->rotateX(-moveY * mMouseScale);
			if(moveX != 0)
			{
				if (mCameras[mSelectedCamera].mCamera->getFixedUp())
					mCameras[mSelectedCamera].mTransform->rotate(-moveX * mMouseScale, mCameras[mSelectedCamera].mCamera->getFixedUpAxis(), game::TRANSFORM_SPACE_WORLD);
				else
					mCameras[mSelectedCamera].mTransform->rotateY(-moveX * mMouseScale);
			}
		}
	}

	void run()
	{
		mEngineManager->initialize();
		mEngineManager->start();
		while (mEngineManager->isRunning())
		{
#if ENGINE_PLATFORM == PLATFORM_WINDOWS
			MSG  msg;
			while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
#endif
			mEngineManager->update(0);
		}

		mEngineManager->uninitialize();
	}

private:

	engine::EngineManager*		mEngineManager;
	input::InputManager*		mInputManager;
	resource::ResourceManager*	mResourceManager;
	game::GameManager*			mGameManager;
	render::RenderManager*		mRenderManager;
	sound::SoundManager*		mSoundManager;
	physics::PhysicsManager*	mPhysicsManager;

	render::RenderWindow* mWin;

	input::Cursor* mCursor;

	/////////////////////////////////
	unsigned int mSelectedCamera;
	CameraInfo mCameras[2];
	
	unsigned int mSelectedModel;
	ModelInfo mModels[2];
	
	CollisionSoundsInfo mCollisionSounds[MAX_SOUNDS];
	///////////////////////////////////

	render::MeshData* mPlaneMeshData;
	render::MeshData* mCubeMeshData;
	render::MeshData* mSphereMeshData;
	physics::BodyData* mPlaneBodyData;
	physics::BodyData* mBoxBodyData;
	physics::BodyData* mSphereBodyData;
	sound::SoundData* mSoundData;
	render::Material* mMaterial;
	render::Font* mFont;

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