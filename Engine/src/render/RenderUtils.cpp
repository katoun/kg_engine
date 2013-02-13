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

#include <render/RenderUtils.h>

namespace render
{

GLenum getGLOriginFormat(resource::PixelFormat mFormat)
{
	switch(mFormat)
	{
	case resource::PF_A8:
		return GL_ALPHA;
	case resource::PF_L8:
		return GL_LUMINANCE;
	case resource::PF_L16:
		return GL_LUMINANCE;
	case resource::PF_BYTE_LA:
		return GL_LUMINANCE_ALPHA;
	case resource::PF_R3G3B2:
		return GL_RGB;
	case resource::PF_A1R5G5B5:
		return GL_BGRA;
	case resource::PF_R5G6B5:
		return GL_RGB;
	case resource::PF_B5G6R5:
		return GL_BGR;
	case resource::PF_A4R4G4B4:
		return GL_BGRA;
#if ENGINE_ARCHITECTURE_ENDIAN == ENDIAN_BIG
		// Formats are in native endian, so R8G8B8 on little endian is
		// BGR, on big endian it is RGB.
	case resource::PF_R8G8B8:
		return GL_RGB;
	case resource::PF_B8G8R8:
		return GL_BGR;
#else
	case resource::PF_R8G8B8:
		return GL_BGR;
	case resource::PF_B8G8R8:
		return GL_RGB;
#endif
	case resource::PF_X8R8G8B8:
	case resource::PF_A8R8G8B8:
		return GL_BGRA;
	case resource::PF_X8B8G8R8:
	case resource::PF_A8B8G8R8:
		return GL_RGBA;
	case resource::PF_B8G8R8A8:
		return GL_BGRA;
	case resource::PF_R8G8B8A8:
		return GL_RGBA;
	case resource::PF_A2R10G10B10:
		return GL_BGRA;
	case resource::PF_A2B10G10R10:
		return GL_RGBA;
	case resource::PF_FLOAT16_R:
		return GL_LUMINANCE;
	case resource::PF_FLOAT16_GR:
		return GL_LUMINANCE_ALPHA;
	case resource::PF_FLOAT16_RGB:
		return GL_RGB;
	case resource::PF_FLOAT16_RGBA:
		return GL_RGBA;
	case resource::PF_FLOAT32_R:
		return GL_LUMINANCE;
	case resource::PF_FLOAT32_GR:
		return GL_LUMINANCE_ALPHA;
	case resource::PF_FLOAT32_RGB:
		return GL_RGB;
	case resource::PF_FLOAT32_RGBA:
		return GL_RGBA;
	case resource::PF_SHORT_RGBA:
		return GL_RGBA;
	case resource::PF_SHORT_RGB:
		return GL_RGB;
	case resource::PF_SHORT_GR:
		return GL_LUMINANCE_ALPHA;
	case resource::PF_DXT1:
		return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	case resource::PF_DXT3:
		return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
	case resource::PF_DXT5:
		return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
	default:
		return GL_NONE;
	}
}

GLenum getGLInternalFormat(resource::PixelFormat mFormat, bool hwGamma)
{
	switch(mFormat)
	{
	case resource::PF_L8:
		return GL_LUMINANCE8;
	case resource::PF_L16:
		return GL_LUMINANCE16;
	case resource::PF_A8:
		return GL_ALPHA8;
	case resource::PF_A4L4:
		return GL_LUMINANCE4_ALPHA4;
	case resource::PF_BYTE_LA:
		return GL_LUMINANCE8_ALPHA8;
	case resource::PF_R3G3B2:
		return GL_R3_G3_B2;
	case resource::PF_A1R5G5B5:
		return GL_RGB5_A1;
	case resource::PF_R5G6B5:
	case resource::PF_B5G6R5:
		return GL_RGB5;
	case resource::PF_A4R4G4B4:
		return GL_RGBA4;
	case resource::PF_R8G8B8:
	case resource::PF_B8G8R8:
	case resource::PF_X8B8G8R8:
	case resource::PF_X8R8G8B8:
		if (hwGamma)
			return GL_SRGB8;
		else
			return GL_RGB8;
	case resource::PF_A8R8G8B8:
	case resource::PF_B8G8R8A8:
		if (hwGamma)
			return GL_SRGB8_ALPHA8;
		else
			return GL_RGBA8;
	case resource::PF_A2R10G10B10:
	case resource::PF_A2B10G10R10:
		return GL_RGB10_A2;
	case resource::PF_FLOAT16_R:
		return GL_LUMINANCE16F_ARB;
	case resource::PF_FLOAT16_RGB:
		return GL_RGB16F_ARB;
	case resource::PF_FLOAT16_GR: 
		return GL_LUMINANCE_ALPHA16F_ARB;
	case resource::PF_FLOAT16_RGBA:
		return GL_RGBA16F_ARB;
	case resource::PF_FLOAT32_R:
		return GL_LUMINANCE32F_ARB;
	case resource::PF_FLOAT32_GR:
		return GL_LUMINANCE_ALPHA32F_ARB;
	case resource::PF_FLOAT32_RGB:
		return GL_RGB32F_ARB;
	case resource::PF_FLOAT32_RGBA:
		return GL_RGBA32F_ARB;
	case resource::PF_SHORT_RGBA:
		return GL_RGBA16;
	case resource::PF_SHORT_RGB:
		return GL_RGB16;
	case resource::PF_SHORT_GR:
		return GL_LUMINANCE16_ALPHA16;
	case resource::PF_DXT1:
		if (hwGamma)
			return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
		else
			return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	case resource::PF_DXT3:
		if (hwGamma)
			return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
		else
			return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
	case resource::PF_DXT5:
		if (hwGamma)
			return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
		else
			return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
	default:
		return GL_NONE;
	}
}

GLenum getClosestGLInternalFormat(resource::PixelFormat mFormat, bool hwGamma)
{
	GLenum format = getGLInternalFormat(mFormat, hwGamma);
	if(format == GL_NONE)
	{
		if (hwGamma)
			return GL_SRGB8;
		else
			return GL_RGBA8;
	}
	else
		return format;
}


GLenum getGLUsage(BufferUsage usage)
{
	switch(usage)
	{
	case BU_STATIC:
	case BU_STATIC_WRITE_ONLY:
		return GL_STATIC_DRAW;
	case BU_DYNAMIC:
	case BU_DYNAMIC_WRITE_ONLY:
		return GL_DYNAMIC_DRAW;
	case BU_DYNAMIC_WRITE_ONLY_DISCARDABLE:
		return GL_STREAM_DRAW;
	default:
		return GL_DYNAMIC_DRAW;
	};
}

GLenum getGLType(ShaderType type)
{
	switch(type)
	{
	case SHADER_TYPE_VERTEX:
		return GL_VERTEX_SHADER;
	case SHADER_TYPE_FRAGMENT:
		return GL_FRAGMENT_SHADER;
	case SHADER_TYPE_GEOMETRY:
		return GL_GEOMETRY_SHADER;
	default:
		return GL_VERTEX_SHADER;
	}
}

GLenum getGLType(VertexElementType type)
{
	switch(type)
	{
	case VERTEX_ELEMENT_TYPE_FLOAT1:
	case VERTEX_ELEMENT_TYPE_FLOAT2:
	case VERTEX_ELEMENT_TYPE_FLOAT3:
	case VERTEX_ELEMENT_TYPE_FLOAT4:
		return GL_FLOAT;
	case VERTEX_ELEMENT_TYPE_SHORT1:
	case VERTEX_ELEMENT_TYPE_SHORT2:
	case VERTEX_ELEMENT_TYPE_SHORT3:
	case VERTEX_ELEMENT_TYPE_SHORT4:
		return GL_SHORT;
	case VERTEX_ELEMENT_TYPE_COLOR:
		return GL_UNSIGNED_BYTE;
	default:
		return 0;
	}
}

GLenum getGLType(RenderOperationType type)
{
	switch (type)
	{
	case render::ROT_POINT_LIST:
		return GL_POINTS;
	case render::ROT_LINE_LIST:
		return GL_LINES;
	case render::ROT_LINE_STRIP:
		return GL_LINE_STRIP;
	case render::ROT_TRIANGLE_LIST:
		return GL_TRIANGLES;
	case render::ROT_TRIANGLE_STRIP:
		return GL_TRIANGLE_STRIP;
	case render::ROT_TRIANGLE_FAN:
		return GL_TRIANGLE_FAN;
	default:
		return GL_FLOAT;
	}
}

GLenum getGLType(TextureType type)
{
	switch(type)
	{
	case TEX_TYPE_1D:
		return GL_TEXTURE_1D;
	case TEX_TYPE_2D:
		return GL_TEXTURE_2D;
	case TEX_TYPE_3D:
		return GL_TEXTURE_3D;
	default:
		return GL_TEXTURE_2D;
	}
}

GLenum getGLType(ShaderParameterType type)
{
	switch(type)
	{
	case SHADER_PARAMETER_TYPE_FLOAT:
		return GL_FLOAT;
	case SHADER_PARAMETER_TYPE_FLOAT2:
		return GL_FLOAT_VEC2;
	case SHADER_PARAMETER_TYPE_FLOAT3:
		return GL_FLOAT_VEC3;
	case SHADER_PARAMETER_TYPE_FLOAT4:
		return GL_FLOAT_VEC4;
	case SHADER_PARAMETER_TYPE_INT:
		return GL_INT;
	case SHADER_PARAMETER_TYPE_INT2:
		return GL_INT_VEC2;
	case SHADER_PARAMETER_TYPE_INT3:
		return GL_INT_VEC3;
	case SHADER_PARAMETER_TYPE_INT4:
		return GL_INT_VEC4;
	case SHADER_PARAMETER_TYPE_MATRIX2:
		return GL_FLOAT_MAT2;
	case SHADER_PARAMETER_TYPE_MATRIX3:
		return GL_FLOAT_MAT3;
	case SHADER_PARAMETER_TYPE_MATRIX4:
		return GL_FLOAT_MAT4;
	case SHADER_PARAMETER_TYPE_SAMPLER1D:
		return GL_SAMPLER_1D;
	case SHADER_PARAMETER_TYPE_SAMPLER2D:
		return GL_SAMPLER_2D;
	case SHADER_PARAMETER_TYPE_SAMPLER3D:
		return GL_SAMPLER_3D;
	default:
		return 0;
	}
}

ShaderParameterType getType(ShaderAutoParameterType type)
{
	switch(type)
	{
	case SHADER_AUTO_PARAMETER_TYPE_MODEL_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_MODEL_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_VIEW_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEW_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_MODEL_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_MODEL_VIEW_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_MODEL_VIEW_PROJECTION_MATRIX:
	case SHADER_AUTO_PARAMETER_TYPE_INVERSE_MODEL_VIEW_PROJECTION_MATRIX:
		return SHADER_PARAMETER_TYPE_MATRIX4;

	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_COUNT:
		return SHADER_PARAMETER_TYPE_INT;
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_OBJECT_SPACE:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_VIEW_SPACE:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_OBJECT_SPACE:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_VIEW_SPACE:
		return SHADER_PARAMETER_TYPE_FLOAT3;

	case SHADER_AUTO_PARAMETER_TYPE_AMBIENT_LIGHT_COLOUR:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIFFUSE_COLOUR:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_SPECULAR_COLOUR:
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_ATTENUATION:
		return SHADER_PARAMETER_TYPE_FLOAT4;
	case SHADER_AUTO_PARAMETER_TYPE_LIGHT_POWER_SCALE:
		return SHADER_PARAMETER_TYPE_FLOAT;

	case SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION:
	case SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION_OBJECT_SPACE:
		return SHADER_PARAMETER_TYPE_FLOAT3;

	default:
		return SHADER_PARAMETER_TYPE_UNKNOWN;
	}
}

ShaderParameterType getType(TextureType type)
{
	switch(type)
	{
	case TEX_TYPE_1D:
		return SHADER_PARAMETER_TYPE_SAMPLER1D;
	case TEX_TYPE_2D:
		return SHADER_PARAMETER_TYPE_SAMPLER2D;
	case TEX_TYPE_3D:
		return SHADER_PARAMETER_TYPE_SAMPLER3D;
	default:
		return SHADER_PARAMETER_TYPE_SAMPLER2D;
	}
}

}// end namespace render
