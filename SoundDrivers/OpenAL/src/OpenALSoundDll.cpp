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

SoundDriver* openalSoundDriver = nullptr;
SoundFactory* openalSoundFactory = nullptr;
resource::ResourceFactory* openalSoundDataFactory = nullptr;

extern "C" void OPENAL_PUBLIC_EXPORT loadPlugin() throw()
{	
	openalSoundDriver = new OpenALSoundDriver();
	openalSoundFactory = new OpenALSoundFactory();
	if (SoundManager::getInstance() != nullptr)
	{
		SoundManager::getInstance()->setSystemDriver((core::SystemDriver*)openalSoundDriver);
		SoundManager::getInstance()->setDefaultSoundFactory(openalSoundFactory);
	}

	openalSoundDataFactory = new OpenALSoundDataFactory();
	if (resource::ResourceManager::getInstance() != nullptr)
		resource::ResourceManager::getInstance()->registerResourceFactory(resource::RESOURCE_TYPE_SOUND_DATA, openalSoundDataFactory);
}

extern "C" void OPENAL_PUBLIC_EXPORT unloadPlugin()
{
	if (SoundManager::getInstance() != nullptr)
	{
		SoundManager::getInstance()->removeSystemDriver();
		SoundManager::getInstance()->removeDefaultSoundFactory();
	}

	if (resource::ResourceManager::getInstance() != nullptr)
		resource::ResourceManager::getInstance()->removeResourceFactory(resource::RESOURCE_TYPE_SOUND_DATA);

	SAFE_DELETE(openalSoundFactory);
	SAFE_DELETE(openalSoundDataFactory);

	SAFE_DELETE(openalSoundDriver);
}

} // end namespace sound
