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
#include <core/Math.h>
#include <core/Sphere3d.h>
#include <core/Aabox3d.h>
#include <core/LogDefines.h>
#include <render/RenderDefines.h>
#include <render/Light.h>
#include <render/Model.h>
#include <render/Viewport.h>
#include <render/Shader.h>
#include <render/VertexBuffer.h>
#include <render/IndexBuffer.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>
#include <game/GameObject.h>
#include <game/ComponentDefines.h>
#include <game/Transform.h>
#include <GLRenderDriver.h>
#include <GLTexture.h>
#include <GLShader.h>
#include <GLMaterial.h>
#include <GLVertexBuffer.h>
#include <GLIndexBuffer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#if ENGINE_PLATFORM == PLATFORM_WINDOWS
#include <win32/Win32Window.h>
#endif

template<> render::GLRenderDriver* core::Singleton<render::GLRenderDriver>::m_Singleton = nullptr;

namespace render
{

GLRenderDriver::GLRenderDriver(): RenderDriver("OpenGL RenderDriver") {}

GLRenderDriver::~GLRenderDriver() {}

RenderWindow* GLRenderDriver::createRenderWindow(int width, int height, int colorDepth, bool fullScreen, int left, int top, bool depthBuffer, void* windowId)
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

	return pRenderWindow;
}

VertexBuffer* GLRenderDriver::createVertexBuffer(VertexBufferType vertexBufferType, VertexElementType vertexElementType, unsigned int numVertices, resource::BufferUsage usage)
{
	VertexBuffer* buf = new GLVertexBuffer(vertexBufferType, vertexElementType, numVertices, usage);

	return buf;
}

void GLRenderDriver::removeVertexBuffer(VertexBuffer* buf)
{
	GLVertexBuffer* GLbuf = static_cast<GLVertexBuffer*>(buf);

	assert(GLbuf);
	SAFE_DELETE(GLbuf);
}

IndexBuffer* GLRenderDriver::createIndexBuffer(IndexType idxType, unsigned int numIndexes, resource::BufferUsage usage)
{
	IndexBuffer* buf = new GLIndexBuffer(idxType, numIndexes, usage);

	return buf;
}

void GLRenderDriver::removeIndexBuffer(IndexBuffer* buf)
{
	GLIndexBuffer* GLbuf = static_cast<GLIndexBuffer*>(buf);

	assert(GLbuf);
	SAFE_DELETE(GLbuf);
}

void GLRenderDriver::beginFrame(Viewport* vp)
{
	// Clear the viewport if required
	if (vp->getClearEveryFrame())
	{
		// Activate the viewport clipping
		glEnable(GL_SCISSOR_TEST);

		Color col = vp->getBackgroundColor();

		glClearColor(col.r, col.g, col.b, col.a);

		glClearDepth(1.0f);									// Depth Buffer Setup
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	}
}

