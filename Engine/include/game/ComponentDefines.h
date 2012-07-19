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

#ifndef _COMPONENT_DEFINES_H_
#define _COMPONENT_DEFINES_H_

namespace game
{

enum ComponentType
{
	COMPONENT_TYPE_UNDEFINED,
	COMPONENT_TYPE_TRANSFORM,
	COMPONENT_TYPE_LIGHT,
	COMPONENT_TYPE_CAMERA,
	COMPONENT_TYPE_RENDERABLE,
	COMPONENT_TYPE_BODY,
	COMPONENT_TYPE_JOINT,
	COMPONENT_TYPE_SOUND,
	COMPONENT_TYPE_LISTENER,
	COMPONENT_TYPE_COUNT
};

} // end namespace game

#endif