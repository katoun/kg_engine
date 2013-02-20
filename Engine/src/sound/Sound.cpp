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
#include <sound/Sound.h>
#include <sound/SoundData.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <game/MessageDefines.h>
#include <resource/ResourceEvent.h>
#include <resource/ResourceManager.h>
#include <core/Utils.h>

namespace sound
{

Sound::Sound(): game::Component()
{
	mType = game::COMPONENT_TYPE_SOUND;

	mSoundData = nullptr;

	mLastPosition = glm::vec3(0, 0, 0);
	mVelocity = glm::vec3(0, 0, 0);

	mPitch = 1;
	mGain = 1;
	mMinDistance = 1;
	mMaxDistance = 10000;
	mInnerConeAngle = 360;
	mOuterConeAngle = 360;
	mOuterConeGain = 1;

	mLoop = false;

	alGetError(); // Clear Error Code

	mSourceNeedsUpdate = true;

	// Generate a Source to playback the Buffer
	alGenSources(1, &mSourceId);
	//if (checkALError("initialize::alGenSources :"))
	//	return;
	if (!mSourceId)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("Sound", "Cannot create OpenAL sound source", core::LOG_LEVEL_ERROR);
		return;
	}

	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 direction = glm::vec3(0, 0, 1);

	alSource3f(mSourceId, AL_POSITION, (ALfloat)position.x, (ALfloat)position.y, (ALfloat)position.z);
	//if (checkALError(core::string("OpenALSound::alSourcefv:AL_POSITION")))
	//	return;
	alSource3f(mSourceId, AL_DIRECTION, (ALfloat)direction.x, (ALfloat)direction.y, (ALfloat)direction.z);
	//if (checkALError("OpenALSound::alSourcefv:AL_DIRECTION"))
	//	return;
	alSource3f(mSourceId, AL_VELOCITY, (ALfloat)mVelocity.x, (ALfloat)mVelocity.y, (ALfloat)mVelocity.z);
	//if (checkALError("OpenALSound::alSourcefv:AL_VELOCITY"))
	//	return;

	alSourcei(mSourceId, AL_LOOPING, mLoop);
}

Sound::~Sound()
{
	if (mSoundData != nullptr)
	{
		mSoundData->removeResourceEventReceiver(this);
	}

	uninitialize();

	stop();

	alSourceStop(mSourceId);
	alDeleteSources(1, &mSourceId);
}

void Sound::setSoundData(const std::string& filename)
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		SoundData* pSoundData = static_cast<SoundData*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_SOUND_DATA, filename));
		setSoundData(pSoundData);
	}
}

void Sound::setSoundData(SoundData* soundData)
{
	if (soundData == nullptr)
		return;

	if (mSoundData != nullptr)
	{
		mSoundData->removeResourceEventReceiver(this);

		uninitialize();
	}

	mSoundData = soundData;
	mSoundData->addResourceEventReceiver(this);

	if (soundData != nullptr)
	{
		alSourcef(mSourceId, AL_PITCH,					mSoundData->getPitch());
		alSourcef(mSourceId, AL_GAIN,					mSoundData->getGain());
		alSourcef(mSourceId, AL_MAX_DISTANCE,			mSoundData->getMaxDistance());
		alSourcef(mSourceId, AL_CONE_OUTER_GAIN,		mSoundData->getOuterConeGain());
		alSourcef(mSourceId, AL_CONE_INNER_ANGLE,		mSoundData->getInnerConeAngle());
		alSourcef(mSourceId, AL_CONE_OUTER_ANGLE,		mSoundData->getOuterConeAngle());
	}
}

SoundData* Sound::getSoundData() const
{
	return mSoundData;
}

void Sound::play()
{
	if (isPlaying())
		return;

	if(mSourceId != AL_NONE)
		alSourcePlay(mSourceId);
}

void Sound::pause()
{
	if (isPaused())
		return;

	if(mSourceId != AL_NONE)
		alSourcePause(mSourceId);
}

void Sound::stop()
{
	if (isStopped())
		return;

	if(mSourceId != AL_NONE)
		alSourceStop(mSourceId);
}

bool Sound::isPlaying() const
{
	if(mSourceId != AL_NONE)
	{
		ALint state;
		alGetSourcei(mSourceId, AL_SOURCE_STATE, &state);

		if (state == AL_PLAYING)
			return true;
	}

	return false;
}

bool Sound::isPaused() const
{
	if(mSourceId != AL_NONE)
	{
		ALint state;
		alGetSourcei(mSourceId, AL_SOURCE_STATE, &state);

		if (state == AL_PAUSED)
			return true;
	}

	return false;
}

bool Sound::isStopped() const
{
	if(mSourceId != AL_NONE)
	{
		ALint state;
		alGetSourcei(mSourceId, AL_SOURCE_STATE, &state);

		if (state == AL_STOPPED)
			return true;
	}

	return false;
}

const glm::vec3& Sound::getVelocity() const
{
	return mVelocity;
}

void Sound::setPitch(float pitch)
{
	if(pitch <= 0)
		return;

	mPitch = pitch;

	if(mSourceId != AL_NONE)
		alSourcef(mSourceId, AL_PITCH, mPitch);
}

float Sound::getPitch() const
{
	return mPitch;
}

void Sound::setGain(float gain)
{
	if(gain < 0)
		return;

	mGain = gain;

	if(mSourceId != AL_NONE)
		alSourcef(mSourceId, AL_GAIN, mGain);
}

float Sound::getGain() const
{
	return mGain;
}

