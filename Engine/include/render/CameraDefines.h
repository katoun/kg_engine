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

#ifndef _CAMERA_DEFINES_H_
#define _CAMERA_DEFINES_H_

namespace render
{

//! Specifies perspective (realistic) or orthographic (architectural) projection.
enum ProjectionType
{
	PROJECTION_TYPE_ORTHOGRAPHIC,
	PROJECTION_TYPE_PERSPECTIVE
};

//! The broad type of detail this camera will render.
enum SceneDetailLevel
{
	SCENE_DETAIL_LEVEL_POINTSONLY,
	SCENE_DETAIL_LEVEL_WIREFRAME,
	SCENE_DETAIL_LEVEL_FLATSHADE,
	SCENE_DETAIL_LEVEL_SMOOTHSHADE,
	SCENE_DETAIL_LEVEL_TEXTURED
};

} //namespace render

#endif