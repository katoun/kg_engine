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

#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

#include <core/Config.h>
#include <core/System.h>
#include <core/Singleton.h>
#include <render/Color.h>
#include <render/Material.h>
#include <render/ShaderParamData.h>

#include <string>
#include <list>
#include <vector>
#include <map>

namespace core
{
class matrix4;
}

namespace resource
{
class Resource;
class Serializer;
enum BufferUsage;
enum PixelFormat;
}

namespace scene
{
class Node;
}

namespace game
{
class ComponentFactory;
}

namespace render
{

class Frustum;
class Light;
class Camera;
class Renderable;
class Model;
class Overlay;
class PanelOverlay;
class TextOverlay;
class Font;
class MeshData;
class Material;
class Viewport;
class RenderWindow;
class RenderDriver;
class FrameEventReceiver;
struct FrameEvent;
class Shader;
class VertexBuffer;
class IndexBuffer;
class VertexDeclaration;
class VertexBufferBinding;
class FontFactory;
class MaterialFactory;
class MeshDataFactory;
enum IndexType;
enum ShaderType;

//! Rendering Manager.
//! 
//! This class handles everything related to rendering.
//! \author: Kat'Oun
//! \version: 1.0
class ENGINE_PUBLIC_EXPORT RenderManager: public core::System, public core::Singleton<RenderManager>
{
public:

	// Default Constructor
	RenderManager();

	~RenderManager();

	RenderWindow* createRenderWindow(signed int width, signed int height, signed int colorDepth, bool fullScreen, signed int left = 0, signed int top = 0, bool depthBuffer = true, void* windowId = NULL);

	//! Sets a pointer to the main render window.
	void setMainWindow(RenderWindow* window);

	//! Retrieve a pointer to the main render window.
	RenderWindow* getMainWindow();

	//! Retrieves a pointer to the a render window.
	RenderWindow* getRenderWindow(const unsigned int& id);
	
	//! Removes a rendering window.
	void removeRenderWindow(const unsigned int& id);

	//! Removes all rendering windows.
	void removeAllRenderWindows();

	void addFrameEventReceiver(FrameEventReceiver* newEventReceiver);
	void removeFrameEventReceiver(FrameEventReceiver* oldEventReceiver);

	//! Gets the height of the destination viewport in pixels.
	signed int getViewportHeight() const;

	//! Gets the width of the destination viewport in pixels.
	signed int getViewportWidth() const;

	//!  Adds a light to be managed by this scene manager.
	void addLight(Light *light);

	//! Removes a light from the rendering.
	void removeLight(Light *light);
	//! Removes a light from the rendering.
	void removeLight(const unsigned int& id);
	//! Removes (and destroys) all lights from the rendering.
	void removeAllLights();

	//!  Adds a camera to be managed by this scene manager.
	void addCamera(Camera* camera);

	//!  Removes a camera from the rendering.
	void removeCamera(Camera* camera);
	//!  Removes a camera from the rendering.
	void removeCamera(const unsigned int& id);
	//! Removes all cameras from the rendering
	void removeAllCameras();

	//!  Creates a renderable to be managed by this scene manager.
	Renderable* createRenderable(scene::Node* parent = NULL);
	Renderable* createRenderable(const std::string& name, scene::Node* parent = NULL);

	//! Retrieves a pointer to a renderable by id.
	Renderable* getRenderable(const unsigned int& id);

	//! Retrieves the total number of created renderables.
	unsigned int getNumberOfRenderables() const;

	//! Removes a renderable from the rendering.
	void removeRenderable(Renderable *renderable);
	//! Removes a renderable from the rendering.
	void removeRenderable(const unsigned int& id);
	//! Removes (and destroys) all renderables from the rendering.
	void removeAllRenderables();

	//! Creates a mesh to be managed by rendering manager.
	Model* createModel(const std::string& meshFilename, scene::Node* parent = NULL);
	Model* createModel(const std::string& name, const std::string& meshFilename, scene::Node* parent = NULL);

	//! Creates a mesh to be managed by rendering manager.
	Model* createModel(MeshData* meshData, scene::Node* parent = NULL);
	Model* createModel(const std::string& name, MeshData* meshData, scene::Node* parent = NULL);

	//! Retrieves a pointer to a mesh by id.
	Model* getModel(const unsigned int& id);

