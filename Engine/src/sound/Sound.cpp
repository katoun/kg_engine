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

#include <sound/Sound.h>
#include <sound/SoundData.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <resource/ResourceEvent.h>
#include <resource/ResourceManager.h>
#include <core/Vector3d.h>
#include <core/Utils.h>

namespace sound
{

Sound::Sound(): game::Component()
{
	mType = game::COMPONENT_TYPE_SOUND;

	mSoundData = nullptr;

	mLastPosition = core::vector3d::ORIGIN_3D;
	mVelocity = core::vector3d::ORIGIN_3D;

	mPitch = 1.0f;
	mGain = 1.0f;
	mMinDistance = 1.0f * ENGINE_UNIT_M;
	mMaxDistance = 10000.0f * ENGINE_UNIT_M;
	mInnerConeAngle = 360.0f;
	mOuterConeAngle = 360.0f;
	mOuterConeGain = 1.0f;

	mLoop = false;
}

Sound::~Sound()
{
	if (mSoundData != nullptr)
	{
		mSoundData->removeResourceEventReceiver(this);
	}

	uninitialize();
}

void Sound::setSoundData(const std::string& filename)
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		SoundData* newSoundData = static_cast<SoundData*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_SOUND_DATA, filename));
		if (newSoundData == nullptr)
			return;

		if (mSoundData != nullptr)
		{
			mSoundData->removeResourceEventReceiver(this);

			uninitialize();
		}

		mSoundData = newSoundData;
		mSoundData->addResourceEventReceiver(this);

		setSoundDataImpl(mSoundData);
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

	setSoundDataImpl(mSoundData);
}

SoundData* Sound::getSoundData() const
{
	return mSoundData;
}

void Sound::play() {}

void Sound::pause() {}

void Sound::stop() {}

bool Sound::isPlaying() const
{
	return false;
}

bool Sound::isPaused() const
{
	return true;
}

bool Sound::isStopped() const
{
	return true;
}

const core::vector3d& Sound::getVelocity() const
{
	return mVelocity;
}

void Sound::setPitch(float pitch)
{
	if(pitch <= 0)
		return;

	mPitch = pitch;
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
}

void Sound::setMaxDistance(float maxDistance)
{
	if(maxDistance < 0)
		return;

	mMaxDistance = maxDistance;
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
}

float Sound::getInnerConeAngle() const
{
	return mInnerConeAngle;
}

void Sound::setOuterConeAngle(float outerConeAngle)
{
	mOuterConeAngle = outerConeAngle;
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
}

float Sound::getOuterConeGain() const
{
	return mOuterConeGain;
}

void Sound::setLoop(bool loop)
{
	mLoop = loop;
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
		mVelocity = core::vector3d::ORIGIN_3D;

		mPitch = 1.0f;
		mGain = 1.0f;
		mMinDistance = 1.0f * ENGINE_UNIT_M;
		mMaxDistance = 10000.0f * ENGINE_UNIT_M;
		mInnerConeAngle = 360.0f;
		mOuterConeAngle = 360.0f;
		mOuterConeGain = 1.0f;

		mLoop = false;

		uninitialize();
	}
}

void Sound::updateImpl(float elapsedTime)
{
	if (elapsedTime == 0.0f)
		return;

	if (mGameObject != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			core::vector3d delta = pTransform->getAbsolutePosition() - mLastPosition;
			mVelocity = delta / elapsedTime;
			mLastPosition = pTransform->getAbsolutePosition();
		}
	}
}

void Sound::setSoundDataImpl(SoundData* soundData) {}

} // end namespace sound
