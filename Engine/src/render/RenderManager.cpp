/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

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

#include <core/Log.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <render/RenderManager.h>
#include <render/RenderDriver.h>
#include <render/Frustum.h>
#include <render/Light.h>
#include <render/LightFactory.h>
#include <render/Camera.h>
#include <render/CameraFactory.h>
#include <render/Material.h>
#include <render/MaterialFactory.h>
#include <render/Model.h>
#include <render/ModelFactory.h>
#include <render/RenderDefines.h>
#include <render/VertexBuffer.h>
#include <render/IndexBuffer.h>
#include <render/VertexIndexData.h>
#include <render/MeshData.h>
#include <render/MeshDataFactory.h>
#include <render/Shader.h>
#include <render/Font.h>
#include <render/FontFactory.h>
#include <render/Viewport.h>
#include <render/RenderWindow.h>
#include <render/FrameEventReceiver.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>
#include <game/GameObject.h>
#include <game/ComponentDefines.h>
#include <game/ComponentFactory.h>
#include <game/Transform.h>
#include <game/GameManager.h>
#include <engine/EngineSettings.h>

#include <algorithm>

template<> render::RenderManager& core::Singleton<render::RenderManager>::ms_Singleton = render::RenderManager();

namespace render
{

std::list<FrameEventReceiver*> RenderManager::mFrameEventReceivers;

RenderManager::RenderManager(): core::System("RenderManager")
{
	mDefaultFontFactory = new FontFactory();
	mDefaultMaterialFactory = new MaterialFactory();
	mDefaultMeshDataFactory = new MeshDataFactory();
	mDefaultCameraFactory = new CameraFactory();
	mDefaultLightFactory = new LightFactory();
	mDefaultModelFactory = new ModelFactory();

	mMainWindow = NULL;
	mRenderDriver = NULL;
	mCurrentViewport = NULL;

	mAmbientLight = Color::Black;

	mDefaultMaterial = NULL;

	mFrustum = NULL;

	mFogMode = FM_NONE;
	mFogColor = Color::White;
	mFogDensity = 0.0f;
	mFogStart = 0.0f;
	mFogEnd = 0.0f;

	mLastViewportWidth = 0;
	mLastViewportHeight = 0;

	mVertexCount = 0;
	mFaceCount = 0;
}

RenderManager::~RenderManager()
{
	if (mDefaultFontFactory != NULL)
	{
		delete mDefaultFontFactory;
	}
	if (mDefaultMaterialFactory != NULL)
	{
		delete mDefaultMaterialFactory;
	}
	if (mDefaultMeshDataFactory != NULL)
	{
		delete mDefaultMeshDataFactory;
	}
	if (mDefaultCameraFactory != NULL)
	{
		delete mDefaultCameraFactory;
	}
	if (mDefaultLightFactory != NULL)
	{
		delete mDefaultLightFactory;
	}
	if (mDefaultModelFactory != NULL)
	{
		delete mDefaultModelFactory;
	}
}

RenderWindow* RenderManager::createRenderWindow(signed int width, signed int height, signed int colorDepth, bool fullScreen, signed int left, signed int top, bool depthBuffer, void* windowId)
{
	if (!mRenderDriver) return NULL;

	RenderWindow* win = mRenderDriver->createRenderWindow(width, height, colorDepth, fullScreen, left, top, depthBuffer, windowId);

	std::string message = "RenderWindow Created --> " + core::intToString(width) + ", " + core::intToString(height) + ", " + core::intToString(colorDepth) + ".";
	core::Log::getInstance().logMessage("RenderManager", message);

	mRenderWindows[win->getID()] = win;

	return win;
}

void RenderManager::setMainWindow(RenderWindow* window)
{
	assert(window != NULL);
	if (window == NULL)
		return;
	
	mMainWindow = window;
}

RenderWindow* RenderManager::getMainWindow()
{
	return mMainWindow;
}

RenderWindow* RenderManager::getRenderWindow(const unsigned int& id)
{
	std::map<unsigned int, RenderWindow*>::const_iterator i = mRenderWindows.find(id);
	if (i != mRenderWindows.end())
		return i->second;

	return NULL;
}

void RenderManager::removeRenderWindow(const unsigned int& id)
{
	std::map<unsigned int, RenderWindow*>::iterator i = mRenderWindows.find(id);
	if (i != mRenderWindows.end())
	{
		delete i->second;
		mRenderWindows.erase(i);
	}
}

void RenderManager::removeAllRenderWindows()
{
	std::map<unsigned int, RenderWindow*>::iterator i; 
	for (i = mRenderWindows.begin(); i != mRenderWindows.end(); ++i)
	{
		delete i->second;
		i->second = NULL;
	}

	mRenderWindows.clear();
}

void RenderManager::addFrameEventReceiver(FrameEventReceiver* newEventReceiver)
{
	mFrameEventReceivers.push_back(newEventReceiver);
}

void RenderManager::removeFrameEventReceiver(FrameEventReceiver* oldEventReceiver)
{
	std::list<FrameEventReceiver*>::iterator i;
	for (i = mFrameEventReceivers.begin(); i != mFrameEventReceivers.end(); ++i)
	{
		if ((*i) == oldEventReceiver)
		{
			mFrameEventReceivers.erase(i);
			return;
		}
	}
}

signed int RenderManager::getViewportHeight() const
{
	return mLastViewportHeight;
}

signed int RenderManager::getViewportWidth() const
{
	return mLastViewportWidth;
}

void RenderManager::addLight(Light *light)
{
	if (light == NULL)
		return;

	mLights[light->getID()] = light;
}

void RenderManager::removeLight(Light *light)
{
	if (light == NULL)
		return;

	removeLight(light->getID());
}

void RenderManager::removeLight(const unsigned int& id)
{
	std::map<unsigned int, Light*>::iterator i = mLights.find(id);
	if (i != mLights.end())
		mLights.erase(i);
}

void RenderManager::removeAllLights()
{
	mLights.clear();
}

void RenderManager::addCamera(Camera* camera)
{
	if (camera == NULL)
		return;

	mCameras[camera->getID()] = camera;
}

void RenderManager::removeCamera(Camera* camera)
{
	if (camera == NULL)
		return;

	removeCamera(camera->getID());
}

void RenderManager::removeCamera(const unsigned int& id)
{
	std::map<unsigned int, Camera*>::iterator i = mCameras.find(id);
	if (i != mCameras.end())
		mCameras.erase(i);
}

void RenderManager::removeAllCameras()
{
	mCameras.clear();
}

void RenderManager::addModel(Model* model)
{
	if (model == NULL)
		return;

	mModels[model->getID()] = model;
}

void RenderManager::removeModel(Model* model)
{
	if (model == NULL)
		return;

	removeModel(model->getID());
}

void RenderManager::removeModel(const unsigned int& id)
{
	std::map<unsigned int, Model*>::iterator i = mModels.find(id);
	if (i != mModels.end())
		mModels.erase(i);
}

void RenderManager::removeAllModels()
{
	mModels.clear();
}

void RenderManager::addUpdatedViewport(Viewport* viewport)
{
	if (viewport == NULL)
		return;

	mUpdatedViewports.push_back(viewport);
}

Font* RenderManager::createFont(const std::string& filename)
{
	Font* newFont = static_cast<Font*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_FONT, filename));
	if (newFont == NULL)
		return NULL;

	mFonts[newFont->getID()] = newFont;

	return newFont;
}

