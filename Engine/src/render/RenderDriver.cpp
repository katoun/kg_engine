/*
-----------------------------------------------------------------------------
Copyright (c) 2006-2012 Catalin Alexandru Nastase

KG game engine (http://k-game.sourceforge.net) is made available under the LGPL License (http://www.gnu.org/copyleft/lgpl.html)

Under the LGPL you may use KG game engine for any purpose you wish, as long as you:
1. Release any modifications to the KG game engine source back to the community
2. Pass on the source to KG game engine with all the copyrights intact, or link back to a place where the source code can be obtained (http://k-game.sourceforge.net)
3. Make it clear where you have customised it.
The above is a precis, please do read the full license agreement.
-----------------------------------------------------------------------------
*/

#include <render/RenderDriver.h>
#include <render/TextureUnit.h>
#include <render/Renderable.h>
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
		mLights[i] = NULL;

	mDisabledTexUnitsFrom = 0;

	mCurrentVertexShader = NULL;
	mCurrentFragmentShader = NULL;
	mCurrentGeometryShader = NULL;
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
	if (tu == NULL)
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
	if (shader == NULL)
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
			mCurrentVertexShader = NULL;
		}
		break;
	case SHADER_TYPE_GEOMETRY:
		if (mCurrentGeometryShader)
		{
			mCurrentGeometryShader->unbind();
			mCurrentGeometryShader = NULL;
		}
		break;
	case SHADER_TYPE_FRAGMENT:
		if (mCurrentFragmentShader)
		{
			mCurrentFragmentShader->unbind();
			mCurrentFragmentShader = NULL;
		}
		break;
	}
}

bool RenderDriver::isShaderBound(ShaderType type)
{
	switch(type)
	{
	case SHADER_TYPE_VERTEX:
		return (mCurrentVertexShader != NULL);
	case SHADER_TYPE_GEOMETRY:
		return (mCurrentGeometryShader != NULL);
	case SHADER_TYPE_FRAGMENT:
		return (mCurrentFragmentShader != NULL);
	}

	return false;
}

void RenderDriver::updateShaderAutoParameters(Shader* shader, ShaderParamData& data)
{
	if (shader == NULL)
		return;

	shader->updateAutoParameters(data);
}

} // end namespace render
