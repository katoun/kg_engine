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

#include <core/Log.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <render/RenderManager.h>
#include <render/RenderDriver.h>
#include <render/Frustum.h>
#include <render/Light.h>
#include <render/Camera.h>
#include <render/Renderable.h>
#include <render/Model.h>
#include <render/Overlay.h>
#include <render/PanelOverlay.h>
#include <render/TextOverlay.h>
#include <render/Font.h>
#include <render/Viewport.h>
#include <render/RenderWindow.h>
#include <render/FrameEventReceiver.h>
#include <render/Shader.h>
#include <render/VertexBuffer.h>
#include <render/IndexBuffer.h>
#include <render/MeshData.h>
#include <render/FontFactory.h>
#include <render/MaterialFactory.h>
#include <render/MeshDataFactory.h>
#include <render/CameraFactory.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>
#include <scene/SceneManager.h>
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

Light* RenderManager::createLight(scene::Node* parent)
{
	Light* newLight = new Light();
	mLights[newLight->getID()] = newLight;

	scene::SceneManager::getInstance().addNode(newLight, parent);

	return newLight;
}

Light* RenderManager::createLight(const std::string& name, scene::Node* parent)
{
	Light* newLight = new Light(name);
	mLights[newLight->getID()] = newLight;

	scene::SceneManager::getInstance().addNode(newLight, parent);

	return newLight;
}

Light* RenderManager::getLight(const unsigned int& id)
{
	std::map<unsigned int, Light*>::const_iterator i = mLights.find(id);
	if (i != mLights.end())
		return i->second;

	return NULL;
}

unsigned int RenderManager::getNumberOfLights() const
{
	return mLights.size();
}

void RenderManager::removeLight(Light *lt)
{
	if (lt == NULL)	return;

	removeLight(lt->getID());
}

void RenderManager::removeLight(const unsigned int& id)
{
	std::map<unsigned int, Light*>::iterator i = mLights.find(id);
	if (i != mLights.end())
		mLights.erase(i);

	scene::SceneManager::getInstance().removeNode(id);
}

void RenderManager::removeAllLights()
{
	std::map<unsigned int, Light*>::const_iterator i;
	for (i = mLights.begin(); i != mLights.end(); ++i)
		scene::SceneManager::getInstance().removeNode(i->second->getID());

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

Renderable* RenderManager::createRenderable(scene::Node* parent)
{
	Renderable* newRenderable = new Renderable();
	mRenderables[newRenderable->getID()] = newRenderable;

	scene::SceneManager::getInstance().addNode(newRenderable, parent);

	return newRenderable;
}

Renderable* RenderManager::createRenderable(const std::string& name, scene::Node* parent)
{
	Renderable* newRenderable = new Renderable(name);
	mRenderables[newRenderable->getID()] = newRenderable;

	scene::SceneManager::getInstance().addNode(newRenderable, parent);

	return newRenderable;
}

Renderable* RenderManager::getRenderable(const unsigned int& id)
{
	std::map<unsigned int, Renderable*>::const_iterator i = mRenderables.find(id);
	if (i != mRenderables.end())
		return i->second;

	return NULL;
}

unsigned int RenderManager::getNumberOfRenderables() const
{
	return mRenderables.size();
}

void RenderManager::removeRenderable(const unsigned int& id)
{
	std::map<unsigned int, Renderable*>::iterator i = mRenderables.find(id);
	if (i != mRenderables.end())
		mRenderables.erase(i);

	scene::SceneManager::getInstance().removeNode(id);
}

void RenderManager::removeRenderable(Renderable *renderable)
{
	if (renderable == NULL)
		return;

	removeRenderable(renderable->getID());
}

void RenderManager::removeAllRenderables()
{
	std::map<unsigned int, Renderable*>::const_iterator i;
	for (i = mRenderables.begin(); i != mRenderables.end(); ++i)
		scene::SceneManager::getInstance().removeNode(i->second->getID());

	mRenderables.clear();
}

Model* RenderManager::createModel(const std::string& meshFilename, scene::Node* parent)
{
	MeshData* newMeshData = static_cast<MeshData*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_MESH_DATA, meshFilename));
	if (newMeshData == NULL) return NULL;

	Model* newModel = new Model(newMeshData);
	Renderable* newRenderable = static_cast<Renderable*>(newModel);
	if (newRenderable == NULL) return NULL;

	mRenderables[newModel->getID()] = newRenderable;

	scene::SceneManager::getInstance().addNode(newModel, parent);

	return newModel;
}

