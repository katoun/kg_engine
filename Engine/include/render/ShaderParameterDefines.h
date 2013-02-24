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

#ifndef _SHADER_PARAMETER_DEFINES_H_
#define _SHADER_PARAMETER_DEFINES_H_

namespace render
{

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
	SHADER_PARAMETER_TYPE_MATRIX2,
	SHADER_PARAMETER_TYPE_MATRIX3,
	SHADER_PARAMETER_TYPE_MATRIX4,
	SHADER_PARAMETER_TYPE_SAMPLER1D,
	SHADER_PARAMETER_TYPE_SAMPLER2D,
	SHADER_PARAMETER_TYPE_SAMPLER3D,
	SHADER_PARAMETER_TYPE_UNKNOWN
};

enum ShaderAutoParameterType
{
	SHADER_AUTO_PARAMETER_TYPE_MODEL_MATRIX,									//! The current model matrix
	SHADER_AUTO_PARAMETER_TYPE_VIEW_MATRIX,										//! The current view matrix
	SHADER_AUTO_PARAMETER_TYPE_PROJECTION_MATRIX,								//! The current projection matrix
	SHADER_AUTO_PARAMETER_TYPE_VIEW_PROJECTION_MATRIX,							//! The current view & projection matrices concatenated
	SHADER_AUTO_PARAMETER_TYPE_MODEL_VIEW_MATRIX,								//! The current model & view matrices concatenated
	SHADER_AUTO_PARAMETER_TYPE_MODEL_VIEW_PROJECTION_MATRIX,					//! The current model, view & projection matrices concatenated

	SHADER_AUTO_PARAMETER_TYPE_MATERIAL_AMBIENT_COLOUR,							//! Material ambient color
	SHADER_AUTO_PARAMETER_TYPE_MATERIAL_DIFFUSE_COLOUR,							//! Material diffuse color
	SHADER_AUTO_PARAMETER_TYPE_MATERIAL_SPECULAR_COLOUR,						//! Material specular color
	SHADER_AUTO_PARAMETER_TYPE_MATERIAL_SPECULAR_POWER,							//! Material specular power
	
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_COUNT,										//! The number of active light sources
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION,									//! Light position in world space
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_AMBIENT_COLOUR,							//! Light ambient color
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIFFUSE_COLOUR,							//! Light diffuse color
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_SPECULAR_COLOUR,							//! Light specular color
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_ATTENUATION,								//! Light attenuation parameters, vec4(range, constant, linear, quadric)
	SHADER_AUTO_PARAMETER_TYPE_LIGHT_POWER_SCALE,								//! Light power level

	SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION,									//! The current camera's position in world space

	SHADER_AUTO_PARAMETER_TYPE_NONE
};

} // end namespace render

#endif