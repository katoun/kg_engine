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

#ifndef _OPENAL_SOUND_DATA_H_
#define _OPENAL_SOUND_DATA_H_

#include <OpenALConfig.h>
#include <sound/SoundData.h>

namespace sound
{

class OPENAL_PUBLIC_EXPORT OpenALSoundData: public SoundData
{
public:

	OpenALSoundData(const std::string& filename, resource::Serializer* serializer);
	~OpenALSoundData();

	ALuint getOpenALBufferID() const;

protected:

	bool loadImpl();
	void unloadImpl();

	ALuint mBufferId;

	ALvoid* mData;
	ALint mDataSize;
	ALint mFrequency;
	ALint mChannels;
	ALint mBPS;
	ALenum mBufferFormat;
	
	static bool checkALError();
	static bool checkALError(const std::string& message);
};

} // end namespace sound

#endif