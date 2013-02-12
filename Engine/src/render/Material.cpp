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
#include <resource/ResourceEvent.h>
#include <resource/ResourceManager.h>

namespace render
{

Material::Material(const std::string& name, resource::Serializer* serializer): resource::Resource(name, serializer)
{
	mResourceType	= resource::RESOURCE_TYPE_RENDER_MATERIAL;
	mGLHandle		= 0;

	mTextureUnits.clear();

	mVertexShader	= nullptr;
	mFragmentShader	= nullptr;
	mGeometryShader	= nullptr;

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
		return;

	Shader* pShader = nullptr;
	if (RenderManager::getInstance() != nullptr)
		pShader = RenderManager::getInstance()->createShader(filename, SHADER_TYPE_VERTEX);

	setVertexShader(pShader);
}

void Material::setVertexShader(Shader* shader)
{
	if (shader && shader->getShaderType() != SHADER_TYPE_VERTEX)
		return;

	if (mVertexShader != nullptr)
		mVertexShader->removeResourceEventReceiver(this);

	mVertexShader = shader;

	if (mVertexShader != nullptr)
		mVertexShader->addResourceEventReceiver(this);
}

Shader* Material::getVertexShader()
{
	return mVertexShader;
}

void Material::setFragmentShader(const std::string& filename)
{
	if (filename.empty())
		return;

	Shader* pShader = nullptr;
	if (RenderManager::getInstance() != nullptr)
		pShader = RenderManager::getInstance()->createShader(filename, SHADER_TYPE_FRAGMENT);

	setFragmentShader(pShader);
}

void Material::setFragmentShader(Shader* shader)
{
	if (shader && shader->getShaderType() != SHADER_TYPE_FRAGMENT)
		return;

	mFragmentShader = shader;

	if (mFragmentShader != nullptr)
		mFragmentShader->removeResourceEventReceiver(this);

	mFragmentShader = shader;

	if (mFragmentShader != nullptr)
		mFragmentShader->addResourceEventReceiver(this);
}

Shader* Material::getFragmentShader()
{
	return mFragmentShader;
}

void Material::setGeometryShader(const std::string& filename)
{
	if (filename.empty())
		return;

	Shader* pShader = nullptr;
	if (RenderManager::getInstance() != nullptr)
		pShader = RenderManager::getInstance()->createShader(filename, SHADER_TYPE_GEOMETRY);

	setGeometryShader(pShader);
}

void Material::setGeometryShader(Shader* shader)
{
	if (shader && shader->getShaderType() != SHADER_TYPE_GEOMETRY)
		return;

	mGeometryShader = shader;

	if (mGeometryShader != nullptr)
		mGeometryShader->removeResourceEventReceiver(this);

	mGeometryShader = shader;

	if (mGeometryShader != nullptr)
		mGeometryShader->addResourceEventReceiver(this);	
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
		pVertexParam = new ShaderVertexParameter();
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

	glUniform4f(parameter->mParameterID, col.r, col.g, col.b, col.a);
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

	glUniform2f(parameter->mParameterID, vec.x, vec.y);
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

	glUniform3f(parameter->mParameterID, vec.x, vec.y, vec.z);
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

	glUniform4f(parameter->mParameterID, vec.x, vec.y, vec.z, vec.w);
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

	glUniformMatrix4fv(parameter->mParameterID, 1, GL_FALSE, &m[0][0]);
}

void Material::resourceLoaded(const resource::ResourceEvent& evt)
{
	if (evt.source != nullptr)
	{
		if (evt.source == mVertexShader)
		{
			Shader* pShader = static_cast<Shader*>(mVertexShader);
			if (pShader != nullptr)
			{
				glAttachShader(mGLHandle, pShader->getGLHandle());
			}
		}

		if (evt.source == mFragmentShader)
		{
			Shader* pShader = static_cast<Shader*>(mFragmentShader);
			if (pShader != nullptr)
			{
				glAttachShader(mGLHandle, pShader->getGLHandle());
			}
		}

		if (evt.source == mGeometryShader)
		{
			Shader* pShader = static_cast<Shader*>(mGeometryShader);
			if (pShader != nullptr)
			{
				glAttachShader(mGLHandle, pShader->getGLHandle());
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
				char   uniformName[GL_PARAMETER_NAME_BUFFERSIZE];
				GLenum		type;
				GLint		size;
				GLint		count;
				glGetProgramiv(mGLHandle, GL_ACTIVE_UNIFORMS, &count);
				for (GLint idx=0; idx<count; idx++)
				{
					glGetActiveUniform(mGLHandle, idx, GL_PARAMETER_NAME_BUFFERSIZE, NULL, &size, &type, uniformName);

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
					ShaderVertexParameter* pShaderVertexParameter = mVertexParameters[i];
					if (pShaderVertexParameter == nullptr)
						continue;

					pShaderVertexParameter->mParameterID = glGetAttribLocation(mGLHandle, pShaderVertexParameter->mName.c_str());
				}

				hashmap<std::string, ShaderParameter*>::iterator pi;
				for (pi = mParameters.begin(); pi != mParameters.end(); ++pi)
				{
					ShaderParameter* pShaderParameter = pi->second;
					if (pShaderParameter == nullptr)
						continue;

					pShaderParameter->mParameterID = glGetUniformLocation(mGLHandle, pi->first.c_str());
				}
				//////////////////////////////////
			}
		}
	}
}

void Material::resourceUnloaded(const resource::ResourceEvent& evt) {}

GLhandleARB Material::getGLHandle() const
{
	return mGLHandle;
}

bool Material::loadImpl()
{
	if (!Resource::loadImpl())
		return false;

	mGLHandle = glCreateProgram();

	return true;
}

void Material::unloadImpl()
{
	glDeleteProgram(mGLHandle);

	// Remove all TextureUnits
	removeAllTextureUnits();

	mVertexShader = nullptr;
	mFragmentShader = nullptr;
	mGeometryShader = nullptr;
}

ShaderParameter* Material::createParameter(const std::string& name, ShaderParameterType type)
{
	ShaderParameter* pShaderParameter = new ShaderParameter();

	if (pShaderParameter != nullptr)
	{
		pShaderParameter->mName = name;
		pShaderParameter->mParameterType = type;
		mParameters[name] = pShaderParameter;
	}

	return pShaderParameter;
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


GLenum Material::getGLType(ShaderParameterType type)
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
