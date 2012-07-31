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
#include <game/Scene.h>
#include <game/GameObject.h>
#include <game/Component.h>
#include <game/ComponentDefines.h>
#include <game/GameManager.h>
#include <resource/ResourceManager.h>
#include <SceneSerializer.h>

#include <Poco/AutoPtr.h>
#include <Poco/Path.h>
#include <Poco/Util/XMLConfiguration.h>

#include <string>

namespace resource
{

game::ComponentType convertComponentType(const std::string& param)
{
	if (param == "transform")
		return game::COMPONENT_TYPE_TRANSFORM;
	else if (param == "light")
		return game::COMPONENT_TYPE_LIGHT;
	else if (param == "camera")
		return game::COMPONENT_TYPE_CAMERA;
	else if (param == "model")
		return game::COMPONENT_TYPE_MODEL;
	else if (param == "body")
		return game::COMPONENT_TYPE_BODY;
	else if (param == "joint")
		return game::COMPONENT_TYPE_JOINT;
	else if (param == "sound")
		return game::COMPONENT_TYPE_SOUND;
	else if (param == "listener")
		return game::COMPONENT_TYPE_LISTENER;
	else
	{
		core::Log::getInstance().logMessage("SceneSerializer", "Invalid component type, using default.", core::LOG_LEVEL_ERROR);

		return game::COMPONENT_TYPE_UNDEFINED;
	}
}

SceneSerializer::SceneSerializer()
{
	// Version number
	mVersion = "[SceneSerializer_v1.00]";
}

SceneSerializer::~SceneSerializer() {}

bool SceneSerializer::importResource(Resource* dest, const std::string& filename)
{
	assert(dest != NULL);
	if (dest == NULL)
		return false;

	if (dest->getResourceType() != RESOURCE_TYPE_SCENE)
	{
		core::Log::getInstance().logMessage("SceneSerializer", "Unable to load scene - invalid resource pointer.", core::LOG_LEVEL_ERROR);
		return false;
	}

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

	//game objects
	unsigned int i = 0;
	std::string key = "game_object[" + core::intToString(i) + "]";
	while (pConf->has(key))
	{
		if (pConf->has(key + "[@name]"))
		{
			std::string name = pConf->getString(key + "[@name]");
		}

		//components
		unsigned int j = 0;
		std::string ckey = key + "component[" + core::intToString(j) + "]";
		while (pConf->has(ckey))
		{
			if (pConf->has(ckey + "[@type]"))
			{
				std::string type = pConf->getString(ckey + "[@type]");


			}

			ckey = key + "component[" + core::intToString(++j) + "]";
		}


		key = "game_object[" + core::intToString(++i) + "]";
	}

	//////////////////////////////////////////////////////////////////////////

	return true;
}

bool SceneSerializer::exportResource(Resource* source, const std::string& filename)
{
	return false;
}

}// end namespace resource
