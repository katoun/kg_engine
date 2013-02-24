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

#include <core/Log.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <render/RenderUtils.h>
#include <render/RenderManager.h>
#include <render/Frustum.h>
#include <render/Light.h>
#include <render/LightFactory.h>
#include <render/Camera.h>
#include <render/CameraFactory.h>
#include <render/Material.h>
#include <render/MaterialFactory.h>
#include <render/Texture.h>
#include <render/TextureFactory.h>
#include <render/Model.h>
#include <render/ModelFactory.h>
#include <render/RenderDefines.h>
#include <render/VertexBuffer.h>
#include <render/VertexBufferDefines.h>
#include <render/IndexBuffer.h>
#include <render/IndexBufferDefines.h>
#include <render/MeshData.h>
#include <render/MeshDataFactory.h>
#include <render/Shader.h>
#include <render/ShaderFactory.h>
#include <render/ShaderParameter.h>
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

#if ENGINE_PLATFORM == PLATFORM_WINDOWS
#include <render/win32/Win32Window.h>
#endif

#include <algorithm>

template<> render::RenderManager* core::Singleton<render::RenderManager>::m_Singleton = nullptr;

namespace render
{

std::list<FrameEventReceiver*> RenderManager::mFrameEventReceivers;

RenderManager::RenderManager(): core::System("RenderManager")
{
	mShaderFactory = new ShaderFactory();
	mMaterialFactory = new MaterialFactory();
	mTextureFactory = new TextureFactory();
	mFontFactory = new FontFactory();
	mMeshDataFactory = new MeshDataFactory();
	mCameraFactory = new CameraFactory();
	mLightFactory = new LightFactory();
	mModelFactory = new ModelFactory();

	mMainWindow = nullptr;
	mCurrentViewport = nullptr;

	mDefaultMaterial = nullptr;

	mFrustum = nullptr;

	mLastViewportWidth = 0;
	mLastViewportHeight = 0;

	mVertexCount = 0;
	mFaceCount = 0;
}

RenderManager::~RenderManager()
{
	SAFE_DELETE(mShaderFactory);
	SAFE_DELETE(mMaterialFactory);
	SAFE_DELETE(mTextureFactory);
	SAFE_DELETE(mFontFactory);
	SAFE_DELETE(mMeshDataFactory);
	SAFE_DELETE(mCameraFactory);
	SAFE_DELETE(mLightFactory);
	SAFE_DELETE(mModelFactory);
}

RenderWindow* RenderManager::createRenderWindow(int width, int height, int colorDepth, bool fullScreen, int left, int top, bool depthBuffer, void* windowId)
{
	
	// Create the window
	RenderWindow* pRenderWindow = nullptr;

#if ENGINE_PLATFORM == PLATFORM_WINDOWS
	pRenderWindow = new Win32Window();
#endif

	if (pRenderWindow != nullptr)
	{
		pRenderWindow->create(width, height, colorDepth, fullScreen, left, top, depthBuffer, windowId);
	}

	std::string message = "RenderWindow Created --> " + core::intToString(width) + ", " + core::intToString(height) + ", " + core::intToString(colorDepth) + ".";
	if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("RenderManager", message);

	mRenderWindows[pRenderWindow->getID()] = pRenderWindow;

	return pRenderWindow;
}

void RenderManager::setMainWindow(RenderWindow* window)
{
	assert(window != nullptr);
	if (window == nullptr)
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

	return nullptr;
}

void RenderManager::removeRenderWindow(const unsigned int& id)
{
	std::map<unsigned int, RenderWindow*>::iterator i = mRenderWindows.find(id);
	if (i != mRenderWindows.end())
	{
		SAFE_DELETE(i->second);
		mRenderWindows.erase(i);
	}
}

void RenderManager::removeAllRenderWindows()
{
	std::map<unsigned int, RenderWindow*>::iterator i; 
	for (i = mRenderWindows.begin(); i != mRenderWindows.end(); ++i)
	{
		SAFE_DELETE(i->second);
		i->second = nullptr;
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

int RenderManager::getViewportHeight() const
{
	return mLastViewportHeight;
}

int RenderManager::getViewportWidth() const
{
	return mLastViewportWidth;
}

void RenderManager::addLight(Light *light)
{
	if (light == nullptr)
		return;

	mLights[light->getID()] = light;
}

void RenderManager::removeLight(Light *light)
{
	if (light == nullptr)
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
	if (camera == nullptr)
		return;

	mCameras[camera->getID()] = camera;
}

void RenderManager::removeCamera(Camera* camera)
{
	if (camera == nullptr)
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
	if (model == nullptr)
		return;

	mModels[model->getID()] = model;
}

void RenderManager::removeModel(Model* model)
{
	if (model == nullptr)
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
	if (viewport == nullptr)
		return;

	mUpdatedViewports.push_back(viewport);
}

Font* RenderManager::createFont(const std::string& filename)
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		Font* newFont = static_cast<Font*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_FONT, filename));
		if (newFont == nullptr)
			return nullptr;

