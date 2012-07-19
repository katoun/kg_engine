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

#ifndef _ENGINE_OPTIONS_H_
#define _ENGINE_OPTIONS_H_

#include <core/Config.h>
#include <core/Singleton.h>

#include <string>

namespace engine
{

class ENGINE_PUBLIC_EXPORT EngineSettings: public core::Singleton<EngineSettings>
{
public:

	EngineSettings();
	~EngineSettings();

	unsigned int getWidth();
	unsigned int getHeight();
	unsigned int getBitdepth();
	bool getFullscreen();
	bool getVSync();
	void* getMainWindowId();

	void setWidth(unsigned int width);
	void setHeight(unsigned int height);
	void setBitdepth(unsigned int bitdepth);
	void setFullscreen(bool fullscreen);
	void setVSync(bool vsync);
	void setMainWindowID(void* windowId);

	//! Method reads a game configuration file and instantiates all options.
	//! \param optionsfile: The file that contains game information.
	void loadOptions(const std::string& optionsfile);

	//! Method saves a game configuration file with all the instantiated options.
	//! \param optionsfile: The file that will contain game information.
	void saveOptions(const std::string& optionsfile);

	static EngineSettings& getInstance();
	static EngineSettings* getInstancePtr();

protected:

	bool mOptionsModified;

	//! Game Options
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mBitdepth;
	bool mFullscreen;
	bool mVSync;

	void* mMainWindowId;
};

} // end namespace engine

#endif