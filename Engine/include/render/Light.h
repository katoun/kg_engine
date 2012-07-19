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