Model* RenderManager::createModel(const std::string& name, const std::string& meshFilename, scene::Node* parent)
{
	MeshData* newMeshData = static_cast<MeshData*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_MESH_DATA, meshFilename));
	if (newMeshData == NULL)	return NULL;

	Model* newModel = new Model(name, newMeshData);
	Renderable* newRenderable = static_cast<Renderable*>(newModel);
	if (newRenderable == NULL) return NULL;

	mRenderables[newModel->getID()] = newRenderable;

	scene::SceneManager::getInstance().addNode(newModel, parent);

	return newModel;
}

Model* RenderManager::createModel(MeshData* meshData, scene::Node* parent)
{
	if (meshData == NULL) return NULL;

	Model* newModel = new Model(meshData);
	Renderable* newRenderable = static_cast<Renderable*>(newModel);
	if (newRenderable == NULL) return NULL;

	mRenderables[newModel->getID()] = newRenderable;

	scene::SceneManager::getInstance().addNode(newModel, parent);

	return newModel;
}

Model* RenderManager::createModel(const std::string& name, MeshData* meshData, scene::Node* parent)
{
	if (meshData == NULL) return NULL;

	Model* newModel = new Model(name, meshData);
	Renderable* newRenderable = static_cast<Renderable*>(newModel);
	if (newRenderable == NULL) return NULL;

	mRenderables[newModel->getID()] = newRenderable;

	scene::SceneManager::getInstance().addNode(newModel, parent);

	return newModel;
}

Model* RenderManager::getModel(const unsigned int& id)
{
	std::map<unsigned int, Renderable*>::const_iterator i = mRenderables.find(id);
	if (i != mRenderables.end())
		return static_cast<Model*>(i->second);

	return NULL;
}

void RenderManager::removeModel(Model *model)
{
	if (model == NULL)
		return;

	removeRenderable(model->getID());
}

void RenderManager::removeModel(const unsigned int& id)
{
	removeRenderable(id);
}

PanelOverlay* RenderManager::createPanelOverlay()
{
	Overlay* newOverlay = new PanelOverlay();
	mOverlays[newOverlay->getID()] = newOverlay;

	return static_cast<PanelOverlay*>(newOverlay);
}

PanelOverlay* RenderManager::createPanelOverlay(const std::string& name)
{
	Overlay* newOverlay = new PanelOverlay(name);
	mOverlays[newOverlay->getID()] = newOverlay;

	return static_cast<PanelOverlay*>(newOverlay);
}

TextOverlay* RenderManager::createTextOverlay()
{
	Overlay* newOverlay = new TextOverlay();
	mOverlays[newOverlay->getID()] = newOverlay;

	return static_cast<TextOverlay*>(newOverlay);
}

TextOverlay* RenderManager::createTextOverlay(const std::string& name)
{
	Overlay* newOverlay = new TextOverlay(name);
	mOverlays[newOverlay->getID()] = newOverlay;

	return static_cast<TextOverlay*>(newOverlay);
}

Overlay* RenderManager::getOverlay(const unsigned int& id)
{
	std::map<unsigned int, Overlay*>::const_iterator i = mOverlays.find(id);
	if (i != mOverlays.end())
		return i->second;

	return NULL;
}

unsigned int RenderManager::getNumberOfOverlays() const
{
	return mOverlays.size();
}

void RenderManager::removeOverlay(Overlay *overlay)
{
	if (overlay == NULL)
		return;

	removeOverlay(overlay->getID());
}

void RenderManager::removeOverlay(const unsigned int& id)
{
	std::map<unsigned int, Overlay*>::iterator i = mOverlays.find(id);
	if (i != mOverlays.end())
	{
		delete i->second;
		mOverlays.erase(i);
	}
}

