/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

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

#include <Poco/AutoPtr.h>
#include <Poco/File.h>
#include <Poco/SplitterChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/Logger.h>
#include <Poco/Message.h>

#include <string>

template<> core::Log& core::Singleton<core::Log>::ms_Singleton = core::Log();

namespace core
{

Poco::AutoPtr<Poco::SplitterChannel> pSplitterChannel;
Poco::AutoPtr<Poco::PatternFormatter> pPatternFormater;
Poco::AutoPtr<Poco::FileChannel> pFileChannel;
Poco::AutoPtr<Poco::FormattingChannel> pFileFormattingChannel;
Poco::AutoPtr<Poco::ConsoleChannel> pConsoleChannel;
Poco::AutoPtr<Poco::FormattingChannel> pConsoleFormattingChannel;

Log::Log(): mDefaultLogger(Poco::Logger::root())
{
	pSplitterChannel = new Poco::SplitterChannel();
	pPatternFormater = new Poco::PatternFormatter();

	pFileChannel = new Poco::FileChannel();

	pFileFormattingChannel = new Poco::FormattingChannel(pPatternFormater, pFileChannel);

	pConsoleChannel = new Poco::ConsoleChannel();
	pConsoleFormattingChannel = new Poco::FormattingChannel(pPatternFormater, pConsoleChannel);

	if (!pPatternFormater.isNull())
		pPatternFormater->setProperty("pattern", "%Y-%m-%d %H:%M:%S [%p] %s: %t");
	
	std::string logFileName = "engine.log";
	Poco::File logFile(logFileName);
	if (logFile.exists())
		logFile.remove();
	
	if (!pFileChannel.isNull())
	{
		pFileChannel->setProperty("path", logFileName);
		pFileChannel->open();
	}
	
	if (!pSplitterChannel.isNull())
	{
		pSplitterChannel->addChannel(pFileFormattingChannel);
		pSplitterChannel->addChannel(pConsoleFormattingChannel);
	}

	mDefaultLogger.setChannel(pSplitterChannel);

	mDefaultLogger.setLevel(Poco::Message::PRIO_INFORMATION);
}

Log::~Log() {}

void Log::logMessage(const std::string& source, const std::string& text, LogLevel logLevel)
{
	Poco::Message message;
	message.setSource(source);
	message.setText(text);

	switch(logLevel)
	{
	case LOG_LEVEL_INFORMATION:
		message.setPriority(Poco::Message::PRIO_INFORMATION);
		break;
	case LOG_LEVEL_WARNING:
		message.setPriority(Poco::Message::PRIO_WARNING);
		break;
	case LOG_LEVEL_ERROR:
		message.setPriority(Poco::Message::PRIO_ERROR);
		break;
	}

	mDefaultLogger.log(message);
}

Log& Log::getInstance()
{
	return core::Singleton<Log>::getInstance();
}

Log* Log::getInstancePtr()
{
	return core::Singleton<Log>::getInstancePtr();
}

} // end namespace core
