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

#include <sound/Sound.h>
#include <sound/SoundData.h>
#include <resource/ResourceEvent.h>
#include <resource/ResourceManager.h>
#include <core/Vector3d.h>
#include <core/Utils.h>

namespace sound
{

unsigned int Sound::msNextGeneratedSoundIndex = 0;

Sound::Sound(SoundData* soundData): scene::Node("Sound_" + core::intToString(msNextGeneratedSoundIndex++))
{
	mNodeType = scene::NT_SOUND;

	mVelocity = core::vector3d::ORIGIN_3D;

	mPitch = 1.0f;
	mGain = 1.0f;
	mMinDistance = 1.0f * ENGINE_UNIT_M;
	mMaxDistance = 10000.0f * ENGINE_UNIT_M;
	mInnerConeAngle = 360.0f;
	mOuterConeAngle = 360.0f;
	mOuterConeGain = 1.0f;

	mLoop = false;

	assert(soundData != NULL);
	if (soundData == NULL)
		return;

	mSoundData = soundData;
	mSoundData->addResourceEventReceiver(this);
}

Sound::Sound(const std::string& name, SoundData* soundData): scene::Node(name)
{
	mNodeType = scene::NT_SOUND;

	mVelocity = core::vector3d::ORIGIN_3D;

	mLoop = false;

	assert(soundData != NULL);
	if (soundData == NULL)
		return;

	mSoundData = soundData;
	mSoundData->addResourceEventReceiver(this);
}

Sound::~Sound()
{
	if (mSoundData != NULL)	mSoundData->removeResourceEventReceiver(this);

	uninitialize();
}

void Sound::setSoundData(const std::string& filename)
{
	SoundData* newSoundData = static_cast<SoundData*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_SOUND_DATA, filename));
	if (newSoundData == NULL)
		return;

	if (mSoundData != NULL)
		mSoundData->removeResourceEventReceiver(this);

	uninitialize();

	mSoundData = newSoundData;
	mSoundData->addResourceEventReceiver(this);
}

void Sound::setSoundData(SoundData* soundData)
{
	if (soundData == NULL)
		return;

	if (mSoundData != NULL)
		mSoundData->removeResourceEventReceiver(this);

	uninitialize();

	mSoundData = soundData;
	mSoundData->addResourceEventReceiver(this);
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

void Sound::setVelocity(float x, float y, float z)
{
	mVelocity.X = x;
	mVelocity.Y = y;
	mVelocity.Z = z;
	mModifiedAbsoluteTransform = true;
}

void Sound::setVelocity(const core::vector3d& vec)
{
	mVelocity = vec;
	mModifiedAbsoluteTransform = true;
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

		initialize();//HERE IS THE PROBLEM!!!
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

} // end namespace sound