void GLRenderDriver::render(RenderStateData& renderStateData)
{
	if (renderStateData.getCurrentMaterial() == nullptr)
		return;

	GLMaterial* pGLMaterial = static_cast<GLMaterial*>(renderStateData.getCurrentMaterial());
	if (pGLMaterial == nullptr)
		return;

	Model* pModel = renderStateData.getCurrentModel();

	if (pModel == nullptr)
		return;

	if (pModel->getVertexBuffer(VERTEX_BUFFER_TYPE_POSITION) == nullptr || pModel->getIndexBuffer() == nullptr)
		return;

	glUseProgram(pGLMaterial->getGLHandle());

	GLShaderParameter* pGLShaderParameter = nullptr;
	GLShaderVertexParameter* pGLShaderVertexParameter = nullptr;

	/////////////Textures/////////////
	std::vector<ShaderTextureParameter*>& shaderTextureParameters = pGLMaterial->getTextureParameters();
	std::list<ShaderTextureParameter*>::const_iterator ti;
	for (unsigned int i = 0; i < shaderTextureParameters.size(); ++i)
	{
		ShaderTextureParameter* pTextureParameter = shaderTextureParameters[i];
		if (pTextureParameter == nullptr)
			continue;
		
		pGLShaderParameter = static_cast<GLShaderParameter*>(pTextureParameter->mParameter);
		GLTexture* glTexture = static_cast<GLTexture*>(pGLMaterial->getTextureUnit(i));
		if (glTexture == nullptr || pGLShaderParameter == nullptr)
			continue;
		
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, glTexture->getGLID());
		glUniform1i(pGLShaderParameter->ParameterID, i);
	}
	//////////////////////////////////

	//////////AutoParameters//////////
	std::list<ShaderAutoParameter*>& shaderAutoParameters = pGLMaterial->getAutoParameters();
	std::list<ShaderAutoParameter*>::const_iterator ai;
	for (ai = shaderAutoParameters.begin(); ai != shaderAutoParameters.end(); ++ai)
	{
		ShaderAutoParameter* pAutoParameter = (*ai);
		if (pAutoParameter == nullptr)
			continue;
		
		pGLShaderParameter = static_cast<GLShaderParameter*>(pAutoParameter->mParameter);
		if (pGLShaderParameter == nullptr)
			continue;

		switch (pAutoParameter->mAutoParameterType)
		{
		case SHADER_AUTO_PARAMETER_TYPE_MODEL_MATRIX:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getModelMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_MODEL_MATRIX:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getInverseModelMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_VIEW_MATRIX:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEW_MATRIX:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getInverseViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_PROJECTION_MATRIX:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_PROJECTION_MATRIX:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getInverseProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_VIEW_PROJECTION_MATRIX:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getViewProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEW_PROJECTION_MATRIX:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getInverseViewProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_MODEL_VIEW_MATRIX:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getModelViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_MODEL_VIEW_MATRIX:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getInverseModelViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_MODEL_VIEW_PROJECTION_MATRIX:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getModelViewProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_MODEL_VIEW_PROJECTION_MATRIX:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getInverseModelViewProjectionMatrix());
			break;

		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getCurrentLightPosition());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_OBJECT_SPACE:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getCurrentLightPositionObjectSpace());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_VIEW_SPACE:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getCurrentLightPositionViewSpace());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getCurrentLightDirection());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_OBJECT_SPACE:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getCurrentLightDirectionObjectSpace());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_VIEW_SPACE:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getCurrentLightDirectionViewSpace());
			break;
		
		case SHADER_AUTO_PARAMETER_TYPE_AMBIENT_LIGHT_COLOUR:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getAmbientLightColour());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIFFUSE_COLOUR:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getCurrentLightDiffuseColour());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_SPECULAR_COLOUR:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getCurrentLightSpecularColour());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_ATTENUATION:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getCurrentLightAttenuation());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getCameraPosition());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION_OBJECT_SPACE:
			pGLMaterial->setParameter(pGLShaderParameter, renderStateData.getCameraPositionObjectSpace());
			break;
		}
	}
	//////////////////////////////////

	/////////////Buffers//////////////
	std::vector<ShaderVertexParameter*>& vertexParameters = pGLMaterial->getVertexParameters();
	for (std::size_t vertexType = VERTEX_BUFFER_TYPE_POSITION; vertexType != VERTEX_BUFFER_TYPE_COUNT; ++vertexType)
	{
		if (vertexParameters[vertexType] == nullptr)
			continue;
		
		pGLShaderVertexParameter = static_cast<GLShaderVertexParameter*>(vertexParameters[vertexType]);

		if (pGLShaderParameter == nullptr)
			continue;

		VertexBuffer* pVertexBuffer = pModel->getVertexBuffer((VertexBufferType)vertexType);
		if (pVertexBuffer == nullptr)
			continue;

		glBindBuffer(GL_ARRAY_BUFFER, ((const GLVertexBuffer*)(pVertexBuffer))->getGLBufferId());

		GLuint index = pGLShaderVertexParameter->ParameterID;
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

		glVertexAttribPointer(
							index,			// The attribute we want to configure
							size,			// size
							type,			// type
							GL_FALSE,		// normalized?
							stride,			// stride
							(void*)0		// array buffer offset
							);
		glEnableVertexAttribArray(index);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((GLIndexBuffer*)(pModel->getIndexBuffer()))->getGLBufferId());

	GLenum primType = getGLType(pModel->getRenderOperationType());
	GLenum indexType = (pModel->getIndexBuffer()->getType() == IT_16BIT) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
	
	glDrawElements(primType, pModel->getIndexBuffer()->getNumIndexes(), indexType, (void*)0);
	//////////////////////////////////

	for (unsigned int i = 0; i < vertexParameters.size(); ++i)
	{
		pGLShaderVertexParameter = static_cast<GLShaderVertexParameter*>(vertexParameters[i]);

		if (pGLShaderVertexParameter == nullptr)
			continue;

		glDisableVertexAttribArray(pGLShaderVertexParameter->ParameterID);
	}
}

