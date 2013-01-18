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
#include <core/LogDefines.h>
#include <core/System.h>
#include <engine/Plugin.h>
#include <engine/EngineSettings.h>

#include <Poco/SharedLibrary.h>

#if ENGINE_PLATFORM == PLATFORM_WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#elif GAME_PLATFORM == PLATFORM_APPLE
#	include "macPlugins.h"
#endif

namespace engine
{

Plugin::Plugin(const std::string& name)
{
	mName = name;

	mFileName = name;

#if ENGINE_PLATFORM == PLATFORM_LINUX
	// dlopen() does not add .so to the filename, like windows does for .dll
#if defined(_DEBUG)
	mFileName += "d.so";
#else
	mFileName += ".so";
#endif
#elif ENGINE_PLATFORM == PLATFORM_APPLE
	// dlopen() does not add .dylib to the filename, like windows does for .dll
#if defined(_DEBUG)
	mFileName += "d.dylib";
#else
	mFileName += ".dylib";
#endif
#elif ENGINE_PLATFORM == PLATFORM_WINDOWS
	// Although LoadLibraryEx will add .dll itself when you only specify the library name,
	// if you include a relative path then it does not. So, add it to be sure.
#if defined(_DEBUG)
	mFileName += "d.dll";
#else
	mFileName += ".dll";
#endif	
#endif

	if (EngineSettings::getInstance() != nullptr) mFileName = EngineSettings::getInstance()->getWorkPath() + "/" + mFileName;

	m_hInst = nullptr;
}

Plugin::~Plugin() {}

const std::string& Plugin::getName() const
{
	return mName;
}

bool Plugin::load()
{
	m_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD(mFileName.c_str());

	if (!m_hInst)
	{
		DWORD err = GetLastError();

		std::string message = mName;
		message += " could not load.";

		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("Plugin", message, core::LOG_LEVEL_ERROR);

		return false;
	}

	// Call startup	
	DLL_LOAD_PLUGIN pLoadFunc = (DLL_LOAD_PLUGIN)getSymbol("loadPlugin");

	if (pLoadFunc != nullptr) pLoadFunc();

	return true;
}

void Plugin::unload()
{
	// Call shutdown
	DLL_UNLOAD_PLUGIN pUnloadFunc = (DLL_UNLOAD_PLUGIN)getSymbol("unloadPlugin");

	if (pUnloadFunc != nullptr) pUnloadFunc();
	
	if (DYNLIB_UNLOAD(m_hInst))
	{
		std::string message = mName;
		message += " could not unload.";

		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("Plugin", message, core::LOG_LEVEL_ERROR);

		return;
	}
}

bool Plugin::reload()
{
	unload();
	return load();
}

bool Plugin::isLoaded() const
{
	return false;
}

void* Plugin::getSymbol(const std::string& strName) const throw()
{
	return (void*)DYNLIB_GETSYM(m_hInst, strName.c_str());
}

std::string Plugin::dynlibError()
{
#if GAME_PLATFORM == PLATFORM_WINDOWS
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
	);
	std::string ret = (c8*)lpMsgBuf;
	// Free the buffer.
	LocalFree(lpMsgBuf);
	return ret;
#elif GAME_PLATFORM == PLATFORM_LINUX
	return std::string(dlerror());
#elif GAME_PLATFORM == PLATFORM_APPLE
	return std::string(mac_errorBundle());
#else
	return std::string("");
#endif
}

} // end namespace engine
