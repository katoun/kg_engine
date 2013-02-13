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

#include <sound/SoundUtils.h>

#include <AL/alure.h>

namespace sound
{

bool checkALError()
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

bool checkALError(const std::string& message)
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

} // end namespace core
