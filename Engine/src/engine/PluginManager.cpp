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
#include <engine/PluginManager.h>
#include <engine/Plugin.h>

#include <Poco/Path.h>
#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>

template<> engine::PluginManager& core::Singleton<engine::PluginManager>::ms_Singleton = engine::PluginManager();

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
	Poco::AutoPtr<Poco::Util::XMLConfiguration> pConf(new Poco::Util::XMLConfiguration());
	try
	{
		pConf->load(mPluginsFile);
	}
	catch(...)
	{
		return;
	}

	Poco::Path dirPath;
	if (pConf->has("PluginFolder"))
	{
		std::string pluginDir = pConf->getString("PluginFolder");
		dirPath = Poco::Path(pluginDir);
	}

	int index = 0;
	std::string sProperty = "Plugin[" + core::intToString(index) + "]";
	
	while(pConf->has(sProperty))
	{
		std::string pluginName = pConf->getString(sProperty);
		std::string pluginFileName = Poco::Path(dirPath, pluginName).toString();

		Plugin* plugin = createPlugin(pluginName, pluginFileName);

		index++;
		sProperty = "Plugin[" + core::intToString(index) + "]";
	}
}

Plugin* PluginManager::createPlugin(const std::string& name, const std::string& filename)
{
	if(name == core::STRING_BLANK || filename == core::STRING_BLANK)
		return NULL;
	
	std::map<std::string, Plugin*>::iterator i = mPlugins.find(name);
	if (i != mPlugins.end())
	{
		Plugin* pPlugin = i->second;
		if (pPlugin != NULL && pPlugin->getFilename() == filename)
			return pPlugin;
	}
	else
	{
		Plugin* newPlugin = new Plugin(name, filename);

		mPlugins[name] = newPlugin;
		mLoadPlugins.push_back(newPlugin);
		
		std::string message = "Plugin: " + newPlugin->getName() + " filename: " + filename + " created.";
		core::Log::getInstance().logMessage("PluginManager", message);

		return newPlugin;
	}

	return NULL;
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
	assert(plugin != NULL);
	if (plugin == NULL)
		return false;

	if (!plugin->load())
		return false;

	std::string message = "Plugin: " + plugin->getName() + " filename: " + plugin->getFilename() + " loaded.";
	core::Log::getInstance().logMessage("PluginManager", message);

	return true;
}

void PluginManager::unloadPlugin(Plugin* plugin)
{
	assert(plugin != NULL);
	if (plugin == NULL)
		return;

	plugin->unload();

	std::string message = "Plugin: " + plugin->getName() + " filename: " + plugin->getFilename() + " unloaded.";
	core::Log::getInstance().logMessage("PluginManager", message);
}

bool PluginManager::reloadPlugin(Plugin* plugin)
{
	assert(plugin != NULL);
	if (plugin == NULL)
		return false;

	if (!plugin->reload())
		return false;

	std::string message = "Plugin: " + plugin->getName() + " filename: " + plugin->getFilename() + " reloaded.";
	core::Log::getInstance().logMessage("PluginManager", message);

	return true;
}

void PluginManager::removePlugin(Plugin* plugin)
{
	if (plugin == NULL)
		return;

	removePlugin(plugin->getName());
}

void PluginManager::removePlugin(const std::string& name)
{
	std::map<std::string, Plugin*>::iterator i = mPlugins.find(name);
	if (i != mPlugins.end())
	{
		Plugin* plugin = i->second;

		assert(plugin != NULL);
		if (plugin == NULL)
			return;

		mPlugins.erase(i);

		unloadPlugin(plugin);
		delete plugin;
	}
}

void PluginManager::removeAllPlugins()
{
	std::map<std::string, Plugin*>::iterator i;
	for (i = mPlugins.begin(); i != mPlugins.end(); ++i)
	{
		delete i->second;
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

PluginManager& PluginManager::getInstance()
{
	return core::Singleton<PluginManager>::getInstance();
}

PluginManager* PluginManager::getInstancePtr()
{
	return core::Singleton<PluginManager>::getInstancePtr();
}

} // end namespace engine
