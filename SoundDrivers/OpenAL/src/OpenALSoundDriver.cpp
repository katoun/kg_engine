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
#include <sound/Listener.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <OpenALSoundDriver.h>
#include <OpenALSoundData.h>
#include <OpenALSound.h>

template<> sound::OpenALSoundDriver* core::Singleton<sound::OpenALSoundDriver>::m_Singleton = nullptr;

namespace sound
{

OpenALSoundDriver::OpenALSoundDriver(): SoundDriver("OpenAL SoundDriver") 
{
	mContext = nullptr;
	mDevice = nullptr;
}

OpenALSoundDriver::~OpenALSoundDriver() {}

void OpenALSoundDriver::updateListener(Listener* listener)
{
	if (listener == nullptr)
		return;

	if (listener->getGameObject() != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(listener->getGameObject()->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			glm::vec3 position = pTransform->getAbsolutePosition();
			glm::vec3 direction = pTransform->getAbsoluteOrientation() * glm::vec3(0, 0, 1);
			glm::vec3 up = pTransform->getAbsoluteOrientation() * glm::vec3(0, 1, 0);
			glm::vec3 velocity = listener->getVelocity();

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

void OpenALSoundDriver::setDopplerFactor(float dopplerFactor)
{
	alDopplerFactor(dopplerFactor);
	if (checkALError("setDopplerFactor::alDopplerFactor:"))
		return;
}

void OpenALSoundDriver::setSoundSpeed(float soundSpeed)
{
	alSpeedOfSound(soundSpeed);
	if (checkALError("setSoundSpeed::alSpeedOfSound:"))
		return;
}

void OpenALSoundDriver::initializeImpl()
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

void OpenALSoundDriver::uninitializeImpl()
{
	// Release the OpenAL Context and the Audio device
	if (mContext != nullptr)
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(mContext);
	}
	if (mDevice != nullptr)
		alcCloseDevice(mDevice);
}

void OpenALSoundDriver::startImpl() {}

void OpenALSoundDriver::stopImpl() {}

void OpenALSoundDriver::updateImpl(float elapsedTime) {}

bool OpenALSoundDriver::checkALError()
{
	ALenum errCode;
	if ((errCode = alGetError()) == AL_NO_ERROR) return false;

	char mStr[256];
	sprintf_s(mStr,"OpenAL error! %s\n", (char*)alGetString(errCode));

#ifdef _DEBUG
	std::cout<<mStr<<std::endl;
#endif

	return true;
}

bool OpenALSoundDriver::checkALError(const std::string& message)
{
	ALenum errCode;
	if ((errCode = alGetError()) == AL_NO_ERROR) return false;

	char mStr[256];
	switch (errCode)
	{
	case AL_INVALID_NAME:
		sprintf_s(mStr,"ERROR SoundManager::%s Invalid Name", message.c_str());
		break;
	case AL_INVALID_ENUM:
		sprintf_s(mStr,"ERROR SoundManager::%s Invalid Enum", message.c_str());
		break;
	case AL_INVALID_VALUE:
		sprintf_s(mStr,"ERROR SoundManager::%s Invalid Value", message.c_str());
		break;
	case AL_INVALID_OPERATION:
		sprintf_s(mStr,"ERROR SoundManager::%s Invalid Operation", message.c_str());
		break;
	case AL_OUT_OF_MEMORY:
		sprintf_s(mStr,"ERROR SoundManager::%s Out Of Memory", message.c_str());
		break;
	default:
		sprintf_s(mStr,"ERROR SoundManager::%s Unknown error (%i) case in testALError()", message.c_str(), errCode);
		break;
	};

#ifdef _DEBUG
	std::cout<<mStr<<std::endl;
#endif

	return true;
}

OpenALSoundDriver* OpenALSoundDriver::getInstance()
{
	return core::Singleton<OpenALSoundDriver>::getInstance();
}

} // end namespace sound
