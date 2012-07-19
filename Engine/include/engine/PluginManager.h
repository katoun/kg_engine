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

#ifndef _PLUGIN_MANAGER_H_
#define _PLUGIN_MANAGER_H_

#include <core/Config.h>
#include <core/System.h>
#include <core/Singleton.h>

#include <string>
#include <map>

namespace engine
{

class Plugin;

//! Manager for Dynamic-loading Libraries.
//! This manager keeps a track of all the open dynamic-loading
//! libraries, opens them and returns references to already-open
//! libraries.
class ENGINE_PUBLIC_EXPORT PluginManager: public core::System, public core::Singleton<PluginManager>
{
public:

	//! Default constructor.
	PluginManager();

	//! Default destructor.
	~PluginManager();

	void setPluginsFile(const std::string& pluginsFile);

	//! Creates plugins from the plugins configuration file.
	void createPlugins();

	//! Creates a plugin.
	Plugin* createPlugin(const std::string& name, const std::string& filename);

	//! Load all plugins waiting for load.
	void loadPlugins();
	//! Unload all resources.
	void unloadPlugins();

	bool loadPlugin(Plugin* plugin);
	void unloadPlugin(Plugin* plugin);
	bool reloadPlugin(Plugin* plugin);

	//! Remove a Plugin from the managed list, calling it's unload() method.
	void removePlugin(Plugin* plugin);
	void removePlugin(const std::string& name);

	void removeAllPlugins();
	
	static PluginManager& getInstance();
	static PluginManager* getInstancePtr();

protected:

	void initializeImpl();
	void uninitializeImpl();

	std::string mPluginsFile;

	std::list<Plugin*> mLoadPlugins;

	std::map<std::string, Plugin*> mPlugins;
};

} // end namespace engine

#endif