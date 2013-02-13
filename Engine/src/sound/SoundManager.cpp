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
#include <sound/SoundManager.h>
#include <sound/Sound.h>
#include <sound/SoundFactory.h>
#include <sound/SoundData.h>
#include <sound/SoundDataFactory.h>
#include <sound/Listener.h>
#include <sound/ListenerFactory.h>
#include <sound/SoundFactory.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <game/ComponentFactory.h>
#include <game/GameManager.h>

template<> sound::SoundManager* core::Singleton<sound::SoundManager>::m_Singleton = nullptr;

namespace sound
{

SoundManager::SoundManager(): core::System("SoundManager")
{
	mActiveListener = nullptr;

	mDopplerFactor = 1.0f;
	mSoundSpeed = 343.3f;

	mSoundDataFactory = new SoundDataFactory();
	mListenerFactory = new ListenerFactory();
	mSoundFactory = new SoundFactory();

	mContext = nullptr;
	mDevice = nullptr;
}

SoundManager::~SoundManager()
{
	SAFE_DELETE(mSoundDataFactory);
	SAFE_DELETE(mListenerFactory);
}

void SoundManager::addSound(Sound* sound)
{
	if (sound == nullptr)
		return;

	mSounds[sound->getID()] = sound;
}

void SoundManager::removeSound(Sound* sound)
{
	if (sound == nullptr)
		return;

	removeSound(sound->getID());
}

void SoundManager::removeSound(const unsigned int& id)
{
	std::map<unsigned int, Sound*>::iterator i = mSounds.find(id);
	if (i != mSounds.end())
		mSounds.erase(i);
}

void SoundManager::removeAllSounds()
{
	mSounds.clear();
}

void SoundManager::setActiveListener(Listener* listener)
{
	if (listener == nullptr)
		return;

	mActiveListener = listener;
}

Listener* SoundManager::getActiveListener()
{
	return mActiveListener;
}

void SoundManager::addListener(Listener* listener)
{
	if (listener == nullptr)
		return;

	mListeners[listener->getID()] = listener;
}

void SoundManager::removeListener(Listener* listener)
{
	if (listener == nullptr)
		return;

	removeListener(listener->getID());
}

void SoundManager::removeListener(const unsigned int& id)
{
	std::map<unsigned int, Listener*>::iterator i = mListeners.find(id);
	if (i != mListeners.end())
		mListeners.erase(i);
}

void SoundManager::removeAllListeners()
{
	mListeners.clear();
}

void SoundManager::setDopplerFactor(float dopplerFactor)
{
	// Negative values are invalid
	if(dopplerFactor < 0)
		return;

	mDopplerFactor = dopplerFactor;

	alDopplerFactor(mDopplerFactor);
	if (checkALError("setDopplerFactor::alDopplerFactor:"))
		return;
}

float SoundManager::getDopplerFactor() const
{
	return mDopplerFactor;
}

void SoundManager::setSoundSpeed(float soundSpeed)
{
	// Negative values are invalid
	if(soundSpeed < 0)
		return;

	mSoundSpeed = soundSpeed;

	alSpeedOfSound(soundSpeed);
	if (checkALError("setSoundSpeed::alSpeedOfSound:"))
		return;
}

float SoundManager::getSoundSpeed() const
{
	return mSoundSpeed;
}

void SoundManager::setDefaultSoundFactory(game::ComponentFactory* factory)
{
	mSoundFactory = factory;
}

void SoundManager::removeDefaultSoundFactory()
{
	mSoundFactory = nullptr;
}

void SoundManager::initializeImpl()
{
	alcGetIntegerv(nullptr, ALC_MAJOR_VERSION, sizeof(mMajorVersion), &mMajorVersion);
	alcGetIntegerv(nullptr, ALC_MINOR_VERSION, sizeof(mMinorVersion), &mMinorVersion);

	// Open an audio device
	mDevice = alcOpenDevice(nullptr);
	if (!mDevice)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("SoundSystem", "OpenAL::initialize could not create sound device", core::LOG_LEVEL_ERROR);
		return;
	}

