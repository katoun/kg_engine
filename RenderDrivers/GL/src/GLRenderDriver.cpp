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
#include <render/TextureUnit.h>
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

	GLfloat mat[16];
	makeGLMatrix(mat, mWorldViewProjectionMatrix);

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mat[0]);

	TextureUnit* tu = material->getTextureUnit(0);
	if (tu != nullptr)
	{
		GLTexture* glTex = static_cast<GLTexture*>(tu->getTexture());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glTex->getGLID());
		glUniform1i(glTex->getGLID(), 0);
	}

	////////////////////

	void* pBufferData = nullptr;

	const std::list<VertexElement*>& elems = model->getVertexData()->vertexDeclaration->getElements();
	std::list<VertexElement*>::const_iterator i;
	for (i = elems.begin(); i != elems.end(); ++i)
	{
		VertexElement* elem = (*i);

		VertexBuffer* vertexBuffer = model->getVertexData()->vertexBufferBinding->getBuffer(elem->getSource());

		glBindBuffer(GL_ARRAY_BUFFER, ((const GLVertexBuffer*)(vertexBuffer))->getGLBufferId());
		pBufferData = VBO_BUFFER_OFFSET(elem->getOffset());

		switch(elem->getSemantic())
		{
		case VES_POSITION:
			glVertexAttribPointer(
								PositionID,  // The attribute we want to configure
								3,                            // size
								GL_FLOAT,                     // type
								GL_FALSE,                     // normalized?
								0,                            // stride
								(void*)0                      // array buffer offset
								);
			glEnableVertexAttribArray(PositionID);
			break;
		case VES_NORMAL:
			glVertexAttribPointer(
								NormalID,    // The attribute we want to configure
								3,                            // size
								GL_FLOAT,                     // type
								GL_FALSE,                     // normalized?
								0,                            // stride
								(void*)0                      // array buffer offset
								);
			glEnableVertexAttribArray(NormalID);
			break;
		case VES_TEXTURE_COORDINATES:
				glVertexAttribPointer(
					TexCoords0ID,				// The attribute we want to configure
					2,                            // size
					GL_FLOAT,                     // type
					GL_FALSE,                     // normalized?
					0,                            // stride
					(void*)0                      // array buffer offset
					);
				glEnableVertexAttribArray(TexCoords0ID);
			break;
		default:
			break;
		}
	}

	// Find the correct type to render
	GLint primType = GL_TRIANGLES;
	switch (model->getRenderOperationType())
	{
	case render::ROT_POINT_LIST:
		primType = GL_POINTS;
		break;
	case render::ROT_LINE_LIST:
		primType = GL_LINES;
		break;
	case render::ROT_LINE_STRIP:
		primType = GL_LINE_STRIP;
		break;
	case render::ROT_TRIANGLE_LIST:
		primType = GL_TRIANGLES;
		break;
	case render::ROT_TRIANGLE_STRIP:
		primType = GL_TRIANGLE_STRIP;
		break;
	case render::ROT_TRIANGLE_FAN:
		primType = GL_TRIANGLE_FAN;
		break;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((GLIndexBuffer*)(model->getIndexData()->indexBuffer))->getGLBufferId());
	pBufferData = VBO_BUFFER_OFFSET(0);
	
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

void GLRenderDriver::initializeImpl()
{
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		MessageBox(nullptr, "Can't Initialize GLew.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Check for OpenGL 4.2
	if(!GLEW_VERSION_4_2)
	{
		MessageBox(nullptr, "Can't Initialize OpenGL 4.2.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

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

	int x = 3;
	//////////TEMP FOR TESING/////////////
}

void GLRenderDriver::uninitializeImpl() {}

void GLRenderDriver::updateImpl(float elapsedTime) {}

void GLRenderDriver::makeGLMatrix(GLfloat gl_matrix[16], const core::matrix4& m)
{
	gl_matrix[ 0] = m[ 0];
	gl_matrix[ 1] = m[ 4];
	gl_matrix[ 2] = m[ 8];
	gl_matrix[ 3] = m[12];

	gl_matrix[ 4] = m[ 1];
	gl_matrix[ 5] = m[ 5];
	gl_matrix[ 6] = m[ 9];
	gl_matrix[ 7] = m[13];

	gl_matrix[ 8] = m[ 2];
	gl_matrix[ 9] = m[ 6];
	gl_matrix[10] = m[10];
	gl_matrix[11] = m[14];

	gl_matrix[12] = m[ 3];
	gl_matrix[13] = m[ 7];
	gl_matrix[14] = m[11];
	gl_matrix[15] = m[15];
}


GLint GLRenderDriver::getGLBlendMode(SceneBlendFactor gameBlend)
{
	switch(gameBlend)
	{
	case SBF_ONE:
		return GL_ONE;
	case SBF_ZERO:
		return GL_ZERO;
	case SBF_DEST_COLOR:
		return GL_DST_COLOR;
	case SBF_SOURCE_COLOR:
		return GL_SRC_COLOR;
	case SBF_ONE_MINUS_DEST_COLOR:
		return GL_ONE_MINUS_DST_COLOR;
	case SBF_ONE_MINUS_SOURCE_COLOR:
		return GL_ONE_MINUS_SRC_COLOR;
	case SBF_DEST_ALPHA:
		return GL_DST_ALPHA;
	case SBF_SOURCE_ALPHA:
		return GL_SRC_ALPHA;
	case SBF_ONE_MINUS_DEST_ALPHA:
		return GL_ONE_MINUS_DST_ALPHA;
	case SBF_ONE_MINUS_SOURCE_ALPHA:
		return GL_ONE_MINUS_SRC_ALPHA;
	};

	return GL_ONE;
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
	case VET_FLOAT1:
	case VET_FLOAT2:
	case VET_FLOAT3:
	case VET_FLOAT4:
		return GL_FLOAT;
	case VET_SHORT1:
	case VET_SHORT2:
	case VET_SHORT3:
	case VET_SHORT4:
		return GL_SHORT;
	case VET_COLOR:
		return GL_UNSIGNED_BYTE;
	default:
		return 0;
	};
}

GLRenderDriver* GLRenderDriver::getInstance()
{
	return core::Singleton<GLRenderDriver>::getInstance();
}

} // end namespace render