void RenderManager::removeFont(Font* font)
{
	if (font == NULL)
		return;

	removeFont(font->getID());
}

void RenderManager::removeFont(const unsigned int& id)
{
	std::map<unsigned int, Font*>::iterator i = mFonts.find(id);
	if (i != mFonts.end())
		mFonts.erase(i);
}

void RenderManager::removeAllFonts()
{
	mFonts.clear();
}

Shader* RenderManager::createShader(const std::string& shaderFilename, const ShaderType& type)
{
	// Get shader (load if required)
	Shader* newShader = static_cast<Shader*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_SHADER, shaderFilename));
	if (newShader == NULL)
		return NULL;

	newShader->setShaderType(type);
	mShaders[newShader->getID()] = newShader;

	return newShader;
}

Shader* RenderManager::getShader(const unsigned int& id)
{
	std::map<unsigned int, Shader*>::const_iterator i = mShaders.find(id);
	if (i != mShaders.end())
		return i->second;

	return NULL;
}

unsigned int RenderManager::getNumberOfShaders() const
{
	return mShaders.size();
}

void RenderManager::removeShader(Shader* shader)
{
	if (shader == NULL)
		return;

	removeShader(shader->getID());
}

void RenderManager::removeShader(const unsigned int& id)
{
	std::map<unsigned int, Shader*>::iterator i = mShaders.find(id);
	if (i != mShaders.end())
	{
		delete i->second;
		mShaders.erase(i);
	}
}

void RenderManager::removeAllShaders()
{
	mShaders.clear();
}

VertexBuffer* RenderManager::createVertexBuffer(unsigned int vertexSize, unsigned int numVertices, resource::BufferUsage usage)
{
	if (mRenderDriver)
	{
		VertexBuffer* buf = mRenderDriver->createVertexBuffer(vertexSize, numVertices, usage);
		if (buf)
			mVertexBuffers.push_back(buf);

		return buf;
	}
	return NULL;
}

