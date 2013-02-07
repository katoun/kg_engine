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
#include <render/Material.h>
#include <render/Shader.h>
#include <render/ShaderParameter.h>
#include <render/Texture.h>
#include <render/RenderManager.h>
#include <resource/ResourceManager.h>

namespace render
{

Material::Material(const std::string& name, resource::Serializer* serializer): resource::Resource(name, serializer)
{
	mResourceType = resource::RESOURCE_TYPE_RENDER_MATERIAL;

	mTextureUnits.clear();

	mVertexShader = nullptr;
	mFragmentShader = nullptr;
	mGeometryShader = nullptr;

	mVertexParameters.reserve(VERTEX_BUFFER_TYPE_COUNT);
	mVertexParameters.resize(VERTEX_BUFFER_TYPE_COUNT, nullptr);
}

Material::~Material()
{
	removeAllParameters();

	if (mVertexShader != nullptr)
			mVertexShader->removeResourceEventReceiver(this);
	if (mFragmentShader != nullptr)
			mFragmentShader->removeResourceEventReceiver(this);
	if (mGeometryShader != nullptr)
			mGeometryShader->removeResourceEventReceiver(this);
}

void Material::addTextureUnit(const std::string& filename)
{
	Texture* pTexture = nullptr;
	
	if (resource::ResourceManager::getInstance() != nullptr)
		pTexture = static_cast<Texture*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_TEXTURE, filename));

	addTextureUnit(pTexture);
}

void Material::addTextureUnit(Texture* texture)
{
	if (texture == nullptr)
		return;

	mTextureUnits.push_back(texture);
}

Texture* Material::getTextureUnit(unsigned int index) const
{
	if (index >= mTextureUnits.size())
		return nullptr;

	std::list<Texture*>::const_iterator i = mTextureUnits.begin();
	for (unsigned int j=0; j<index; ++j)
		++i;

	return (*i);
}

unsigned int Material::getNumTextureUnits() const
{
	return mTextureUnits.size();
}

void Material::removeTextureUnit(unsigned int index)
{
	std::list<Texture*>::iterator i = mTextureUnits.begin();
	for (unsigned int j=0; j<index; ++j)
		++i;
	mTextureUnits.erase(i);
}

void Material::removeAllTextureUnits()
{
	mTextureUnits.clear();
}

void Material::setVertexShader(const std::string& filename)
{
	if (filename.empty())
	{
		mVertexShader = nullptr;
	}
	else
	{
		if (RenderManager::getInstance() != nullptr)
			mVertexShader = RenderManager::getInstance()->createShader(filename, SHADER_TYPE_VERTEX);

		if (mVertexShader != nullptr)
			mVertexShader->addResourceEventReceiver(this);
	}
}

void Material::setVertexShader(Shader* shader)
{
	if (shader && shader->getShaderType() != SHADER_TYPE_VERTEX)
		return;

	mVertexShader = shader;	
}

Shader* Material::getVertexShader()
{
	return mVertexShader;
}

void Material::setFragmentShader(const std::string& filename)
{
	if (filename.empty())
	{
		mFragmentShader = nullptr;
	}
	else
	{
		if (RenderManager::getInstance() != nullptr)
			mFragmentShader = RenderManager::getInstance()->createShader(filename, SHADER_TYPE_FRAGMENT);

		if (mFragmentShader != nullptr)
			mFragmentShader->addResourceEventReceiver(this);
	}
}

void Material::setFragmentShader(Shader* shader)
{
	if (shader && shader->getShaderType() != SHADER_TYPE_FRAGMENT)
		return;

	mFragmentShader = shader;	
}

Shader* Material::getFragmentShader()
{
	return mFragmentShader;
}

void Material::setGeometryShader(const std::string& filename)
{
	if (filename.empty())
	{
		mGeometryShader = nullptr;
	}
	else
	{
		if (RenderManager::getInstance() != nullptr)
			mGeometryShader = RenderManager::getInstance()->createShader(filename, SHADER_TYPE_GEOMETRY);

		if (mGeometryShader != nullptr)
			mGeometryShader->addResourceEventReceiver(this);
	}
}

