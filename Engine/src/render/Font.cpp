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

#include <render/Font.h>
#include <resource/ResourceManager.h>
#include <render/Material.h>

namespace render
{

Font::Font(const std::string& name, resource::Serializer* serializer): resource::Resource(name, serializer)
{
	mResourceType = resource::RESOURCE_TYPE_FONT;

	mMaterial = nullptr;
}

Font::~Font() {}

void Font::setMaterial(const std::string& filename)
{
	if (resource::ResourceManager::getInstance() != nullptr)
		mMaterial = static_cast<Material*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_RENDER_MATERIAL, filename));
}

void Font::setMaterial(Material* material)
{
	if (material == nullptr)
		return;

	mMaterial = material;
}

Material* Font::getMaterial() const
{
	return mMaterial;
}

void Font::setCharTexCoords(char id, float u1, float v1, float u2, float v2)
{
	unsigned char idx = GAME_CHAR_INDEX(id);

	mTexCoordsU1[idx] = u1;
	mTexCoordsV1[idx] = v1;
	mTexCoordsU2[idx] = u2;
	mTexCoordsV2[idx] = v2;

	mAspectRatio[idx] = (u2 - u1)/(v2 - v1);
}

void Font::getCharTexCoords(char id, float& u1, float& v1, float& u2, float& v2) const
{
	unsigned char idx = GAME_CHAR_INDEX(id);

	u1 = mTexCoordsU1[idx];
	v1 = mTexCoordsV1[idx];
	u2 = mTexCoordsU2[idx];
	v2 = mTexCoordsV2[idx];
}

float Font::getCharAspectRatio(char id) const
{
	unsigned char idx = GAME_CHAR_INDEX(id);

	return mAspectRatio[idx];
}

} //namespace render