void RenderManager::removeVertexBuffer(VertexBuffer* buf)
{
	std::list<VertexBuffer*>::iterator i;
	for (i = mVertexBuffers.begin(); i != mVertexBuffers.end(); ++i)
	{
		if ((*i) == buf)
		{
			mVertexBuffers.erase(i);

			if (mRenderDriver)
				mRenderDriver->removeVertexBuffer((*i));
			return;
		}
	}
}

void RenderManager::removeAllVertexBuffers()
{
	std::list<VertexBuffer*>::iterator i;
	for (i = mVertexBuffers.begin(); i != mVertexBuffers.end(); ++i)
	{
		if (mRenderDriver)
			mRenderDriver->removeVertexBuffer((*i));
	}
	mVertexBuffers.clear();
}

IndexBuffer* RenderManager::createIndexBuffer(IndexType idxType, unsigned int numIndexes, resource::BufferUsage usage)
{
	if (mRenderDriver)
	{
		IndexBuffer* buf = mRenderDriver->createIndexBuffer(idxType, numIndexes, usage);
		if (buf)
			mIndexBuffers.push_back(buf);

		return buf;
	}
	return NULL;
}

void RenderManager::removeIndexBuffer(IndexBuffer* buf)
{
	std::list<IndexBuffer*>::iterator i;
	for (i = mIndexBuffers.begin(); i != mIndexBuffers.end(); ++i)
	{
		if ((*i) == buf)
		{
			mIndexBuffers.erase(i);

			if (mRenderDriver)
				mRenderDriver->removeIndexBuffer((*i));
			return;
		}
	}
}

void RenderManager::removeAllIndexBuffers()
{
	std::list<IndexBuffer*>::iterator i;
	for (i = mIndexBuffers.begin(); i != mIndexBuffers.end(); ++i)
	{
		if (mRenderDriver)
			mRenderDriver->removeIndexBuffer((*i));
	}
	mIndexBuffers.clear();
}

VertexDeclaration* RenderManager::createVertexDeclaration()
{
	VertexDeclaration* decl = new VertexDeclaration();
	mVertexDeclarations.push_back(decl);
	return decl;
}

void RenderManager::removeVertexDeclaration(VertexDeclaration* decl)
{
	std::list<VertexDeclaration*>::iterator i;
	for (i = mVertexDeclarations.begin(); i != mVertexDeclarations.end(); ++i)
	{
		if ((*i) == decl)
		{
			delete decl;
			mVertexDeclarations.erase(i);
			return;
		}
	}
}

void RenderManager::removeAllVertexDeclarations()
{
	std::list<VertexDeclaration*>::iterator i;
	for (i = mVertexDeclarations.begin(); i != mVertexDeclarations.end(); ++i)
		delete (*i);

	mVertexDeclarations.clear();
}

VertexBufferBinding* RenderManager::createVertexBufferBinding()
{
	VertexBufferBinding* binding = new VertexBufferBinding();
	mVertexBufferBindings.push_back(binding);
	return binding;
}

void RenderManager::removeVertexBufferBinding(VertexBufferBinding* binding)
{
	std::list<VertexBufferBinding*>::iterator i;
	for (i = mVertexBufferBindings.begin(); i != mVertexBufferBindings.end(); ++i)
	{
		if ((*i) == binding)
		{
			delete binding;
			mVertexBufferBindings.erase(i);
			return;
		}
	}
}

void RenderManager::removeAllVertexBufferBindings()
{
	std::list<VertexBufferBinding*>::iterator i;
	for (i = mVertexBufferBindings.begin(); i != mVertexBufferBindings.end(); ++i)
		delete (*i);

	mVertexBufferBindings.clear();
}

void RenderManager::setAmbientLight(const Color& ambient)
{
	mAmbientLight = ambient;
}

void RenderManager::setFog(FogMode mode, const Color& color, float density, float start, float end)
{
	mFogMode = mode;
	mFogColor = color;
	mFogDensity = density;
	mFogStart = start;
	mFogEnd = end;
}

void RenderManager::convertProjectionMatrix(const core::matrix4& matrix, core::matrix4& dest)
{
	if (mRenderDriver)
		mRenderDriver->convertProjectionMatrix( matrix, dest);
}

float RenderManager::getMinimumDepthInputValue()
{
	if (mRenderDriver)
		return mRenderDriver->getMinimumDepthInputValue();

	return -1.0f;
}

float RenderManager::getMaximumDepthInputValue()
{
	if (mRenderDriver)
		return mRenderDriver->getMaximumDepthInputValue();

	return 1.0f;
}

float RenderManager::getHorizontalTexelOffset()
{
	if (mRenderDriver)
		return mRenderDriver->getHorizontalTexelOffset();

	return 0.0f;
}

