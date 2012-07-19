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

#ifndef _SHADER_DEFINES_H_
#define _SHADER_DEFINES_H_

namespace render
{

//! Enum identifying the shader type
enum ShaderType
{
	SHADER_TYPE_VERTEX,
	SHADER_TYPE_FRAGMENT,
	SHADER_TYPE_GEOMETRY,
	SHADER_TYPE_UNKNOWN
};

enum ShaderParameterType
{
	SHADER_PARAMETER_TYPE_FLOAT,
	SHADER_PARAMETER_TYPE_FLOAT2,
	SHADER_PARAMETER_TYPE_FLOAT3,
	SHADER_PARAMETER_TYPE_FLOAT4,
	SHADER_PARAMETER_TYPE_INT,
	SHADER_PARAMETER_TYPE_INT2,
	SHADER_PARAMETER_TYPE_INT3,
	SHADER_PARAMETER_TYPE_INT4,
	SHADER_PARAMETER_TYPE_MATRIX_2X2,
	SHADER_PARAMETER_TYPE_MATRIX_2X3,
	SHADER_PARAMETER_TYPE_MATRIX_2X4,
	SHADER_PARAMETER_TYPE_MATRIX_3X2,
	SHADER_PARAMETER_TYPE_MATRIX_3X3,
	SHADER_PARAMETER_TYPE_MATRIX_3X4,
	SHADER_PARAMETER_TYPE_MATRIX_4X2,
	SHADER_PARAMETER_TYPE_MATRIX_4X3,
	SHADER_PARAMETER_TYPE_MATRIX_4X4,
	SHADER_PARAMETER_TYPE_SAMPLER1D,
	SHADER_PARAMETER_TYPE_SAMPLER2D,
	SHADER_PARAMETER_TYPE_SAMPLER3D,
	SHADER_PARAMETER_TYPE_SAMPLERCUBE,
	SHADER_PARAMETER_TYPE_SAMPLER1DSHADOW,
	SHADER_PARAMETER_TYPE_SAMPLER2DSHADOW,
	SHADER_PARAMETER_TYPE_UNKNOWN
};

enum ShaderAutoParameterType
{
	SHADER_AUTO_PARAMETER_TYPE_WORLD_MATRIX,								//! The current world matrix
	SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLD_MATRIX,						//! The current world matrix, inverted
	SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLD_MATRIX,					//! Provides transpose of world matrix.
	SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLD_MATRIX,			//! The current world matrix, inverted & transposed
	SHADER_AUTO_PARAMETER_TYPE_VIEW_MATRIX,								//! The current view matrix
	SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEW_MATRIX,						//! The current view matrix, inverted
	SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_VIEW_MATRIX,						//! Provides transpose of view matrix.
	SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_VIEW_MATRIX,				//! Provides inverse transpose of view matrix.
	SHADER_AUTO_PARAMETER_TYPE_PROJECTION_MATRIX,							//! The current projection matrix
	SHADER_AUTO_PARAMETER_TYPE_INVERSE_PROJECTION_MATRIX,					//! Provides inverse of projection matrix.
	SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_PROJECTION_MATRIX,				//! Provides transpose of projection matrix.
	SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_PROJECTION_MATRIX,		//! Provides inverse transpose of projection matrix.
	SHADER_AUTO_PARAMETER_TYPE_VIEWPROJ_MATRIX,							//! The current view & projection matrices concatenated
	SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEWPROJ_MATRIX,					//! Provides inverse of concatenated view and projection matrices.
	SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_VIEWPROJ_MATRIX,					//! Provides transpose of concatenated view and projection matrices.
	SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_VIEWPROJ_MATRIX,			//! Provides inverse transpose of concatenated view and projection matrices.
	SHADER_AUTO_PARAMETER_TYPE_WORLDVIEW_MATRIX,							//! The current world & view matrices concatenated
	SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLDVIEW_MATRIX,					//! The current world & view matrices concatenated, then inverted
	SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLDVIEW_MATRIX,				//! Provides transpose of concatenated world and view matrices.
	SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLDVIEW_MATRIX,		//! The current world & view matrices concatenated, then inverted & transposed
	SHADER_AUTO_PARAMETER_TYPE_WORLDVIEWPROJ_MATRIX,						//! The current world, view & projection matrices concatenated
	SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLDVIEWPROJ_MATRIX,				//! Provides inverse of concatenated world, view and projection matrices.
	SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLDVIEWPROJ_MATRIX,			//! Provides transpose of concatenated world, view and projection matrices.
	SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLDVIEWPROJ_MATRIX,	//! Provides inverse transpose of concatenated world, view and projection
	
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_COUNT,								//! The number of active light sources
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION,							//! A light position in world space
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_OBJECT_SPACE,				//! A light position in object space
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_VIEW_SPACE,					//! A light position in view space
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION,							//! A light direction in world space
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_OBJECT_SPACE,				//! A light direction in object space
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_VIEW_SPACE,				//! A light direction in view space

	SHADER_AUTO_PARAMETER_TYPE_AMBIENT_LIGHT_COLOUR,						//! The ambient light color set in the scene
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIFFUSE_COLOUR,						//! Light diffuse color
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_SPECULAR_COLOUR,						//! Light specular color
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_ATTENUATION,							//! Light attenuation parameters, Vector4(range, constant, linear, quadric)
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_POWER_SCALE,							//! Light power level

	SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION,							//! The current camera's position in world space
	SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION_OBJECT_SPACE,				//! The current camera's position in object space 

	SHADER_AUTO_PARAMETER_TYPE_NONE
};

} // end namespace render

#endif