	//! Removes a mesh from the rendering.
	void removeModel(Model *model);
	//! Removes a mesh from the rendering.
	void removeModel(const unsigned int& id);

	//! Creates an panel overlay to be managed by rendering manager.
	PanelOverlay* createPanelOverlay();
	PanelOverlay* createPanelOverlay(const std::string& name);

	//! Creates an text overlay to be managed by rendering manager.
	TextOverlay* createTextOverlay();
	TextOverlay* createTextOverlay(const std::string& name);

	//! Retrieves a pointer to an overlay by id.
	Overlay* getOverlay(const unsigned int& id);

	//! Retrieves the total number of created overlays.
	unsigned int getNumberOfOverlays() const;

	//! Removes an ovelay from the rendering.
	void removeOverlay(Overlay *overlay);
	//! Removes an overlay from the rendering.
	void removeOverlay(const unsigned int& id);
	//! Removes (and destroys) all overlays from the rendering.
	void removeAllOverlays();

	//!  Creates a font to be managed by this render manager.
	Font* createFont(const std::string& fontFilename);

	//! Retrieves a pointer to a font by id.
	Font* getFont(const unsigned int& id);

	//! Retrieves the total number of created fonts.
	unsigned int getNumberOfFonts() const;

	//!  Removes a font from the rendering.
	void removeFont(Font *fnt);
	//!  Removes a font from the rendering.
	void removeFont(const unsigned int& id);
	//! Removes (and destroys) all fonts from the rendering
	void removeAllFonts();

	//! Creates a shader to be managed by script manager.
	Shader* createShader(const std::string& shaderFilename, const ShaderType& type);

	//! Retrieves a pointer to a shader by id.
	Shader* getShader(const unsigned int& id);

	//! Retrieves the total number of created shaders.
	unsigned int getNumberOfShaders() const;

	//! Removes a shader.
	void removeShader(Shader* shader);
	//! Removes a shader.
	void removeShader(const unsigned int& id);
	//! Removes (and destroys) all shaders.
	void removeAllShaders();

	//! Create a vertex buffer.
	VertexBuffer* createVertexBuffer(unsigned int vertexSize, unsigned int numVertices, resource::BufferUsage usage);
	//! Removes a vertex buffer.
	void removeVertexBuffer(VertexBuffer* buf);
	//! Removes all vertex buffers.
	void removeAllVertexBuffers();
	
	//! Create an index buffer.
	IndexBuffer* createIndexBuffer(IndexType idxType, unsigned int numIndexes, resource::BufferUsage usage);
	//! Removes an index buffer.
	void removeIndexBuffer(IndexBuffer* buf);
	//! Removes all index buffers.
	void removeAllIndexBuffers();

	//! Creates a vertex declaration.
	VertexDeclaration* createVertexDeclaration();
	//! Removes a vertex declaration.
	void removeVertexDeclaration(VertexDeclaration* decl);
	//! Removes all vertex declarations.
	void removeAllVertexDeclarations();

	//! Creates a new vertex buffer binding.
	VertexBufferBinding* createVertexBufferBinding();
	//! Removes a vertex buffer binding.
	void removeVertexBufferBinding(VertexBufferBinding* binding);
	//! Removes all vertex buffer bindings.
	void removeAllVertexBufferBindings();

	void setAmbientLight(const Color& ambient = Color::White);

	void setFog(FogMode mode = FM_NONE, const Color& color = Color::White, float density = 0.001f, float start = 0.0f, float end = 1.0f);

	//! Converts a uniform projection matrix suitable for the current render system.
	void convertProjectionMatrix(const core::matrix4& matrix, core::matrix4& dest);

	//! Gets the minimum (closest) depth value to be used when rendering using identity transforms.
	float getMinimumDepthInputValue();
	//! Gets the maximum (farthest) depth value to be used when rendering using identity transforms.
	float getMaximumDepthInputValue();

	//! Returns the horizontal texel offset value required for mapping texel origins to pixel origins.
	float getHorizontalTexelOffset();
	//! Returns the vertical texel offset value required for mapping texel origins to pixel origins.
	float getVerticalTexelOffset();

