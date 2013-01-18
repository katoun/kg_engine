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
#include <core/LogDefines.h>
#include <core/System.h>
#include <render/Shader.h>
#include <render/Light.h>
#include <render/Texture.h>
#include <resource/ResourceManager.h>

#include <stdio.h>
#include <limits>
#include <iostream>
#include <fstream>

namespace render
{

ShaderParameter::ShaderParameter()
{
	paramType = SHADER_PARAMETER_TYPE_UNKNOWN;
	autoParamType = SHADER_AUTO_PARAMETER_TYPE_NONE;
	index = 0;
	elemCount = 0;
}

Shader::Shader(const std::string& name, resource::Serializer* serializer): resource::Resource(name, serializer)
{
	mResourceType = resource::RESOURCE_TYPE_SHADER;

	mShaderType = SHADER_TYPE_VERTEX;

	mSource = "";
}

Shader::~Shader()
{
	removeAllParameters();
}

void Shader::setShaderType(const ShaderType& type)
{
	mShaderType = type;
}

ShaderType Shader::getShaderType()
{
	return mShaderType;
}

void Shader::setSource(const std::string& source)
{
	mSource = source;
}

void Shader::setEntryPoint(const std::string& entry)
{
	mEntryPoint = entry;
}

void Shader::bind() {}

void Shader::unbind() {}

void Shader::updateAutoParameters(ShaderParamData& data)
{
	hashmap<std::string, ShaderParameter*>::iterator i;
	for (i = mParameters.begin(); i != mParameters.end(); ++i)
	{
		ShaderParameter* param = i->second;
		std::string name = i->first;

		if (!param) continue;

		switch (param->autoParamType)
		{
		case SHADER_AUTO_PARAMETER_TYPE_WORLD_MATRIX:
			setParameter(name, data.getWorldMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLD_MATRIX:
			setParameter(name, data.getInverseWorldMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLD_MATRIX:
			setParameter(name, data.getTransposedWorldMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLD_MATRIX:
			setParameter(name, data.getInverseTransposedWorldMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_VIEW_MATRIX:
			setParameter(name, data.getViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEW_MATRIX:
			setParameter(name, data.getInverseViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_VIEW_MATRIX:
			setParameter(name, data.getTransposedViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_VIEW_MATRIX:
			setParameter(name, data.getInverseTransposedViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_PROJECTION_MATRIX:
			setParameter(name, data.getProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_PROJECTION_MATRIX:
			setParameter(name, data.getInverseProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_PROJECTION_MATRIX:
			setParameter(name, data.getTransposedProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_PROJECTION_MATRIX:
			setParameter(name, data.getInverseTransposedProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_VIEWPROJ_MATRIX:
			setParameter(name, data.getViewProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEWPROJ_MATRIX:
			setParameter(name, data.getInverseViewProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_VIEWPROJ_MATRIX:
			setParameter(name, data.getTransposedViewProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_VIEWPROJ_MATRIX:
			setParameter(name, data.getInverseTransposedViewProjectionMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_WORLDVIEW_MATRIX:
			setParameter(name, data.getWorldViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLDVIEW_MATRIX:
			setParameter(name, data.getInverseWorldViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLDVIEW_MATRIX:
			setParameter(name, data.getTransposedWorldViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLDVIEW_MATRIX:
			setParameter(name, data.getInverseTransposedWorldViewMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_WORLDVIEWPROJ_MATRIX:
			setParameter(name, data.getWorldViewProjMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLDVIEWPROJ_MATRIX:
			setParameter(name, data.getInverseWorldViewProjMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLDVIEWPROJ_MATRIX:
			setParameter(name, data.getTransposedWorldViewProjMatrix());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLDVIEWPROJ_MATRIX:
			setParameter(name, data.getInverseTransposedWorldViewProjMatrix());
			break;

		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION:
			setParameter(name, data.getCurrentLightPosition());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_OBJECT_SPACE:
			setParameter(name, data.getCurrentLightPositionObjectSpace());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_VIEW_SPACE:
			setParameter(name, data.getCurrentLightPositionViewSpace());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION:
			setParameter(name, data.getCurrentLightDirection());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_OBJECT_SPACE:
			setParameter(name, data.getCurrentLightDirectionObjectSpace());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_VIEW_SPACE:
			setParameter(name, data.getCurrentLightDirectionViewSpace());
			break;
		
		case SHADER_AUTO_PARAMETER_TYPE_AMBIENT_LIGHT_COLOUR:
			setParameter(name, data.getAmbientLightColour());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIFFUSE_COLOUR:
			setParameter(name, data.getCurrentLightDiffuseColour());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_SPECULAR_COLOUR:
			setParameter(name, data.getCurrentLightSpecularColour());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_LIGHT_ATTENUATION:
			setParameter(name, data.getCurrentLightAttenuation());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION:
			setParameter(name, data.getCameraPosition());
			break;
		case SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION_OBJECT_SPACE:
			setParameter(name, data.getCameraPositionObjectSpace());
			break;
		}
	}
}

void Shader::setParameter(const std::string& name, const Color& col)
{
	ShaderParameter* param = findParameter(name);
	if (param == nullptr)
	{
		param = createParameter(name, SHADER_PARAMETER_TYPE_FLOAT4, mFloatParameterData.size(), getElementCount(SHADER_PARAMETER_TYPE_FLOAT4));

		// Expand at buffer end
		mFloatParameterData.insert(mFloatParameterData.end(), getElementCount(SHADER_PARAMETER_TYPE_FLOAT4), 0.0f);
	}

	writedParameterData(param->index, col.get(), getElementCount(SHADER_PARAMETER_TYPE_FLOAT4));
}

void Shader::setParameter(const std::string& name, const core::vector2d& vec)
{
	ShaderParameter* param = findParameter(name);
	if (param == nullptr)
	{
		param = createParameter(name, SHADER_PARAMETER_TYPE_FLOAT2, mFloatParameterData.size(), getElementCount(SHADER_PARAMETER_TYPE_FLOAT2));

		// Expand at buffer end
		mFloatParameterData.insert(mFloatParameterData.end(), getElementCount(SHADER_PARAMETER_TYPE_FLOAT2), 0.0f);
	}

	writedParameterData(param->index, vec.get(), getElementCount(SHADER_PARAMETER_TYPE_FLOAT2));
}

void Shader::setParameter(const std::string& name, const core::vector3d& vec)
{
	ShaderParameter* param = findParameter(name);
	if (param == nullptr)
	{
		param = createParameter(name, SHADER_PARAMETER_TYPE_FLOAT3, mFloatParameterData.size(), getElementCount(SHADER_PARAMETER_TYPE_FLOAT3));

		// Expand at buffer end
		mFloatParameterData.insert(mFloatParameterData.end(), getElementCount(SHADER_PARAMETER_TYPE_FLOAT3), 0.0f);
	}

	writedParameterData(param->index, vec.get(), getElementCount(SHADER_PARAMETER_TYPE_FLOAT3));
}

void Shader::setParameter(const std::string& name, const core::vector4d& vec)
{
	ShaderParameter* param = findParameter(name);
	if (param == nullptr)
	{
		param = createParameter(name, SHADER_PARAMETER_TYPE_FLOAT4, mFloatParameterData.size(), getElementCount(SHADER_PARAMETER_TYPE_FLOAT4));

		// Expand at buffer end
		mFloatParameterData.insert(mFloatParameterData.end(), getElementCount(SHADER_PARAMETER_TYPE_FLOAT4), 0.0f);
	}

	writedParameterData(param->index, vec.get(), getElementCount(SHADER_PARAMETER_TYPE_FLOAT4));
}

void Shader::setParameter(const std::string& name, const core::matrix4& m)
{
	ShaderParameter* param = findParameter(name);
	if (param == nullptr)
	{
		param = createParameter(name, SHADER_PARAMETER_TYPE_MATRIX_4X4, mFloatParameterData.size(), getElementCount(SHADER_PARAMETER_TYPE_MATRIX_4X4));

		// Expand at buffer end
		mFloatParameterData.insert(mFloatParameterData.end(), getElementCount(SHADER_PARAMETER_TYPE_MATRIX_4X4), 0.0f);
	}

	writedParameterData(param->index, m.get(), getElementCount(SHADER_PARAMETER_TYPE_MATRIX_4X4));
}

void Shader::setParameter(const std::string& name, const float* val, unsigned int count)
{
	if (count == 0)
		return;

	ShaderParameter* param = findParameter(name);
	if (param == nullptr)
	{
		param = createParameter(name, SHADER_PARAMETER_TYPE_FLOAT, mFloatParameterData.size(), count);
		
		// Expand at buffer end
		mFloatParameterData.insert(mFloatParameterData.end(), count, 0.0f);
	}

	writedParameterData(param->index, val, count);
}

void Shader::setParameter(const std::string& name, const int* val, unsigned int count)
{
	if (count == 0)
		return;

	ShaderParameter* param = findParameter(name);
	if (param == nullptr)
	{
		param = createParameter(name, SHADER_PARAMETER_TYPE_INT, mIntParameterData.size(), count);

		// Expand at buffer end
		mIntParameterData.insert(mIntParameterData.end(), count, 0);
	}

	writedParameterData(param->index, val, count);
}

void Shader::addParameter(const std::string& name, ShaderParameterType type)
{
	ShaderParameter* param = findParameter(name);

	if (param == nullptr)
	{
		if (isFloat(type))
		{
			param = createParameter(name, type, mFloatParameterData.size(), getElementCount(type));

			// Expand at buffer end
			mFloatParameterData.insert(mFloatParameterData.end(), getElementCount(type), 0.0f);
		}
		else
		{
			param = createParameter(name, type, mIntParameterData.size(), getElementCount(type));

			// Expand at buffer end
			mIntParameterData.insert(mIntParameterData.end(), getElementCount(type), 0);
		}
	}
}

void Shader::setAutoParameter(const std::string& name, ShaderAutoParameterType type)
{
	ShaderParameter* param = findParameter(name);

	if (param != nullptr)
	{
		if (isFloat(param->paramType) != isFloat(type) || getElementCount(param->paramType) != getElementCount(type))
		{
			if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("Shader", "Invalid auto parameter type or element count found.", core::LOG_LEVEL_ERROR);

			return;
		}
	}
	else
	{
		if (isFloat(type))
		{
			param = createParameter(name, getType(type), mFloatParameterData.size(), getElementCount(type));

			// Expand at buffer end
			mFloatParameterData.insert(mFloatParameterData.end(), getElementCount(type), 0.0f);
		}
		else
		{
			param = createParameter(name, getType(type), mIntParameterData.size(), getElementCount(type));

			// Expand at buffer end
			mIntParameterData.insert(mIntParameterData.end(), getElementCount(type), 0);
		}
	}

	param->autoParamType = type;
}

bool Shader::loadImpl()
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		std::string filePath = resource::ResourceManager::getInstance()->getDataPath() + "/" + mFilename;
	
		FILE* pFile = fopen(filePath.c_str(), "rb");
	
		if (pFile != nullptr)
		{
			fseek(pFile, 0, SEEK_END);
			mSize = (unsigned int)ftell(pFile);

			fseek(pFile, 0, SEEK_SET);

			char* pBuffer = new char[mSize];
			fread(pBuffer, mSize, 1, pFile);

			mSource.insert(0, (const char*)pBuffer, mSize);
			delete []pBuffer;

			fclose(pFile);

			return true;
		}
	}

	return false;
}

void Shader::unloadImpl() {}

ShaderParameter* Shader::createParameter(const std::string& name, ShaderParameterType type, unsigned int index, unsigned int elemCount)
{
	ShaderParameter* paramDef = createParameterImpl(name);
	paramDef->paramType = type;
	paramDef->index = index;
	paramDef->elemCount = elemCount;
	mParameters[name] = paramDef;

	return paramDef;
}

ShaderParameter* Shader::createParameterImpl(const std::string& name)
{
	return new ShaderParameter();
}

ShaderParameter* Shader::findParameter(const std::string& name)
{
	hashmap<std::string, ShaderParameter*>::const_iterator i = mParameters.find(name);
	if (i == mParameters.end()) return nullptr;

	return i->second;
}

void Shader::writedParameterData(unsigned int index, const float* val, unsigned int count)
{
	assert(index + count <= mFloatParameterData.size());
	memcpy(&mFloatParameterData[index], val, sizeof(int) * count);
}

void Shader::writedParameterData(unsigned int index, const int* val, unsigned int count)
{
	assert(index + count <= mIntParameterData.size());
	memcpy(&mIntParameterData[index], val, sizeof(int) * count);
}

float* Shader::getFloatPrameterData(unsigned int index)
{
	assert(index < mFloatParameterData.size());
	return &mFloatParameterData[index];
}
const float* Shader::getFloatPrameterData(unsigned int index) const
{
	assert(index < mFloatParameterData.size());
	return &mFloatParameterData[index];
}

int* Shader::getIntPrameterData(unsigned int index)
{
	assert(index < mIntParameterData.size());
	return &mIntParameterData[index];
}

const int* Shader::getIntPrameterData(unsigned int index) const
{
	assert(index < mIntParameterData.size());
	return &mIntParameterData[index];
}

void Shader::removeAllParameters()
{
	hashmap<std::string, ShaderParameter*>::iterator i;
	for (i = mParameters.begin(); i != mParameters.end(); ++i)
	{
		delete i->second;
		i->second = nullptr;
	}

	mParameters.clear();
	mFloatParameterData.clear();
	mIntParameterData.clear();
}

bool Shader::isFloat(ShaderParameterType type)
{
	switch(type)
	{
	case SHADER_PARAMETER_TYPE_INT:
	case SHADER_PARAMETER_TYPE_INT2:
	case SHADER_PARAMETER_TYPE_INT3:
	case SHADER_PARAMETER_TYPE_INT4:
	case SHADER_PARAMETER_TYPE_SAMPLER1D:
	case SHADER_PARAMETER_TYPE_SAMPLER2D:
	case SHADER_PARAMETER_TYPE_SAMPLER3D:
	case SHADER_PARAMETER_TYPE_SAMPLERCUBE:
	case SHADER_PARAMETER_TYPE_SAMPLER1DSHADOW:
	case SHADER_PARAMETER_TYPE_SAMPLER2DSHADOW:
		return false;
	default:
		return true;
	};
}

bool Shader::isFloat(ShaderAutoParameterType type)
{
	return true;
}

bool Shader::isSampler(ShaderParameterType type)
{
	switch(type)
	{
	case SHADER_PARAMETER_TYPE_SAMPLER1D:
	case SHADER_PARAMETER_TYPE_SAMPLER2D:
	case SHADER_PARAMETER_TYPE_SAMPLER3D:
	case SHADER_PARAMETER_TYPE_SAMPLERCUBE:
	case SHADER_PARAMETER_TYPE_SAMPLER1DSHADOW:
	case SHADER_PARAMETER_TYPE_SAMPLER2DSHADOW:
		return true;
	default:
		return false;
	};
}

unsigned int Shader::getElementCount(ShaderParameterType type)
{
	switch(type)
	{
	case SHADER_PARAMETER_TYPE_FLOAT:
	case SHADER_PARAMETER_TYPE_INT:
	case SHADER_PARAMETER_TYPE_SAMPLER1D:
	case SHADER_PARAMETER_TYPE_SAMPLER2D:
	case SHADER_PARAMETER_TYPE_SAMPLER3D:
	case SHADER_PARAMETER_TYPE_SAMPLERCUBE:
	case SHADER_PARAMETER_TYPE_SAMPLER1DSHADOW:
	case SHADER_PARAMETER_TYPE_SAMPLER2DSHADOW:
		return 1;
	case SHADER_PARAMETER_TYPE_FLOAT2:
	case SHADER_PARAMETER_TYPE_INT2:
		return 2;
	case SHADER_PARAMETER_TYPE_FLOAT3:
	case SHADER_PARAMETER_TYPE_INT3:
		return 3;
	case SHADER_PARAMETER_TYPE_FLOAT4:
	case SHADER_PARAMETER_TYPE_INT4:
		return 4;
	case SHADER_PARAMETER_TYPE_MATRIX_2X2:
		return 4;
	case SHADER_PARAMETER_TYPE_MATRIX_2X3:
	case SHADER_PARAMETER_TYPE_MATRIX_3X2:
		return 6;
	case SHADER_PARAMETER_TYPE_MATRIX_2X4:
	case SHADER_PARAMETER_TYPE_MATRIX_4X2:
		return 8; 
	case SHADER_PARAMETER_TYPE_MATRIX_3X3:
		return 9;
	case SHADER_PARAMETER_TYPE_MATRIX_3X4:
	case SHADER_PARAMETER_TYPE_MATRIX_4X3:
		return 12; 
	case SHADER_PARAMETER_TYPE_MATRIX_4X4:
		return 16; 
	default:
		return 4;
	};
}

unsigned int Shader::getElementCount(ShaderAutoParameterType type)
{
	switch(type)
	{
	case SHADER_AUTO_PARAMETER_TYPE_WORLD_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLD_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLD_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLD_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_VIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_VIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_VIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_WORLDVIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLDVIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLDVIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLDVIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_WORLDVIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLDVIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLDVIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLDVIEWPROJ_MATRIX:
		return 16;

	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_COUNT:
		return 1;
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_OBJECT_SPACE:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_VIEW_SPACE:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_OBJECT_SPACE:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_VIEW_SPACE:
		return 3;

	case SHADER_AUTO_PARAMETER_TYPE_AMBIENT_LIGHT_COLOUR:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIFFUSE_COLOUR:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_SPECULAR_COLOUR:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_ATTENUATION:
		return 4;
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POWER_SCALE:
		return 4;

	case SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION:
	case SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION_OBJECT_SPACE:
		return 3;

	default:
		return 4;
	}
}

ShaderParameterType Shader::getType(ShaderAutoParameterType type)
{
	switch(type)
	{
	case SHADER_AUTO_PARAMETER_TYPE_WORLD_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLD_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLD_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLD_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_VIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_VIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_VIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_WORLDVIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLDVIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLDVIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLDVIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_WORLDVIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLDVIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLDVIEWPROJ_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLDVIEWPROJ_MATRIX:
		return SHADER_PARAMETER_TYPE_MATRIX_4X4;
	
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_COUNT:
		return SHADER_PARAMETER_TYPE_INT;
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_OBJECT_SPACE:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_VIEW_SPACE:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_OBJECT_SPACE:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_VIEW_SPACE:
		return SHADER_PARAMETER_TYPE_FLOAT3;
	
	case SHADER_AUTO_PARAMETER_TYPE_AMBIENT_LIGHT_COLOUR:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIFFUSE_COLOUR:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_SPECULAR_COLOUR:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_ATTENUATION:
		return SHADER_PARAMETER_TYPE_FLOAT4;
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POWER_SCALE:
		return SHADER_PARAMETER_TYPE_FLOAT;

	case SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION:
	case SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION_OBJECT_SPACE:
		return SHADER_PARAMETER_TYPE_FLOAT3;
	
	default:
		return SHADER_PARAMETER_TYPE_FLOAT;
	}
}

ShaderParameterType Shader::getType(TextureType type)
{
	switch(type)
	{
	case TEX_TYPE_1D:
		return SHADER_PARAMETER_TYPE_SAMPLER1D;
	case TEX_TYPE_2D:
		return SHADER_PARAMETER_TYPE_SAMPLER2D;
	case TEX_TYPE_3D:
		return SHADER_PARAMETER_TYPE_SAMPLER3D;
	default:
		return SHADER_PARAMETER_TYPE_SAMPLER2D;
	}
}

} // end namespace render
