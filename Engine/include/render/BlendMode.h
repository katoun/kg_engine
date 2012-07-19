/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

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

#ifndef _BLEND_MODE_H_
#define _BLEND_MODE_H_

#include <core/Config.h>
#include <render/Color.h>

namespace render
{

//! Types of blending that you can specify between an object and the existing contents of the scene.
//!
//! As opposed to the LayerBlendType, which classifies blends between texture layers, these blending
//! types blend between the output of the texture units and the pixels already in the viewport,
//! allowing for object transparency, glows, etc.
//! These types are provided to give quick and easy access to common effects. You can also use
//! the more manual method of supplying source and destination blending factors.
enum SceneBlendType
{ 
	SBT_TRANSPARENT_ALPHA,	//! Make the object transparent based on the final alpha values in the texture.
	SBT_TRANSPARENT_COLOR,	//! Make the object transparent based on the color values in the texture (brighter = more opaque).
	SBT_ADD,				//! Add the texture values to the existing scene content.
	SBT_MODULATE,			//! Multiply the 2 colors together.
	SBT_REPLACE				//! The default blend mode where source replaces destination.
	// TODO : more
};

//! Blending factors for manually blending objects with the scene. If there isn't a predefined
//! SceneBlendType that you like, then you can specify the blending factors directly to affect the
//! combination of object and the existing scene.
enum SceneBlendFactor
{
	SBF_ONE,
	SBF_ZERO,
	SBF_DEST_COLOR,
	SBF_SOURCE_COLOR,
	SBF_ONE_MINUS_DEST_COLOR,
	SBF_ONE_MINUS_SOURCE_COLOR,
	SBF_DEST_ALPHA,
	SBF_SOURCE_ALPHA,
	SBF_ONE_MINUS_DEST_ALPHA,
	SBF_ONE_MINUS_SOURCE_ALPHA
};

//! Type of texture blend mode.
enum LayerBlendType
{
	LBT_COLOR,
	LBT_ALPHA
};

//! Expert list of valid texture blending operations. It's worth
//! noting that these operations are for blending <i>between texture layers</i> and not between rendered objects
//! and the existing scene.
enum LayerBlendOperation
{
	LBX_SOURCE1,				/// use source1 without modification
	LBX_SOURCE2,				/// use source2 without modification
	LBX_MODULATE,				/// multiply source1 and source2 together
	LBX_MODULATE_X2,			/// as LBX_MODULATE but brighten afterwards (x2)
	LBX_MODULATE_X4,			/// as LBX_MODULATE but brighten more afterwards (x4)
	LBX_ADD,					/// add source1 and source2 together
	LBX_ADD_SIGNED,				/// as LBX_ADD, but subtract 0.5 from the result
	LBX_ADD_SMOOTH,				/// as LBX_ADD, but subtract product from the sum
	LBX_SUBTRACT,				/// subtract source2 from source1
	LBX_BLEND_DIFFUSE_ALPHA,	/// use interpolated alpha value from vertices to scale source1, then add source2 scaled by (1-alpha)
	LBX_BLEND_TEXTURE_ALPHA,	/// as LBX_BLEND_DIFFUSE_ALPHA, but use alpha from texture
	LBX_BLEND_CURRENT_ALPHA,	/// as LBX_BLEND_DIFFUSE_ALPHA, but use current alpha from previous stages
	LBX_BLEND_MANUAL,			/// as LBX_BLEND_DIFFUSE_ALPHA but use a constant manual blend value (0.0-1.0)
	LBX_DOTPRODUCT,				/// dotproduct of color1 and color2 
	LBX_BLEND_DIFFUSE_COLOR		/// use interpolated color values from vertices to scale source1, then add source2 scaled by (1-color)
};

//! List of valid sources of values for blending operations.
enum LayerBlendSource
{
	LBS_CURRENT,	//! the color as built up from previous stages
	LBS_TEXTURE,	//! the color derived from the texture assigned to this layer
	LBS_DIFFUSE,	//! the interpolated diffuse color from the vertices
	LBS_SPECULAR,	//! the interpolated specular color from the vertices
	LBS_MANUAL		//! a color supplied manually as a separate argument
};

//! Class which manages blending of both color and alpha components.
//!
//! This class is a utility class used by TextureUnit to wrap up the details of a 
//! blending operation. This blending operation could be used for blending color 
//! or alpha in a texture layer.
//! It's worth noting that these operations are for blending <i>between texture
//! layers</i> and not between rendered objects and the existing scene. If
//! you wish to make an object blend with others in the scene, e.g. to make
//! transparent objects etc, use the Material::setSceneBlending method.
class ENGINE_PUBLIC_EXPORT LayerBlendMode
{
public:

	LayerBlendType blendType;			//! The type of blending (color or alpha).
	LayerBlendOperation operation;		//! The operation to be applied.

	LayerBlendSource source1;			//! The first source of color/alpha.
	LayerBlendSource source2;			//! The second source of color/alpha.

	Color colorArg1;					//! Manual color for manual source1.
	Color colorArg2;					//! Manual color for manual source2.

	float alphaArg1;						//! Manual alpha value for manual source1.
	float alphaArg2;						//! Manual alpha value for manual source2.

	float factor;							//! Manual blending factor.

	bool operator==(const LayerBlendMode& rhs) const;

	bool operator!=(const LayerBlendMode& rhs) const;
};

} //namespace render

#endif