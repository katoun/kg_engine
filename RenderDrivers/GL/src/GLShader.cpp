/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2012 Catalin Alexandru Nastase

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

CGShaderParameter::CGShaderParameter(): ShaderParameter()
{
	cgParameter = NULL;
}

GLShader::GLShader(const std::string& name, resource::Serializer* serializer): Shader(name, serializer)
{
	mCgContext = NULL;
	mCgProgram = NULL;
	mSelectedCgProfile = CG_PROFILE_UNKNOWN;
	mCgArguments = NULL;
}

GLShader::~GLShader() {}

void GLShader::bind()
{
	cgGLBindProgram(mCgProgram);
	cgGLEnableProfile(mSelectedCgProfile);
}

void GLShader::unbind()
{
	cgGLDisableProfile(mSelectedCgProfile);
}

void GLShader::setParameter(const std::string& name, const Color& col)
{
	Shader::setParameter(name, col);

	ShaderParameter* param = findParameter(name);
	CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	if (cgParam && cgParam->cgParameter) cgSetParameter4fv(cgParam->cgParameter, col.get());
}

void GLShader::setParameter(const std::string& name, const core::vector2d& vec)
{
	Shader::setParameter(name, vec);

	ShaderParameter* param = findParameter(name);
	CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	if (cgParam && cgParam->cgParameter) cgSetParameter2fv(cgParam->cgParameter, vec.get());
}

void GLShader::setParameter(const std::string& name, const core::vector3d& vec)
{
	Shader::setParameter(name, vec);

	ShaderParameter* param = findParameter(name);
	CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	if (cgParam && cgParam->cgParameter) cgSetParameter3fv(cgParam->cgParameter, vec.get());
}

void GLShader::setParameter(const std::string& name, const core::vector4d& vec)
{
	Shader::setParameter(name, vec);

	ShaderParameter* param = findParameter(name);
	CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	if (cgParam && cgParam->cgParameter) cgSetParameter4fv(cgParam->cgParameter, vec.get());
}

void GLShader::setParameter(const std::string& name, const core::matrix4& m)
{
	Shader::setParameter(name, m);

	ShaderParameter* param = findParameter(name);
	CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	if (cgParam && cgParam->cgParameter) cgSetMatrixParameterfr(cgParam->cgParameter, m.get());
}

void GLShader::setParameter(const std::string& name, const float* val, unsigned int count)
{
	Shader::setParameter(name, val, count);

	ShaderParameter* param = findParameter(name);
	CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	if (cgParam && cgParam->cgParameter) cgSetParameterValuefr(cgParam->cgParameter, count, val);
}

void GLShader::setParameter(const std::string& name, const signed int* val, unsigned int count)
{
	Shader::setParameter(name, val, count);

	ShaderParameter* param = findParameter(name);
	CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
	if (cgParam && cgParam->cgParameter) cgSetParameterValueir(cgParam->cgParameter, count, val);
}

bool GLShader::loadImpl()
{
	if (!Shader::loadImpl())
		return false;

	//if (GLRenderDriver::getInstance() == NULL)
	//	return false;

	mCgContext = GLRenderDriver::getInstance().getCGContext();
	if (mCgContext == NULL)
		return false;
	
	mSelectedCgProfile = cgGLGetLatestProfile(GLRenderDriver::getCGGLType(mShaderType));
	cgGLSetOptimalOptions(mSelectedCgProfile);
	if (GLRenderDriver::checkForCgError(mCgContext))
		return false;
	
	mCgProgram = cgCreateProgram(mCgContext, CG_SOURCE, mSource.c_str(), mSelectedCgProfile, mEntryPoint.c_str(), const_cast<const char**>(mCgArguments));
	if (GLRenderDriver::checkForCgError(mCgContext))
		return false;

	cgGLLoadProgram(mCgProgram);
	if (GLRenderDriver::checkForCgError(mCgContext))
		return false;

	//populate parameters
	CGparameter parameter = cgGetFirstParameter(mCgProgram, CG_PROGRAM);
	while (parameter != 0)
	{
		CGtype paramType = cgGetParameterType(parameter);
		CGbool paramRef = cgIsParameterReferenced(parameter);

		if (cgGetParameterVariability(parameter) == CG_UNIFORM && cgGetParameterDirection(parameter) != CG_OUT && !isSampler(paramType))
		{
			if (paramType != CG_STRUCT && paramType != CG_ARRAY)// Normal path (leaf)
			{
				std::string paramName = cgGetParameterName(parameter);

				CGresource res = cgGetParameterResource(parameter);
				if (res != CG_COMBINER_STAGE_CONST0 && res != CG_COMBINER_STAGE_CONST1)// normal constant
				{
					ShaderParameterType type = getShaderPrameterType(paramType);

					ShaderParameter* param = findParameter(paramName);
					if (!param)
					{
						addParameter(paramName, getShaderPrameterType(paramType));
						
						ShaderParameter* param = findParameter(paramName);
						CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
						if (cgParam) cgParam->cgParameter = parameter;
					}
					else
					{
						CGShaderParameter* cgParam = static_cast<CGShaderParameter*>(param);
						if (cgParam)
						{
							cgParam->cgParameter = parameter;

							switch(type)
							{
							case SHADER_PARAMETER_TYPE_FLOAT:
								cgSetParameter1fv(cgParam->cgParameter, getFloatPrameterData(cgParam->index));
								break;
							case SHADER_PARAMETER_TYPE_FLOAT2:
								cgSetParameter2fv(cgParam->cgParameter, getFloatPrameterData(cgParam->index));
								break;
							case SHADER_PARAMETER_TYPE_FLOAT3:
								cgSetParameter3fv(cgParam->cgParameter, getFloatPrameterData(cgParam->index));
								break;
							case SHADER_PARAMETER_TYPE_FLOAT4:
								cgSetParameter4fv(cgParam->cgParameter, getFloatPrameterData(cgParam->index));
								break;
							case SHADER_PARAMETER_TYPE_INT:
								cgSetParameter1iv(cgParam->cgParameter, getIntPrameterData(cgParam->index));
								break;
							case SHADER_PARAMETER_TYPE_INT2:
								cgSetParameter2iv(cgParam->cgParameter, getIntPrameterData(cgParam->index));
								break;
							case SHADER_PARAMETER_TYPE_INT3:
								cgSetParameter3iv(cgParam->cgParameter, getIntPrameterData(cgParam->index));
								break;
							case SHADER_PARAMETER_TYPE_INT4:
								cgSetParameter4iv(cgParam->cgParameter, getIntPrameterData(cgParam->index));
								break;
							case SHADER_PARAMETER_TYPE_MATRIX_4X4:
								cgSetMatrixParameterfr(cgParam->cgParameter, getFloatPrameterData(cgParam->index));
								break;
							}

							if (GLRenderDriver::checkForCgError(mCgContext))
								return false;
						}
					}
				}
			}
		}

		parameter = cgGetNextParameter(parameter);// Get next
	}

	return true;
}