float RenderManager::getVerticalTexelOffset()
{
	if (mRenderDriver)
		return mRenderDriver->getVerticalTexelOffset();

	return 0.0f;
}

void RenderManager::initializeImpl()
{
	mDefaultMaterial = static_cast<Material*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_RENDER_MATERIAL, "materials/DefaultMaterial.xml"));

	mSolidModels.reserve(1024);
	mTransparentModels.reserve(1024);
	mDistanceLights.reserve(512);
}

void RenderManager::uninitializeImpl()
{
	// Remove all Lights
	removeAllLights();

	// Remove all Cameras
	removeAllCameras();

	// Remove all Models
	removeAllModels();

	// Remove all Fonts
	removeAllFonts();

	// Remove all Render Windows
	removeAllRenderWindows();

	// Remove all Vertex Declarations
	removeAllVertexDeclarations();

	// Remove all VertexBuffer Bindings
	removeAllVertexBufferBindings();

	//! Removes all Vertex Buffers.
	removeAllVertexBuffers();

	//! Removes all Index Buffers.
	removeAllIndexBuffers();

	// Remove SolidModels
	mSolidModels.clear();
	// Remove TransparentModels
	mTransparentModels.clear();
	// Remove DistanceLights
	mDistanceLights.clear();

	mLightsAffectingFrustum.clear();		

	mMainWindow = NULL;
	mFrustum = NULL;
}

void RenderManager::startImpl()
{
	// Init stats
	std::map<unsigned int, RenderWindow*>::const_iterator i;
	for (i = mRenderWindows.begin(); i != mRenderWindows.end(); ++i)
		i->second->resetStatistics();
}

void RenderManager::stopImpl() {}

void RenderManager::updateImpl(float elapsedTime)
{	
	fireFrameStarted();

	// Update all render elements

	// Update RenderWindows
	std::map<unsigned int, RenderWindow*>::const_iterator j;
	for (j = mRenderWindows.begin(); j != mRenderWindows.end(); ++j)
	{
		RenderWindow* window = j->second;

		if(window != NULL && window->isActive())
		{
			window->update(elapsedTime);

			//Go through the viewports of this RenderTarget
			std::list<Viewport*>::const_iterator k;
			for (k = window->getViewports().begin(); k != window->getViewports().end(); ++k)
			{
				Viewport* pViewport = (*k);
				if (pViewport != NULL && pViewport->isVisible())
					render(pViewport->getCamera(), pViewport);
			}
		}
	}

	fireFrameEnded();
}

void RenderManager::setSystemDriverImpl(core::SystemDriver* systemDriver)
{
	mRenderDriver = static_cast<RenderDriver*>(systemDriver);
}

void RenderManager::removeSystemDriverImpl()
{
	mRenderDriver = NULL;
}

void RenderManager::registerDefaultFactoriesImpl()
{
	resource::ResourceManager::getInstance().registerResourceFactory(resource::RESOURCE_TYPE_FONT, mDefaultFontFactory);
	resource::ResourceManager::getInstance().registerResourceFactory(resource::RESOURCE_TYPE_RENDER_MATERIAL, mDefaultMaterialFactory);
	resource::ResourceManager::getInstance().registerResourceFactory(resource::RESOURCE_TYPE_MESH_DATA, mDefaultMeshDataFactory);

	game::GameManager::getInstance().registerComponentFactory(game::COMPONENT_TYPE_CAMERA, mDefaultCameraFactory);
	game::GameManager::getInstance().registerComponentFactory(game::COMPONENT_TYPE_LIGHT, mDefaultLightFactory);
	game::GameManager::getInstance().registerComponentFactory(game::COMPONENT_TYPE_MODEL, mDefaultModelFactory);
}

void RenderManager::removeDefaultFactoriesImpl()
{
	resource::ResourceManager::getInstance().removeResourceFactory(resource::RESOURCE_TYPE_FONT);
	resource::ResourceManager::getInstance().removeResourceFactory(resource::RESOURCE_TYPE_RENDER_MATERIAL);
	resource::ResourceManager::getInstance().removeResourceFactory(resource::RESOURCE_TYPE_MESH_DATA);

	game::GameManager::getInstance().removeComponentFactory(game::COMPONENT_TYPE_CAMERA);
	game::GameManager::getInstance().removeComponentFactory(game::COMPONENT_TYPE_LIGHT);
	game::GameManager::getInstance().removeComponentFactory(game::COMPONENT_TYPE_MODEL);
}

void RenderManager::fireFrameStarted()
{	
	// Do frame start event
	std::list<FrameEventReceiver*>::const_iterator i;
	for (i = mFrameEventReceivers.begin(); i != mFrameEventReceivers.end(); ++i)
	{
		(*i)->frameStarted();
	}
}

