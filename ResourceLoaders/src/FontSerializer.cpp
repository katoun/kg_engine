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

#include <core/Log.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <render/Font.h>
#include <resource/ResourceManager.h>
#include <FontSerializer.h>

#include <Poco/AutoPtr.h>
#include <Poco/Path.h>
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
	Poco::Path path(resource::ResourceManager::getInstance().getDataPath());
	path.append(filename);
	std::string filePath = path.toString();

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
