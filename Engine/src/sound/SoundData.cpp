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

#include <sound/SoundData.h>

namespace sound
{

SoundData::SoundData(const std::string& name, resource::Serializer* serializer): resource::Resource(name, serializer)
{
	mResourceType = resource::RESOURCE_TYPE_SOUND_DATA;

	mPitch = 1.0f;
	mGain = 1.0f;
	mMinDistance = 1.0f * ENGINE_UNIT_M;
	mMaxDistance = 10000.0f * ENGINE_UNIT_M;
	mInnerConeAngle = 360.0f;
	mOuterConeAngle = 360.0f;
	mOuterConeGain = 1.0f;
}

SoundData::~SoundData() {}

void SoundData::setPitch(float pitch)
{
	if(pitch <= 0)
		return;

	mPitch = pitch;
}

float SoundData::getPitch() const
{
	return mPitch;
}

void SoundData::setGain(float gain)
{
	if(gain < 0)
		return;

	mGain = gain;
}

float SoundData::getGain() const
{
	return mGain;
}

void SoundData::setDistanceValues(float minDistance, float maxDistance)
{
	setMinDistance(minDistance);
	setMaxDistance(maxDistance);
}

void SoundData::setMinDistance(float minDistance)
{
	if(minDistance < 0)
		return;

	mMinDistance = minDistance;
}

void SoundData::setMaxDistance(float maxDistance)
{
	if(maxDistance < 0)
		return;

	mMaxDistance = maxDistance;
}

float SoundData::getMinDistance() const
{
	return mMinDistance;
}

float SoundData::getMaxDistance() const
{
	return mMaxDistance;
}

void SoundData::setConeSettings(float innerConeAngle, float outerConeAngle, float outerConeGain)
{
	setInnerConeAngle(innerConeAngle);
	setOuterConeAngle(outerConeAngle);
	setOuterConeGain(outerConeGain);
}

void SoundData::setInnerConeAngle(float innerConeAngle)
{
	mInnerConeAngle = innerConeAngle;
}

float SoundData::getInnerConeAngle() const
{
	return mInnerConeAngle;
}

void SoundData::setOuterConeAngle(float outerConeAngle)
{
	mOuterConeAngle = outerConeAngle;
}

float SoundData::getOuterConeAngle() const
{
	return mOuterConeAngle;
}

void SoundData::setOuterConeGain(float outerConeGain)
{
	if(outerConeGain < 0 || outerConeGain > 1)
		return;

	mOuterConeGain = outerConeGain;
}

float SoundData::getOuterConeGain() const
{
	return mOuterConeGain;
}

void SoundData::unloadImpl()
{
	mPitch = 1.0f;
	mGain = 1.0f;
	mMinDistance = 1.0f * ENGINE_UNIT_M;
	mMaxDistance = 10000.0f * ENGINE_UNIT_M;
	mInnerConeAngle = 360.0f;
	mOuterConeAngle = 360.0f;
	mOuterConeGain = 1.0f;
}

} // end namespace sound