void RenderManager::removeAllOverlays()
{
	std::map<unsigned int, Overlay*>::iterator i;
	for (i = mOverlays.begin(); i != mOverlays.end(); ++i)
	{
		delete i->second;
		i->second = NULL;
	}

	mOverlays.clear();
}

Font* RenderManager::createFont(const std::string& fontFilename)
{
	// Get font (load if required)
	Font* newFont = static_cast<Font*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_FONT, fontFilename));
	if (newFont == NULL)
		return NULL;

	mFonts[newFont->getID()] = newFont;

	return newFont;
}

Font* RenderManager::getFont(const unsigned int& id)
{
	std::map<unsigned int, Font*>::const_iterator i = mFonts.find(id);
	if (i != mFonts.end())
		return i->second;

	return NULL;
}

unsigned int RenderManager::getNumberOfFonts() const
{
	return mFonts.size();
}

void RenderManager::removeFont(Font *fnt)
{
	if (fnt == NULL)
		return;

	removeFont(fnt->getID());
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
	engine::EngineSettings& engineOptions = engine::EngineSettings::getInstance();


	mMainWindow = createRenderWindow(engineOptions.getWidth(), engineOptions.getHeight(), engineOptions.getBitdepth(), engineOptions.getFullscreen(), 0, 0, true, engineOptions.getMainWindowId());

	mDefaultMaterial = static_cast<Material*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_RENDER_MATERIAL, "materials/DefaultMaterial.xml"));

	mSolidRenderables.reserve(1024);
	mTransparentRenderables.reserve(1024);
	mDistanceLights.reserve(512);
}

void RenderManager::uninitializeImpl()
{
	// Remove all Lights
	removeAllLights();

	// Remove all Cameras
	removeAllCameras();

	// Remove all Renderables
	removeAllRenderables();

	// Remove all Overlays
	removeAllOverlays();

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
	mSolidRenderables.clear();
	// Remove TransparentModels
	mTransparentRenderables.clear();
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

	//// Update Overlays
	std::map<unsigned int, Overlay*>::const_iterator i;
	for (i = mOverlays.begin(); i != mOverlays.end(); ++i)
	{
		Overlay* overlay = i->second;
		if (overlay != NULL) overlay->update(elapsedTime);
	}

	//std::map<unsigned int, Overlay*>::const_iterator oit;
	//for (oit = mOverlays2.begin(); oit != mOverlays2.end(); ++oit)
	//	oit->second->update();

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
}

void RenderManager::removeDefaultFactoriesImpl()
{
	resource::ResourceManager::getInstance().removeResourceFactory(resource::RESOURCE_TYPE_FONT);
	resource::ResourceManager::getInstance().removeResourceFactory(resource::RESOURCE_TYPE_RENDER_MATERIAL);
	resource::ResourceManager::getInstance().removeResourceFactory(resource::RESOURCE_TYPE_MESH_DATA);

	game::GameManager::getInstance().removeComponentFactory(game::COMPONENT_TYPE_CAMERA);
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

void RenderManager::addGeometruCount(Renderable* renderable)
{
	// Update stats
	if (renderable->getVertexData() == NULL || renderable->getIndexData() == NULL)
		return;

	unsigned int val = renderable->getIndexData()->indexCount;
	
	switch(renderable->getRenderOperationType())
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

	mVertexCount += renderable->getVertexData()->vertexCount;
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

	// Set the current viewport
	setCurrentViewport(viewport);

	mShaderParamData.setCurrentViewport(viewport);

	mRenderDriver->setAmbientLight(mAmbientLight.R, mAmbientLight.G, mAmbientLight.B, mAmbientLight.A);

	if (mLastViewportWidth != viewport->getActualWidth() || mLastViewportHeight != viewport->getActualHeight())
	{
		mLastViewportWidth = viewport->getActualWidth();
		mLastViewportHeight = viewport->getActualHeight();
	}

	findLightsAffectingFrustum(camera);

	findVisibleRenderables(camera);

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
		if (pLight != NULL && pLight->getVisibleAxis())
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

			mRenderDriver->renderAxes(pLight->getAbsolutePosition(), pLight->getAbsoluteOrientation());
		}
	}
	
	renderVisibleRenderables();

	renderVisibleOverlays(camera, viewport);

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

	// Check if viewport is different
	if (viewport != mCurrentViewport)
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

