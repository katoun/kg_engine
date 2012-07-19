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

#include <OpenALConfig.h>
#include <core/SystemDriver.h>
#include <sound/SoundManager.h>
#include <resource/ResourceDefines.h>
#include <resource/ResourceFactory.h>
#include <resource/ResourceManager.h>
#include <OpenALSoundDriver.h>
#include <OpenALSoundFactory.h>
#include <OpenALSoundDataFactory.h>

namespace sound
{

SoundDriver* openalSoundDriver = NULL;
SoundFactory* openalSoundFactory = NULL;
resource::ResourceFactory* openalSoundDataFactory = NULL;

extern "C" void OPENAL_PUBLIC_EXPORT loadPlugin() throw()
{	
	openalSoundDriver = OpenALSoundDriver::getInstancePtr();
	openalSoundFactory = new OpenALSoundFactory();
	SoundManager::getInstance().setSystemDriver((core::SystemDriver*)openalSoundDriver);
	SoundManager::getInstance().setSoundFactory(openalSoundFactory);

	openalSoundDataFactory = new OpenALSoundDataFactory();
	resource::ResourceManager::getInstance().registerResourceFactory(resource::RESOURCE_TYPE_SOUND_DATA, openalSoundDataFactory);
}

extern "C" void OPENAL_PUBLIC_EXPORT unloadPlugin()
{
	SoundManager::getInstance().removeSystemDriver();
	SoundManager::getInstance().removeSoundFactory();

	resource::ResourceManager::getInstance().removeResourceFactory(resource::RESOURCE_TYPE_SOUND_DATA);

	if (openalSoundFactory != NULL)
		delete openalSoundFactory;
	if (openalSoundDataFactory != NULL)
		delete openalSoundDataFactory;
}

} // end namespace sound
