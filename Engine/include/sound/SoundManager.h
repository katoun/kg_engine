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

#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include <core/Config.h>
#include <core/System.h>
#include <core/Singleton.h>
#include <engine/EngineSettings.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>

#include <string>
#include <list>
#include <map>

namespace core
{
class vector3d;
class quaternion;
}

namespace scene
{
class Node;
}

namespace sound
{

class Sound;
class SoundData;
class Listener;
class SoundDriver;
class SoundFactory;

//! Sound Manager.
//! 
//! This class handles everything related to sound.
//! \author: Kat'Oun
//! \version: 1.0
class ENGINE_PUBLIC_EXPORT SoundManager: public core::System, public core::Singleton<SoundManager>
{
	friend class ResourceManager;

public:

	// Default Constructor
	SoundManager();

	~SoundManager();

	//! Initialize all sound elements waiting to be initialized.
	void initializeObjects();

	//! Creates a sound to be managed by sound manager.
	Sound* createSound(const std::string& filename, scene::Node* parent = NULL);
	Sound* createSound(const std::string& name, const std::string& filename, scene::Node* parent = NULL);

	Sound* createSound(SoundData* soundData, scene::Node* parent = NULL);
	Sound* createSound(const std::string& name, SoundData* soundData, scene::Node* parent = NULL);

	//! Retrieves a pointer to a sound by id.
	Sound* getSound(const unsigned int& id);

	//! Retrieves the total number of created sounds.
	unsigned int getNumberOfSounds() const;

	//! Removes a sound.
	void removeSound(Sound *sound);
	//! Removes a sound.
	void removeSound(const unsigned int& id);
	//! Removes (and destroys) all sounds.
	void removeAllSounds();

	//! Creates a listener to be managed by sound manager.
	Listener* createListener(scene::Node* parent = NULL);
	Listener* createListener(const std::string& name, scene::Node* parent = NULL);

	//! Sets a pointer to the active listener.
	void setActiveListener(Listener *listener);

	//! Retrieve a pointer to the active listener.
	Listener* getActiveListener();

	//! Retrieves a pointer to the a listener.
	Listener* getListener(const unsigned int& id);

	//! Retrieves the total number of created listeners.
	unsigned int getNumberOfListeners() const;

	//! Removes a listener.
	void removeListener(Listener *listener);
	//! Removes a listener.
	void removeListener(const unsigned int& id);

	//! Removes all listeners.
	void removeAllListeners();
	
	//! Sets the Doppler factor.
	//! The Doppler factor is a simple scaling factor of the source and listener
	//! velocities to exaggerate or deemphasize the Doppler (pitch) shift resulting
	//! from the Doppler Effect Calculations.
	void setDopplerFactor(float dopplerFactor);
	//! Returns the Doppler factor.
	float getDopplerFactor() const;
	
	//! Sets the speed of sound used in the Doppler calculations.
	//! This sets the propagation speed used in the Doppler calculations.
	//! The default is 343.3 m/s (Speed of sound through air).
	void setSoundSpeed(float soundSpeed);
	//! Returns the speed of sound.
	float getSoundSpeed() const;

	void setSoundFactory(SoundFactory* factory);
	void removeSoundFactory();

	static SoundManager& getInstance();
	static SoundManager* getInstancePtr();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void startImpl();
	void stopImpl();
	void updateImpl(float elapsedTime);
	void setSystemDriverImpl(core::SystemDriver* systemDriver);
	void removeSystemDriverImpl();

	SoundDriver* mSoundDriver;

	//! Central list of sounds - for easy memory management and lookup.
	std::map<unsigned int, Sound*> mSounds;

	//! Central list of listeners - for easy memory management and lookup.
	std::map<unsigned int, Listener*> mListeners;
	Listener* mActiveListener;

	SoundFactory* mSoundFactory;

	float mDopplerFactor;
	float mSoundSpeed;
};

} // end namespace sound

#endif