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


#ifndef _RENDER_UTILS_H_
#define _RENDER_UTILS_H_

#include <EngineConfig.h>
#include <render/GLConfig.h>
#include <render/ShaderDefines.h>
#include <render/ShaderParameterDefines.h>
#include <render/BufferDefines.h>
#include <render/VertexBufferDefines.h>
#include <render/RenderDefines.h>
#include <render/TextureDefines.h>
#include <resource/PixelFormat.h>


#include <string>

namespace render
{

//!* Takes the pixel format and returns the appropriate GL one.
inline ENGINE_PUBLIC_EXPORT GLenum getGLOriginFormat(resource::PixelFormat mFormat);

//!	Takes the pixel format and returns the type that must be provided to GL as internal format.
inline ENGINE_PUBLIC_EXPORT GLenum getGLInternalFormat(resource::PixelFormat mFormat, bool hwGamma = false);

//!	Takes the pixel format and returns the type that must be provided to GL as internal format.
//!If no match exists, returns the closest match.
inline ENGINE_PUBLIC_EXPORT GLenum getClosestGLInternalFormat(resource::PixelFormat mFormat, bool hwGamma = false);

inline ENGINE_PUBLIC_EXPORT GLenum getGLUsage(BufferUsage usage);
inline ENGINE_PUBLIC_EXPORT GLenum getGLType(ShaderType type);
inline ENGINE_PUBLIC_EXPORT GLenum getGLType(VertexElementType type);
inline ENGINE_PUBLIC_EXPORT GLenum getGLType(RenderOperationType type);
inline ENGINE_PUBLIC_EXPORT GLenum getGLType(TextureType type);
inline ENGINE_PUBLIC_EXPORT GLenum getGLType(ShaderParameterType type);
inline ENGINE_PUBLIC_EXPORT ShaderParameterType getType(ShaderAutoParameterType type);
inline ENGINE_PUBLIC_EXPORT ShaderParameterType getType(TextureType type);

} // end namespace render

#endif