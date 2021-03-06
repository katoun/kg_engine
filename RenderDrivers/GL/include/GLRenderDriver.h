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
#include <render/RenderDefines.h>
#include <render/RenderDriver.h>
#include <core/Singleton.h>

namespace resource
{
class Resource;
class Serializer;
}

namespace render
{

class VertexBuffer;
class IndexBuffer;
enum VertexBufferType;
enum VertexElementType;
enum IndexType;

class GLRenderDriver: public RenderDriver, public core::Singleton<GLRenderDriver>
{
public:

	GLRenderDriver();
	~GLRenderDriver();

	RenderWindow* createRenderWindow(int width, int height, int colorDepth, bool fullScreen, int left = 0, int top = 0, bool												depthBuffer = true, void* windowId = nullptr);

	VertexBuffer* createVertexBuffer(VertexBufferType vertexBufferType, VertexElementType vertexElementType, unsigned int numVertices, resource::BufferUsage usage);
	
	void removeVertexBuffer(VertexBuffer* buf);

	IndexBuffer* createIndexBuffer(IndexType idxType, unsigned int numIndexes, resource::BufferUsage usage);
	
	void removeIndexBuffer(IndexBuffer* buf);

	void beginFrame(Viewport* vp);

	void render(RenderStateData& renderStateData);

	void endFrame();

	void setViewport(Viewport* viewport);

	//! Utility function to get the correct GL types.
	static GLenum getGLUsage(resource::BufferUsage usage);
	static GLenum getGLType(ShaderType type);
	static GLenum getGLType(VertexElementType type);
	static GLenum getGLType(RenderOperationType type);

	static GLRenderDriver* getInstance();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void updateImpl(float elapsedTime);
};

} // end namespace render

#endif // _GL_RENDER_DRIVER_H_