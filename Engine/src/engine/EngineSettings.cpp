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

#include <engine/EngineSettings.h>
#include <core/Utils.h>

#include <Poco/XML/XML.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Text.h>
#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>

template<> engine::EngineSettings& core::Singleton<engine::EngineSettings>::ms_Singleton = engine::EngineSettings();

namespace engine
{

EngineSettings::EngineSettings()
{
	mOptionsModified = false;

	//init default options
	mWidth = 800;
	mHeight = 600;
	mBitdepth = 16;
	mFullscreen = false;
	mVSync = false;
	mMainWindowId = NULL;
}

EngineSettings::~EngineSettings() {}

unsigned int EngineSettings::getWidth()
{
	return mWidth;
}

unsigned int EngineSettings::getHeight()
{
	return mHeight;
}

unsigned int EngineSettings::getBitdepth()
{
	return mBitdepth;
}

bool EngineSettings::getFullscreen()
{
	return mFullscreen;
}

bool EngineSettings::getVSync()
{
	return mVSync;
}

void* EngineSettings::getMainWindowId()
{
	return mMainWindowId;
}

void EngineSettings::setWidth(unsigned int width)
{
	mWidth = width;
	mOptionsModified = true;
}

void EngineSettings::setHeight(unsigned int height)
{
	mHeight = height;
	mOptionsModified = true;
}

void EngineSettings::setBitdepth(unsigned int bitdepth)
{
	mBitdepth = bitdepth;
	mOptionsModified = true;
}

void EngineSettings::setFullscreen(bool fullscreen)
{
	mFullscreen = fullscreen;
	mOptionsModified = true;
}

void EngineSettings::setVSync(bool vsync)
{
	mVSync = vsync;
	mOptionsModified = true;
}

void EngineSettings::setMainWindowID(void* windowId)
{
	mMainWindowId = windowId;
}

void EngineSettings::loadOptions(const std::string& optionsfile)
{
	Poco::AutoPtr<Poco::Util::XMLConfiguration> pConf(new Poco::Util::XMLConfiguration());
	try
	{
		pConf->load(optionsfile);
	}
	catch(...)
	{
		mOptionsModified = true;
		saveOptions(optionsfile);
		return;
	}

	if (pConf->has("VideoSize.Width"))
		mWidth = pConf->getInt("VideoSize.Width");

	if (pConf->has("VideoSize.Height"))
		mHeight = pConf->getInt("VideoSize.Height");

	if (pConf->has("Bitdepth"))
		mBitdepth = pConf->getInt("Bitdepth");

	if (pConf->has("Fullscreen"))
		mFullscreen = pConf->getBool("Fullscreen");

	if (pConf->has("VSync"))
		mVSync = pConf->getBool("VSync");
}

void EngineSettings::saveOptions(const std::string& optionsfile)
{
	if (!mOptionsModified)
		return;

	Poco::AutoPtr<Poco::XML::Document> pDoc = new Poco::XML::Document;
	
	Poco::AutoPtr<Poco::XML::Element> pRoot = pDoc->createElement("Engine");
	pDoc->appendChild(pRoot);

	//VideoSize
	Poco::AutoPtr<Poco::XML::Element> pVideoSize = pDoc->createElement("VideoSize");
	
	Poco::AutoPtr<Poco::XML::Element> pWidth = pDoc->createElement("Width");

	Poco::AutoPtr<Poco::XML::Text> pWidthText = pDoc->createTextNode(core::intToString(mWidth));
	pWidth->appendChild(pWidthText);

	pVideoSize->appendChild(pWidth);

	Poco::AutoPtr<Poco::XML::Element> pHeight = pDoc->createElement("Height");

	Poco::AutoPtr<Poco::XML::Text> pHeightText = pDoc->createTextNode(core::intToString(mHeight));
	pHeight->appendChild(pHeightText);

	pVideoSize->appendChild(pHeight);

	pRoot->appendChild(pVideoSize);
	//VideoSize

	//pBitdepth
	Poco::AutoPtr<Poco::XML::Element> pBitdepth = pDoc->createElement("Bitdepth");
	
	Poco::AutoPtr<Poco::XML::Text> pBitdepthText = pDoc->createTextNode(core::intToString(mBitdepth));
	pBitdepth->appendChild(pBitdepthText);
	
	pRoot->appendChild(pBitdepth);
	//pBitdepth

	//Fullscreen
	Poco::AutoPtr<Poco::XML::Element> pFullscreen = pDoc->createElement("Fullscreen");
	
	Poco::AutoPtr<Poco::XML::Text> pFullscreenText = pDoc->createTextNode(mFullscreen ? "Yes" : "No");
	pFullscreen->appendChild(pFullscreenText);
	
	pRoot->appendChild(pFullscreen);
	//Fullscreen

	//VSync
	Poco::AutoPtr<Poco::XML::Element> pVSync = pDoc->createElement("VSync");
	
	Poco::AutoPtr<Poco::XML::Text> pVSyncText = pDoc->createTextNode(mVSync ? "Yes" : "No");
	pVSync->appendChild(pVSyncText);
	
	pRoot->appendChild(pVSync);
	//VSync

	Poco::AutoPtr<Poco::Util::XMLConfiguration> pConf(new Poco::Util::XMLConfiguration(pDoc));

	pConf->save(optionsfile);
}

EngineSettings& EngineSettings::getInstance()
{
	return core::Singleton<EngineSettings>::getInstance();
}

EngineSettings* EngineSettings::getInstancePtr()
{
	return core::Singleton<EngineSettings>::getInstancePtr();
}

} // end namespace engine
