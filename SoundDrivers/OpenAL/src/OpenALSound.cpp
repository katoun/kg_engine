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
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <game/MessageDefines.h>
#include <OpenALSound.h>
#include <OpenALSoundData.h>
#include <OpenALSoundDriver.h>

namespace sound
{

OpenALSound::OpenALSound(): Sound()
{
	alGetError(); // Clear Error Code

	mSourceNeedsUpdate = true;

	// Generate a Source to playback the Buffer
	alGenSources(1, &mSourceId);
	//if (checkALError("initialize::alGenSources :"))
	//	return;
	if (!mSourceId)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("OpenALSound", "Cannot create OpenAL sound source", core::LOG_LEVEL_ERROR);
		return;
	}

	core::vector3d position = core::vector3d::ORIGIN_3D;
	core::vector3d direction = core::vector3d::NEGATIVE_UNIT_Z;

	alSource3f(mSourceId, AL_POSITION, (ALfloat)position.X, (ALfloat)position.Y, (ALfloat)position.Z);
	//if (checkALError(core::string("OpenALSound::alSourcefv:AL_POSITION")))
	//	return;
	alSource3f(mSourceId, AL_DIRECTION, (ALfloat)direction.X, (ALfloat)direction.Y, (ALfloat)direction.Z);
	//if (checkALError("OpenALSound::alSourcefv:AL_DIRECTION"))
	//	return;
	alSource3f(mSourceId, AL_VELOCITY, (ALfloat)mVelocity.X, (ALfloat)mVelocity.Y, (ALfloat)mVelocity.Z);
	//if (checkALError("OpenALSound::alSourcefv:AL_VELOCITY"))
	//	return;
	
	alSourcei(mSourceId, AL_LOOPING, mLoop);
}

OpenALSound::~OpenALSound()
{
	stop();

	alSourceStop(mSourceId);
	alDeleteSources(1, &mSourceId);
}

void OpenALSound::play()
{
	Sound::play();

	if (isPlaying())
		return;

	if(mSourceId != AL_NONE)
		alSourcePlay(mSourceId);
}

void OpenALSound::pause()
{
	Sound::pause();

	if (isPaused())
		return;
	
	if(mSourceId != AL_NONE)
		alSourcePause(mSourceId);
}

void OpenALSound::stop()
{
	Sound::stop();

	if (isStopped())
		return;

	if(mSourceId != AL_NONE)
		alSourceStop(mSourceId);
}

bool OpenALSound::isPlaying() const
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

bool OpenALSound::isPaused() const
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

bool OpenALSound::isStopped() const
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

void OpenALSound::setPitch(float pitch)
{
	Sound::setPitch(pitch);

	if(mSourceId != AL_NONE)
		alSourcef(mSourceId, AL_PITCH, mSoundData->getPitch());
}

void OpenALSound::setGain(float gain)
{
	Sound::setGain(gain);

	if(mSourceId != AL_NONE)
		alSourcef(mSourceId, AL_GAIN, mSoundData->getGain());
}

void OpenALSound::setMinDistance(float minDistance)
{
	Sound::setMinDistance(minDistance);
}

void OpenALSound::setMaxDistance(float maxDistance)
{
	Sound::setMaxDistance(maxDistance);

	if(mSourceId != AL_NONE)
		alSourcef(mSourceId, AL_MAX_DISTANCE, mSoundData->getMaxDistance());
}

void OpenALSound::setInnerConeAngle(float innerConeAngle)
{
	Sound::setInnerConeAngle(innerConeAngle);

	if(mSourceId != AL_NONE)
		alSourcef(mSourceId, AL_CONE_INNER_ANGLE, mSoundData->getInnerConeAngle());
}

void OpenALSound::setOuterConeAngle(float outerConeAngle)
{
	Sound::setOuterConeAngle(outerConeAngle);

	if(mSourceId != AL_NONE)
		alSourcef(mSourceId, AL_CONE_OUTER_ANGLE, mSoundData->getOuterConeAngle());
}

void OpenALSound::setOuterConeGain(float outerConeGain)
{
	Sound::setOuterConeGain(outerConeGain);

	if(mSourceId != AL_NONE)
		alSourcef(mSourceId, AL_CONE_OUTER_GAIN, mSoundData->getOuterConeGain());
}

void OpenALSound::setLoop(bool loop)
{
	Sound::setLoop(loop);

	if(mSourceId != AL_NONE)
		alSourcei(mSourceId, AL_LOOPING, mLoop);
}

void OpenALSound::initializeImpl()
{
	if (mSoundData == nullptr)
		return;

	OpenALSoundData* pOpenalSoundData = static_cast<OpenALSoundData*>(mSoundData);
	if (pOpenalSoundData == nullptr)
		return;
	
	alSourcei(mSourceId, AL_BUFFER, pOpenalSoundData->getOpenALBufferID());

	alSourcei(mSourceId, AL_LOOPING, mLoop);
}

void OpenALSound::uninitializeImpl()
{
	alSourcei(mSourceId, AL_BUFFER, -1);//katoun TODO see if it works!!!
}

void OpenALSound::updateImpl(float elapsedTime)
{
	Sound::updateImpl(elapsedTime);
	
	if (mSourceNeedsUpdate)
	{
		if (mGameObject != nullptr)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != nullptr)
			{
				core::vector3d position = pTransform->getAbsolutePosition();
				core::vector3d direction = pTransform->getAbsoluteOrientation() * core::vector3d::NEGATIVE_UNIT_Z;

				alSource3f(mSourceId, AL_POSITION, (ALfloat)position.X, (ALfloat)position.Y, (ALfloat)position.Z);
				//if (checkALError(core::string("OpenALSound::alSourcefv:AL_POSITION")))
				//	return;
				alSource3f(mSourceId, AL_DIRECTION, (ALfloat)direction.X, (ALfloat)direction.Y, (ALfloat)direction.Z);
				//if (checkALError("OpenALSound::alSourcefv:AL_DIRECTION"))
				//	return;
				alSource3f(mSourceId, AL_VELOCITY, (ALfloat)mVelocity.X, (ALfloat)mVelocity.Y, (ALfloat)mVelocity.Z);
				//if (checkALError("OpenALSound::alSourcefv:AL_VELOCITY"))
				//	return;
			}
		}

		mSourceNeedsUpdate = false;
	}	
}

void OpenALSound::onMessageImpl(unsigned int messageID)
{
	if (messageID == game::MESSAGE_TRANSFORM_NEEDS_UPDATE)
	{
		mSourceNeedsUpdate = true;
	}
}

void OpenALSound::setSoundDataImpl(SoundData* soundData)
{
	if (soundData == nullptr)
		return;

	alSourcef(mSourceId, AL_PITCH,					mSoundData->getPitch());
	alSourcef(mSourceId, AL_GAIN,					mSoundData->getGain());
	alSourcef(mSourceId, AL_MAX_DISTANCE,			mSoundData->getMaxDistance());
	alSourcef(mSourceId, AL_CONE_OUTER_GAIN,		mSoundData->getOuterConeGain());
	alSourcef(mSourceId, AL_CONE_INNER_ANGLE,		mSoundData->getInnerConeAngle());
	alSourcef(mSourceId, AL_CONE_OUTER_ANGLE,		mSoundData->getOuterConeAngle());
}

bool OpenALSound::checkALError()
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

bool OpenALSound::checkALError(const std::string& message)
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

} // end namespace sound
