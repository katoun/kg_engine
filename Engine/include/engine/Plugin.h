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