	static RenderManager& getInstance();
	static RenderManager* getInstancePtr();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void startImpl();
	void stopImpl();
	void updateImpl(float elapsedTime);
	void setSystemDriverImpl(core::SystemDriver* systemDriver);
	void removeSystemDriverImpl();
	void registerDefaultFactoriesImpl();
	void removeDefaultFactoriesImpl();

	FontFactory* mDefaultFontFactory;
	MaterialFactory* mDefaultMaterialFactory;
	MeshDataFactory* mDefaultMeshDataFactory;

	game::ComponentFactory* mDefaultCameraFactory;
	game::ComponentFactory* mDefaultLightFactory;

	RenderDriver* mRenderDriver;

	static std::list<FrameEventReceiver*> mFrameEventReceivers;

	// Available rendering windows
	std::map<unsigned int, RenderWindow*> mRenderWindows;
	RenderWindow* mMainWindow;

	//! Central list of lights - for easy memory management and lookup.
	std::map<unsigned int, Light*> mLights;

	//! Central list of renderables - for easy memory management and lookup.
	std::map<unsigned int, Renderable*> mRenderables;

	//! Central list of overlays - for easy memory management and lookup.
	std::map<unsigned int, Overlay*> mOverlays;

	//! Central list of fonts - for easy memory management and lookup.
	std::map<unsigned int, Font*> mFonts;

	//! Central list of cameras - for easy memory management and lookup.
	std::map<unsigned int, Camera*> mCameras;

	//! Central list of shaders - for easy memory management and lookup.
	std::map<unsigned int, Shader*> mShaders;

	std::list<VertexDeclaration*> mVertexDeclarations;
	std::list<VertexBufferBinding*> mVertexBufferBindings;
	std::list<VertexBuffer*> mVertexBuffers;
	std::list<IndexBuffer*> mIndexBuffers;

	struct SolidRenderable
	{
		SolidRenderable()
		{
			renderable = NULL;
			materialID = 0;
		}

		bool operator < (const SolidRenderable& other) const
		{
			return (materialID < other.materialID);
		}

		Renderable* renderable;
		unsigned int materialID;//Renderable material ID
	};

	struct TransparentRenderable
	{
		TransparentRenderable()
		{
			renderable = NULL;
			distance = 0;
		}

		bool operator < (const TransparentRenderable& other) const
		{
			return (distance > other.distance);
		}

		Renderable* renderable;
		float distance;//Renderable distance to camera
	};

	struct DistanceLight
	{
		DistanceLight()
		{
			light = NULL;
			distance = 0;
		}

		bool operator < (const DistanceLight& other) const
		{
			return (distance < other.distance);
		}

		Light* light;
		float distance;//Light distance to renderable
	};
	
	std::vector<SolidRenderable> mSolidRenderables;
	std::vector<TransparentRenderable> mTransparentRenderables;
	std::vector<DistanceLight> mDistanceLights;

	std::list<Light*> mLightsAffectingFrustum;

	//! Current ambient light.
	Color mAmbientLight;

	//! Shader auto parameter data.
	ShaderParamData mShaderParamData;

	//! The default material.
	Material* mDefaultMaterial;

	//! Current viewport (dest for current rendering operations).
	Viewport* mCurrentViewport;

	Frustum* mFrustum;

	// Fog
	FogMode mFogMode;
	Color mFogColor;
	float mFogDensity;
	float mFogStart;
	float mFogEnd;

	signed int mLastViewportWidth;
	signed int mLastViewportHeight;

	unsigned int mFaceCount;
	unsigned int mVertexCount;

	void fireFrameStarted();

	void fireFrameEnded();

	void beginGeometryCount();

	void addGeometruCount(Renderable* renderable);

	unsigned int getVertexCount();

	unsigned int getFaceCount();

	void endGeometryCount();

	void render(Camera* camera, Viewport* viewport);

	void setCurrentViewport(Viewport* viewport);

	void beginFrame(Viewport* viewport);

	void findLightsAffectingFrustum(Camera* camera);

	void findLightsAffectingRenderables(Renderable* renderable);

	void findVisibleRenderables(Camera* camera);

	void renderVisibleRenderables();

	void renderVisibleOverlays(Camera* camera, Viewport* viewport);
	void renderSingleOverlay(Overlay* overlay, Camera* camera, Viewport* viewport);

	void renderSingleRenderable(Renderable* renderable);

	void setMaterial(Material* material);

	void endFrame();
};

} // end namespace render

#endif