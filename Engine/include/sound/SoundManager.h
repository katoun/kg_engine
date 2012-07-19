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

namespace game
{
class ComponentFactory;
}

namespace sound
{

class Sound;
class SoundData;
class Listener;
class SoundDriver;
class SoundFactory;
class ListenerFactory;

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

	//! Sets a pointer to the active listener.
	void setActiveListener(Listener *listener);

	//! Gets the active listener.
	Listener* getActiveListener();

	//!  Adds a listener to be managed by this sound manager.
	void addListener(Listener *listener);

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
	void registerDefaultFactoriesImpl();
	void removeDefaultFactoriesImpl();

	game::ComponentFactory* mDefaultListenerFactory;

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