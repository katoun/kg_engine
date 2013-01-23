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

GLShaderParameter::GLShaderParameter(): ShaderParameter()
{
	//TODO!!!
}

GLShader::GLShader(const std::string& name, resource::Serializer* serializer): Shader(name, serializer)
{
	mGLHandle = 0;
}

GLShader::~GLShader()
{
}

void GLShader::bind()
{
	//TODOO!!!
}

void GLShader::unbind()
{
	//TODO!!!
}

void GLShader::setParameter(const std::string& name, const Color& col)
{
	Shader::setParameter(name, col);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetParameter4fv(cgParam->cgParameter, col.get());
}

void GLShader::setParameter(const std::string& name, const core::vector2d& vec)
{
	Shader::setParameter(name, vec);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetParameter2fv(cgParam->cgParameter, vec.get());
}

void GLShader::setParameter(const std::string& name, const core::vector3d& vec)
{
	Shader::setParameter(name, vec);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetParameter3fv(cgParam->cgParameter, vec.get());
}

void GLShader::setParameter(const std::string& name, const core::vector4d& vec)
{
	Shader::setParameter(name, vec);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetParameter4fv(cgParam->cgParameter, vec.get());
}

void GLShader::setParameter(const std::string& name, const core::matrix4& m)
{
	Shader::setParameter(name, m);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetMatrixParameterfr(cgParam->cgParameter, m.get());
}

void GLShader::setParameter(const std::string& name, const float* val, unsigned int count)
{
	Shader::setParameter(name, val, count);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetParameterValuefr(cgParam->cgParameter, count, val);
}

void GLShader::setParameter(const std::string& name, const int* val, unsigned int count)
{
	Shader::setParameter(name, val, count);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetParameterValueir(cgParam->cgParameter, count, val);
}

GLhandleARB GLShader::getGLHandle() const
{
	return mGLHandle;
}

bool GLShader::loadImpl()
{
	if (!Shader::loadImpl())
		return false;

	mGLHandle = glCreateShaderObjectARB(getShaderType(mShaderType));

	const char* source = mSource.c_str();
	glShaderSourceARB(mGLHandle, 1, &source, NULL);

	GLint compiled;
	glCompileShaderARB(mGLHandle);
	// check for compile errors
	glGetObjectParameterivARB(mGLHandle, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);

	return (compiled == 1);
}

void GLShader::unloadImpl()
{
	glDeleteObjectARB(mGLHandle);

	Shader::unloadImpl();
}

ShaderParameter* GLShader::createParameterImpl(const std::string& name)
{
	GLShaderParameter* glParam = new GLShaderParameter();

	return glParam;
}

GLenum GLShader::getShaderType(ShaderType type)
{
	switch(type)
	{
	case SHADER_TYPE_VERTEX:
		return GL_VERTEX_SHADER_ARB;
	case SHADER_TYPE_FRAGMENT:
		return GL_FRAGMENT_SHADER_ARB;
	case SHADER_TYPE_GEOMETRY:
		return GL_GEOMETRY_SHADER_EXT;
	default:
		return 0x0000;
	}

	return 0x0000;
}

} // end namespace render
