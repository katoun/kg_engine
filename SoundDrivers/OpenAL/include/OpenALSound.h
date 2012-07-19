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

#ifndef _OPENAL_SOUND_H_
#define _OPENAL_SOUND_H_

#include <OpenALConfig.h>
#include <sound/Sound.h>

namespace sound
{

class OPENAL_PUBLIC_EXPORT OpenALSound: public Sound
{
public:

	OpenALSound(SoundData* soundData);
	OpenALSound(const std::string& name, SoundData* soundData);
	~OpenALSound();

	void play();

	void pause();

	void stop();

	bool isPlaying() const;

	bool isPaused() const;

	bool isStopped() const;

	void setPitch(float pitch);
	
	void setGain(float gain);
	
	void setMinDistance(float minDistance);
	
	void setMaxDistance(float maxDistance);
	
	void setInnerConeAngle(float innerConeAngle);
	
	void setOuterConeAngle(float outerConeAngle);

	void setOuterConeGain(float outerConeGain);

	void setLoop(bool loop);

protected:

	void initializeImpl();

	void updateTransformImpl();

	ALuint mSourceId;

	static bool checkALError();
	static bool checkALError(const std::string& message);
};

} // end namespace sound

#endif