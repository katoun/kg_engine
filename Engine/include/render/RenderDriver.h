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

#ifndef _RENDER_DRIVER_H_
#define _RENDER_DRIVER_H_

#include <core/Config.h>
#include <core/Vector3d.h>
#include <core/Quaternion.h>
#include <core/Matrix4.h>
#include <core/SystemDriver.h>
#include <render/Material.h>
#include <render/BlendMode.h>
#include <render/Color.h>

#include <vector>

namespace core
{
class sphere3d;
}

namespace resource
{
enum BufferUsage;
enum PixelFormat;
}

namespace render
{

class Frustum;
class Light;
class Camera;
class Model;
class MeshData;
class Font;
class Viewport;
class RenderWindow;
class FrameEventReceiver;
struct FrameEvent;
class Shader;
class VertexBuffer;
class IndexBuffer;
class Texture;
class ShaderParamData;
enum IndexType;
enum ShaderType;

//! Defines the functionality of a 3D Rendering API
//!
//! The RenderSystem class provides a base interface
//! which abstracts the general functionality of the 3D API
//! e.g. Direct3D or OpenGL. Whilst a few of the general
//! methods have implementations, most of this class is
//! abstract, requiring a subclass based on a specific API
//! to be constructed to provide the full functionality.
class ENGINE_PUBLIC_EXPORT RenderDriver: public core::SystemDriver
{
public:

	// Default Constructor
	RenderDriver(const std::string& name);

	// Destructor
	virtual ~RenderDriver();
	
	//! Creates a new rendering window.
	virtual RenderWindow* createRenderWindow(int width, int height, int colorDepth,
		bool fullScreen, int left = 0, int top = 0, bool depthBuffer = true, void* windowId = nullptr) = 0;

	//! Create a vertex buffer.
	virtual VertexBuffer* createVertexBuffer(unsigned int vertexSize, unsigned int numVertices, resource::BufferUsage usage) = 0;
	//! Removes a vertex buffer.
	virtual void removeVertexBuffer(VertexBuffer* buf) = 0;

	//! Create an index buffer.
	virtual IndexBuffer* createIndexBuffer(IndexType idxType, unsigned int numIndexes, resource::BufferUsage usage) = 0;
	//! Removes an index buffer.
	virtual void removeIndexBuffer(IndexBuffer* buf) = 0;

	//! Signifies the beginning of a frame, ie the start of rendering on a single viewport. Will occur
	//! several times per complete frame if multiple viewports exist.
	virtual void beginFrame(Viewport *vp) = 0;

	//! Render a model to the active viewport.
	virtual void renderModel(Model* model) = 0;

	virtual void renderGridPlane(unsigned int zfar) = 0;

	virtual void renderWorldAxes() = 0;

	virtual void renderAxes(const core::vector3d& position, const core::quaternion& orientation) = 0;//for debugging

	virtual void renderBoundingSphere(const core::sphere3d& sphere) = 0;//for debugging

	virtual void renderBoundingBox(const core::aabox3d& box) = 0;//for debugging

	virtual void renderFrustumVolume(const core::vector3d* corners) = 0;//for debugging

	//! Ends rendering of a frame to the current viewport.
	virtual void endFrame() = 0;

	virtual void setViewport(Viewport *vp) = 0;

	virtual void setWorldMatrix(const core::matrix4& m) = 0;

	virtual void setViewMatrix(const core::matrix4& m) = 0;

	virtual void setProjectionMatrix(const core::matrix4& m) = 0;

	core::matrix4& getWorldMatrix();

	core::matrix4& getViewMatrix();

	core::matrix4& getProjectionMatrix();

	//! Converts a uniform projection matrix to suitable for this render system.
	virtual void convertProjectionMatrix(const core::matrix4& matrix, core::matrix4& dest);

	virtual void setShadingType(ShadeOptions so) = 0;

	//! Sets the surface properties to be used for future rendering.
	//! \param ambient: The amount of ambient (sourceless and directionless)
	//! light an object reflects. Affected by the color/amount of ambient light in the scene.
	//! \param diffuse: The amount of light from directed sources that is
	//! reflected (affected by color/amount of point, directed and spot light sources)
	//! \param specular: The amount of specular light reflected. This is also
	//! affected by directed light sources but represents the color at the
	//! highlights of the object.
	//! \param emissive: The color of light emitted from the object. Note that
	//! this will make an object seem brighter and not dependent on lights in
	//! the scene, but it will not act as a light, so will not illuminate other objects.
	//! \param shininess: A value which only has an effect on specular highlights (so
	//! specular must be non-black). The higher this value, the smaller and crisper the
	//! specular highlights will be, imitating a more highly polished surface.
	//! This value is not constrained to 0.0-1.0, in fact it is likely to
	//! be more (10.0 gives a modest sheen to an object).
	virtual void setSurfaceParams(const Color& ambient, const Color& diffuse,
			const Color& specular, const Color& emissive, float shininess) = 0;

