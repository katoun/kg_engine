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
#ifndef _LOG_H_
#define _LOG_H_

#include <Config.h>
#include <core/Singleton.h>
#include <core/LogDefines.h>

#include <string>
#include <fstream>

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

	static Log* getInstance();

protected:

	//! Default log.
	std::ofstream mLogStream;
};

} // end namespace core

#endif