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

#include <core/Log.h>
#include <core/Utils.h>
#include <engine/PluginManager.h>
#include <engine/Plugin.h>

#include <tinyxml2.h>

template<> engine::PluginManager* core::Singleton<engine::PluginManager>::m_Singleton = nullptr;

namespace engine
{

PluginManager::PluginManager(): core::System("PluginManager")
{
	mPluginsFile = "Plugins.xml";
}

PluginManager::~PluginManager()
{
	// Remove all Plugins
	removeAllPlugins();
}

void PluginManager::setPluginsFile(const std::string& pluginsFile)
{
	mPluginsFile = pluginsFile;
}

void PluginManager::createPlugins()
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(mPluginsFile.c_str()) != tinyxml2::XML_SUCCESS)
		return;
	
	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("Plugins");
	if (pRoot != nullptr)
	{
		const char* value;
		tinyxml2::XMLElement* pElement = nullptr;
		pElement = pRoot->FirstChildElement("Plugin");
		while (pElement != nullptr)
		{
			value = pElement->GetText();
			if (value != nullptr)
			{
				std::string pluginName = value;

				//Remove extension
				size_t pos = pluginName.find_last_of('.');
				if (pos != std::string::npos)
				{
					pluginName = pluginName.substr(0, pos);
				}

				Plugin* plugin = createPlugin(pluginName);
			}

			pElement = pElement->NextSiblingElement("Plugin");
		}
	}
}

Plugin* PluginManager::createPlugin(const std::string& name)
{
	if (name.empty())
		return nullptr;
	
	std::map<std::string, Plugin*>::iterator i = mPlugins.find(name);
	if (i != mPlugins.end())
	{
		return i->second;
	}
	else
	{
		Plugin* newPlugin = new Plugin(name);

		mPlugins[name] = newPlugin;
		mLoadPlugins.push_back(newPlugin);
		
		std::string message = "Plugin: " + newPlugin->getName() + " created.";
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("PluginManager", message);

		return newPlugin;
	}

	return nullptr;
}

void PluginManager::loadPlugins()
{
	std::list<Plugin*>::iterator i;
	for (i = mLoadPlugins.begin(); i != mLoadPlugins.end(); ++i)
		loadPlugin(*i);

	mLoadPlugins.clear();
}

void PluginManager::unloadPlugins()
{
	std::map<std::string, Plugin*>::iterator i;
	for (i = mPlugins.begin(); i != mPlugins.end(); ++i)
		unloadPlugin(i->second);
}

bool PluginManager::loadPlugin(Plugin* plugin)
{
	assert(plugin != nullptr);
	if (plugin == nullptr)
		return false;

	if (!plugin->load())
		return false;

	std::string message = "Plugin: " + plugin->getName() + " loaded.";
	if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("PluginManager", message);

	return true;
}

void PluginManager::unloadPlugin(Plugin* plugin)
{
	assert(plugin != nullptr);
	if (plugin == nullptr)
		return;

	plugin->unload();

	std::string message = "Plugin: " + plugin->getName() + " unloaded.";
	if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("PluginManager", message);
}

bool PluginManager::reloadPlugin(Plugin* plugin)
{
	assert(plugin != nullptr);
	if (plugin == nullptr)
		return false;

	if (!plugin->reload())
		return false;

	std::string message = "Plugin: " + plugin->getName() + " reloaded.";
	if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("PluginManager", message);

	return true;
}

void PluginManager::removePlugin(Plugin* plugin)
{
	if (plugin == nullptr)
		return;

	removePlugin(plugin->getName());
}

void PluginManager::removePlugin(const std::string& name)
{
	std::map<std::string, Plugin*>::iterator i = mPlugins.find(name);
	if (i != mPlugins.end())
	{
		Plugin* plugin = i->second;

		assert(plugin != nullptr);
		if (plugin == nullptr)
			return;

		mPlugins.erase(i);

		unloadPlugin(plugin);
		SAFE_DELETE(plugin);
	}
}

void PluginManager::removeAllPlugins()
{
	std::map<std::string, Plugin*>::iterator i;
	for (i = mPlugins.begin(); i != mPlugins.end(); ++i)
	{
		SAFE_DELETE(i->second);
	}

	mPlugins.clear();
}

void PluginManager::initializeImpl()
{
	createPlugins();
	loadPlugins();
}

void PluginManager::uninitializeImpl()
{
	unloadPlugins();
}

PluginManager* PluginManager::getInstance()
{
	return core::Singleton<PluginManager>::getInstance();
}

} // end namespace engine
