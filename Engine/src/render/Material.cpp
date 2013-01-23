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
#include <render/TextureUnit.h>
#include <render/RenderManager.h>

namespace render
{

Material::Material(const std::string& name, resource::Serializer* serializer): resource::Resource(name, serializer)
{
	mResourceType = resource::RESOURCE_TYPE_RENDER_MATERIAL;

	mAmbient = mDiffuse = Color::White;
	mSpecular = mEmissive = Color::Black;
	mShininess = 0.0f;
	mLightingEnabled = false;
	mShadeOptions = SO_GOURAUD;

	mFogOverride = false;
	mFogMode = FM_NONE;
	mFogColor = Color::White;
	mFogDensity = 0.001f;
	mFogStart = 0.0f;
	mFogEnd = 1.0f;

	// Default blending
	mSourceBlendFactor = SBF_ONE;
	mDestBlendFactor = SBF_ZERO;

	mDepthCheck = true;
	mDepthWrite = true;

	mTextureUnits.clear();

	mVertexShader = nullptr;
	mFragmentShader = nullptr;
	mGeometryShader = nullptr;
}

Material::~Material() {}

void Material::setAmbient(float red, float green, float blue)
{
	mAmbient = Color(red, green, blue);
}

void Material::setAmbient(const Color& ambient)
{
	mAmbient = ambient;
}

void Material::setDiffuse(float red, float green, float blue)
{
	mDiffuse = Color(red, green, blue);
}

void Material::setDiffuse(const Color& diffuse)
{
	mDiffuse = diffuse;
}

void Material::setSpecular(float red, float green, float blue)
{
	mSpecular = Color(red, green, blue);
}

void Material::setSpecular(const Color& specular)
{
	mSpecular = specular;
}

void Material::setEmissive(float red, float green, float blue)
{
	mEmissive = Color(red, green, blue);
}

void Material::setEmissive(const Color& emissive)
{
	mEmissive = emissive;
}

void Material::setShininess(float shininess)
{
	mShininess = shininess;
}

const Color& Material::getAmbient() const
{
	return mAmbient;
}

const Color& Material::getDiffuse() const
{
	return mDiffuse;
}

const Color& Material::getSpecular() const
{
	return mSpecular;
}

const Color& Material::getEmissive() const
{
	return mEmissive;
}

float Material::getShininess() const
{
	return mShininess;
}

void Material::setLightingEnabled(bool enabled)
{
	mLightingEnabled = enabled;
}

bool Material::getLightingEnabled() const
{
	return mLightingEnabled;
}

void Material::setShadingMode(ShadeOptions mode)
{
	mShadeOptions = mode;
}

ShadeOptions Material::getShadingMode() const
{
	return mShadeOptions;
}

void Material::setFog(bool overrideScene, FogMode mode, const Color& color, float density, float start, float end)
{
	mFogOverride = overrideScene;
	if (overrideScene)
	{
		mFogMode = mode;
		mFogColor = color;
		mFogDensity = density;
		mFogStart = start;
		mFogEnd = end;
	}
}

bool Material::getFogOverride() const
{
	 return mFogOverride;
}

FogMode Material::getFogMode() const
{
	return mFogMode;
}

const Color& Material::getFogColor() const
{
	return mFogColor;
}

float Material::getFogDensity() const
{
	return mFogDensity;
}

float Material::getFogStart() const
{
	return mFogStart;
}

float Material::getFogEnd() const
{
	return mFogEnd;
}

void Material::setSceneBlending(const SceneBlendType sbt)
{
	// Turn predefined type into blending factors
	switch (sbt)
	{
	case SBT_TRANSPARENT_ALPHA:
		setSceneBlending(SBF_SOURCE_ALPHA, SBF_ONE_MINUS_SOURCE_ALPHA);
		break;
	case SBT_TRANSPARENT_COLOR:
		setSceneBlending(SBF_SOURCE_COLOR, SBF_ONE_MINUS_SOURCE_COLOR);
		break;
	case SBT_ADD:
		setSceneBlending(SBF_ONE, SBF_ONE);
		break;
	case SBT_MODULATE:
		setSceneBlending(SBF_DEST_COLOR, SBF_ZERO);
		break;
	case SBT_REPLACE:
		setSceneBlending(SBF_ONE, SBF_ZERO);
		break;
	// TODO: more
	}
}

void Material::setSceneBlending(const SceneBlendFactor sourceFactor, const SceneBlendFactor destFactor)
{
	mSourceBlendFactor = sourceFactor;
	mDestBlendFactor = destFactor;
}

SceneBlendFactor Material::getSourceBlendFactor() const
{
	return mSourceBlendFactor;
}

SceneBlendFactor Material::getDestBlendFactor() const
{
	return mDestBlendFactor;
}

bool Material::isTransparent() const
{
	// Transparent if any of the destination color is taken into account
	if (mDestBlendFactor == SBF_ZERO &&
		mSourceBlendFactor != SBF_DEST_COLOR &&
		mSourceBlendFactor != SBF_ONE_MINUS_DEST_COLOR &&
		mSourceBlendFactor != SBF_DEST_ALPHA &&
		mSourceBlendFactor != SBF_ONE_MINUS_DEST_ALPHA)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Material::setDepthCheckEnabled(bool enabled)
{
	mDepthCheck = enabled;
}

bool Material::getDepthCheckEnabled() const
{
	return mDepthCheck;
}

void Material::setDepthWriteEnabled(bool enabled)
{
	mDepthWrite = enabled;
}

bool Material::getDepthWriteEnabled() const
{
	return mDepthWrite;
}

void Material::addTextureUnit(TextureUnit* tu)
{
	if (tu == nullptr)
		return;

	mTextureUnits.push_back(tu);
}

TextureUnit* Material::getTextureUnit(unsigned int index) const
{
	if (index >= mTextureUnits.size())
		return nullptr;

	std::list<TextureUnit*>::const_iterator i = mTextureUnits.begin();
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
	std::list<TextureUnit*>::iterator i = mTextureUnits.begin();
	for (unsigned int j=0; j<index; ++j)
		++i;

	delete (*i);

	mTextureUnits.erase(i);
}

void Material::removeAllTextureUnits()
{
	std::list<TextureUnit*>::iterator i;
	for (i = mTextureUnits.begin(); i != mTextureUnits.end(); ++i)
		delete (*i);

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

bool Material::isProgrammable()
{
	return (mVertexShader != nullptr || mFragmentShader != nullptr || mGeometryShader != nullptr);
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

	// Default to white ambient & diffuse, no specular / emissive
	mAmbient = mDiffuse = Color::White;
	mSpecular = mEmissive = Color::Black;
	mShininess = 0.0f;
	mLightingEnabled = false;
	mShadeOptions = SO_GOURAUD;

	mFogOverride = false;
	mFogMode = FM_NONE;
	mFogColor = Color::White;
	mFogDensity = 0.001f;
	mFogStart = 0.0f;
	mFogEnd = 1.0f;

	// Default blending
	mSourceBlendFactor = SBF_ONE;
	mDestBlendFactor = SBF_ZERO;

	mDepthCheck = true;
	mDepthWrite = true;

	mVertexShader = nullptr;
	mFragmentShader = nullptr;
	mGeometryShader = nullptr;
}

} //namespace render