void RenderManager::fireFrameEnded()
{		
	// Do frame ended event
	std::list<FrameEventReceiver*>::const_iterator i;
	for (i = mFrameEventReceivers.begin(); i != mFrameEventReceivers.end(); ++i)
	{
		(*i)->frameEnded();
	}
}

void RenderManager::beginGeometryCount()
{
	mVertexCount = 0;
	mFaceCount = 0;
}

void RenderManager::addGeometruCount(Model* model)
{
	if (model == NULL)
		return;
	
	// Update stats
	if (model->getVertexData() == NULL || model->getIndexData() == NULL)
		return;

	unsigned int val = model->getIndexData()->indexCount;
	
	switch(model->getRenderOperationType())
	{
	case ROT_TRIANGLE_LIST:
		mFaceCount += val / 3;
		break;
	case ROT_TRIANGLE_STRIP:
	case ROT_TRIANGLE_FAN:
		mFaceCount += val - 2;
		break;
	case ROT_POINT_LIST:
	case ROT_LINE_LIST:
	case ROT_LINE_STRIP:
		break;
	}

	mVertexCount += model->getVertexData()->vertexCount;
}

unsigned int RenderManager::getVertexCount()
{
	return mVertexCount;
}

unsigned int RenderManager::getFaceCount()
{
	return mFaceCount;
}

void RenderManager::endGeometryCount() {}