void Material::setGeometryShader(Shader* shader)
{
	if (shader && shader->getShaderType() != SHADER_TYPE_GEOMETRY)
		return;

	mGeometryShader = shader;	
}

Shader* Material::getGeometryShader()
{
	return mGeometryShader;
}

std::vector<ShaderVertexParameter*>& Material::getVertexParameters()
{
	return mVertexParameters;
}

std::vector<ShaderTextureParameter*>& Material::getTextureParameters()
{
	return mTextureParameters;
}

std::list<ShaderAutoParameter*>& Material::getAutoParameters()
{
	return mAutoParameters;
}

void Material::addVertexParameter(const std::string& name, VertexBufferType type)
{
	ShaderVertexParameter* pVertexParam = mVertexParameters[(std::size_t)type];
	if (pVertexParam == nullptr)
	{
		pVertexParam = createVertexParameterImpl();
	}

	if (pVertexParam == nullptr)
		return;

	pVertexParam->mName = name;
	pVertexParam->mVertexBufferType = type;
	mVertexParameters[(std::size_t)type] = pVertexParam;
}

void Material::addTextureParameter(const std::string& name, ShaderParameterType type)
{
	if (type != SHADER_PARAMETER_TYPE_SAMPLER1D && type != SHADER_PARAMETER_TYPE_SAMPLER2D && type != SHADER_PARAMETER_TYPE_SAMPLER3D)
		return;
	
	ShaderParameter* param = findParameter(name);

	if (param != nullptr)
	{
		if (param->mParameterType != type)
		{
			if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("Shader", "Invalid texture parameter type found.", core::LOG_LEVEL_ERROR);

			return;
		}
	}
	else
	{
		param = createParameter(name, type);
	}

	if (param != nullptr)
	{
		ShaderTextureParameter* textureParam = findTextureParameter(param);
		if (textureParam == nullptr)
		{
			textureParam = new ShaderTextureParameter();
			textureParam->mParameter = param;
			mTextureParameters.push_back(textureParam);
		}
	}
}

void Material::addAutoParameter(const std::string& name, ShaderAutoParameterType type)
{
	ShaderParameter* param = findParameter(name);

	if (param != nullptr)
	{
		if (param->mParameterType != getType(type))
		{
			if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("Shader", "Invalid auto parameter type found.", core::LOG_LEVEL_ERROR);

			return;
		}
	}
	else
	{
		param = createParameter(name, getType(type));
	}

	if (param != nullptr)
	{
		ShaderAutoParameter* autoParam = findAutoParameter(param);
		if (autoParam == nullptr)
		{
			autoParam = new ShaderAutoParameter();
			autoParam->mParameter = param;
			mAutoParameters.push_back(autoParam);
		}

		autoParam->mAutoParameterType = type;
	}
}

void Material::addParameter(const std::string& name, ShaderParameterType type)
{
	ShaderParameter* param = findParameter(name);

	if (param == nullptr)
	{
		param = createParameter(name, type);
	}
}

void Material::setParameter(const std::string& name, const Color& col)
{
	ShaderParameter* param = findParameter(name);
	setParameter(param, col);
}

void Material::setParameter(ShaderParameter* parameter, const Color& col)
{
	if (parameter == nullptr)
		return;

	if (parameter->mParameterType != SHADER_PARAMETER_TYPE_FLOAT4)
		return;

	setParameterImpl(parameter, col);
}

void Material::setParameter(const std::string& name, const glm::vec2& vec)
{
	ShaderParameter* param = findParameter(name);
	setParameter(param, vec);
}

void Material::setParameter(ShaderParameter* parameter, const glm::vec2& vec)
{
	if (parameter == nullptr)
		return;

	if (parameter->mParameterType != SHADER_PARAMETER_TYPE_FLOAT2)
		return;

	setParameterImpl(parameter, vec);
}

void Material::setParameter(const std::string& name, const glm::vec3& vec)
{
	ShaderParameter* param = findParameter(name);
	setParameter(param, vec);
}

void Material::setParameter(ShaderParameter* parameter, const glm::vec3& vec)
{
	if (parameter == nullptr)
		return;

	if (parameter->mParameterType != SHADER_PARAMETER_TYPE_FLOAT3)
		return;

	setParameterImpl(parameter, vec);
}

