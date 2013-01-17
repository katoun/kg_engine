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

#include <tinyxml2.h>

#if ENGINE_PLATFORM == PLATFORM_WINDOWS
#include <windows.h>
#endif

template<> engine::EngineSettings* core::Singleton<engine::EngineSettings>::m_Singleton = nullptr;

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
	mDataPath = "";
	mWorkPath = "";
	mMainWindowId = nullptr;

#if ENGINE_PLATFORM == PLATFORM_WINDOWS
	char fullPath[MAX_PATH_SIZE]; 
	GetModuleFileName(NULL, fullPath, MAX_PATH_SIZE);

	mWorkPath = fullPath;

	size_t pos = mWorkPath.find_last_of('\\');
	if (pos != std::string::npos)
	{
		mWorkPath = mWorkPath.substr(0, pos);
	}

	for(int i = 0; i < mWorkPath.length(); i++)
	{
		if (mWorkPath[i] == '\\')
		{
			mWorkPath[i] = '/';
		}
	}
#endif
}

EngineSettings::~EngineSettings() {}

const unsigned int EngineSettings::getWidth()
{
	return mWidth;
}

const unsigned int EngineSettings::getHeight()
{
	return mHeight;
}

const unsigned int EngineSettings::getBitdepth()
{
	return mBitdepth;
}

const bool EngineSettings::getFullscreen()
{
	return mFullscreen;
}

const bool EngineSettings::getVSync()
{
	return mVSync;
}

const std::string& EngineSettings::getDataPath()
{
	return mDataPath;
}

const std::string& EngineSettings::getWorkPath()
{
	return mWorkPath;
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

void EngineSettings::setDataPath(const std::string& dataPath)
{
	mDataPath = dataPath;
	mOptionsModified = true;
}

void EngineSettings::setMainWindowID(void* windowId)
{
	mMainWindowId = windowId;
}

void EngineSettings::loadOptions(const std::string& optionsfile)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(optionsfile.c_str()) != tinyxml2::XML_SUCCESS)
		return;

	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("Engine");
	if (pRoot != nullptr)
	{
		int ivalue = 0;
		bool bvalue = false;
		const char* svalue;
		tinyxml2::XMLElement* pElement = nullptr;

		pElement = pRoot->FirstChildElement("VideoSize");
		if (pElement != nullptr)
		{
			tinyxml2::XMLElement* pSubElement = nullptr;

			pSubElement = pElement->FirstChildElement("Width");
			if (pSubElement != nullptr)
			{
				if (pSubElement->QueryIntAttribute("value", &ivalue) == tinyxml2::XML_SUCCESS)
				{
					mWidth = (unsigned int)ivalue;
				}
			}

			pSubElement = pElement->FirstChildElement("Height");
			if (pSubElement != nullptr)
			{
				if (pSubElement->QueryIntAttribute("value", &ivalue) == tinyxml2::XML_SUCCESS)
				{
					mHeight = (unsigned int)ivalue;
				}
			}
		}

		pElement = pRoot->FirstChildElement("Bitdepth");
		if (pElement != nullptr)
		{
			if (pElement->QueryIntAttribute("value", &ivalue) == tinyxml2::XML_SUCCESS)
			{
				mBitdepth = (unsigned int)ivalue;
			}
		}

		pElement = pRoot->FirstChildElement("Fullscreen");
		if (pElement != nullptr)
		{
			svalue = pElement->Attribute("value");
			if (svalue != nullptr)
			{
				mFullscreen = (std::string(svalue) == "Yes") ? true : false;
			}
		}

		pElement = pRoot->FirstChildElement("VSync");
		if (pElement != nullptr)
		{
			svalue = pElement->Attribute("value");
			if (svalue != nullptr)
			{
				mVSync = (std::string(svalue) == "Yes") ? true : false;
			}
		}

		pElement = pRoot->FirstChildElement("DataPath");
		if (pElement != nullptr)
		{
			svalue = pElement->Attribute("value");
			if (svalue != nullptr)
			{
				mDataPath = svalue;
			}
		}
	}
}

void EngineSettings::saveOptions(const std::string& optionsfile)
{
	if (!mOptionsModified)
		return;

	tinyxml2::XMLDocument doc;

	tinyxml2::XMLElement* pRoot = doc.NewElement("Engine");
	if (pRoot != nullptr)
	{
		doc.InsertEndChild(pRoot);

		tinyxml2::XMLElement* pElement = nullptr;

		pElement = doc.NewElement("VideoSize");
		if (pElement != nullptr)
		{
			pRoot->InsertEndChild(pElement);
			tinyxml2::XMLElement* pSubElement = nullptr;
		
			pSubElement = doc.NewElement("Width");
			if (pSubElement != nullptr)
			{
				pElement->InsertEndChild(pSubElement);

				pSubElement->SetAttribute("value", (int)mWidth);
			}

			pSubElement = doc.NewElement("Height");
			if (pSubElement != nullptr)
			{
				pElement->InsertEndChild(pSubElement);

				pSubElement->SetAttribute("value", (int)mHeight);
			}
		}

		pElement = doc.NewElement("Bitdepth");
		if (pElement != nullptr)
		{
			pRoot->InsertEndChild(pElement);

			pElement->SetAttribute("value", (int)mBitdepth);
		}

		pElement = doc.NewElement("Fullscreen");
		if (pElement != nullptr)
		{
			pRoot->InsertEndChild(pElement);

			pElement->SetAttribute("value", mFullscreen ? "Yes" : "No");
		}

		pElement = doc.NewElement("VSync");
		if (pElement != nullptr)
		{
			pRoot->InsertEndChild(pElement);

			pElement->SetAttribute("value", mVSync ? "Yes" : "No");
		}

		pElement = doc.NewElement("DataPath");
		if (pElement != nullptr)
		{
			pRoot->InsertEndChild(pElement);

			pElement->SetAttribute("value", mDataPath.c_str());
		}
	}

	if (doc.SaveFile(optionsfile.c_str()) != tinyxml2::XML_SUCCESS)
		return;
}

EngineSettings* EngineSettings::getInstance()
{
	return core::Singleton<EngineSettings>::getInstance();
}

} // end namespace engine
