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

#include <core/Log.h>
#include <core/LogDefines.h>
#include <sound/SoundUtils.h>
#include <sound/SoundData.h>
#include <resource/ResourceManager.h>

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

	mData = nullptr;
	mDataSize = 0;
	mFrequency = 0;
	mChannels = 0;
	mBPS = 0;
	mBufferFormat = 0;

	// Generate an AL Buffer
	alGenBuffers(1, &mBufferId);

	if (!mBufferId)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("OpenALSoundData", "Cannot create OpenAL sound buffer", core::LOG_LEVEL_ERROR);
		return;
	}
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

ALuint SoundData::getOpenALBufferID() const
{
	return mBufferId;
}

bool SoundData::loadImpl()
{
	std::string extention;

	// Get extension.
	size_t pos = mFilename.find_last_of('.');
	if (pos == std::string::npos)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("OpenALSoundData", "Unable to load sound - invalid extension.", core::LOG_LEVEL_ERROR);
		return false;
	}

	if (resource::ResourceManager::getInstance() == nullptr)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("OpenALSoundData", "Unable to load sound - resources data path not set.", core::LOG_LEVEL_ERROR);
		return false;
	}

	extention = mFilename.substr(pos + 1, mFilename.size() - pos);

	std::string filePath = resource::ResourceManager::getInstance()->getDataPath() + "/" + mFilename;

	if (extention == "wav" || extention == "ogg")
	{
		alGetError(); // Clear Error Code

		ALboolean ret = alureBufferDataFromFile(filePath.c_str(), mBufferId);

		if (checkALError("OpenALSoundData::loadImpl()::alBufferData:"))
			return false;
	}
	else
	{
		std::string message = "Unable to load sound - ";
		message += extention;
		message += " unsupported extension.";

		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("OpenALSoundData", message, core::LOG_LEVEL_ERROR);
		return false;
	}

	return true;
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

	alDeleteBuffers(1, &mBufferId);
}

} // end namespace sound
