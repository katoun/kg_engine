/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

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

#ifndef _OPENAL_SOUND_DRIVER_H_
#define _OPENAL_SOUND_DRIVER_H_

#include <OpenALConfig.h>
#include <core/Singleton.h>
#include <sound/SoundDriver.h>

namespace resource
{
class Resource;
class Serializer;
class DataStream;
}

namespace sound
{

class OpenALSoundDriver: public SoundDriver, public core::Singleton<OpenALSoundDriver>
{
	friend class ResourceManager;

public:
	// Default constructor / destructor
	OpenALSoundDriver();
	~OpenALSoundDriver();

	void updateListener(Listener* listener);

	void setDopplerFactor(float dopplerFactor);

	void setSoundSpeed(float soundSpeed);

	static OpenALSoundDriver& getInstance();
	static OpenALSoundDriver* getInstancePtr();
	
protected:

	virtual void initializeImpl();
	virtual void uninitializeImpl();
	virtual void startImpl();
	virtual void stopImpl();
	virtual void updateImpl(float elapsedTime);

	signed int mMajorVersion;
	signed int mMinorVersion;

	ALCcontext* mContext;
	ALCdevice* mDevice;

	static bool checkALError();
	static bool checkALError(const std::string& message);
};

} // end namespace sound

#endif