	// Create OpenAL Context
	mContext = alcCreateContext(mDevice, nullptr);
	if (!mContext)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("SoundSystem", "OpenAL::initialize could not create sound context", core::LOG_LEVEL_ERROR);
		return;
	}

	alcMakeContextCurrent(mContext);
	if (checkALError("initialize()"))
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("SoundSystem", "OpenAL::initialize could not make sound context current and active.", core::LOG_LEVEL_ERROR);
		return;
	}

	alListenerf(AL_MAX_DISTANCE, 10000.0f);
	alListenerf(AL_MIN_GAIN, 0.0f);
	alListenerf(AL_MAX_GAIN, 1.0f);
	alListenerf(AL_GAIN, 1.0f);

	// Initialize Doppler
	alDopplerFactor(1.0f); // 1.2 = exaggerate the pitch shift by 20%
	alDopplerVelocity(343.0f); // m/s this may need to be scaled at some point
}

void SoundManager::uninitializeImpl()
{
	// Remove all Sounds
	removeAllSounds();

	// Release the OpenAL Context and the Audio device
	if (mContext != nullptr)
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(mContext);
	}
	if (mDevice != nullptr)
		alcCloseDevice(mDevice);
}

void SoundManager::startImpl() {}

void SoundManager::stopImpl() {}

void SoundManager::updateImpl(float elapsedTime)
{
	if (mActiveListener == nullptr)
		return;

	if (mActiveListener->getGameObject() != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mActiveListener->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			glm::vec3 position = pTransform->getAbsolutePosition();
			glm::vec3 direction = pTransform->getAbsoluteOrientation() * glm::vec3(0, 0, 1);
			glm::vec3 up = pTransform->getAbsoluteOrientation() * glm::vec3(0, 1, 0);
			glm::vec3 velocity = mActiveListener->getVelocity();

			// Initial orientation of the listener = direction + direction up
			ALfloat lorientation[6];
			lorientation[0] = -direction.x;
			lorientation[1] = -direction.y;
			lorientation[2] = -direction.z;
			lorientation[3] = up.x;
			lorientation[4] = up.y;
			lorientation[5] = up.z;

			alListener3f(AL_POSITION, position.x, position.y, position.z);
			if (checkALError("setListenerPosition::alListenerfv:AL_POSITION"))
				return;
			alListenerfv(AL_ORIENTATION, lorientation);
			if (checkALError("setListenerPosition::alListenerfv:AL_DIRECTION"))
				return;
			alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
			if (checkALError("setListenerPosition::alListenerfv:AL_VELOCITY"))
				return;
		}
	}
}

void SoundManager::registerFactoriesImpl()
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		resource::ResourceManager::getInstance()->registerResourceFactory(resource::RESOURCE_TYPE_SOUND_DATA, mSoundDataFactory);
	}

	if (game::GameManager::getInstance() != nullptr)
	{
		game::GameManager::getInstance()->registerComponentFactory(game::COMPONENT_TYPE_LISTENER, mListenerFactory);
		game::GameManager::getInstance()->registerComponentFactory(game::COMPONENT_TYPE_SOUND, mSoundFactory);
	}
}

void SoundManager::removeFactoriesImpl()
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		resource::ResourceManager::getInstance()->removeResourceFactory(resource::RESOURCE_TYPE_SOUND_DATA);
	}

	if (game::GameManager::getInstance() != nullptr)
	{
		game::GameManager::getInstance()->removeComponentFactory(game::COMPONENT_TYPE_LISTENER);
		game::GameManager::getInstance()->removeComponentFactory(game::COMPONENT_TYPE_SOUND);
	}
}

SoundManager* SoundManager::getInstance()
{
	return core::Singleton<SoundManager>::getInstance();
}

} // end namespace sound
