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

#ifndef _RESOURCE_DEFINES_H_
#define _RESOURCE_DEFINES_H_

namespace resource
{

//! Resource type
enum ResourceType
{
	RESOURCE_TYPE_UNDEFINED,
	RESOURCE_TYPE_FONT,
	RESOURCE_TYPE_MESH_DATA,
	RESOURCE_TYPE_RENDER_MATERIAL,
	RESOURCE_TYPE_TEXTURE,
	RESOURCE_TYPE_SHADER,
	RESOURCE_TYPE_SOUND_DATA,
	RESOURCE_TYPE_BODY_DATA,
	RESOURCE_TYPE_PHYSICS_MATERIAL,
	RESOURCE_TYPE_COUNT
};

enum ResourceState
{
	RESOURCE_STATE_UNDEFINED,
	RESOURCE_STATE_UNLOADED,
	RESOURCE_STATE_LOADING,
	RESOURCE_STATE_LOADED,
	RESOURCE_STATE_UNLOADING,
	RESOURCE_STATE_COUNT
};

}// end namespace resource

#endif
