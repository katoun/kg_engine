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

#ifndef _SYSTEM_DEFINES_H_
#define _SYSTEM_DEFINES_H_

namespace core
{

enum SystemState
{
	SYSTEM_STATE_UNDEFINED,
	SYSTEM_STATE_INITIALIZING,
	SYSTEM_STATE_INITIALIZED,
	SYSTEM_STATE_STARTING,
	SYSTEM_STATE_STARTED,
	SYSTEM_STATE_STOPING,
	SYSTEM_STATE_STOPED,
	SYSTEM_STATE_UPDATING,
	SYSTEM_STATE_UPDATED,
	SYSTEM_STATE_UNINITIALIZING,
	SYSTEM_STATE_UNINITIALIZED,
	SYSTEM_STATE_COUNT
};

}// end namespace core

#endif
