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

#include <render/Light.h>
#include <core/Utils.h>

namespace render
{

unsigned int Light::msNextGeneratedLightIndex = 0;

Light::Light(): scene::Node("Light_" + core::intToString(msNextGeneratedLightIndex++))
{
	mNodeType = scene::NT_LIGHT;

	mLightType = LIGHT_TYPE_POINT;
	mVisible = true;
	mAmbient = render::Color::White;
	mDiffuse = render::Color::White;
	mSpecular = render::Color::Black;

	mSpotOuter = 40.0f;
	mSpotInner = 30.0f;
	mSpotFalloff = 1.0f;
	mRange = 1000.0f;
	mAttenuationConst = 1.0f;
	mAttenuationLinear = 0.0f;
	mAttenuationQuad = 0.0f;
	mPowerScale = 1.0f;
}

Light::Light(const std::string& name): scene::Node(name)
{
	mNodeType = scene::NT_LIGHT;

	mLightType = LIGHT_TYPE_POINT;
	mVisible = true;
	mAmbient = render::Color::White;
	mDiffuse = render::Color::White;
	mSpecular = render::Color::Black;

	mSpotOuter = 40.0f;
	mSpotInner = 30.0f;
	mSpotFalloff = 1.0f;
	mRange = 1000.0f;
	mAttenuationConst = 1.0f;
	mAttenuationLinear = 0.0f;
	mAttenuationQuad = 0.0f;
	mPowerScale = 1.0f;
}

Light::~Light() {}

Light& Light::operator=(const Light& other)
{
	mPosition = other.mPosition;
	mOrientation = other.mOrientation;
	mScale = other.mScale;
	mModifiedAbsoluteTransform = true;

	mLightType = other.mLightType;
	mVisible = other.mVisible;
	mAmbient = other.mAmbient;
	mDiffuse = other.mDiffuse;
	mSpecular = other.mSpecular;

	mSpotOuter = other.mSpotOuter;
	mSpotInner = other.mSpotInner;
	mSpotFalloff = other.mSpotFalloff;

	mRange = other.mRange;
	mAttenuationConst = other.mAttenuationConst;
	mAttenuationLinear = other.mAttenuationLinear;
	mAttenuationQuad = other.mAttenuationQuad;
	mPowerScale = other.mPowerScale;

	return *this;
}

void Light::setLightType(LightType type)
{
	mLightType = type;
	mModifiedAbsoluteTransform = true;
}

LightType Light::getLightType()
{
	return mLightType;
}

void Light::setAmbientColor(float red, float green, float blue)
{
	mAmbient.R = red;
	mAmbient.G = green;
	mAmbient.B = blue;
	mModifiedAbsoluteTransform = true;
}

void Light::setAmbientColor(const render::Color& color)
{
	mAmbient = color;
	mModifiedAbsoluteTransform = true;
}

const render::Color& Light::getAmbientColor() const
{
	return mAmbient;
}

void Light::setDiffuseColor(float red, float green, float blue)
{
	mDiffuse.R = red;
	mDiffuse.G = green;
	mDiffuse.B = blue;
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
	mSpecular.R = red;
	mSpecular.G = green;
	mSpecular.B = blue;
}

void Light::setSpecularColor(const render::Color& color)
{
	mSpecular = color;
}

const render::Color& Light::getSpecularColor() const
{
	return mSpecular;
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

void Light::setPowerScale(float power)
{
	mPowerScale = power;
}

float Light::getPowerScale() const
{
	return mPowerScale;
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
