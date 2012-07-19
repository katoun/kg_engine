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

#ifndef _LOAD_EVENT_RECEIVER_H_
#define _LOAD_EVENT_RECEIVER_H_

#include <core/Config.h>

namespace resource
{

struct LoadEvent;

class ENGINE_PUBLIC_EXPORT LoadEventReceiver
{	
public:

	virtual ~LoadEventReceiver();

	//! Called when the loading is about to begin.
	virtual void loadStarted(const LoadEvent& evt);

	//! Called after a resource has been loaded.
	virtual void loadUpdate(const LoadEvent& evt);

	//! Called after the loading has ended.
	virtual void loadEnded(const LoadEvent& evt);
};

} // end namespace resource

#endif