void RenderManager::findLightsAffectingRenderables(Renderable* renderable)
{
	if (renderable == NULL)
		return;
	
	mDistanceLights.clear();

	std::list<Light*>::const_iterator i;
	for (i = mLightsAffectingFrustum.begin(); i!= mLightsAffectingFrustum.end(); ++i)
	{
		if ((*i)->getLightType() == LIGHT_TYPE_DIRECTIONAL)
		{
			DistanceLight newLight;
			newLight.light = (*i);
			newLight.distance = 0;
			// No distance
			mDistanceLights.push_back(newLight);
		}
		else
		{
			// Calculate squared distance
			float distance = ((*i)->getAbsolutePosition() - renderable->getAbsolutePosition()).getLengthSQ();
			// only add in-range lights
			float range = (*i)->getAttenuationRange();
			float maxDist = range + renderable->getBoundingSphere().Radius;
			//if (distance <= (maxDist * maxDist))//Katoun TEMP REM for testing!!!
			{
				DistanceLight newLight;
				newLight.light = (*i);
				newLight.distance = distance;
				mDistanceLights.push_back(newLight);
			}
		}
	}

	std::sort(mDistanceLights.begin(), mDistanceLights.end());
}

void RenderManager::findVisibleRenderables(Camera* camera)
{
	if (camera == NULL)
		return;

	mSolidRenderables.clear();
	mTransparentRenderables.clear();

	mFrustum = camera->getFrustum();

	// Go through all the renderables
	std::map<unsigned int, Renderable*>::const_iterator mi;
	for (mi = mRenderables.begin(); mi != mRenderables.end(); ++mi)
	{
		if (mFrustum->isVisible(mi->second->getBoundingSphere()) && mFrustum->isVisible(mi->second->getBoundingBox()))
		{
			Material* material = mi->second->getMaterial();

			if (material != NULL)
			{
				if (material->isTransparent())
				{
					TransparentRenderable newRenderable;
					newRenderable.renderable = mi->second;
					newRenderable.distance = 0.0f;

					game::Transform* pTransform = NULL;
					if (camera->getGameObject() != NULL)
					{
						pTransform = static_cast<game::Transform*>(camera->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));

						// Calculate squared distance
						newRenderable.distance = (mi->second->getAbsolutePosition() - pTransform->getAbsolutePosition()).getLengthSQ();
					}

					mTransparentRenderables.push_back(newRenderable);
				}
				else
				{
					SolidRenderable newRenderable;
					newRenderable.renderable = mi->second;
					newRenderable.materialID = material->getID();
					mSolidRenderables.push_back(newRenderable);
				}
			}
			else
			{
					SolidRenderable newRenderable;
					newRenderable.renderable = mi->second;
					newRenderable.materialID = mDefaultMaterial->getID();
					mSolidRenderables.push_back(newRenderable);
			}
		}
	}

	std::sort(mSolidRenderables.begin(), mSolidRenderables.end());
	std::sort(mTransparentRenderables.begin(), mTransparentRenderables.end());
}

void RenderManager::renderVisibleRenderables()
{
	// Go through all the solid meshes	
	for (unsigned int i = 0; i < mSolidRenderables.size(); ++i)
	{
		renderSingleRenderable(mSolidRenderables[i].renderable);
	}

	// Go through all the transparent meshes
	for (unsigned int i = 0; i < mTransparentRenderables.size(); ++i)
	{
		renderSingleRenderable(mTransparentRenderables[i].renderable);
	}
}

