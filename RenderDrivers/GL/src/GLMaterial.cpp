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

#include <GLMaterial.h>
#include <GLShader.h>
#include <render/Shader.h>
#include <resource/ResourceEvent.h>

namespace render
{

GLShaderParameter::GLShaderParameter(): ShaderParameter()
{
	//TODO!!!
}

GLMaterial::GLMaterial(const std::string& name, resource::Serializer* serializer): Material(name, serializer)
{
	mGLHandle = 0;
}

GLMaterial::~GLMaterial() {}

void GLMaterial::resourceLoaded(const resource::ResourceEvent& evt)
{
	if (evt.source != nullptr)
	{
		if (evt.source == mVertexShader)
		{
			GLShader* pGLShader = static_cast<GLShader*>(mVertexShader);
			if (pGLShader != nullptr)
			{
				glAttachShader(mGLHandle, pGLShader->getGLHandle());
			}
		}

		if (evt.source == mFragmentShader)
		{
			GLShader* pGLShader = static_cast<GLShader*>(mFragmentShader);
			if (pGLShader != nullptr)
			{
				glAttachShader(mGLHandle, pGLShader->getGLHandle());
			}
		}

		if (evt.source == mGeometryShader)
		{
			GLShader* pGLShader = static_cast<GLShader*>(mGeometryShader);
			if (pGLShader != nullptr)
			{
				glAttachShader(mGLHandle, pGLShader->getGLHandle());
			}
		}

		if ((mVertexShader != nullptr && mVertexShader->getState() == resource::RESOURCE_STATE_LOADED) &&
			(mFragmentShader != nullptr && mFragmentShader->getState() == resource::RESOURCE_STATE_LOADED) &&
			(mGeometryShader != nullptr && mGeometryShader->getState() == resource::RESOURCE_STATE_LOADED))
		{

			GLint linked;
			glLinkProgram(mGLHandle);

			glGetProgramiv(mGLHandle, GL_LINK_STATUS, &linked);
		}
	}
}

void GLMaterial::resourceUnloaded(const resource::ResourceEvent& evt) {}

void GLMaterial::setParameter(const std::string& name, const Color& col)
{
	Material::setParameter(name, col);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetParameter4fv(cgParam->cgParameter, col.get());
}

void GLMaterial::setParameter(const std::string& name, const core::vector2d& vec)
{
	Material::setParameter(name, vec);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetParameter2fv(cgParam->cgParameter, vec.get());
}

void GLMaterial::setParameter(const std::string& name, const core::vector3d& vec)
{
	Material::setParameter(name, vec);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetParameter3fv(cgParam->cgParameter, vec.get());
}

void GLMaterial::setParameter(const std::string& name, const core::vector4d& vec)
{
	Material::setParameter(name, vec);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetParameter4fv(cgParam->cgParameter, vec.get());
}

void GLMaterial::setParameter(const std::string& name, const core::matrix4& m)
{
	Material::setParameter(name, m);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetMatrixParameterfr(cgParam->cgParameter, m.get());
}

void GLMaterial::setParameter(const std::string& name, const float* val, unsigned int count)
{
	Material::setParameter(name, val, count);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetParameterValuefr(cgParam->cgParameter, count, val);
}

void GLMaterial::setParameter(const std::string& name, const int* val, unsigned int count)
{
	Material::setParameter(name, val, count);

	ShaderParameter* param = findParameter(name);
	//TODO!!!
	//CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	//if (cgParam && cgParam->cgParameter) cgSetParameterValueir(cgParam->cgParameter, count, val);
}

bool GLMaterial::loadImpl()
{
	if (!Material::loadImpl()) return false;

	mGLHandle = glCreateProgram();

	return true;
}

void GLMaterial::unloadImpl()
{
	glDeleteProgram(mGLHandle);
}

ShaderParameter* GLMaterial::createParameterImpl(const std::string& name)
{
	GLShaderParameter* glParam = new GLShaderParameter();

	return glParam;
}

} //namespace render
