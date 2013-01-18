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

#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include <core/Config.h>

#include <string>

#if ENGINE_PLATFORM == PLATFORM_WINDOWS
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryEx( a, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#elif ENGINE_PLATFORM == PLATFORM_LINUX
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )

#elif ENGINE_PLATFORM == PLATFORM_APPLE
#    define DYNLIB_HANDLE CFBundleRef
#    define DYNLIB_LOAD( a ) mac_loadExeBundle( a )
#    define DYNLIB_GETSYM( a, b ) mac_getBundleSym( a, b )
#    define DYNLIB_UNLOAD( a ) mac_unloadExeBundle( a )
#endif

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
	Plugin(const std::string& name);

	//! Default destructor.
	~Plugin();

	//! Get the name of the plugin.
	const std::string& getName() const;

	//! Load the plugin.
	bool load();
	//! Unload the plugin.
	void unload();
	//! Reload the plugin.
	bool reload();

	bool isLoaded() const;

	//! Returns the address of the given symbol from the loaded library.
	//! \param strName: The name of the symbol to search for
	//! \return If the function succeeds, the returned value is a handle to
	//! the symbol.
	//! If the function fails, the returned value is <b>NULL</b>.
	void* getSymbol(const std::string& name) const throw();
	
protected:

	std::string mName;

	std::string mFileName;
	
	//! Gets the last loading error
	std::string dynlibError();

	// Handle to the loaded library.
	DYNLIB_HANDLE m_hInst;
};

} // end namespace engine

#endif