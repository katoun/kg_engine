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
