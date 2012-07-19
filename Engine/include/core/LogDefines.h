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

#ifndef _LOG_DEFINES_H_
#define _LOG_DEFINES_H_

namespace core
{

enum LogLevel
{	
	LOG_LEVEL_INFORMATION	= 0,	//! High log level, warnings, errors and information texts are printed out.	
	LOG_LEVEL_WARNING		= 1,	//! Default log level, warnings and errors are printed out.	
	LOG_LEVEL_ERROR		= 2,	//! Low log level, only errors are printed into the log	
	LOG_LEVEL_NONE			= 3		//! Nothing is printed out to the log
};

} // end namespace core

#endif