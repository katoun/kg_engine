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

#include <core/Utils.h>
#include <GLShader.h>
#include <GLTexture.h>
#include <GLRenderDriver.h>

namespace render
{

GLShader::GLShader(const std::string& name, resource::Serializer* serializer): Shader(name, serializer)
{
	mGLHandle = 0;
}

GLShader::~GLShader() {}

GLhandleARB GLShader::getGLHandle() const
{
	return mGLHandle;
}

bool GLShader::loadImpl()
{
	if (!Shader::loadImpl())
		return false;

	mGLHandle = glCreateShader(getShaderType(mShaderType));
	
	const char* source = mSource.c_str();
	glShaderSource(mGLHandle, 1, &source, NULL);

	int InfoLogLength;
	GLint compiled;
	glCompileShader(mGLHandle);
	// check for compile errors
	glGetShaderiv(mGLHandle, GL_COMPILE_STATUS, &compiled);
#ifdef _DEBUG	
	glGetShaderiv(mGLHandle, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(mGLHandle, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}
#endif

	return (compiled == 1);
}

void GLShader::unloadImpl()
{
	glDeleteShader(mGLHandle);

	Shader::unloadImpl();
}

GLenum GLShader::getShaderType(ShaderType type)
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
		return 0x0000;
	}

	return 0x0000;
}

} // end namespace render
