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
