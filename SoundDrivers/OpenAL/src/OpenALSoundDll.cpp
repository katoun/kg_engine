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
