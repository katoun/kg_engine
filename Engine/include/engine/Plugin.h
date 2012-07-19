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

#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include <core/Config.h>

#include <Poco/SharedLibrary.h>

#include <string>

namespace engine
{

typedef void (*DLL_LOAD_PLUGIN)();
typedef void (*DLL_UNLOAD_PLUGIN)();

//! Resource holding data about a plugin(dynamic library).
//! This class holds the data required to get symbols from
//! libraries loaded at run-time.
class ENGINE_PUBLIC_EXPORT Plugin
{
public:
	//! Default constructor.
	Plugin(const std::string& name, const std::string& filename);

	//! Default destructor.
	~Plugin();

	//! Get the name of the plugin.
	const std::string& getName() const;

	//! Get the file name of the plugin.
	const std::string& getFilename() const;

	//! Load the plugin.
	bool load();
	//! Unload the plugin.
	void unload();
	//! Reload the plugin.
	bool reload();
	
protected:

	std::string mName;

	std::string mFileName;
	
	//! Gets the last loading error
	std::string dynlibError();

	Poco::SharedLibrary mSharedLibrary;
};

} // end namespace engine

#endif