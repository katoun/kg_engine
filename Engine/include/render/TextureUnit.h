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