/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2013 Catalin Alexandru Nastase

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

#ifndef _RENDER_BUFFER_DEFINES_H_
#define _RENDER_BUFFER_DEFINES_H_

#include <EngineConfig.h>

namespace render
{

//! Locking options
enum ENGINE_PUBLIC_EXPORT BufferLocking
{
	BL_NORMAL,		//! Normal mode, allows read/write and contents are preserved.    
	BL_DISCARD,		//! Discards the entire buffer while locking; Only allowed with the BU_DYNAMIC flag.   
	BL_READ_ONLY,	//! Lock the buffer for reading only. Not allowed with the BU_WRITE_ONLY flag. Mandatory on static buffers.    
	BL_NO_OVERWRITE	//! As BL_NORMAL, except the application do not overwrite any region of the buffer which has already been used in this frame.
};

//! Enum describing buffer usage; not mutually exclusive
enum ENGINE_PUBLIC_EXPORT BufferUsage
{
	BU_STATIC = 1,							//! Static buffer which the application rarely modifies once created.
	BU_DYNAMIC = 2,							//! Dynamic buffer which the application fairly often modifies once created.    
	BU_WRITE_ONLY = 4,						//! The application will never read the contents of the buffer back, it will only ever write data.
	BU_STATIC_WRITE_ONLY = 5,				//! Combination of BU_STATIC and BU_WRITE_ONLY.
	BU_DYNAMIC_WRITE_ONLY = 6,				//! Combination of BU_DYNAMIC and BU_WRITE_ONLY.
	BU_DISCARDABLE = 8,						//! Discardable buffer which the application will be refilling the contents of the buffer regularly (not just updating, but generating the contents from scratch)
	BU_DYNAMIC_WRITE_ONLY_DISCARDABLE = 14	//! Combination of BU_DYNAMIC, BU_WRITE_ONLY and BU_DISCARDABLE
};

}// end namespace render

#endif// _RENDER_BUFFER_DEFINES_H_