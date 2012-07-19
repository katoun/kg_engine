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

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <core/Config.h>
#include <scene/Node.h>
#include <render/LightDefines.h>
#include <render/Color.h>

namespace render
{

//! Representation of a light in the render world.
class ENGINE_PUBLIC_EXPORT Light: public scene::Node
{
public:

	Light();
	Light(const std::string& name);

	~Light();

	inline Light& operator=(const Light& other);

	void setLightType(LightType type);
	LightType getLightType();

	void setAmbientColor(float red, float green, float blue);
	void setAmbientColor(const render::Color& color);
	const render::Color& getAmbientColor() const;

	void setDiffuseColor(float red, float green, float blue);
	void setDiffuseColor(const render::Color& color);
	const render::Color& getDiffuseColor() const;

	void setSpecularColor(float red, float green, float blue);
	void setSpecularColor(const render::Color& color);
	const render::Color& getSpecularColor() const;

	void setAttenuation(float range, float constant, float linear, float quadratic);
	void setAttenuationRange(float range);
	void setAttenuationConstant(float constant);
	void setAttenuationLinear(float linear);
	void setAttenuationQuadric(float quadratic);
	float getAttenuationRange() const;
	float getAttenuationConstant() const;
	float getAttenuationLinear() const;
	float getAttenuationQuadric() const;

	void setSpotlightRange(float innerAngle, float outerAngle, float falloff);
	void setSpotlightInnerAngle(float innerAngle);
	void setSpotlightOuterAngle(float outerAngle);
	void setSpotlightFalloff(float falloff);
	float getSpotlightInnerAngle() const;
	float getSpotlightOuterAngle() const;
	float getSpotlightFalloff() const;

	void setPowerScale(float power);
	float getPowerScale() const;

	void setVisible(bool visible);
	bool isVisible();

private:

	// Incremented count for next index
	static unsigned int msNextGeneratedLightIndex;

	LightType mLightType;

	render::Color mAmbient;
	render::Color mDiffuse;
	render::Color mSpecular;

	float mSpotOuter;
	float mSpotInner;
	float mSpotFalloff;
	float mRange;
	float mAttenuationConst;
	float mAttenuationLinear;
	float mAttenuationQuad;
	float mPowerScale;

	bool mVisible;
};

} //namespace render

#endif