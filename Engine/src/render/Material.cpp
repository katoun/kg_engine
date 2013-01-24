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

#include <render/Material.h>
#include <render/Shader.h>
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
}

Material::~Material() {}

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

bool Material::hasVertexShader()
{
	return mVertexShader != nullptr;
}

bool Material::hasFragmentShader()
{
	return mFragmentShader != nullptr;
}

bool Material::hasGeometryShader()
{
	return mGeometryShader != nullptr;
}

void Material::unloadImpl()
{
	// Remove all TextureUnits
	removeAllTextureUnits();

	mVertexShader = nullptr;
	mFragmentShader = nullptr;
	mGeometryShader = nullptr;
}

} //namespace render