void Material::setParameter(const std::string& name, const glm::vec4& vec)
{
	ShaderParameter* param = findParameter(name);
	setParameter(param, vec);
}

void Material::setParameter(ShaderParameter* parameter, const glm::vec4& vec)
{
	if (parameter == nullptr)
		return;

	if (parameter->mParameterType != SHADER_PARAMETER_TYPE_FLOAT4)
		return;

	setParameterImpl(parameter, vec);
}

void Material::setParameter(const std::string& name, const glm::mat4x4& m)
{
	ShaderParameter* param = findParameter(name);
	setParameter(param, m);
}

void Material::setParameter(ShaderParameter* parameter, const glm::mat4x4& m)
{
	if (parameter == nullptr)
		return;

	if (parameter->mParameterType != SHADER_PARAMETER_TYPE_MATRIX4)
		return;

	setParameterImpl(parameter, m);
}

void Material::unloadImpl()
{
	// Remove all TextureUnits
	removeAllTextureUnits();

	mVertexShader = nullptr;
	mFragmentShader = nullptr;
	mGeometryShader = nullptr;
}

ShaderParameter* Material::createParameter(const std::string& name, ShaderParameterType type)
{
	ShaderParameter* pShaderParameter = createParameterImpl();

	if (pShaderParameter != nullptr)
	{
		pShaderParameter->mName = name;
		pShaderParameter->mParameterType = type;
		mParameters[name] = pShaderParameter;
	}

	return pShaderParameter;
}

ShaderVertexParameter* Material::createVertexParameterImpl()
{
	return nullptr;
}

ShaderParameter* Material::createParameterImpl()
{
	return nullptr;
}

ShaderParameter* Material::findParameter(const std::string& name)
{
	hashmap<std::string, ShaderParameter*>::const_iterator i = mParameters.find(name);
	if (i == mParameters.end()) return nullptr;

	return i->second;
}

ShaderTextureParameter* Material::findTextureParameter(ShaderParameter* parameter)
{
	if (parameter == nullptr)
		return nullptr;

	for (unsigned int i = 0; i < mTextureParameters.size(); ++i)
	{
		if (mTextureParameters[i]->mParameter == parameter)
			return mTextureParameters[i];
	}

	return nullptr;
}

ShaderAutoParameter* Material::findAutoParameter(ShaderParameter* parameter)
{
	if (parameter == nullptr)
		return nullptr;

	std::list<ShaderAutoParameter*>::const_iterator i;
	for (i = mAutoParameters.begin(); i != mAutoParameters.end(); ++i)
	{
		if ((*i)->mParameter == parameter)
			return (*i);
	}

	return nullptr;
}

void Material::removeAllParameters()
{
	hashmap<std::string, ShaderParameter*>::iterator i;
	for (i = mParameters.begin(); i != mParameters.end(); ++i)
	{
		SAFE_DELETE(i->second);
	}

	mParameters.clear();
	mTextureParameters.clear();
	mAutoParameters.clear();
}

void Material::setParameterImpl(ShaderParameter* parameter, const Color& col) {}
void Material::setParameterImpl(ShaderParameter* parameter, const glm::vec2& vec) {}
void Material::setParameterImpl(ShaderParameter* parameter, const glm::vec3& vec) {}
void Material::setParameterImpl(ShaderParameter* parameter, const glm::vec4& vec) {}
void Material::setParameterImpl(ShaderParameter* parameter, const glm::mat4x4& m) {}

ShaderParameterType Material::getType(ShaderAutoParameterType type)
{
	switch(type)
	{
	case SHADER_AUTO_PARAMETER_TYPE_MODEL_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_MODEL_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_VIEW_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEW_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_MODEL_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_MODEL_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_MODEL_VIEW_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_MODEL_VIEW_PROJECTION_MATRIX:
		return SHADER_PARAMETER_TYPE_MATRIX4;
	
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
		return SHADER_PARAMETER_TYPE_UNKNOWN;
	}
}

ShaderParameterType Material::getType(TextureType type)
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

} //namespace render
