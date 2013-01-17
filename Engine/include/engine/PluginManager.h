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
	Plugin* createPlugin(const std::string& name);

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
	
	static PluginManager* getInstance();

protected:

	void initializeImpl();
	void uninitializeImpl();

	std::string mPluginsFile;

	std::list<Plugin*> mLoadPlugins;

	std::map<std::string, Plugin*> mPlugins;
};

} // end namespace engine

#endif