void GLShader::unloadImpl()
{
	if (mCgProgram != NULL)
		cgDestroyProgram(mCgProgram);

	Shader::unloadImpl();
}

ShaderParameter* GLShader::createParameterImpl(const std::string& name)
{
	CGShaderParameter* cgParam = new CGShaderParameter();

	if (mState == resource::RESOURCE_STATE_LOADED)
	{
		cgParam->cgParameter = cgGetNamedParameter(mCgProgram, name.c_str());
		if (GLRenderDriver::checkForCgError(mCgContext)) return NULL;
	}

	return cgParam;
}

bool GLShader::isSampler(CGtype type)
{
	switch(type)
	{
	case CG_SAMPLER1D:
	case CG_SAMPLER2D:
	case CG_SAMPLER3D:
	case CG_SAMPLERCUBE:
	case CG_SAMPLERRECT:
		return true;
	default:
		return false;
	};
}

ShaderParameterType GLShader::getShaderPrameterType(CGtype type)
{
	switch(type)
	{
	case CG_FLOAT:
	case CG_FLOAT1:
	case CG_HALF:
	case CG_HALF1:
		return SHADER_PARAMETER_TYPE_FLOAT;
	case CG_FLOAT2:
	case CG_HALF2:
		return SHADER_PARAMETER_TYPE_FLOAT2;
	case CG_FLOAT3:
	case CG_HALF3:
		return SHADER_PARAMETER_TYPE_FLOAT3;
	case CG_FLOAT4:
	case CG_HALF4:
		return SHADER_PARAMETER_TYPE_FLOAT4;
	case CG_INT:
	case CG_INT1:
		return SHADER_PARAMETER_TYPE_INT;
	case CG_INT2:
		return SHADER_PARAMETER_TYPE_INT2;
	case CG_INT3:
		return SHADER_PARAMETER_TYPE_INT3;
	case CG_INT4:
		return SHADER_PARAMETER_TYPE_INT4;
	case CG_FLOAT2x2:
	case CG_HALF2x2:
		return SHADER_PARAMETER_TYPE_MATRIX_2X2;
	case CG_FLOAT2x3:
	case CG_HALF2x3:
		return SHADER_PARAMETER_TYPE_MATRIX_2X3;
	case CG_FLOAT2x4:
	case CG_HALF2x4:
		return SHADER_PARAMETER_TYPE_MATRIX_2X4;
	case CG_FLOAT3x2:
	case CG_HALF3x2:
		return SHADER_PARAMETER_TYPE_MATRIX_3X2;
	case CG_FLOAT3x3:
	case CG_HALF3x3:
		return SHADER_PARAMETER_TYPE_MATRIX_3X3;
	case CG_FLOAT3x4:
	case CG_HALF3x4:
		return SHADER_PARAMETER_TYPE_MATRIX_3X4;
	case CG_FLOAT4x2:
	case CG_HALF4x2:
		return SHADER_PARAMETER_TYPE_MATRIX_4X2;
	case CG_FLOAT4x3:
	case CG_HALF4x3:
		return SHADER_PARAMETER_TYPE_MATRIX_4X3;
	case CG_FLOAT4x4:
	case CG_HALF4x4:
		return SHADER_PARAMETER_TYPE_MATRIX_4X4;
	case CG_SAMPLER:
	case CG_SAMPLER1D:
		return SHADER_PARAMETER_TYPE_SAMPLER1D;
	case CG_SAMPLER2D:
		return SHADER_PARAMETER_TYPE_SAMPLER2D;
	case CG_SAMPLER3D:
		return SHADER_PARAMETER_TYPE_SAMPLER3D;
	case CG_SAMPLERCUBE:
		return SHADER_PARAMETER_TYPE_SAMPLERCUBE;
	case CG_SAMPLER1DSHADOW:
		return SHADER_PARAMETER_TYPE_SAMPLER1DSHADOW;
	case CG_SAMPLER2DSHADOW:
		return SHADER_PARAMETER_TYPE_SAMPLER2DSHADOW;
	}

	return SHADER_PARAMETER_TYPE_UNKNOWN;
}

} // end namespace render
