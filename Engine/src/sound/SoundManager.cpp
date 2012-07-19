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

#include <sound/SoundManager.h>
#include <sound/Sound.h>
#include <sound/SoundData.h>
#include <sound/Listener.h>
#include <sound/SoundFactory.h>
#include <sound/SoundDriver.h>
#include <scene/SceneManager.h>

template<> sound::SoundManager& core::Singleton<sound::SoundManager>::ms_Singleton = sound::SoundManager();

namespace sound
{

SoundManager::SoundManager(): core::System("SoundManager")
{
	mSoundDriver = NULL;
	mActiveListener = NULL;

	mDopplerFactor = 1.0f;
	mSoundSpeed = 343.3f;

	mSoundFactory = NULL;
}

SoundManager::~SoundManager() {}

Sound* SoundManager::createSound(const std::string& filename, scene::Node* parent)
{
	if (mSoundFactory == NULL)
		return NULL;

	SoundData* newSoundData = static_cast<SoundData*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_SOUND_DATA, filename));
	if (!newSoundData) return NULL;

	Sound* newSound = NULL;
	newSound = mSoundFactory->createSound(newSoundData);
	if (newSound == NULL)
		return NULL;

	mSounds[newSound->getID()] = newSound;

	scene::SceneManager::getInstance().addNode(newSound, parent);

	return newSound;
}

Sound* SoundManager::createSound(const std::string& name, const std::string& filename, scene::Node* parent)
{
	if (mSoundFactory == NULL) return NULL;

	SoundData* newSoundData = static_cast<SoundData*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_SOUND_DATA, filename));
	if (newSoundData = NULL) return NULL;

	Sound* newSound = NULL;
	newSound = mSoundFactory->createSound(name, newSoundData);
	if (newSound == NULL) return NULL;

	mSounds[newSound->getID()] = newSound;

	scene::SceneManager::getInstance().addNode(newSound, parent);

	return newSound;
}

Sound* SoundManager::createSound(SoundData* soundData, scene::Node* parent)
{
	if (mSoundFactory == NULL) return NULL;
	if (soundData == NULL) return NULL;

	Sound* newSound = NULL;
	newSound = mSoundFactory->createSound(soundData);
	if (newSound == NULL) return NULL;

	mSounds[newSound->getID()] = newSound;

	scene::SceneManager::getInstance().addNode(newSound, parent);

	return newSound;
}

Sound* SoundManager::createSound(const std::string& name, SoundData* soundData, scene::Node* parent)
{
	if (mSoundFactory == NULL) return NULL;
	if (soundData == NULL) return NULL;

	Sound* newSound = NULL;
	newSound = mSoundFactory->createSound(name, soundData);
	if (newSound == NULL) return NULL;

	mSounds[newSound->getID()] = newSound;

	scene::SceneManager::getInstance().addNode(newSound, parent);

	return newSound;
}

Sound* SoundManager::getSound(const unsigned int& id)
{
	std::map<unsigned int, Sound*>::const_iterator i = mSounds.find(id);
	if (i != mSounds.end())
		return i->second;

	return NULL;
}

unsigned int SoundManager::getNumberOfSounds() const
{
	return mSounds.size();
}

void SoundManager::removeSound(Sound *sound)
{
	if (sound == NULL)
		return;

	removeSound(sound->getID());
}

void SoundManager::removeSound(const unsigned int& id)
{
	std::map<unsigned int, Sound*>::iterator i = mSounds.find(id);
	if (i != mSounds.end())
		mSounds.erase(i);

	scene::SceneManager::getInstance().removeNode(id);
}

void SoundManager::removeAllSounds()
{
	std::map<unsigned int, Sound*>::const_iterator i;
	for (i = mSounds.begin(); i != mSounds.end(); ++i)
		scene::SceneManager::getInstance().removeNode(i->second->getID());

	mSounds.clear();
}

Listener* SoundManager::createListener(scene::Node* parent)
{
	Listener* newListener = NULL;
	newListener = new Listener();

	mListeners[newListener->getID()] = newListener;

	scene::SceneManager::getInstance().addNode(newListener, parent);

	return newListener;
}

Listener* SoundManager::createListener(const std::string& name, scene::Node* parent)
{
	Listener* newListener = NULL;
	newListener = new Listener(name);

	mListeners[newListener->getID()] = newListener;

	scene::SceneManager::getInstance().addNode(newListener, parent);

	return newListener;
}

void SoundManager::setActiveListener(Listener* listener)
{
	assert(listener);

	if (!listener)
		return;

	mActiveListener = listener;
}

Listener* SoundManager::getActiveListener()
{
	return mActiveListener;
}

Listener* SoundManager::getListener(const unsigned int& id)
{
	std::map<unsigned int, Listener*>::const_iterator i = mListeners.find(id);
	if (i != mListeners.end())
		return i->second;

	return NULL;
}

unsigned int SoundManager::getNumberOfListeners() const
{
	return mListeners.size();
}

void SoundManager::removeListener(Listener *listener)
{
	if (!listener)
		return;

	removeListener(listener->getID());
}

void SoundManager::removeListener(const unsigned int& id)
{
	std::map<unsigned int, Listener*>::iterator i = mListeners.find(id);
	if (i != mListeners.end())
		mListeners.erase(i);

	scene::SceneManager::getInstance().removeNode(id);
}

void SoundManager::removeAllListeners()
{
	std::map<unsigned int, Listener*>::const_iterator i;
	for (i = mListeners.begin(); i != mListeners.end(); ++i)
		scene::SceneManager::getInstance().removeNode(i->second->getID());

	mListeners.clear();
}

void SoundManager::setDopplerFactor(float dopplerFactor)
{
	// Negative values are invalid
	if(dopplerFactor < 0)
		return;

	mDopplerFactor = dopplerFactor;
	if (mSoundDriver) mSoundDriver->setDopplerFactor(dopplerFactor);
}

float SoundManager::getDopplerFactor() const
{
	return mDopplerFactor;
}

void SoundManager::setSoundSpeed(float soundSpeed)
{
	// Negative values are invalid
	if(soundSpeed < 0)
		return;

	mSoundSpeed = soundSpeed;
	if (mSoundDriver) mSoundDriver->setSoundSpeed(soundSpeed);
}

float SoundManager::getSoundSpeed() const
{
	return mSoundSpeed;
}

void SoundManager::initializeImpl() {}

void SoundManager::uninitializeImpl()
{
	// Remove all Sounds
	removeAllSounds();
}

void SoundManager::startImpl() {}

void SoundManager::stopImpl() {}

void SoundManager::updateImpl(float elapsedTime)
{
	if (mSoundDriver != NULL && mActiveListener != NULL)
	{
		mSoundDriver->updateListener(mActiveListener);
	}
}

void SoundManager::setSystemDriverImpl(core::SystemDriver* systemDriver)
{
	mSoundDriver = static_cast<SoundDriver*>(systemDriver);
}

void SoundManager::removeSystemDriverImpl()
{
	mSoundDriver = NULL;
}

void SoundManager::setSoundFactory(SoundFactory* factory)
{
	mSoundFactory = factory;
}

void SoundManager::removeSoundFactory()
{
	mSoundFactory = NULL;
}

SoundManager& SoundManager::getInstance()
{
	return core::Singleton<SoundManager>::getInstance();
}

SoundManager* SoundManager::getInstancePtr()
{
	return core::Singleton<SoundManager>::getInstancePtr();
}

} // end namespace sound
