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
#include <render/VertexIndexData.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>
#include <game/GameObject.h>
#include <game/ComponentDefines.h>
#include <game/Transform.h>
#include <GLRenderDriver.h>
#include <GLTexture.h>
#include <GLShader.h>
#include <GLVertexBuffer.h>
#include <GLIndexBuffer.h>

#if ENGINE_PLATFORM == PLATFORM_WINDOWS
#include <win32/Win32Window.h>
#endif

#define VBO_BUFFER_OFFSET(i) ((char *)nullptr + (i))

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

VertexBuffer* GLRenderDriver::createVertexBuffer(unsigned int vertexSize, unsigned int numVertices, resource::BufferUsage usage)
{
	VertexBuffer* buf = new GLVertexBuffer(vertexSize, numVertices, usage);

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

		glClearColor(col.R, col.G, col.B, col.A);

		glClearDepth(1.0f);									// Depth Buffer Setup
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	}
}

void GLRenderDriver::renderModel(Model* model, Material* material)
{
	if (model == nullptr)
		return;

	if (model->getVertexData() == nullptr || model->getIndexData() == nullptr)
		return;

	glUseProgram(ProgramID);

	core::matrix4 mWorldViewProjectionMatrix = mProjMatrix * mViewMatrix * mWorldMatrix;

	glUniformMatrix4fv(MatrixID, 1, GL_TRUE/*GL_FALSE*/, mWorldViewProjectionMatrix.get());

	/////////////Textures/////////////
	for (unsigned int i=0; i<material->getNumTextureUnits(); ++i)
	{
		GLTexture* glTex = static_cast<GLTexture*>(material->getTextureUnit(i));

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, glTex->getGLID());
		glUniform1i(glTex->getGLID(), i);
	}
	//////////////////////////////////

	/////////////Buffers/////////////
	void* pBufferData = nullptr;

	const std::list<VertexElement*>& elems = model->getVertexData()->vertexDeclaration->getElements();
	std::list<VertexElement*>::const_iterator i;
	for (i = elems.begin(); i != elems.end(); ++i)
	{
		VertexElement* pVertexElement = (*i);

		VertexBuffer* vertexBuffer = model->getVertexData()->vertexBufferBinding->getBuffer(pVertexElement->getSource());

		glBindBuffer(GL_ARRAY_BUFFER, ((const GLVertexBuffer*)(vertexBuffer))->getGLBufferId());
		pBufferData = VBO_BUFFER_OFFSET(pVertexElement->getOffset());

		GLuint index = 0;
		GLint size = pVertexElement->getCount();
		GLenum type = getGLType(pVertexElement->getType());
		GLsizei stride = (GLsizei)(vertexBuffer->getVertexSize());

		switch(pVertexElement->getSemantic())
		{
		case VERTEX_ELEMENT_SEMANTIC_POSITION:
			index = PositionID;
			break;
		case VERTEX_ELEMENT_SEMANTIC_NORMAL:
			index = NormalID;
			break;
		case VERTEX_ELEMENT_SEMANTIC_TEXTURE_COORDINATES:
			index = TexCoords0ID;
			break;
		default:
			break;
		}

		glVertexAttribPointer(
							index,			// The attribute we want to configure
							size,			// size
							type,			// type
							GL_FALSE,		// normalized?
							stride,			// stride
							pBufferData		// array buffer offset
							);
		glEnableVertexAttribArray(index);
	}
	//////////////////////////////////

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((GLIndexBuffer*)(model->getIndexData()->indexBuffer))->getGLBufferId());
	pBufferData = VBO_BUFFER_OFFSET(0);

	GLenum primType = getGLType(model->getRenderOperationType());
	GLenum indexType = (model->getIndexData()->indexBuffer->getType() == IT_16BIT) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
	
	glDrawElements(primType, model->getIndexData()->indexCount, indexType, pBufferData);

	glDisableVertexAttribArray(PositionID);
	glDisableVertexAttribArray(NormalID);
	glDisableVertexAttribArray(TexCoords0ID);
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

float GLRenderDriver::getMinimumDepthInputValue()
{
	// Range [-1.0f, 1.0f]
	return -1.0f;
}

float GLRenderDriver::getMaximumDepthInputValue()
{
	// Range [-1.0f, 1.0f]
	return 1.0f;
}

float GLRenderDriver::getHorizontalTexelOffset()
{
	// No offset in GL
	return 0.0f;
}
	  
float GLRenderDriver::getVerticalTexelOffset()
{
	// No offset in GL
	return 0.0f;
}

void GLRenderDriver::initializeImpl()
{
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		MessageBox(nullptr, "Can't Initialize GLew.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Check for OpenGL 2.0
	if(!GLEW_VERSION_2_0)
	{
		MessageBox(nullptr, "Can't Initialize OpenGL 2.0.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

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


	//////////TEMP FOR TESING/////////////
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string str_source = "";
	str_source += "attribute vec3 position;\n";
	str_source += "attribute vec3 normal;\n";
	str_source += "attribute vec2 texCoords0;\n";
	str_source += "\n";
	str_source += "uniform mat4 worldViewProj;\n";
	str_source += "\n";
	str_source += "varying vec2 texCoords;\n";
	str_source += "\n";
	str_source += "void main()\n";
	str_source += "{\n";
	str_source += "	texCoords = texCoords0;\n";
	str_source += "	gl_Position = worldViewProj * vec4(position, 1.0);\n";
	str_source += "}";
	
	const char* source = str_source.c_str();
	glShaderSource(VertexShaderID, 1, &source, NULL);

	int InfoLogLength;
	GLint compiled;
	glCompileShader(VertexShaderID);
	// check for compile errors
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &compiled);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
		int x = 3;
	}

	str_source = "";
	str_source += "uniform sampler2D diffuseMap;\n";
	str_source += "\n";
	str_source += "varying vec2 texCoords;\n";
	str_source += "\n";
	str_source += "void main()\n";
	str_source += "{\n";
	str_source += "	vec4 diffuse = texture2D(diffuseMap, texCoords);\n";
	str_source += "\n";
	str_source += "	gl_FragColor = diffuse;\n";
	str_source += "}";

	source = str_source.c_str();
	glShaderSource(FragmentShaderID, 1, &source, NULL);

	compiled;
	glCompileShader(FragmentShaderID);
	// check for compile errors
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &compiled);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
		int x = 3;
	}


	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);

	GLint linked;
	glLinkProgram(ProgramID);

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &linked);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
		int x = 3;
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	PositionID			= glGetAttribLocation(ProgramID, "position");
	NormalID			= glGetAttribLocation(ProgramID, "normal");
	TexCoords0ID		= glGetAttribLocation(ProgramID, "texCoords0");
	
	MatrixID			= glGetUniformLocation(ProgramID, "worldViewProj");

	DiffuseTextureID	= glGetUniformLocation(ProgramID, "diffuseMap");
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		int x = 3;
	}

	int x = 3;
	//////////TEMP FOR TESING/////////////
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
