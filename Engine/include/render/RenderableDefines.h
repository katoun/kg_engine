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

#ifndef _RENDERABLE_DEFINES_H_
#define _RENDERABLE_DEFINES_H_

namespace render
{

enum RenderableType
{
	RENDERABLE_TYPE_UNDEFINED,
	RENDERABLE_TYPE_MODEL,
	RENDERABLE_TYPE_OVERLAY,
	RENDERABLE_TYPE_COUNT
};

enum RenderOperationType 
{
	ROT_POINT_LIST,
	ROT_LINE_LIST,
	ROT_LINE_STRIP,
	ROT_TRIANGLE_LIST,
	ROT_TRIANGLE_STRIP,
	ROT_TRIANGLE_FAN
};

} //namespace render

#endif