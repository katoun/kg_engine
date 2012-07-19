/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2012 Catalin Alexandru Nastase

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
