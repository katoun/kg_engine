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

template<> core::Log* core::Singleton<core::Log>::m_Singleton = nullptr;

namespace core
{

Log::Log()
{
	mLogStream.open("engine.log");
}

Log::~Log()
{
	mLogStream.close();
}

void Log::logMessage(const std::string& source, const std::string& text, LogLevel logLevel)
{
	mLogStream.fill('-');
	mLogStream.width(20);
#ifdef _DEBUG
	std::cerr.fill('-');
	std::cerr.width(20);
#endif

	mLogStream<<std::left<<source;
#ifdef _DEBUG
	std::cerr<<std::left<<source;
#endif

	mLogStream<<">";
#ifdef _DEBUG
	std::cerr<<">";
#endif

	switch(logLevel)
	{
	case LOG_LEVEL_INFORMATION:
		mLogStream<<" "<<text<<std::endl;
#ifdef _DEBUG
		std::cerr<<" "<<text<<std::endl;
#endif
		break;
	case LOG_LEVEL_WARNING:
		mLogStream<<" (w)"<<text<<std::endl;
#ifdef _DEBUG
		std::cerr<<" (w) "<<text<<std::endl;
#endif
		break;
	case LOG_LEVEL_ERROR:
		mLogStream<<" (!)"<<text<<std::endl;
#ifdef _DEBUG
		std::cerr<<" (!) "<<text<<std::endl;
#endif
		break;
	}

	// Flush stcmdream to ensure it is written (incase of a crash, we need log to be up to date)
	mLogStream.flush();
}

Log* Log::getInstance()
{
	return core::Singleton<Log>::getInstance();
}

} // end namespace core