		mFonts[newFont->getID()] = newFont;

		return newFont;
	}

	return nullptr;
}

void RenderManager::removeFont(Font* font)
{
	if (font == nullptr)
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
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		Shader* newShader = static_cast<Shader*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_SHADER, shaderFilename));
		if (newShader == nullptr)
			return nullptr;

		newShader->setShaderType(type);
		mShaders[newShader->getID()] = newShader;

		return newShader;
	}

	return nullptr;
}

Shader* RenderManager::getShader(const unsigned int& id)
{
	std::map<unsigned int, Shader*>::const_iterator i = mShaders.find(id);
	if (i != mShaders.end())
		return i->second;

	return nullptr;
}

unsigned int RenderManager::getNumberOfShaders() const
{
	return mShaders.size();
}

void RenderManager::removeShader(Shader* shader)
{
	if (shader == nullptr)
		return;

	removeShader(shader->getID());
}

void RenderManager::removeShader(const unsigned int& id)
{
	std::map<unsigned int, Shader*>::iterator i = mShaders.find(id);
	if (i != mShaders.end())
	{
		SAFE_DELETE(i->second);
		mShaders.erase(i);
	}
}

void RenderManager::removeAllShaders()
{
	mShaders.clear();
}

VertexBuffer* RenderManager::createVertexBuffer(VertexBufferType vertexBufferType, VertexElementType vertexElementType, unsigned int numVertices, BufferUsage usage)
{
	VertexBuffer* pVertexBuffer = new VertexBuffer(vertexBufferType, vertexElementType, numVertices, usage);
	if (pVertexBuffer != nullptr)
	{
		mVertexBuffers.push_back(pVertexBuffer);
	}

	return pVertexBuffer;
}

void RenderManager::removeVertexBuffer(VertexBuffer* buf)
{
	std::list<VertexBuffer*>::iterator i;
	for (i = mVertexBuffers.begin(); i != mVertexBuffers.end(); ++i)
	{
		if ((*i) == buf)
		{
			VertexBuffer* pVertexBuffer = (*i);
			mVertexBuffers.erase(i);
			SAFE_DELETE(pVertexBuffer);

			return;
		}
	}
}

void RenderManager::removeAllVertexBuffers()
{
	std::list<VertexBuffer*>::iterator i;
	for (i = mVertexBuffers.begin(); i != mVertexBuffers.end(); ++i)
	{
		VertexBuffer* pVertexBuffer = (*i);
		SAFE_DELETE(pVertexBuffer);
	}
	mVertexBuffers.clear();
}

IndexBuffer* RenderManager::createIndexBuffer(IndexType idxType, unsigned int numIndexes, BufferUsage usage)
{
	IndexBuffer* pIndexBuffer = new IndexBuffer(idxType, numIndexes, usage);
	if (pIndexBuffer != nullptr)
	{
		mIndexBuffers.push_back(pIndexBuffer);
	}

	return pIndexBuffer;
}