void RenderManager::render(Camera* camera, Viewport* viewport)
{
	if (camera == NULL)
		return;
	
	if (viewport == NULL)
		return;

	setCurrentViewport(viewport);

	mShaderParamData.setCurrentViewport(viewport);

	mRenderDriver->setAmbientLight(mAmbientLight.R, mAmbientLight.G, mAmbientLight.B, mAmbientLight.A);

	if (mLastViewportWidth != viewport->getActualWidth() || mLastViewportHeight != viewport->getActualHeight())
	{
		mLastViewportWidth = viewport->getActualWidth();
		mLastViewportHeight = viewport->getActualHeight();
	}

	findLightsAffectingFrustum(camera);

	findVisibleModels(camera);

	beginGeometryCount();

	beginFrame(viewport);

	mShaderParamData.setCurrentCamera(camera);

	mRenderDriver->setProjectionMatrix(camera->getProjectionMatrixRS());
	mRenderDriver->setViewMatrix(camera->getViewMatrix());

	mRenderDriver->setWorldMatrix(core::matrix4::IDENTITY);//it seems this resets the world matrix before transforming the geometry

	setMaterial(mDefaultMaterial);

	mRenderDriver->setLightingEnabled(false);
	
	mRenderDriver->renderGridPlane(100 * ENGINE_UNIT_M);
	
	mRenderDriver->disableTextureUnitsFrom(0);
	mRenderDriver->renderWorldAxes();

	// Go through all the cameras
	std::map<unsigned int, Camera*>::const_iterator ci;
	for (ci = mCameras.begin(); ci != mCameras.end(); ++ci)
	{
		Camera* pCamera = ci->second;
		if (pCamera != NULL && pCamera->getID() != camera->getID())
		{
			game::Transform* pTransform = NULL;
			if (pCamera->getGameObject() != NULL)
			{
				pTransform = static_cast<game::Transform*>(pCamera->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			}

			if ((pTransform != NULL && pTransform->getVisibleAxis()) || pCamera->getVisibleFrustum())
			{
				mRenderDriver->unbindShader(SHADER_TYPE_VERTEX);
				mRenderDriver->unbindShader(SHADER_TYPE_FRAGMENT);
				mRenderDriver->unbindShader(SHADER_TYPE_GEOMETRY);

				mRenderDriver->setDepthBufferCheckEnabled(true);
				mRenderDriver->setDepthBufferWriteEnabled(true);
				mRenderDriver->disableTextureUnitsFrom(0);
				mRenderDriver->setLightingEnabled(false);
				mRenderDriver->setSceneBlending(SBF_ONE, SBF_ZERO);
				mRenderDriver->setFog(FM_NONE);

				mRenderDriver->setWorldMatrix(core::matrix4::IDENTITY);

				if (pCamera->getVisibleFrustum())
				{
					mFrustum = pCamera->getFrustum();
					if (mFrustum != NULL)
						mRenderDriver->renderFrustumVolume(mFrustum->getCorners());
				}
				if (pTransform != NULL && pTransform->getVisibleAxis())
				{
					mRenderDriver->renderAxes(pTransform->getAbsolutePosition(), pTransform->getAbsoluteOrientation());
				}
			}
		}
	}

	// Go through all the lights
	std::map<unsigned int, Light*>::const_iterator li;
	for (li = mLights.begin(); li != mLights.end(); ++li)
	{
		Light* pLight = li->second;
		if (pLight != NULL)
		{
			game::Transform* pTransform = NULL;
			if (pLight->getGameObject() != NULL)
			{
				pTransform = static_cast<game::Transform*>(pLight->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			}

			if (pTransform != NULL && pTransform->getVisibleAxis())
			{
				mRenderDriver->unbindShader(SHADER_TYPE_VERTEX);
				mRenderDriver->unbindShader(SHADER_TYPE_FRAGMENT);
				mRenderDriver->unbindShader(SHADER_TYPE_GEOMETRY);

				mRenderDriver->setDepthBufferCheckEnabled(true);
				mRenderDriver->setDepthBufferWriteEnabled(true);
				mRenderDriver->disableTextureUnitsFrom(0);
				mRenderDriver->setLightingEnabled(false);
				mRenderDriver->setSceneBlending(SBF_ONE, SBF_ZERO);
				mRenderDriver->setFog(FM_NONE);

				mRenderDriver->setWorldMatrix(core::matrix4::IDENTITY);

				mRenderDriver->renderAxes(pTransform->getAbsolutePosition(), pTransform->getAbsoluteOrientation());
			}
		}
	}
	
	renderVisibleModels();

	mRenderDriver->setViewMatrix(core::matrix4::IDENTITY);
	
	endFrame();

	endGeometryCount();

	// Notify viewport of rendered faces
	viewport->notifyRenderedFaces(getFaceCount());

#ifdef _DEBUG
	//std::cout<<"Vertex Count: "<<getVertexCount()<<std::endl;
	//std::cout<<"Face Count: "<<getFaceCount()<<std::endl;
#endif
}

void RenderManager::setCurrentViewport(Viewport* viewport)
{
	if (viewport == NULL)
		return;

	bool viewportChanged = false;

	// Check if viewport is different
	if (viewport == mCurrentViewport)
	{
		std::list<Viewport*>::const_iterator i;
		for (i = mUpdatedViewports.begin(); i != mUpdatedViewports.end(); ++i)
		{
			Viewport* pViewport = (*i);
			if (pViewport != NULL && pViewport == viewport)
			{
				mUpdatedViewports.erase(i);
				viewportChanged = true;
				break;
			}
		}
	}
	else
	{
		viewportChanged = true;
	}
	
	if (viewportChanged)
	{
		mCurrentViewport = viewport;

		if (mRenderDriver != NULL)
		{
			mRenderDriver->setViewport(viewport);
		}
	}
}

void RenderManager::beginFrame(Viewport* viewport)
{
	if (viewport == NULL)
	{
		core::Log::getInstance().logMessage("RenderManager", "::beginFrame() Cannot begin frame - no viewport selected.", core::LOG_LEVEL_ERROR);
		return;
	}
	
	// Clear the viewport if required
	mRenderDriver->beginFrame(viewport);
}

void RenderManager::findLightsAffectingFrustum(Camera* camera)
{
	if (camera == NULL)
		return;
	
	mLightsAffectingFrustum.clear();

	mFrustum = camera->getFrustum();

	core::sphere3d sphere;
	std::map<unsigned int, Light*>::const_iterator li;
	for (li = mLights.begin(); li != mLights.end(); ++li)
	{
		mLightsAffectingFrustum.push_back(li->second);//Katoun TEMP for testings!!!

		//////////////////////////////////////////////////////////////////////////

		//if (li->second->getLightType() == LIGHT_TYPE_DIRECTIONAL)
		//{
		//	mLightsAffectingFrustum.push_back(li->second);
		//}
		//else
		//{
		//	// Just see if the lights attenuation range is within the frustum
		//	sphere.set(li->second->getAbsolutePosition(), li->second->getAttenuationRange());
		//	
		//	if (mFrustum->isVisible(sphere))
		//	{
		//		mLightsAffectingFrustum.push_back(li->second);
		//	}
		//}
	}
}

void RenderManager::findLightsAffectingModel(Model* model)
{
	if (model == NULL)
		return;
	
	mDistanceLights.clear();

	std::list<Light*>::const_iterator i;
	for (i = mLightsAffectingFrustum.begin(); i!= mLightsAffectingFrustum.end(); ++i)
	{
		Light* pLight = (*i);
		if (pLight != NULL)
		{
			if (pLight->getLightType() == LIGHT_TYPE_DIRECTIONAL)
			{
				DistanceLight newLight;
				newLight.light = pLight;
				newLight.distance = 0;
				// No distance
				mDistanceLights.push_back(newLight);
			}
			else
			{
				// Calculate squared distance
				float distance = 0.0f;
				game::Transform* pLightTransform = NULL;
				game::Transform* pModelTransform = NULL;
				if (pLight->getGameObject() != NULL && model->getGameObject() != NULL)
				{
					pLightTransform = static_cast<game::Transform*>(pLight->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
					pModelTransform = static_cast<game::Transform*>(model->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
					if (pLightTransform != NULL && pModelTransform)
					{
						distance = (pLightTransform->getAbsolutePosition() - pModelTransform->getAbsolutePosition()).getLengthSQ();
					}
				}

				// only add in-range lights
				float range = pLight->getAttenuationRange();
				float maxDist = range + model->getBoundingSphere().Radius;
				//if (distance <= (maxDist * maxDist))//Katoun TEMP REM for testing!!!
				{
					DistanceLight newLight;
					newLight.light = pLight;
					newLight.distance = distance;
					mDistanceLights.push_back(newLight);
				}
			}
		}	
	}

	std::sort(mDistanceLights.begin(), mDistanceLights.end());
}

void RenderManager::findVisibleModels(Camera* camera)
{
	if (camera == NULL)
		return;

	mSolidModels.clear();
	mTransparentModels.clear();

	mFrustum = camera->getFrustum();

	// Go through all the models
	std::map<unsigned int, Model*>::const_iterator i;
	for (i = mModels.begin(); i != mModels.end(); ++i)
	{
		Model* pModel = i->second;
		if (pModel != NULL)
		{
			if (mFrustum->isVisible(pModel->getBoundingSphere()) && mFrustum->isVisible(pModel->getBoundingBox()))
			{
				Material* pMaterial = pModel->getMaterial();

				if (pMaterial != NULL)
				{
					if (pMaterial->isTransparent())
					{
						TransparentModel newTransparentModel;
						newTransparentModel.model = pModel;
						newTransparentModel.distance = 0.0f;

						game::Transform* pCameraTransform = NULL;
						game::Transform* pModelTransform = NULL;
						if (camera->getGameObject() != NULL && pModel->getGameObject() != NULL)
						{
							pCameraTransform = static_cast<game::Transform*>(camera->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
							pModelTransform = static_cast<game::Transform*>(pModel->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
							if (pCameraTransform != NULL && pModelTransform != NULL)
							{
								// Calculate squared distance
								newTransparentModel.distance = (pModelTransform->getAbsolutePosition() - pCameraTransform->getAbsolutePosition()).getLengthSQ();
							}
						}

						mTransparentModels.push_back(newTransparentModel);
					}
					else
					{
						SolidModel newSolidModel;
						newSolidModel.model = pModel;
						newSolidModel.materialID = mDefaultMaterial->getID();
						mSolidModels.push_back(newSolidModel);
					}
				}
				else
				{
					SolidModel newSolidModel;
					newSolidModel.model = pModel;
					newSolidModel.materialID = mDefaultMaterial->getID();
					mSolidModels.push_back(newSolidModel);
				}
			}
		}
	}

	std::sort(mSolidModels.begin(), mSolidModels.end());
	std::sort(mTransparentModels.begin(), mTransparentModels.end());
}

void RenderManager::renderVisibleModels()
{
	// Go through all the solid models	
	for (unsigned int i = 0; i < mSolidModels.size(); ++i)
	{
		renderSingleModel(mSolidModels[i].model);
	}

	// Go through all the transparent models
	for (unsigned int i = 0; i < mTransparentModels.size(); ++i)
	{
		renderSingleModel(mTransparentModels[i].model);
	}
}

void RenderManager::renderSingleModel(Model* model)
{
	if (model == NULL)
		return;

	if (model->getGameObject() != NULL)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(model->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != NULL)
		{
			if (pTransform->getVisibleAxis() || model->getVisibleBoundingBox() || model->getVisibleBoundingSphere())
			{
				mRenderDriver->unbindShader(SHADER_TYPE_VERTEX);
				mRenderDriver->unbindShader(SHADER_TYPE_FRAGMENT);
				mRenderDriver->unbindShader(SHADER_TYPE_GEOMETRY);

				mRenderDriver->setDepthBufferCheckEnabled(true);
				mRenderDriver->setDepthBufferWriteEnabled(true);
				mRenderDriver->disableTextureUnitsFrom(0);
				mRenderDriver->setLightingEnabled(false);
				mRenderDriver->setSceneBlending(SBF_ONE, SBF_ZERO);
				mRenderDriver->setFog(FM_NONE);

				mRenderDriver->setWorldMatrix(core::matrix4::IDENTITY);

				if (pTransform->getVisibleAxis())
					mRenderDriver->renderAxes(pTransform->getAbsolutePosition(), pTransform->getAbsoluteOrientation());
				if (model->getVisibleBoundingBox())
					mRenderDriver->renderBoundingBox(model->getBoundingBox());
				if (model->getVisibleBoundingSphere())
					mRenderDriver->renderBoundingSphere(model->getBoundingSphere());
			}

			findLightsAffectingModel(model);

			// Create local light list for faster light iteration setup
			static std::vector<Light*> localLightList;

			for (unsigned int i = 0; i < mDistanceLights.size(); ++i)
			{
				localLightList.push_back(mDistanceLights[i].light);
			}

			mRenderDriver->setWorldMatrix(core::matrix4::IDENTITY);
			mRenderDriver->setLights(localLightList);
			
			mRenderDriver->setWorldMatrix(model->getWorldMatrix());

			mShaderParamData.setCurrentModel(model);
			if (mDistanceLights.size() != 0)
			{
				mShaderParamData.setCurrentLight(mDistanceLights[0].light);
			}

			localLightList.clear();

			if (model->getMaterial() != NULL)
				setMaterial(model->getMaterial());
			else
				setMaterial(mDefaultMaterial);

			addGeometruCount(model);
			mRenderDriver->renderModel(model);
		}
	}
}

void RenderManager::setMaterial(Material* material)
{
	if (material == NULL)
		return;

	bool surfaceAndLightParams = true;
	bool fogParams = true;

	if (material->hasVertexShader())
	{
		material->getVertexShader()->updateAutoParameters(mShaderParamData);
		mRenderDriver->bindShader(material->getVertexShader());

		surfaceAndLightParams = false;//material->getVertexProgram()->getSurfaceAndLightStates();
	}
	else
	{
		if (mRenderDriver->isShaderBound(SHADER_TYPE_VERTEX))
			mRenderDriver->unbindShader(SHADER_TYPE_VERTEX);
	}

	if (material->hasGeometryShader())
	{
		mRenderDriver->bindShader(material->getGeometryShader());
	}
	else
	{
		if (mRenderDriver->isShaderBound(SHADER_TYPE_GEOMETRY))
			mRenderDriver->unbindShader(SHADER_TYPE_GEOMETRY);
	}
	
	if (surfaceAndLightParams)
	{
		if (material->getLightingEnabled())
		{
			mRenderDriver->setSurfaceParams(material->getAmbient(), material->getDiffuse(), material->getSpecular(), material->getEmissive(), material->getShininess());
		}

		mRenderDriver->setLightingEnabled(material->getLightingEnabled());
	}

	if (material->hasFragmentShader())
	{
		material->getFragmentShader()->updateAutoParameters(mShaderParamData);
		mRenderDriver->bindShader(material->getFragmentShader());

		fogParams = true;//material->getFragmentShader()->getFogStates();
	}
	else
	{
		if (mRenderDriver->isShaderBound(SHADER_TYPE_FRAGMENT))
			mRenderDriver->unbindShader(SHADER_TYPE_FRAGMENT);
	}

	if (fogParams)
	{
		// New fog params can either be from scene or from material
		FogMode newFogMode;
		Color newFogColor;
		float newFogDensity, newFogStart, newFogEnd;
		if (material->getFogOverride())
		{
			// New fog params from material
			newFogMode = material->getFogMode();
			newFogColor = material->getFogColor();
			newFogStart = material->getFogStart();
			newFogEnd = material->getFogEnd();
			newFogDensity = material->getFogDensity();
		}
		else
		{
			// New fog params from scene
			newFogMode = mFogMode;
			newFogColor = mFogColor;
			newFogStart = mFogStart;
			newFogEnd = mFogEnd;
			newFogDensity = mFogDensity;
		}

		mRenderDriver->setFog(newFogMode, newFogColor, newFogDensity, newFogStart, newFogEnd);
	}

	mRenderDriver->setSceneBlending(material->getSourceBlendFactor(), material->getDestBlendFactor());

	for (unsigned int i=0; i<ENGINE_MAX_TEXTURE_LAYERS; ++i)
	{
		TextureUnit* tu = material->getTextureUnit(i);

		mRenderDriver->setTextureUnitSettings(i, tu);
	}

	// Disable remaining texture units
	mRenderDriver->disableTextureUnitsFrom(material->getNumTextureUnits());

	mRenderDriver->setDepthBufferCheckEnabled(material->getDepthCheckEnabled());
	mRenderDriver->setDepthBufferWriteEnabled(material->getDepthWriteEnabled());

	// Shading
	mRenderDriver->setShadingType(material->getShadingMode());
}

void RenderManager::endFrame()
{
	mRenderDriver->endFrame();
}

RenderManager& RenderManager::getInstance()
{
	return core::Singleton<RenderManager>::getInstance();
}

RenderManager* RenderManager::getInstancePtr()
{
	return core::Singleton<RenderManager>::getInstancePtr();
}

} // end namespace render
