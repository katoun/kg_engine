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