void RenderManager::removeIndexBuffer(IndexBuffer* buf)
{
	std::list<IndexBuffer*>::iterator i;
	for (i = mIndexBuffers.begin(); i != mIndexBuffers.end(); ++i)
	{
		if ((*i) == buf)
		{
			IndexBuffer* pIndexBuffer = (*i);
			mIndexBuffers.erase(i);
			SAFE_DELETE(pIndexBuffer);

			return;
		}
	}
}

void RenderManager::removeAllIndexBuffers()
{
	std::list<IndexBuffer*>::iterator i;
	for (i = mIndexBuffers.begin(); i != mIndexBuffers.end(); ++i)
	{
		IndexBuffer* pIndexBuffer = (*i);
		SAFE_DELETE(pIndexBuffer);
	}
	mIndexBuffers.clear();
}

void RenderManager::initializeImpl()
{
	if (resource::ResourceManager::getInstance() != nullptr)
		mDefaultMaterial = static_cast<Material*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_RENDER_MATERIAL, "materials/Default.xml"));

	mModelMaterialPairs.reserve(1024);

	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("RenderManager", "Can't Initialize GLew.", core::LOG_LEVEL_ERROR);
		return;
	}

	// Check for OpenGL 3.3
	if(!GLEW_VERSION_3_3)
	{
		MessageBox(nullptr, "Can't Initialize OpenGL 3.3.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Check for OpenGL 4.2
	/*if(!GLEW_VERSION_4_2)
	{
		MessageBox(nullptr, "Can't Initialize OpenGL 4.2.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}*/

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
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

	//! Removes all Vertex Buffers.
	removeAllVertexBuffers();

	//! Removes all Index Buffers.
	removeAllIndexBuffers();

	// Remove all Render Windows
	removeAllRenderWindows();

	// Remove ModelMaterialPairs
	mModelMaterialPairs.clear();

	mMainWindow = nullptr;
	mFrustum = nullptr;
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

		if(window != nullptr && window->isActive())
		{
			window->update(elapsedTime);

			//Go through the viewports of this RenderTarget
			std::list<Viewport*>::const_iterator k;
			for (k = window->getViewports().begin(); k != window->getViewports().end(); ++k)
			{
				Viewport* pViewport = (*k);
				if (pViewport != nullptr && pViewport->isVisible())
					render(pViewport->getCamera(), pViewport);
			}
		}
	}

	fireFrameEnded();
}

void RenderManager::registerFactoriesImpl()
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		resource::ResourceManager::getInstance()->registerResourceFactory(resource::RESOURCE_TYPE_SHADER, mShaderFactory);
		resource::ResourceManager::getInstance()->registerResourceFactory(resource::RESOURCE_TYPE_RENDER_MATERIAL, mMaterialFactory);
		resource::ResourceManager::getInstance()->registerResourceFactory(resource::RESOURCE_TYPE_TEXTURE, mTextureFactory);
		resource::ResourceManager::getInstance()->registerResourceFactory(resource::RESOURCE_TYPE_FONT, mFontFactory);
		resource::ResourceManager::getInstance()->registerResourceFactory(resource::RESOURCE_TYPE_MESH_DATA, mMeshDataFactory);
	}

	if (game::GameManager::getInstance() != nullptr)
	{
		game::GameManager::getInstance()->registerComponentFactory(game::COMPONENT_TYPE_CAMERA, mCameraFactory);
		game::GameManager::getInstance()->registerComponentFactory(game::COMPONENT_TYPE_LIGHT, mLightFactory);
		game::GameManager::getInstance()->registerComponentFactory(game::COMPONENT_TYPE_MODEL, mModelFactory);
	}
}

