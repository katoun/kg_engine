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

#include <sound/SoundManager.h>
#include <sound/Sound.h>
#include <sound/SoundData.h>
#include <sound/Listener.h>
#include <sound/ListenerFactory.h>
#include <sound/SoundFactory.h>
#include <sound/SoundDriver.h>
#include <scene/SceneManager.h>
#include <game/GameObject.h>
#include <game/ComponentDefines.h>
#include <game/ComponentFactory.h>
#include <game/GameManager.h>

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

	mDefaultListenerFactory = new ListenerFactory();
}

SoundManager::~SoundManager()
{
	if (mDefaultListenerFactory != NULL)
	{
		delete mDefaultListenerFactory;
	}
}

Sound* SoundManager::createSound(const std::string& filename, scene::Node* parent)
{
	if (mSoundFactory == NULL)
		return NULL;

	SoundData* newSoundData = static_cast<SoundData*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_SOUND_DATA, filename));
	if (!newSoundData)
		return NULL;

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
	if (mSoundFactory == NULL)
		return NULL;

	SoundData* newSoundData = static_cast<SoundData*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_SOUND_DATA, filename));
	if (newSoundData = NULL)
		return NULL;

	Sound* newSound = NULL;
	newSound = mSoundFactory->createSound(name, newSoundData);
	if (newSound == NULL)
		return NULL;

	mSounds[newSound->getID()] = newSound;

	scene::SceneManager::getInstance().addNode(newSound, parent);

	return newSound;
}

Sound* SoundManager::createSound(SoundData* soundData, scene::Node* parent)
{
	if (mSoundFactory == NULL)
		return NULL;

	if (soundData == NULL)
		return NULL;

	Sound* newSound = NULL;
	newSound = mSoundFactory->createSound(soundData);
	if (newSound == NULL)
		return NULL;

	mSounds[newSound->getID()] = newSound;

	scene::SceneManager::getInstance().addNode(newSound, parent);

	return newSound;
}

Sound* SoundManager::createSound(const std::string& name, SoundData* soundData, scene::Node* parent)
{
	if (mSoundFactory == NULL)
		return NULL;

	if (soundData == NULL)
		return NULL;

	Sound* newSound = NULL;
	newSound = mSoundFactory->createSound(name, soundData);
	if (newSound == NULL)
		return NULL;

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

void SoundManager::setActiveListener(Listener* listener)
{
	if (listener = NULL)
		return;

	mActiveListener = listener;
}

Listener* SoundManager::getActiveListener()
{
	return mActiveListener;
}

void SoundManager::addListener(Listener *listener)
{
	if (listener == NULL)
		return;

	mListeners[listener->getID()] = listener;
}

void SoundManager::removeListener(Listener *listener)
{
	if (listener == NULL)
		return;

	removeListener(listener->getID());
}

void SoundManager::removeListener(const unsigned int& id)
{
	std::map<unsigned int, Listener*>::iterator i = mListeners.find(id);
	if (i != mListeners.end())
		mListeners.erase(i);
}

void SoundManager::removeAllListeners()
{
	mListeners.clear();
}

void SoundManager::setDopplerFactor(float dopplerFactor)
{
	// Negative values are invalid
	if(dopplerFactor < 0)
		return;

	mDopplerFactor = dopplerFactor;
	if (mSoundDriver != NULL)
		mSoundDriver->setDopplerFactor(dopplerFactor);
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
	if (mSoundDriver != NULL)
		mSoundDriver->setSoundSpeed(soundSpeed);
}

float SoundManager::getSoundSpeed() const
{
	return mSoundSpeed;
}

void SoundManager::setSoundFactory(SoundFactory* factory)
{
	mSoundFactory = factory;
}

void SoundManager::removeSoundFactory()
{
	mSoundFactory = NULL;
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

void SoundManager::registerDefaultFactoriesImpl()
{
	game::GameManager::getInstance().registerComponentFactory(game::COMPONENT_TYPE_LISTENER, mDefaultListenerFactory);
}

void SoundManager::removeDefaultFactoriesImpl()
{
	game::GameManager::getInstance().removeComponentFactory(game::COMPONENT_TYPE_LISTENER);
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
