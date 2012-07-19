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

#ifndef _SOUND_DATA_H_
#define _SOUND_DATA_H_

#include <core/Config.h>
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