void GLRenderDriver::endFrame()
{
	// Deactivate the viewport clipping
	glDisable(GL_SCISSOR_TEST);
}

void GLRenderDriver::setViewport(Viewport* viewport)
{
	if (viewport == nullptr)
		return;

	GLsizei x, y, w, h;

	RenderTarget* target;
	target = viewport->getTarget();

	// Calculate the "lower-left" corner of the viewport
	w = viewport->getActualWidth();
	h = viewport->getActualHeight();
	x = viewport->getActualLeft();
	y = target->getHeight() - viewport->getActualTop() - h;

	glViewport(x, y, w, h);

	// Configure the viewport clipping
	glScissor(x, y, w, h);
}

void GLRenderDriver::initializeImpl()
{
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		MessageBox(nullptr, "Can't Initialize GLew.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Check for OpenGL 3.1
	/*if(!GLEW_VERSION_3_1)
	{
		MessageBox(nullptr, "Can't Initialize OpenGL 3.1.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}*/

	// Check for OpenGL 4.2
	/*if(!GLEW_VERSION_4_2)
	{
		MessageBox(nullptr, "Can't Initialize OpenGL 4.2.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}*/

	glClearDepth(1.0f);
	glColor4f(1.0f,1.0f,1.0f,1.0f);						// Set Color to initial value

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glDepthMask(GL_TRUE);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	glDisable(GL_BLEND);								// Turn Blending Off
}

void GLRenderDriver::uninitializeImpl() {}

void GLRenderDriver::updateImpl(float elapsedTime) {}

GLenum GLRenderDriver::getGLUsage(resource::BufferUsage usage)
{
	switch(usage)
	{
	case resource::BU_STATIC:
	case resource::BU_STATIC_WRITE_ONLY:
		return GL_STATIC_DRAW;
	case resource::BU_DYNAMIC:
	case resource::BU_DYNAMIC_WRITE_ONLY:
		return GL_DYNAMIC_DRAW;
	case resource::BU_DYNAMIC_WRITE_ONLY_DISCARDABLE:
		return GL_STREAM_DRAW;
	default:
		return GL_DYNAMIC_DRAW;
	};
}

GLenum GLRenderDriver::getGLType(ShaderType type)
{
	switch(type)
	{
	case SHADER_TYPE_VERTEX:
		return GL_VERTEX_SHADER;
	case SHADER_TYPE_FRAGMENT:
		return GL_FRAGMENT_SHADER;
	case SHADER_TYPE_GEOMETRY:
		return GL_GEOMETRY_SHADER;
	default:
		return GL_VERTEX_SHADER;
	}
}

GLenum GLRenderDriver::getGLType(VertexElementType type)
{
	switch(type)
	{
	case VERTEX_ELEMENT_TYPE_FLOAT1:
	case VERTEX_ELEMENT_TYPE_FLOAT2:
	case VERTEX_ELEMENT_TYPE_FLOAT3:
	case VERTEX_ELEMENT_TYPE_FLOAT4:
		return GL_FLOAT;
	case VERTEX_ELEMENT_TYPE_SHORT1:
	case VERTEX_ELEMENT_TYPE_SHORT2:
	case VERTEX_ELEMENT_TYPE_SHORT3:
	case VERTEX_ELEMENT_TYPE_SHORT4:
		return GL_SHORT;
	case VERTEX_ELEMENT_TYPE_COLOR:
		return GL_UNSIGNED_BYTE;
	default:
		return 0;
	}
}

GLenum GLRenderDriver::getGLType(RenderOperationType type)
{
	switch (type)
	{
	case render::ROT_POINT_LIST:
		 return GL_POINTS;
	case render::ROT_LINE_LIST:
		return GL_LINES;
	case render::ROT_LINE_STRIP:
		return GL_LINE_STRIP;
	case render::ROT_TRIANGLE_LIST:
		return GL_TRIANGLES;
	case render::ROT_TRIANGLE_STRIP:
		return GL_TRIANGLE_STRIP;
	case render::ROT_TRIANGLE_FAN:
		return GL_TRIANGLE_FAN;
	default:
		return GL_FLOAT;
	}
}

GLRenderDriver* GLRenderDriver::getInstance()
{
	return core::Singleton<GLRenderDriver>::getInstance();
}

} // end namespace render