	//! Sets the global blending factors for combining subsequent renders with the existing frame contents.
	virtual void setSceneBlending(SceneBlendFactor sourceFactor, SceneBlendFactor destFactor) = 0;
	
	//! Utility function for setting all the properties of a texture unit at once.
	virtual void setTextureUnitSettings(unsigned int texUnit, TextureUnit* tu);

	//! Turns off a texture unit.
	virtual void disableTextureUnit(unsigned int texUnit);

	//! Turns off all the texture units starting from a given texture unit.
	virtual void disableTextureUnitsFrom(unsigned int texUnit);
	
	//! Sets the status of a single texture stage.
	//!
	//! \param enabled: boolean to turn an unit on/off
	//! \param unit: The index of the texture unit to modify. Multi-texturing hardware can support multiple units.
	//! \param tex: The the texture to use.
	virtual void setTexture(bool enabled, unsigned int unit, Texture* tex) = 0;

	//! Sets the texture coordinate set to use for a texture unit.
	virtual void setTextureCoordSet(unsigned int unit, unsigned int index);

	//! Sets the texture blend modes from a TextureUnit.
	//! \param unit: Texture unit.
	//! \param bm: Details of the blending mode.
	virtual void setTextureBlendMode(unsigned int unit, const LayerBlendMode& bm) = 0;

	virtual void setLightingEnabled(bool enabled) = 0;

	virtual void setAmbientLight(float red, float green, float blue, float alpha) = 0;

	///! Sets a list of lights to the renderer.
	virtual void setLights(const std::vector<Light*>& lights) = 0;

	//! Sets whether or not the depth buffer check is performed before a pixel write.
	virtual void setDepthBufferCheckEnabled(bool enabled = true) = 0;

	//! Sets whether or not the depth buffer is updated after a pixel write.
	virtual void setDepthBufferWriteEnabled(bool enabled = true) = 0;

	//! Sets the fogging mode for future geometry.
	virtual void setFog(FogMode mode = FM_NONE, const Color& color = Color::White, float expDensity = 1.0f, float linearStart = 0.0f, float linearEnd = 1.0f) = 0;

	//! Return the number of texture units available.
	virtual unsigned int getNumTextureUnits() = 0;

	//! Binds a given Shader (but not the parameters). 
	virtual void bindShader(Shader* shader);

	//! Unbinds a given Shader.
	virtual void unbindShader(ShaderType type);

	//! Returns whether or not a Shader of the given type is currently bound.
	virtual bool isShaderBound(ShaderType type);

	//! update Shader parameters to a given Shader.
	virtual void updateShaderAutoParameters(Shader* shader, ShaderParamData& data);

	//! Gets the minimum (closest) depth value to be used when rendering using identity transforms.
	virtual float getMinimumDepthInputValue() = 0;
	//! Gets the maximum (farthest) depth value to be used when rendering using identity transforms.
	virtual float getMaximumDepthInputValue() = 0;

	//! Returns the horizontal texel offset value required for mapping texel origins to pixel origins.
	virtual float getHorizontalTexelOffset() = 0;
	//! Returns the vertical texel offset value required for mapping texel origins to pixel origins.
	virtual float getVerticalTexelOffset() = 0;

protected:
	
	core::matrix4 mWorldMatrix;
	core::matrix4 mViewMatrix;
	core::matrix4 mProjMatrix;

	unsigned int mTextureCoordIndex[ENGINE_MAX_TEXTURE_COORD_SETS];

	// Array of up to 8 lights, indexed as per API
	// Note that a nullptr value indicates a free slot
	Light* mLights[ENGINE_MAX_SIMULTANEOUS_LIGHTS];

	// Saved manual color blends
	Color mManualBlendColors[ENGINE_MAX_TEXTURE_LAYERS][2];

	// Texture units from this upwards are disabled
	unsigned int mDisabledTexUnitsFrom;

	Shader* mCurrentVertexShader;
	Shader* mCurrentFragmentShader;
	Shader* mCurrentGeometryShader;
};

} // end namespace render

#endif