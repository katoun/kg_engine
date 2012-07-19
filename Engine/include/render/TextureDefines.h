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

#ifndef _TEXTURE_DEFINES_H_
#define _TEXTURE_DEFINES_H_

namespace render
{

//! Enum identifying the texture type
enum TextureType
{
	TEX_TYPE_1D = 1,		//! 1D texture, used in combination with 1D texture coordinates
	TEX_TYPE_2D = 2,		//! 2D texture, used in combination with 2D texture coordinates (default)
	TEX_TYPE_3D = 3,		//! 3D texture, used in combination with 3D texture coordinates
};

enum ImageFlags
{
	IF_COMPRESSED = 0x00000001,
	IF_CUBEMAP    = 0x00000002,
	IF_3D_TEXTURE = 0x00000004
};

} // end namespace render

#endif