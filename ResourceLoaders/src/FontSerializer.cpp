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

#include <core/Log.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <render/Font.h>
#include <resource/ResourceManager.h>
#include <FontSerializer.h>

#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>

#include <string>

namespace resource
{
FontSerializer::FontSerializer()
{
	// Version number
	mVersion = "[FontSerializer_v1.00]";
}

FontSerializer::~FontSerializer() {}

bool FontSerializer::importResource(Resource* dest, const std::string& filename)
{
	assert(dest != NULL);
	if (dest == NULL)
		return false;

	if (dest->getResourceType() != RESOURCE_TYPE_FONT)
	{
		core::Log::getInstance().logMessage("FontSerializer", "Unable to load font - invalid resource pointer.", core::LOG_LEVEL_ERROR);
		return false;
	}

	render::Font* font = static_cast<render::Font*>(dest);
	assert(font != NULL);
	if (font == NULL)
		return false;

	//////////////////////////////////////////////////////////////////////////
	std::string filePath = ResourceManager::getInstance().getPath(filename);
	Poco::AutoPtr<Poco::Util::XMLConfiguration> pConf(new Poco::Util::XMLConfiguration());
	try
	{
		pConf->load(filePath);
	}
	catch(...)
	{
		return false;
	}

	//material
	if (pConf->has("material[@value]"))
	{
		std::string material = pConf->getString("material[@value]");
		font->setMaterial(material);
	}

	//chars
	unsigned int i = 0;
	std::string key = "char[" + core::intToString(i) + "]";
	while (pConf->has(key))
	{
		std::string valuekey = key + "[@value]";
		std::string u1key = key + "[@u1]";
		std::string v1key = key + "[@v1]";
		std::string u2key = key + "[@u2]";
		std::string v2key = key + "[@v2]";

		std::string value;
		float u1 = 0.0f;
		float v1 = 0.0f;
		float u2 = 0.0f;
		float v2 = 0.0f;

		if (pConf->has(valuekey))
			value = pConf->getString(valuekey);
		if (pConf->has(u1key))
			u1 = (float)pConf->getDouble(u1key);
		if (pConf->has(v1key))
			v1 = (float)pConf->getDouble(v1key);
		if (pConf->has(u2key))
			u2 = (float)pConf->getDouble(u2key);
		if (pConf->has(v2key))
			v2 = (float)pConf->getDouble(v2key);

		font->setCharTexCoords(value[0], u1, v1, u2, v2);

		key = "char[" + core::intToString(++i) + "]";
	}

	return true;
}

bool FontSerializer::exportResource(Resource* source, const std::string& filename)
{
	return false;
}

}// end namespace resource
