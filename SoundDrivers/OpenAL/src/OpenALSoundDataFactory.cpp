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

#include <OpenALSoundDataFactory.h>
#include <OpenALSoundData.h>

namespace sound
{

resource::Resource* OpenALSoundDataFactory::createResource(const std::string& filename, resource::Serializer* serializer)
{
	return new OpenALSoundData(filename, serializer);
}

void OpenALSoundDataFactory::destroyResource(resource::Resource* resource)
{
	OpenALSoundData* soundData = static_cast<OpenALSoundData*>(resource);

	assert(soundData != NULL);
	if (soundData != NULL) delete soundData;
}


} // end namespace sound
