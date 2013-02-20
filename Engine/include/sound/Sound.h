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

#ifndef _SOUND_H_
#define _SOUND_H_

#include <EngineConfig.h>
#include <game/Component.h>
#include <resource/ResourceEventReceiver.h>

#include <AL/alure.h>
#include <glm/glm.hpp>

namespace core
{
class vector3d;
}

namespace resource
{
	struct ResourceEvent;
}

namespace sound
{

class SoundData;
class SoundProperties;

//!  Defines a sound in the sound world.
class ENGINE_PUBLIC_EXPORT Sound: public game::Component, public resource::ResourceEventReceiver
{
public:

	Sound();
	virtual ~Sound();

	//! Sets the sound data this sound will use.
	void setSoundData(const std::string& filename);
	void setSoundData(SoundData* soundData);

	SoundData* getSoundData() const;

	//! Plays the sound.
	void play();

	//! Pauses the sound. Upon calling play again, the sound will resume where it left off.
	void pause();

	//! Stops the sound. Upon calling play again, the sound will resume from the begining.
	void stop();

	//! Returns true if the sound is playing.
	 bool isPlaying() const;

	//! Returns true if the sound is paused.
	bool isPaused() const;

	//! Returns true if the sound is stopped.
	bool isStopped() const;

	const glm::vec3& getVelocity() const;

	//! Sets the pitch multiplier.
	void setPitch(float pitch);
	//! Returns the pitch multiplier.
	float getPitch() const;

	//! Sets the gain.
	void setGain(float gain);
	//! Returns the gain.
	float getGain() const;

	//! Sets the variables used in the distance attenuation calculation.
	//! \param minDistance: The min distance that the sound emitter will cease to continue growing louder at (as it approaches the listener).
	//! \param maxDistance: The max distance a sound stops attenuating at.
	void setDistanceValues(float minDistance, float maxDistance);
	//! Sets the Min Distance.
	void setMinDistance(float minDistance);
	//! Sets the Max Distance.
	void setMaxDistance(float maxDistance);
	
	//! Returns the min distance.
	float getMinDistance() const;
	//! Returns the max distance.
	float getMaxDistance() const;

	//! Sets the variables used in the sound projection cone.
	//! \param innerConeAngle: The inside cone angle.
	//! \param outerConeAngle: The outside cone angle.
	//! \param outerConeGain: The outer cone gain.
	void setConeSettings(float innerConeAngle, float outerConeAngle, float outerConeGain);
	
	//! Sets the inner angle of the sound cone for a directional sound.
	void setInnerConeAngle(float innerConeAngle);
	//! Return the innerConeAngle.
	float getInnerConeAngle() const;
	
	//! Sets the outer angle of the sound cone for a directional sound.
	void setOuterConeAngle(float outerConeAngle);
	//! Returns the outerConeAngle.
	float getOuterConeAngle() const;

	//! Sets the gain outside the sound cone of a directional sound.
	void setOuterConeGain(float outerConeGain);
	//! Returns the outerConeGain.
	float getOuterConeGain() const;

	//! Sets the looping state.
	void setLoop(bool loop);
	//! Returns looping state.
	bool isLooping() const;

	void resourceLoaded(const resource::ResourceEvent& evt);
	void resourceUnloaded(const resource::ResourceEvent& evt);

protected:

	void initializeImpl();
	void uninitializeImpl();
	void updateImpl(float elapsedTime);
	void onMessageImpl(unsigned int messageID);

	ALuint mSourceId;

	bool mSourceNeedsUpdate;

	SoundData* mSoundData;

	glm::vec3 mLastPosition;
	glm::vec3 mVelocity;

	float mPitch;
	float mGain;
	float mMinDistance;
	float mMaxDistance;
	float mInnerConeAngle;
	float mOuterConeAngle;
	float mOuterConeGain;

	bool mLoop;
};

} // end namespace sound

#endif