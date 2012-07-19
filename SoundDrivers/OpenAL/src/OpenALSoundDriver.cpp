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

#include <core/Log.h>
#include <core/LogDefines.h>
#include <sound/Listener.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>
#include <OpenALSoundDriver.h>
#include <OpenALSoundData.h>
#include <OpenALSound.h>

template<> sound::OpenALSoundDriver& core::Singleton<sound::OpenALSoundDriver>::ms_Singleton = sound::OpenALSoundDriver();

namespace sound
{

OpenALSoundDriver::OpenALSoundDriver(): SoundDriver("OpenAL SoundDriver") 
{
	mContext = NULL;
	mDevice = NULL;
}

OpenALSoundDriver::~OpenALSoundDriver() {}

void OpenALSoundDriver::updateListener(Listener* listener)
{
	assert(listener != NULL);
	if (listener == NULL)
		return;

	core::vector3d position = listener->getAbsolutePosition();
	core::vector3d direction = listener->getAbsoluteOrientation() * core::vector3d::NEGATIVE_UNIT_Z;
	core::vector3d up = listener->getAbsoluteOrientation() * core::vector3d::UNIT_Y;
	core::vector3d velocity = listener->getVelocity();

	// Initial orientation of the listener = direction + direction up
	ALfloat lorientation[6];
	lorientation[0] = -direction.X;
	lorientation[1] = -direction.Y;
	lorientation[2] = -direction.Z;
	lorientation[3] = up.X;
	lorientation[4] = up.Y;
	lorientation[5] = up.Z;

	alListener3f(AL_POSITION, position.X, position.Y, position.Z);
	if (checkALError("setListenerPosition::alListenerfv:AL_POSITION"))
		return;
	alListenerfv(AL_ORIENTATION, lorientation);
	if (checkALError("setListenerPosition::alListenerfv:AL_DIRECTION"))
		return;
	alListener3f(AL_VELOCITY, velocity.X, velocity.Y, velocity.Z);
	if (checkALError("setListenerPosition::alListenerfv:AL_VELOCITY"))
		return;
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
	alcGetIntegerv(NULL, ALC_MAJOR_VERSION, sizeof(mMajorVersion), &mMajorVersion);
	alcGetIntegerv(NULL, ALC_MINOR_VERSION, sizeof(mMinorVersion), &mMinorVersion);

	// Open an audio device
	mDevice = alcOpenDevice(NULL);
	if (!mDevice)
	{
		core::Log::getInstance().logMessage("SoundSystem", "OpenAL::initialize could not create sound device", core::LOG_LEVEL_ERROR);
		return;
	}

	// Create OpenAL Context
	mContext = alcCreateContext(mDevice, NULL);
	if (!mContext)
	{
		core::Log::getInstance().logMessage("SoundSystem", "OpenAL::initialize could not create sound context", core::LOG_LEVEL_ERROR);
		return;
	}

	alcMakeContextCurrent(mContext);
	if (checkALError("initialize()"))
	{
		core::Log::getInstance().logMessage("SoundSystem", "OpenAL::initialize could not make sound context current and active.", core::LOG_LEVEL_ERROR);
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
	if (mContext != NULL)
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(mContext);
	}
	if (mDevice != NULL)
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

OpenALSoundDriver& OpenALSoundDriver::getInstance()
{
	return core::Singleton<OpenALSoundDriver>::getInstance();
}

OpenALSoundDriver *OpenALSoundDriver::getInstancePtr()
{
	return core::Singleton<OpenALSoundDriver>::getInstancePtr();
}

} // end namespace sound
