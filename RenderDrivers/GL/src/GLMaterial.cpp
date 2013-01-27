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

#define PARAMETER_NAME_BUFFERSIZE 255

namespace render
{

GLShaderParameter::GLShaderParameter(): ShaderParameter()
{
	ParameterID = 0;
}

GLShaderVertexParameter::GLShaderVertexParameter(): ShaderVertexParameter()
{
	ParameterID = 0;
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

		if (((mVertexShader == nullptr) || (mVertexShader != nullptr && mVertexShader->getState() == resource::RESOURCE_STATE_LOADED)) &&
			((mFragmentShader == nullptr) || (mFragmentShader != nullptr && mFragmentShader->getState() == resource::RESOURCE_STATE_LOADED)) &&
			((mGeometryShader == nullptr) || (mGeometryShader != nullptr && mGeometryShader->getState() == resource::RESOURCE_STATE_LOADED)))
		{

			GLint linked;
			glLinkProgram(mGLHandle);

			glGetProgramiv(mGLHandle, GL_LINK_STATUS, &linked);

			if (linked == 1)
			{
				char   uniformName[PARAMETER_NAME_BUFFERSIZE];
				GLenum		type;
				GLint		size;
				GLint		count;
				glGetProgramiv(mGLHandle, GL_ACTIVE_UNIFORMS, &count);
				for (GLint idx=0; idx<count; idx++)
				{
					glGetActiveUniform(mGLHandle, idx, PARAMETER_NAME_BUFFERSIZE, NULL, &size, &type, uniformName);

					if (type != GL_SAMPLER_1D && type != GL_SAMPLER_2D && type != GL_SAMPLER_3D)
						continue;

					ShaderParameterType paramType = SHADER_PARAMETER_TYPE_UNKNOWN;
					switch (type)
					{
					case GL_SAMPLER_1D:
						paramType = SHADER_PARAMETER_TYPE_SAMPLER1D;
						break;
					case GL_SAMPLER_2D:
						paramType = SHADER_PARAMETER_TYPE_SAMPLER2D;
						break;
					case GL_SAMPLER_3D:
						paramType = SHADER_PARAMETER_TYPE_SAMPLER3D;
						break;
					}

					addTextureParameter(std::string(uniformName), paramType);
				}
				//////////////////////////////////

				for (unsigned int i = 0; i < mVertexParameters.size(); i++)
				{
					GLShaderVertexParameter* pGLShaderVertexParameter = static_cast<GLShaderVertexParameter*>(mVertexParameters[i]);
					if (pGLShaderVertexParameter == nullptr)
						continue;

					pGLShaderVertexParameter->ParameterID = glGetAttribLocation(mGLHandle, pGLShaderVertexParameter->mName.c_str());
				}

				hashmap<std::string, ShaderParameter*>::iterator pi;
				for (pi = mParameters.begin(); pi != mParameters.end(); ++pi)
				{
					GLShaderParameter* pGLShaderParameter = static_cast<GLShaderParameter*>(pi->second);
					if (pGLShaderParameter == nullptr)
						continue;
					
					pGLShaderParameter->ParameterID = glGetUniformLocation(mGLHandle, pi->first.c_str());
				}
				//////////////////////////////////
			}
		}
	}
}

void GLMaterial::resourceUnloaded(const resource::ResourceEvent& evt) {}

GLhandleARB GLMaterial::getGLHandle() const
{
	return mGLHandle;
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

ShaderVertexParameter* GLMaterial::createVertexParameterImpl()
{
	return new GLShaderVertexParameter();
}

ShaderParameter* GLMaterial::createParameterImpl()
{
	return new GLShaderParameter();
}

void GLMaterial::setParameterImpl(ShaderParameter* parameter, const Color& col)
{
	GLShaderParameter* glParam = static_cast<GLShaderParameter*>(parameter);
	if (glParam != nullptr)
	{
		glUniform4f(glParam->ParameterID, col.R, col.G, col.B, col.A);
	}
}

void GLMaterial::setParameterImpl(ShaderParameter* parameter, const core::vector2d& vec)
{
	GLShaderParameter* glParam = static_cast<GLShaderParameter*>(parameter);
	if (glParam != nullptr)
	{
		glUniform2f(glParam->ParameterID, vec.X, vec.Y);
	}
}

void GLMaterial::setParameterImpl(ShaderParameter* parameter, const core::vector3d& vec)
{
	GLShaderParameter* glParam = static_cast<GLShaderParameter*>(parameter);
	if (glParam != nullptr)
	{
		glUniform3f(glParam->ParameterID, vec.X, vec.Y, vec.Z);
	}
}

void GLMaterial::setParameterImpl(ShaderParameter* parameter, const core::vector4d& vec)
{
	GLShaderParameter* glParam = static_cast<GLShaderParameter*>(parameter);
	if (glParam != nullptr)
	{
		glUniform4f(glParam->ParameterID, vec.X, vec.Y, vec.Z, vec.W);
	}
}

void GLMaterial::setParameterImpl(ShaderParameter* parameter, const core::matrix4& m)
{
	GLShaderParameter* glParam = static_cast<GLShaderParameter*>(parameter);
	if (glParam != nullptr)
	{
		glUniformMatrix4fv(glParam->ParameterID, 1, GL_TRUE/*GL_FALSE*/, m.get());
	}
}

GLenum GLMaterial::getGLType(ShaderParameterType type)
{
	switch(type)
	{
	case SHADER_PARAMETER_TYPE_FLOAT:
		return GL_FLOAT;
	case SHADER_PARAMETER_TYPE_FLOAT2:
		return GL_FLOAT_VEC2;
	case SHADER_PARAMETER_TYPE_FLOAT3:
		return GL_FLOAT_VEC3;
	case SHADER_PARAMETER_TYPE_FLOAT4:
		return GL_FLOAT_VEC4;
	case SHADER_PARAMETER_TYPE_INT:
		return GL_INT;
	case SHADER_PARAMETER_TYPE_INT2:
		return GL_INT_VEC2;
	case SHADER_PARAMETER_TYPE_INT3:
		return GL_INT_VEC3;
	case SHADER_PARAMETER_TYPE_INT4:
		return GL_INT_VEC4;
	case SHADER_PARAMETER_TYPE_MATRIX2:
		return GL_FLOAT_MAT2;
	case SHADER_PARAMETER_TYPE_MATRIX3:
		return GL_FLOAT_MAT3;
	case SHADER_PARAMETER_TYPE_MATRIX4:
		return GL_FLOAT_MAT4;
	case SHADER_PARAMETER_TYPE_SAMPLER1D:
		return GL_SAMPLER_1D;
	case SHADER_PARAMETER_TYPE_SAMPLER2D:
		return GL_SAMPLER_2D;
	case SHADER_PARAMETER_TYPE_SAMPLER3D:
		return GL_SAMPLER_3D;
	default:
		return 0;
	}
}

} //namespace render