void Sound::setDistanceValues(float minDistance, float maxDistance)
{
	setMinDistance(minDistance);
	setMaxDistance(maxDistance);
}

void Sound::setMinDistance(float minDistance)
{
	if(minDistance < 0)
		return;

	mMinDistance = minDistance;

	//TODO!!!
}

void Sound::setMaxDistance(float maxDistance)
{
	if(maxDistance < 0)
		return;

	mMaxDistance = maxDistance;

	if(mSourceId != AL_NONE)
		alSourcef(mSourceId, AL_MAX_DISTANCE, mMaxDistance);
}

float Sound::getMinDistance() const
{
	return mMinDistance;
}

float Sound::getMaxDistance() const
{
	return mMaxDistance;
}

void Sound::setConeSettings(float innerConeAngle, float outerConeAngle, float outerConeGain)
{
	setInnerConeAngle(innerConeAngle);
	setOuterConeAngle(outerConeAngle);
	setOuterConeGain(outerConeGain);
}

void Sound::setInnerConeAngle(float innerConeAngle)
{
	mInnerConeAngle = innerConeAngle;

	if(mSourceId != AL_NONE)
		alSourcef(mSourceId, AL_CONE_INNER_ANGLE, mInnerConeAngle);
}

float Sound::getInnerConeAngle() const
{
	return mInnerConeAngle;
}

void Sound::setOuterConeAngle(float outerConeAngle)
{
	mOuterConeAngle = outerConeAngle;

	if(mSourceId != AL_NONE)
		alSourcef(mSourceId, AL_CONE_OUTER_ANGLE, mOuterConeAngle);
}

float Sound::getOuterConeAngle() const
{
	return mOuterConeAngle;
}

void Sound::setOuterConeGain(float outerConeGain)
{
	if(outerConeGain < 0 || outerConeGain > 1)
		return;

	mOuterConeGain = outerConeGain;

	if(mSourceId != AL_NONE)
		alSourcef(mSourceId, AL_CONE_OUTER_GAIN, mOuterConeGain);
}

float Sound::getOuterConeGain() const
{
	return mOuterConeGain;
}

void Sound::setLoop(bool loop)
{
	mLoop = loop;

	if(mSourceId != AL_NONE)
		alSourcei(mSourceId, AL_LOOPING, mLoop);
}

bool Sound::isLooping() const
{
	return mLoop;
}

void Sound::resourceLoaded(const resource::ResourceEvent& evt)
{
	if (evt.source && evt.source == mSoundData)
	{
		mPitch = mSoundData->getPitch();
		mGain = mSoundData->getGain();
		mMinDistance = mSoundData->getMinDistance();
		mMaxDistance = mSoundData->getMaxDistance();
		mInnerConeAngle = mSoundData->getInnerConeAngle();
		mOuterConeAngle = mSoundData->getOuterConeAngle();
		mOuterConeGain = mSoundData->getOuterConeGain();

		initialize();
	}
}

void Sound::resourceUnloaded(const resource::ResourceEvent& evt)
{
	if (evt.source && evt.source == mSoundData)
	{
		mVelocity = glm::vec3(0, 0, 0);

		mPitch = 1;
		mGain = 1;
		mMinDistance = 1;
		mMaxDistance = 10000;
		mInnerConeAngle = 360;
		mOuterConeAngle = 360;
		mOuterConeGain = 1;

		mLoop = false;

		uninitialize();
	}
}

void Sound::initializeImpl()
{
	if (mSoundData == nullptr)
		return;

	alSourcei(mSourceId, AL_BUFFER, mSoundData->getOpenALBufferID());

	alSourcei(mSourceId, AL_LOOPING, mLoop);
}

void Sound::uninitializeImpl()
{
	alSourcei(mSourceId, AL_BUFFER, -1);//TODO: see if it works!!!
}

void Sound::updateImpl(float elapsedTime)
{
	if (elapsedTime == 0)
		return;

	if (mGameObject != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			glm::vec3 delta = pTransform->getAbsolutePosition() - mLastPosition;
			mVelocity = delta / elapsedTime;
			mLastPosition = pTransform->getAbsolutePosition();
		}
	}

	if (mSourceNeedsUpdate)
	{
		if (mGameObject != nullptr)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != nullptr)
			{
				glm::vec3 position = pTransform->getAbsolutePosition();
				glm::vec3 direction = pTransform->getAbsoluteOrientation() * glm::vec3(0, 0, 1);

				alSource3f(mSourceId, AL_POSITION, (ALfloat)position.x, (ALfloat)position.y, (ALfloat)position.z);
				//if (checkALError(core::string("OpenALSound::alSourcefv:AL_POSITION")))
				//	return;
				alSource3f(mSourceId, AL_DIRECTION, (ALfloat)direction.x, (ALfloat)direction.y, (ALfloat)direction.z);
				//if (checkALError("OpenALSound::alSourcefv:AL_DIRECTION"))
				//	return;
				alSource3f(mSourceId, AL_VELOCITY, (ALfloat)mVelocity.x, (ALfloat)mVelocity.y, (ALfloat)mVelocity.z);
				//if (checkALError("OpenALSound::alSourcefv:AL_VELOCITY"))
				//	return;
			}
		}

		mSourceNeedsUpdate = false;
	}
}

void Sound::onMessageImpl(unsigned int messageID)
{
	if (messageID == game::MESSAGE_TRANSFORM_NEEDS_UPDATE)
	{
		mSourceNeedsUpdate = true;
	}
}

} // end namespace sound
