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

#include <render/RenderDriver.h>
#include <render/TextureUnit.h>
#include <render/Shader.h>

namespace render
{

RenderDriver::RenderDriver(const std::string& name): core::SystemDriver(name)
{
	mWorldMatrix = core::matrix4::IDENTITY;
	mViewMatrix = core::matrix4::IDENTITY;
	mProjMatrix = core::matrix4::IDENTITY;

	for(unsigned int i=0; i<ENGINE_MAX_TEXTURE_COORD_SETS; ++i)
		mTextureCoordIndex[i] = 0;


	for(unsigned int i=0; i<ENGINE_MAX_SIMULTANEOUS_LIGHTS; ++i)
		mLights[i] = nullptr;

	mDisabledTexUnitsFrom = 0;

	mCurrentVertexShader = nullptr;
	mCurrentFragmentShader = nullptr;
	mCurrentGeometryShader = nullptr;
}

RenderDriver::~RenderDriver() {}

core::matrix4& RenderDriver::getWorldMatrix()
{
	return mWorldMatrix;
}

core::matrix4& RenderDriver::getViewMatrix()
{
	return mViewMatrix;
}

core::matrix4& RenderDriver::getProjectionMatrix()
{
	return mProjMatrix;
}

void RenderDriver::convertProjectionMatrix(const core::matrix4& matrix, core::matrix4& dest)
{
	// no any convertion
	dest = matrix;
}

void RenderDriver::setTextureUnitSettings(unsigned int texUnit, TextureUnit* tu)
{
	if (tu == nullptr)
		return;
	
	setTexture(true, texUnit, tu->getTexture());
	
	// Set texture coordinate set
	setTextureCoordSet(texUnit, tu->getTextureCoordSet());

	// Set blend modes
	setTextureBlendMode(texUnit, tu->getColorBlendMode());
	setTextureBlendMode(texUnit, tu->getAlphaBlendMode());
}

void RenderDriver::disableTextureUnit(unsigned int texUnit)
{
	setTexture(false, texUnit, 0);
}

void RenderDriver::disableTextureUnitsFrom(unsigned int texUnit)
{
	unsigned int disableTo = getNumTextureUnits();
	if (disableTo > mDisabledTexUnitsFrom)
		disableTo = mDisabledTexUnitsFrom;
	mDisabledTexUnitsFrom = texUnit;
	for (unsigned int i = texUnit; i < disableTo; ++i)
	{
		disableTextureUnit(i);
	}
}

void RenderDriver::setTextureCoordSet(unsigned int unit, unsigned int index)
{
	mTextureCoordIndex[unit] = index;
}

void RenderDriver::bindShader(Shader* shader)
{
	if (shader == nullptr)
		return;

	switch(shader->getShaderType())
	{
	case SHADER_TYPE_VERTEX:
		if (mCurrentVertexShader != shader)
		{
			if (mCurrentVertexShader) mCurrentVertexShader->unbind();
			mCurrentVertexShader = shader;
		}
		break;
	case SHADER_TYPE_GEOMETRY:
		if (mCurrentGeometryShader != shader)
		{
			if (mCurrentGeometryShader) mCurrentGeometryShader->unbind();
			mCurrentGeometryShader = shader;
		}
		break;
	case SHADER_TYPE_FRAGMENT:
		if (mCurrentFragmentShader != shader)
		{
			if (mCurrentFragmentShader) mCurrentFragmentShader->unbind();
			mCurrentFragmentShader = shader;
		}
		break;
	}

	shader->bind();
}

void RenderDriver::unbindShader(ShaderType type)
{
	switch(type)
	{
	case SHADER_TYPE_VERTEX:
		if (mCurrentVertexShader)
		{
			mCurrentVertexShader->unbind();
			mCurrentVertexShader = nullptr;
		}
		break;
	case SHADER_TYPE_GEOMETRY:
		if (mCurrentGeometryShader)
		{
			mCurrentGeometryShader->unbind();
			mCurrentGeometryShader = nullptr;
		}
		break;
	case SHADER_TYPE_FRAGMENT:
		if (mCurrentFragmentShader)
		{
			mCurrentFragmentShader->unbind();
			mCurrentFragmentShader = nullptr;
		}
		break;
	}
}

bool RenderDriver::isShaderBound(ShaderType type)
{
	switch(type)
	{
	case SHADER_TYPE_VERTEX:
		return (mCurrentVertexShader != nullptr);
	case SHADER_TYPE_GEOMETRY:
		return (mCurrentGeometryShader != nullptr);
	case SHADER_TYPE_FRAGMENT:
		return (mCurrentFragmentShader != nullptr);
	}

	return false;
}

void RenderDriver::updateShaderAutoParameters(Shader* shader, ShaderParamData& data)
{
	if (shader == nullptr)
		return;

	shader->updateAutoParameters(data);
}

} // end namespace render
