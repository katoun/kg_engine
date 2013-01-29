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

#ifndef _ENGINE_OPTIONS_H_
#define _ENGINE_OPTIONS_H_

#include <EngineConfig.h>
#include <core/Singleton.h>

#include <string>

namespace engine
{

class ENGINE_PUBLIC_EXPORT EngineSettings: public core::Singleton<EngineSettings>
{
public:

	EngineSettings();
	~EngineSettings();

	const unsigned int getWidth();
	const unsigned int getHeight();
	const unsigned int getBitdepth();
	const bool getFullscreen();
	const bool getVSync();
	const std::string& getDataPath();
	const std::string& getWorkPath();
	void* getMainWindowId();

	void setWidth(unsigned int width);
	void setHeight(unsigned int height);
	void setBitdepth(unsigned int bitdepth);
	void setFullscreen(bool fullscreen);
	void setVSync(bool vsync);
	void setDataPath(const std::string& dataPath);
	void setMainWindowID(void* windowId);

	//! Method reads a game configuration file and instantiates all options.
	//! \param optionsfile: The file that contains game information.
	void loadOptions(const std::string& optionsfile);

	//! Method saves a game configuration file with all the instantiated options.
	//! \param optionsfile: The file that will contain game information.
	void saveOptions(const std::string& optionsfile);

	static EngineSettings* getInstance();

protected:

	bool mOptionsModified;

	//! Game Options
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mBitdepth;
	bool mFullscreen;
	bool mVSync;
	std::string mDataPath;
	std::string mWorkPath;

	void* mMainWindowId;
};

} // end namespace engine

#endif