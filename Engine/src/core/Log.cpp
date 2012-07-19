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
