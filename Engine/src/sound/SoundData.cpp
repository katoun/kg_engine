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

#include <sound/SoundData.h>

namespace sound
{

SoundData::SoundData(const std::string& name, resource::Serializer* serializer): resource::Resource(name, serializer)
{
	mResourceType = resource::RESOURCE_TYPE_SOUND_DATA;

	mPitch = 1;
	mGain = 1;
	mMinDistance = 1;
	mMaxDistance = 10000;
	mInnerConeAngle = 360;
	mOuterConeAngle = 360;
	mOuterConeGain = 1;
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
	mPitch = 1;
	mGain = 1;
	mMinDistance = 1;
	mMaxDistance = 10000;
	mInnerConeAngle = 360;
	mOuterConeAngle = 360;
	mOuterConeGain = 1;
}

} // end namespace sound