void RenderManager::renderSingleRenderable(Renderable* renderable)
{
	if (renderable == NULL)
		return;

	if (renderable->getVisibleAxis() || renderable->getVisibleBoundingBox() || renderable->getVisibleBoundingSphere())
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

		if (renderable->getVisibleAxis())
			mRenderDriver->renderAxes(renderable->getAbsolutePosition(), renderable->getAbsoluteOrientation());
		if (renderable->getVisibleBoundingBox())
			mRenderDriver->renderBoundingSphere(renderable->getBoundingSphere());
		if (renderable->getVisibleBoundingSphere())
			mRenderDriver->renderBoundingBox(renderable->getBoundingBox());

	}

	findLightsAffectingRenderables(renderable);

	// Create local light list for faster light iteration setup
	static std::vector<Light*> localLightList;

	for (unsigned int i = 0; i < mDistanceLights.size(); ++i)
	{
		localLightList.push_back(mDistanceLights[i].light);
	}

	mRenderDriver->setWorldMatrix(core::matrix4::IDENTITY);
	mRenderDriver->setLights(localLightList);
			
	mRenderDriver->setWorldMatrix(renderable->getWorldMatrix());

	mShaderParamData.setCurrentRenderable(renderable);
	if (mDistanceLights.size() != 0) mShaderParamData.setCurrentLight(mDistanceLights[0].light);

	localLightList.clear();

	if (renderable->getMaterial() != NULL)
		setMaterial(renderable->getMaterial());
	else
		setMaterial(mDefaultMaterial);

	addGeometruCount(renderable);
	mRenderDriver->render(renderable);
}

void RenderManager::renderVisibleOverlays(Camera* camera, Viewport* viewport)
{
	if (camera == NULL)
		return;

	if (viewport == NULL)
		return;
	
	if (viewport->getShowOverlays())
	{
		//Go through the overlays of this viewport
		std::map<unsigned int, Overlay*>::const_iterator it;
		for (it = mOverlays.begin(); it != mOverlays.end(); ++it)
		{
			if (!it->second->isVisible())
				continue;
			
			core::matrix4 proj, world;
			switch(it->second->getMetricsMode())
			{	
				case MM_PIXELS :
					proj.buildProjectionMatrixOrtho(viewport->getActualWidth() * 2.0f, viewport->getActualHeight() * 2.0f, -1.0f, 1.0f);
					break;
				case MM_RELATIVE_ASPECT_ADJUSTED :
				case MM_RELATIVE :
					proj.buildProjectionMatrixOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
					break;
			}
			world = it->second->getWorldMatrix();
			world.setTranslation(world.getTranslation() + core::vector3d(0, 0, camera->getNearClipDistance() - 1));

			mRenderDriver->setProjectionMatrix(proj);
			mRenderDriver->setWorldMatrix(world);

			if (it->second->getMaterial() != NULL)
				setMaterial(it->second->getMaterial());
			else
				setMaterial(mDefaultMaterial);

			mRenderDriver->setLightingEnabled(false);
			mRenderDriver->setFog(FM_NONE);

			addGeometruCount(it->second);
			mRenderDriver->render(it->second);
		}
	}
}

void RenderManager::renderSingleOverlay(Overlay* overlay, Camera* camera, Viewport* viewport)
{
	if (overlay == NULL)
		return;
	
	if (camera == NULL)
		return;

	if (viewport == NULL)
		return;
	
	core::matrix4 proj, world;
	switch(overlay->getMetricsMode())
	{	
	case MM_PIXELS :
		proj.buildProjectionMatrixOrtho(viewport->getActualWidth() * 2.0f, viewport->getActualHeight() * 2.0f, -1.0f, 1.0f);
		break;
	case MM_RELATIVE_ASPECT_ADJUSTED :
	case MM_RELATIVE :
		proj.buildProjectionMatrixOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
		break;
	}
	world = overlay->getWorldMatrix();
	world.setTranslation(world.getTranslation() + core::vector3d(0, 0, camera->getNearClipDistance() - 1));

	mRenderDriver->setProjectionMatrix(proj);
	mRenderDriver->setWorldMatrix(world);

	if (overlay->getMaterial() != NULL)
		setMaterial(overlay->getMaterial());
	else
		setMaterial(mDefaultMaterial);

	mRenderDriver->setLightingEnabled(false);
	mRenderDriver->setFog(FM_NONE);

	addGeometruCount(overlay);
	mRenderDriver->render(overlay);
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
