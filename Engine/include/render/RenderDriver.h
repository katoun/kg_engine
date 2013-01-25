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
	virtual void renderModel(Model* model, Material* material) = 0;

	//! Ends rendering of a frame to the current viewport.
	virtual void endFrame() = 0;

	virtual void setViewport(Viewport *vp) = 0;

	virtual void setWorldMatrix(const core::matrix4& m);

	virtual void setViewMatrix(const core::matrix4& m);

	virtual void setProjectionMatrix(const core::matrix4& m);

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

	Shader* mCurrentVertexShader;
	Shader* mCurrentFragmentShader;
	Shader* mCurrentGeometryShader;
};

} // end namespace render

#endif