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