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

#ifndef _GL_RENDER_DRIVER_H_
#define _GL_RENDER_DRIVER_H_

#include <GLConfig.h>
#include <render/RenderDriver.h>
#include <render/VertexBuffer.h>
#include <core/Singleton.h>

namespace resource
{
class Resource;
class Serializer;
}

namespace render
{

class BufferManager;
class ShaderParameters;
enum ShaderType;

class GLRenderDriver: public RenderDriver, public core::Singleton<GLRenderDriver>
{
public:

	GLRenderDriver();
	~GLRenderDriver();

	RenderWindow* createRenderWindow(int width, int height, int colorDepth, bool fullScreen, int left = 0, int top = 0, bool												depthBuffer = true, void* windowId = nullptr);

	VertexBuffer* createVertexBuffer(unsigned int vertexSize, unsigned int numVertices, resource::BufferUsage usage);
	
	void removeVertexBuffer(VertexBuffer* buf);

	IndexBuffer* createIndexBuffer(IndexType idxType, unsigned int numIndexes, resource::BufferUsage usage);
	
	void removeIndexBuffer(IndexBuffer* buf);

	void beginFrame(Viewport* vp);

	void renderModel(Model* model, Material* material);

	void renderGridPlane(unsigned int zfar);

	void renderWorldAxes();

	void renderAxes(const core::vector3d& position, const core::quaternion& orientation);

	void renderBoundingSphere(const core::sphere3d& sphere);

	void renderBoundingBox(const core::aabox3d& box);

	void renderFrustumVolume(const core::vector3d* corners);

	void endFrame();

	void setViewport(Viewport* viewport);

	void setWorldMatrix(const core::matrix4 &m);

	void setViewMatrix(const core::matrix4 &m);

	void setProjectionMatrix(const core::matrix4 &m);

	//////////////////////////////////////////////////////////

	void setShadingType(ShadeOptions so);

	void setSceneBlending(SceneBlendFactor sourceFactor, SceneBlendFactor destFactor);

	void setSurfaceParams(const Color& ambient, const Color& diffuse, const Color& specular, const Color& emissive, float shininess);

	void setTexture(bool enabled, unsigned int unit, Texture* tex);

	void setTextureBlendMode(unsigned int unit, const LayerBlendMode& bm);

	void setLights(const std::vector<Light*>& lights);

	void setDepthBufferCheckEnabled(bool enabled = true);

	void setDepthBufferWriteEnabled(bool enabled = true);

	void setFog(FogMode mode = FM_NONE, const Color& color = Color::White, float expDensity = 1.0f, float linearStart = 0.0f, float linearEnd = 1.0f);

	unsigned int getNumTextureUnits();

	float getMinimumDepthInputValue();

	float getMaximumDepthInputValue();

	float getHorizontalTexelOffset();

	float getVerticalTexelOffset();

	//! Utility function to get the correct GL usage based on HBU's.
	static GLenum getGLUsage(resource::BufferUsage usage);

	static GLenum getGLType(ShaderType type);

	static GLRenderDriver* getInstance();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void updateImpl(float elapsedTime);

	void makeGLMatrix(GLfloat gl_matrix[16], const core::matrix4& m);
	void setGLLight(unsigned int index, Light* light);
	void setGLLightPositionDirection(unsigned int index, Light* light);
	static GLint getGLBlendMode(SceneBlendFactor gameBlend);

	//! Utility function to get the correct GL type based on VET's.
	static GLenum getGLType(VertexElementType type);

	//////////TEMP FOR TESING/////////////
	GLuint ProgramID;
	
	GLuint PositionID;
	GLuint NormalID;
	GLuint TexCoords0ID;
	
	GLuint MatrixID;

	GLuint DiffuseTextureID;
	//////////TEMP FOR TESING/////////////
};

} // end namespace render

#endif // _GL_RENDER_DRIVER_H_