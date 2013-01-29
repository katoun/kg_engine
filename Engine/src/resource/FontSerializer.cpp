/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2013 Catalin Alexandru Nastase

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

#include <resource/FontSerializer.h>
#include <resource/ResourceManager.h>
#include <core/Log.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <render/Font.h>

#include <tinyxml2.h>

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
	assert(dest != nullptr);
	if (dest == nullptr)
		return false;

	if (dest->getResourceType() != RESOURCE_TYPE_FONT)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("FontSerializer", "Unable to load font - invalid resource pointer.", core::LOG_LEVEL_ERROR);
		return false;
	}

	render::Font* font = static_cast<render::Font*>(dest);
	assert(font != nullptr);
	if (font == nullptr)
		return false;

	if (resource::ResourceManager::getInstance() == nullptr)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("FontSerializer", "Unable to load font - resources data path not set.", core::LOG_LEVEL_ERROR);
		return false;
	}

	std::string filePath = resource::ResourceManager::getInstance()->getDataPath() + "/" + filename;

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS)
		return false;

	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("font");
	if (pRoot != nullptr)
	{
		double dvalue = 0.0;
		const char* svalue;
		tinyxml2::XMLElement* pElement = nullptr;
		
		pElement = pRoot->FirstChildElement("material");
		if (pElement != nullptr)
		{
			svalue = pElement->Attribute("value");
			if (svalue != nullptr)
			{
				std::string material = svalue;
				font->setMaterial(material);
			}
		}

		pElement = pRoot->FirstChildElement("char");
		while (pElement != nullptr)
		{
			std::string value;
			float u1 = 0.0f;
			float v1 = 0.0f;
			float u2 = 0.0f;
			float v2 = 0.0f;
			
			svalue = pElement->Attribute("value");
			if (svalue != nullptr)
			{
				value = svalue;
			}

			if (pElement->QueryDoubleAttribute("u1", &dvalue) == tinyxml2::XML_SUCCESS)
			{
				u1 = (float)dvalue;
			}

			if (pElement->QueryDoubleAttribute("v1", &dvalue) == tinyxml2::XML_SUCCESS)
			{
				v1 = (float)dvalue;
			}

			if (pElement->QueryDoubleAttribute("u2", &dvalue) == tinyxml2::XML_SUCCESS)
			{
				u2 = (float)dvalue;
			}

			if (pElement->QueryDoubleAttribute("v2", &dvalue) == tinyxml2::XML_SUCCESS)
			{
				v2 = (float)dvalue;
			}

			font->setCharTexCoords(value[0], u1, v1, u2, v2);

			pElement = pElement->NextSiblingElement("char");
		}
	}

	return true;
}

bool FontSerializer::exportResource(Resource* source, const std::string& filename)
{
	return false;
}

}// end namespace resource
