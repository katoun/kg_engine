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

#include <render/TextureUnit.h>
#include <render/Texture.h>
#include <resource/ResourceManager.h>

namespace render
{

TextureUnit::TextureUnit()
{
	mTextureCoordSetIndex = 0;

	colorBlendMode.blendType = LBT_COLOR;
	colorBlendMode.operation = LBX_MODULATE;
	colorBlendMode.source1 = LBS_TEXTURE;
	colorBlendMode.source2 = LBS_CURRENT;
	colorBlendMode.colorArg1 = Color::White;
	colorBlendMode.colorArg2 = Color::White;
	colorBlendMode.factor = 0.0f;

	alphaBlendMode.blendType = LBT_ALPHA;
	alphaBlendMode.operation = LBX_MODULATE;
	alphaBlendMode.source1 = LBS_TEXTURE;
	alphaBlendMode.source2 = LBS_CURRENT;
	colorBlendMode.alphaArg1 = 1.0f;
	colorBlendMode.alphaArg2 = 1.0f;
	colorBlendMode.factor = 0.0f;
}

void TextureUnit::setTexture(const std::string& filename)
{
	mTexture = static_cast<Texture*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_TEXTURE, filename));
}

void TextureUnit::setTexture(Texture* texture)
{
	if (texture == NULL)
		return;

	mTexture = texture;
}

Texture* TextureUnit::getTexture() const
{
	return mTexture;
}

void TextureUnit::setTextureCoordSet(unsigned int set)
{
	mTextureCoordSetIndex = set;
}

unsigned int TextureUnit::getTextureCoordSet() const
{
	return mTextureCoordSetIndex;
}

void TextureUnit::setColorOperation(LayerBlendOperation op, LayerBlendSource source1, LayerBlendSource source2,
			const Color& arg1, const Color& arg2, float manualBlend)
{
	colorBlendMode.operation = op;
	colorBlendMode.source1 = source1;
	colorBlendMode.source2 = source2;
	colorBlendMode.colorArg1 = arg1;
	colorBlendMode.colorArg2 = arg2;
	colorBlendMode.factor = manualBlend;
}

void TextureUnit::setAlphaOperation(LayerBlendOperation op,	LayerBlendSource source1, LayerBlendSource source2,
			float arg1, float arg2, float manualBlend)
{
	alphaBlendMode.operation = op;
	alphaBlendMode.source1 = source1;
	alphaBlendMode.source2 = source2;
	alphaBlendMode.alphaArg1 = arg1;
	alphaBlendMode.alphaArg2 = arg2;
	alphaBlendMode.factor = manualBlend;
}

LayerBlendMode TextureUnit::getColorBlendMode() const
{
	return colorBlendMode;
}

LayerBlendMode TextureUnit::getAlphaBlendMode() const
{
	return alphaBlendMode;
}

} //namespace render