void RenderManager::removeFactoriesImpl()
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		resource::ResourceManager::getInstance()->removeResourceFactory(resource::RESOURCE_TYPE_SHADER);
		resource::ResourceManager::getInstance()->removeResourceFactory(resource::RESOURCE_TYPE_RENDER_MATERIAL);
		resource::ResourceManager::getInstance()->removeResourceFactory(resource::RESOURCE_TYPE_TEXTURE);
		resource::ResourceManager::getInstance()->removeResourceFactory(resource::RESOURCE_TYPE_FONT);
		resource::ResourceManager::getInstance()->removeResourceFactory(resource::RESOURCE_TYPE_MESH_DATA);
	}

	if (game::GameManager::getInstance() != nullptr)
	{
		game::GameManager::getInstance()->removeComponentFactory(game::COMPONENT_TYPE_CAMERA);
		game::GameManager::getInstance()->removeComponentFactory(game::COMPONENT_TYPE_LIGHT);
		game::GameManager::getInstance()->removeComponentFactory(game::COMPONENT_TYPE_MODEL);
	}
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

void RenderManager::addGeometryCount(Model* model)
{
	if (model == nullptr)
		return;
	
	// Update stats
	if (model->getVertexBuffer(VERTEX_BUFFER_TYPE_POSITION) == nullptr && model->getIndexBuffer() == nullptr)
		return;

	unsigned int val = model->getIndexBuffer()->getNumIndexes();
	
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

	mVertexCount += model->getVertexBuffer(VERTEX_BUFFER_TYPE_POSITION)->getNumVertices();
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
	if (camera == nullptr)
		return;
	
	if (viewport == nullptr)
		return;

	setCurrentViewport(viewport);

	mRenderStateData.setCurrentViewport(viewport);

	if (mLastViewportWidth != viewport->getActualWidth() || mLastViewportHeight != viewport->getActualHeight())
	{
		mLastViewportWidth = viewport->getActualWidth();
		mLastViewportHeight = viewport->getActualHeight();
	}

	findVisibleModels(camera);

	beginGeometryCount();

	beginFrame(viewport);

	mRenderStateData.setCurrentCamera(camera);

	std::map<unsigned int, Light*>::const_iterator i = mLights.begin();
	if (i != mLights.end())
	{
		mRenderStateData.setCurrentLight(i->second);
	}	

	renderVisibleModels();
	
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
	if (viewport == nullptr)
		return;

	bool viewportChanged = false;

	// Check if viewport is different
	if (mCurrentViewport == viewport)
	{
		std::list<Viewport*>::const_iterator i;
		for (i = mUpdatedViewports.begin(); i != mUpdatedViewports.end(); ++i)
		{
			Viewport* pViewport = (*i);
			if (pViewport != nullptr && pViewport == viewport)
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

		GLsizei x, y, w, h;

		RenderTarget* target;
		target = viewport->getTarget();

		// Calculate the "lower-left" corner of the viewport
		w = viewport->getActualWidth();
		h = viewport->getActualHeight();
		x = viewport->getActualLeft();
		y = target->getHeight() - viewport->getActualTop() - h;

		glViewport(x, y, w, h);
	}
}

void RenderManager::beginFrame(Viewport* viewport)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
}

void RenderManager::findVisibleModels(Camera* camera)
{
	if (camera == nullptr)
		return;

	mModelMaterialPairs.clear();

	mFrustum = camera->getFrustum();
	if (mFrustum == nullptr)
		return;

	// Go through all the models
	std::map<unsigned int, Model*>::const_iterator i;
	for (i = mModels.begin(); i != mModels.end(); ++i)
	{
		Model* pModel = i->second;
		if (pModel != nullptr)
		{
			//if (mFrustum->isVisible(pModel->getBoundingSphere()) && mFrustum->isVisible(pModel->getBoundingBox()))//TEMP REM FOR TESTING!!!
			{
				Material* pMaterial = pModel->getMaterial();

				ModelMaterialPair newPair;
				newPair.model = pModel;
				newPair.materialID = pMaterial != nullptr ? pMaterial->getID() : mDefaultMaterial->getID();
				mModelMaterialPairs.push_back(newPair);
			}
		}
	}

	std::sort(mModelMaterialPairs.begin(), mModelMaterialPairs.end());
}

