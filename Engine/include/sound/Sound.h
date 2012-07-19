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

#ifndef _SOUND_H_
#define _SOUND_H_

#include <core/Config.h>
#include <scene/Node.h>
#include <resource/ResourceEventReceiver.h>

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
//!  Author: Kat'Oun
//!  version: 1.0
class ENGINE_PUBLIC_EXPORT Sound: public scene::Node, public resource::ResourceEventReceiver
{
public:

	Sound(SoundData* soundData);
	Sound(const std::string& name, SoundData* soundData);

	virtual ~Sound();

	//! Sets the sound data this sound will use.
	void setSoundData(const std::string& filename);
	void setSoundData(SoundData* soundData);

	SoundData* getSoundData() const;

	//! Plays the sound.
	virtual void play();

	//! Pauses the sound. Upon calling play again, the sound will resume where it left off.
	virtual void pause();

	//! Stops the sound. Upon calling play again, the sound will resume from the begining.
	virtual void stop();

	//! Returns true if the sound is playing.
	virtual bool isPlaying() const;

	//! Returns true if the sound is paused.
	virtual bool isPaused() const;

	//! Returns true if the sound is stopped.
	virtual bool isStopped() const;

	void setVelocity(float x, float y, float z);
	void setVelocity(const core::vector3d& vec);
	const core::vector3d& getVelocity() const;

	//! Sets the pitch multiplier.
	virtual void setPitch(float pitch);
	//! Returns the pitch multiplier.
	float getPitch() const;

	//! Sets the gain.
	virtual void setGain(float gain);
	//! Returns the gain.
	float getGain() const;

	//! Sets the variables used in the distance attenuation calculation.
	//! \param minDistance: The min distance that the sound emitter will cease to continue growing louder at (as it approaches the listener).
	//! \param maxDistance: The max distance a sound stops attenuating at.
	void setDistanceValues(float minDistance, float maxDistance);
	//! Sets the Min Distance.
	virtual void setMinDistance(float minDistance);
	//! Sets the Max Distance.
	virtual void setMaxDistance(float maxDistance);
	
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
	virtual void setInnerConeAngle(float innerConeAngle);
	//! Return the innerConeAngle.
	float getInnerConeAngle() const;
	
	//! Sets the outer angle of the sound cone for a directional sound.
	virtual void setOuterConeAngle(float outerConeAngle);
	//! Returns the outerConeAngle.
	float getOuterConeAngle() const;

	//! Sets the gain outside the sound cone of a directional sound.
	virtual void setOuterConeGain(float outerConeGain);
	//! Returns the outerConeGain.
	float getOuterConeGain() const;

	//! Sets the looping state.
	virtual void setLoop(bool loop);
	//! Returns looping state.
	bool isLooping() const;

	void resourceLoaded(const resource::ResourceEvent& evt);
	void resourceUnloaded(const resource::ResourceEvent& evt);

protected:

	// Incremented count for next index
	static unsigned int msNextGeneratedSoundIndex;

	SoundData* mSoundData;

	core::vector3d mVelocity;

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