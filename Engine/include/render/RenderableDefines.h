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