void RenderManager::renderVisibleModels()
{
	// Go through all the model material pairs	
	for (unsigned int i = 0; i < mModelMaterialPairs.size(); ++i)
	{
		renderSingleModel(mModelMaterialPairs[i].model);
	}
}

void RenderManager::renderSingleModel(Model* model)
{
	if (model == nullptr)
		return;

	if (model->getGameObject() != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(model->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			if (model->getMaterial() != nullptr)
				mRenderStateData.setCurrentMaterial(model->getMaterial());
			else
				mRenderStateData.setCurrentMaterial(mDefaultMaterial);

			mRenderStateData.setCurrentModel(model);

			addGeometryCount(model);

			render(mRenderStateData);
		}
	}
}

void RenderManager::render(RenderStateData& renderStateData)
{
	if (renderStateData.getCurrentMaterial() == nullptr)
		return;

	Material* pMaterial = renderStateData.getCurrentMaterial();
	if (pMaterial == nullptr)
		return;

	Model* pModel = renderStateData.getCurrentModel();

	if (pModel == nullptr)
		return;

	if (pModel->getVertexBuffer(VERTEX_BUFFER_TYPE_POSITION) == nullptr || pModel->getIndexBuffer() == nullptr)
		return;

	glUseProgram(pMaterial->getGLHandle());

	ShaderParameter* pShaderParameter = nullptr;
	ShaderVertexParameter* pShaderVertexParameter = nullptr;

	/////////////Textures/////////////
	std::vector<ShaderTextureParameter*>& shaderTextureParameters = pMaterial->getTextureParameters();
	std::list<ShaderTextureParameter*>::const_iterator ti;
	for (unsigned int i = 0; i < shaderTextureParameters.size(); ++i)
	{
		ShaderTextureParameter* pTextureParameter = shaderTextureParameters[i];
		if (pTextureParameter == nullptr)
			continue;

		pShaderParameter = pTextureParameter->mParameter;
		Texture* pTexture = pMaterial->getTextureUnit(i);
		if (pTexture == nullptr || pShaderParameter == nullptr)
			continue;

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, pTexture->getGLID());
		glUniform1i(pShaderParameter->mParameterID, i);
	}
	//////////////////////////////////

	//////////AutoParameters//////////
	std::list<ShaderAutoParameter*>& shaderAutoParameters = pMaterial->getAutoParameters();
	std::list<ShaderAutoParameter*>::const_iterator ai;
	for (ai = shaderAutoParameters.begin(); ai != shaderAutoParameters.end(); ++ai)
	{
		ShaderAutoParameter* pAutoParameter = (*ai);
		if (pAutoParameter == nullptr)
			continue;

		pShaderParameter = pAutoParameter->mParameter;
		if (pShaderParameter == nullptr)
			continue;

		switch (pAutoParameter->mAutoParameterType)
		{
		case SHADER_AUTO_PARAMETER_TYPE_MODEL_MATRIX:
			pMaterial->setParameter(pShaderParameter, renderStateData.getModelMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_VIEW_MATRIX:
			pMaterial->setParameter(pShaderParameter, renderStateData.getViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_PROJECTION_MATRIX:
			pMaterial->setParameter(pShaderParameter, renderStateData.getProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_VIEW_PROJECTION_MATRIX:
			pMaterial->setParameter(pShaderParameter, renderStateData.getViewProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_MODEL_VIEW_MATRIX:
			pMaterial->setParameter(pShaderParameter, renderStateData.getModelViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_MODEL_VIEW_PROJECTION_MATRIX:
			pMaterial->setParameter(pShaderParameter, renderStateData.getModelViewProjectionMatrix());
			break;

		case SHADER_AUTO_PARAMETER_TYPE_MATERIAL_AMBIENT_COLOUR:
			pMaterial->setParameter(pShaderParameter, renderStateData.getMaterialAmbientColour());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_MATERIAL_DIFFUSE_COLOUR:
			pMaterial->setParameter(pShaderParameter, renderStateData.getMaterialDiffuseColour());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_MATERIAL_SPECULAR_COLOUR:
			pMaterial->setParameter(pShaderParameter, renderStateData.getMaterialSpecularColour());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_MATERIAL_SPECULAR_POWER:
			pMaterial->setParameter(pShaderParameter, renderStateData.getMaterialSpecularPower());
			break;
		
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION:
			pMaterial->setParameter(pShaderParameter, renderStateData.getLightPosition());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_AMBIENT_COLOUR:
			pMaterial->setParameter(pShaderParameter, renderStateData.getLightAmbientColour());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIFFUSE_COLOUR:
			pMaterial->setParameter(pShaderParameter, renderStateData.getLightDiffuseColour());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_SPECULAR_COLOUR:
			pMaterial->setParameter(pShaderParameter, renderStateData.getLightSpecularColour());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_ATTENUATION:
			pMaterial->setParameter(pShaderParameter, renderStateData.getLightAttenuation());
			break;

		case SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION:
			pMaterial->setParameter(pShaderParameter, renderStateData.getCameraPosition());
			break;
		}
	}
	//////////////////////////////////

	/////////////Buffers//////////////
	std::vector<ShaderVertexParameter*>& vertexParameters = pMaterial->getVertexParameters();
	for (std::size_t vertexType = VERTEX_BUFFER_TYPE_POSITION; vertexType != VERTEX_BUFFER_TYPE_COUNT; ++vertexType)
	{
		if (vertexParameters[vertexType] == nullptr)
			continue;

		pShaderVertexParameter = vertexParameters[vertexType];

		if (pShaderParameter == nullptr)
			continue;

		VertexBuffer* pVertexBuffer = pModel->getVertexBuffer((VertexBufferType)vertexType);
		if (pVertexBuffer == nullptr)
			continue;

		glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer->getGLBufferId());

		GLuint index = pShaderVertexParameter->mParameterID;
		GLenum type = getGLType(pVertexBuffer->getVertexElementType());
		GLint size = 0;
		switch (pVertexBuffer->getVertexElementType())
		{
		case VERTEX_ELEMENT_TYPE_COLOR:
			size = 1;
			break;
		case VERTEX_ELEMENT_TYPE_FLOAT1:
			size = 1;
			break;
		case VERTEX_ELEMENT_TYPE_FLOAT2:
			size = 2;
			break;
		case VERTEX_ELEMENT_TYPE_FLOAT3:
			size = 3;
			break;
		case VERTEX_ELEMENT_TYPE_FLOAT4:
			size = 4;
			break;
		case VERTEX_ELEMENT_TYPE_SHORT1:
			size = 1;
			break;
		case VERTEX_ELEMENT_TYPE_SHORT2:
			size = 2;
			break;
		case VERTEX_ELEMENT_TYPE_SHORT3:
			size = 3;
			break;
		case VERTEX_ELEMENT_TYPE_SHORT4:
			size = 4;
			break;
		}

		GLsizei stride = (GLsizei)(pVertexBuffer->getVertexSize());

		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index,			// The attribute we want to configure
			size,			// size
			type,			// type
			GL_FALSE,		// normalized?
			stride,			// stride
			0				// array buffer offset
			);
	}

	if (pModel->getIndexBuffer() == nullptr)
		return;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (pModel->getIndexBuffer())->getGLBufferId());

	GLenum primType = getGLType(pModel->getRenderOperationType());
	GLenum indexType = (pModel->getIndexBuffer()->getType() == IT_16BIT) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;

	glDrawElements(primType, pModel->getIndexBuffer()->getNumIndexes(), indexType, 0);
	//////////////////////////////////

	for (unsigned int i = 0; i < vertexParameters.size(); ++i)
	{
		pShaderVertexParameter = vertexParameters[i];

		if (pShaderVertexParameter == nullptr)
			continue;

		glDisableVertexAttribArray(pShaderVertexParameter->mParameterID);
	}
}

void RenderManager::endFrame() {}

RenderManager* RenderManager::getInstance()
{
	return core::Singleton<RenderManager>::getInstance();
}

} // end namespace render
