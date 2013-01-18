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

#ifndef _TEXTURE_UNIT_H_
#define _TEXTURE_UNIT_H_

#include <core/Config.h>
#include <render/BlendMode.h>
#include <render/Color.h>

#include <string>

namespace render
{

class Texture;

///! Class representing a single texture unit of a Material.
//!
//! Texture units are pipelines for retrieving texture data for rendering onto
//! your objects in the world. Using them is common to both the fixed-function and 
//! the programmable (vertex and fragment program) pipeline, but some of the 
//! settings will only have an effect in the fixed-function pipeline.
class ENGINE_PUBLIC_EXPORT TextureUnit
{
public:

	TextureUnit();

	//! Sets the name of the texture associated with this texture layer.
	void setTexture(const std::string& filename);
	void setTexture(Texture* texture);

	//! Gets the texture associated with this texture layer.
	Texture* getTexture() const;

	//! Sets the index of the set of texture coords this texture layer uses.
	void setTextureCoordSet(unsigned int set);

	//! Gets the index of the set of texture coords this texture layer uses.
	unsigned int getTextureCoordSet() const;

	void setColorOperation(LayerBlendOperation op, LayerBlendSource source1 = LBS_TEXTURE, LayerBlendSource source2 = LBS_CURRENT,
				const Color& arg1 = Color::White, const Color& arg2 = Color::White, float manualBlend = 0.0f);

	void setAlphaOperation(LayerBlendOperation op, LayerBlendSource source1 = LBS_TEXTURE, LayerBlendSource source2 = LBS_CURRENT,
				float arg1 = 1.0f, float arg2 = 1.0f, float manualBlend = 0.0f);

	//! Get multi texturing color blending mode.
	LayerBlendMode getColorBlendMode() const;

	//! Get multi texturing alpha blending mode.
	LayerBlendMode getAlphaBlendMode() const;

protected:

	Texture* mTexture;

	//! The index of the set of texture coords this layer
	//! should use. Default is 0 for all layers. Only change
	//! this if you have provided multiple texture coords per
	//! vertex.
	unsigned int mTextureCoordSetIndex;

	LayerBlendMode colorBlendMode;
	//SceneBlendFactor colorBlendFallbackSrc;
	//SceneBlendFactor colorBlendFallbackDest;

	LayerBlendMode alphaBlendMode;
};

} //namespace render

#endif