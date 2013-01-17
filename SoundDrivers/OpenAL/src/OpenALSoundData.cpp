/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2012 Catalin Alexandru Nastase

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
#include <resource/ResourceManager.h>
#include <OpenALSoundData.h>

namespace sound
{

#define BUFFER_SIZE 1024

OpenALSoundData::OpenALSoundData(const std::string& filename, resource::Serializer* serializer): SoundData(filename, serializer)
{
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

OpenALSoundData::~OpenALSoundData()
{
}

ALuint OpenALSoundData::getOpenALBufferID() const
{
	return mBufferId;
}

bool OpenALSoundData::loadImpl()
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

void OpenALSoundData::unloadImpl()
{
	SoundData::unloadImpl();

	alDeleteBuffers(1, &mBufferId);
}

bool OpenALSoundData::checkALError()
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

bool OpenALSoundData::checkALError(const std::string& message)
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