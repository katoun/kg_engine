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
#ifndef _LOG_H_
#define _LOG_H_

#include <core/Config.h>
#include <core/Singleton.h>
#include <core/LogDefines.h>

#include <Poco/Logger.h>

#include <string>
#include <map>
#include <list>

namespace core
{

enum LogLevel;

//! The log handles logging messages.
class ENGINE_PUBLIC_EXPORT Log: public Singleton<Log>
{
public:

	Log();
	~Log();

	//! Log a message a text into the log
	//! \param source: The source form were this log is sent.
	//! \param text: Text to print out.
	//! \param level: Log level of the text.
	void logMessage(const std::string& source, const std::string& text, LogLevel logLevel = LOG_LEVEL_INFORMATION);

	static Log& getInstance();
	static Log* getInstancePtr();

protected:

	//! Default log.
	Poco::Logger& mDefaultLogger;
};

} // end namespace core

#endif