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

#include <render/Light.h>
#include <game/ComponentDefines.h>
#include <core/Utils.h>

namespace render
{

Light::Light(): game::Component()
{
	mType = game::COMPONENT_TYPE_LIGHT;

	mLightType = LIGHT_TYPE_POINT;
	mVisible = true;
	mAmbient = render::Color::White;
	mDiffuse = render::Color::Black;
	mSpecular = render::Color::White;

	mPowerScale = 1.0f;

	mSpotOuter = 40.0f;
	mSpotInner = 30.0f;
	mSpotFalloff = 1.0f;
	mRange = 1000.0f;
	mAttenuationConst = 1.0f;
	mAttenuationLinear = 0.0f;
	mAttenuationQuad = 0.0f;
}

Light::~Light() {}

void Light::setLightType(LightType type)
{
	mLightType = type;
}

LightType Light::getLightType()
{
	return mLightType;
}

void Light::setAmbientColor(float red, float green, float blue)
{
	mAmbient.r = red;
	mAmbient.g = green;
	mAmbient.b = blue;
}

void Light::setAmbientColor(const render::Color& color)
{
	mAmbient = color;
}

const render::Color& Light::getAmbientColor() const
{
	return mAmbient;
}

void Light::setDiffuseColor(float red, float green, float blue)
{
	mDiffuse.r = red;
	mDiffuse.g = green;
	mDiffuse.b = blue;
}

void Light::setDiffuseColor(const render::Color& color)
{
	mDiffuse = color;
}

const render::Color& Light::getDiffuseColor() const
{
	return mDiffuse;
}

void Light::setSpecularColor(float red, float green, float blue)
{
	mSpecular.r = red;
	mSpecular.g = green;
	mSpecular.b = blue;
}

void Light::setSpecularColor(const render::Color& color)
{
	mSpecular = color;
}

const render::Color& Light::getSpecularColor() const
{
	return mSpecular;
}

void Light::setPowerScale(float power)
{
	mPowerScale = power;
}

float Light::getPowerScale() const
{
	return mPowerScale;
}

void Light::setAttenuation(float range, float constant, float linear, float quadratic)
{
	mRange = range;
	mAttenuationConst = constant;
	mAttenuationLinear = linear;
	mAttenuationQuad = quadratic;
}

void Light::setAttenuationRange(float range)
{
	mRange = range;
}

void Light::setAttenuationConstant(float constant)
{
	mAttenuationConst = constant;
}

void Light::setAttenuationLinear(float linear)
{
	mAttenuationLinear = linear;
}

void Light::setAttenuationQuadric(float quadratic)
{
	mAttenuationQuad = quadratic;
}

float Light::getAttenuationRange() const
{
	return mRange;
}

float Light::getAttenuationConstant() const
{
	return mAttenuationConst;
}

float Light::getAttenuationLinear() const
{
	return mAttenuationLinear;
}

float Light::getAttenuationQuadric() const
{
	return mAttenuationQuad;
}

void Light::setSpotlightRange(float innerAngle, float outerAngle, float falloff)
{
	if (mLightType == LIGHT_TYPE_SPOTLIGHT)//setSpotlightRange is only valid for spotlights
	{
		mSpotInner = innerAngle;
		mSpotOuter = outerAngle;
		mSpotFalloff = falloff;
	}
	//else do nothing(do not affect)
}

void Light::setSpotlightInnerAngle(float innerAngle)
{
	if (mLightType == LIGHT_TYPE_SPOTLIGHT)//setSpotlightRange is only valid for spotlights
		mSpotInner = innerAngle;
	//else do nothing(do not affect)
}

void Light::setSpotlightOuterAngle(float outerAngle)
{
	if (mLightType == LIGHT_TYPE_SPOTLIGHT)//setSpotlightRange is only valid for spotlights
		mSpotOuter = outerAngle;
	//else do nothing(do not affect)
}

void Light::setSpotlightFalloff(float falloff)
{
	if (mLightType == LIGHT_TYPE_SPOTLIGHT)//setSpotlightRange is only valid for spotlights
		mSpotFalloff = falloff;
	//else do nothing(do not affect)
}

float Light::getSpotlightInnerAngle() const
{
	return mSpotInner;
}

float Light::getSpotlightOuterAngle() const
{
	return mSpotOuter;
}

float Light::getSpotlightFalloff() const
{
	return mSpotFalloff;
}

void Light::setVisible(bool visible)
{
	mVisible = visible;
}

bool Light::isVisible()
{
	return mVisible;
}

} //namespace render
