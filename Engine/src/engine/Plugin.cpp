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
#include <core/LogDefines.h>
#include <core/System.h>
#include <engine/Plugin.h>



namespace engine
{

Plugin::Plugin(const std::string& name, const std::string& filename)
{
	mName = name;

	mFileName = filename;
}

Plugin::~Plugin() {}

const std::string& Plugin::getName() const
{
	return mName;
}

const std::string& Plugin::getFilename() const
{
	return mFileName;
}

bool Plugin::load()
{	
	std::string filename = mFileName;
	filename.append(Poco::SharedLibrary::suffix());

	mSharedLibrary.load(filename);

	//if (!m_hInst)
	if (!mSharedLibrary.isLoaded())
	{
		DWORD err = GetLastError();
		
		std::string message = mName;
		message += " could not load.";

		core::Log::getInstance().logMessage("Plugin", message, core::LOG_LEVEL_ERROR);

		return false;
	}

	// Call startup	
	DLL_LOAD_PLUGIN pLoadFunc = NULL;
	if (mSharedLibrary.hasSymbol("loadPlugin"))
		pLoadFunc = (DLL_LOAD_PLUGIN)mSharedLibrary.getSymbol("loadPlugin");

	if (pLoadFunc != NULL)
		pLoadFunc();

	return true;
}

void Plugin::unload()
{
	// Call shutdown
	DLL_UNLOAD_PLUGIN pUnloadFunc = NULL;
	if (mSharedLibrary.hasSymbol("unloadPlugin"))
		pUnloadFunc = (DLL_UNLOAD_PLUGIN)mSharedLibrary.getSymbol("unloadPlugin");

	if (pUnloadFunc != NULL)
		pUnloadFunc();
	
	mSharedLibrary.unload();

	if (mSharedLibrary.isLoaded())
	{
		std::string message = mName;
		message += " could not unload.";

		core::Log::getInstance().logMessage("Plugin", message, core::LOG_LEVEL_ERROR);

		return;
	}
}

bool Plugin::reload()
{
	unload();
	return load();
}

} // end namespace engine
