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

#ifndef _SOUND_DATA_H_
#define _SOUND_DATA_H_

#include <EngineConfig.h>
#include <resource/Resource.h>

#include <string>

namespace resource
{
class Serializer;
}

namespace sound
{

//!  Defines a sound data resource.
//!  Author: Kat'Oun
//!  version: 1.0
class ENGINE_PUBLIC_EXPORT SoundData: public resource::Resource
{
public:

	SoundData(const std::string& name, resource::Serializer* serializer);
	virtual ~SoundData();

	//! Sets the pitch multiplier.
	void setPitch(float pitch);
	//! Returns the pitch multiplier.
	float getPitch() const;

	//! Sets the gain.
	void setGain(float gain);
	//! Returns the gain.
	float getGain() const;

	//! Sets the variables used in the distance attenuation calculation.
	//! \param minDistance: The min distance that the sound emitter will cease to continue growing louder at (as it approaches the listener).
	//! \param maxDistance: The max distance a sound stops attenuating at.
	void setDistanceValues(float minDistance, float maxDistance);
	//! Sets the Min Distance.
	void setMinDistance(float minDistance);
	//! Sets the Max Distance.
	void setMaxDistance(float maxDistance);

	//! Returns the min distance.
	float getMinDistance() const;
	//! Returns the max distance.
	float getMaxDistance() const;

	//! Sets the variables used in the sound projection cone.
	//! \param innerConeAngle: The inside cone angle.
	//! \param outerConeAngle: The outside cone angle.
	//! \param outerConeGain: The outer cone gain.
	void setConeSettings(float innerConeAngle, float outerConeAngle, float outerConeGain);

	//! Sets the inner angle of the sound cone for a directional sound.
	void setInnerConeAngle(float innerConeAngle);
	//! Return the innerConeAngle.
	float getInnerConeAngle() const;

	//! Sets the outer angle of the sound cone for a directional sound.
	void setOuterConeAngle(float outerConeAngle);
	//! Returns the outerConeAngle.
	float getOuterConeAngle() const;

	//! Sets the gain outside the sound cone of a directional sound.
	void setOuterConeGain(float outerConeGain);
	//! Returns the outerConeGain.
	float getOuterConeGain() const;
	
protected:

	virtual void unloadImpl();

	float mPitch;
	float mGain;
	float mMinDistance;
	float mMaxDistance;
	float mInnerConeAngle;
	float mOuterConeAngle;
	float mOuterConeGain;
};

} // end namespace sound

#endif