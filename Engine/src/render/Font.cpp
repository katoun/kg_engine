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

#include <render/Font.h>
#include <resource/ResourceManager.h>
#include <render/Material.h>

namespace render
{

Font::Font(const std::string& name, resource::Serializer* serializer): resource::Resource(name, serializer)
{
	mResourceType = resource::RESOURCE_TYPE_FONT;

	mMaterial = NULL;
}

Font::~Font() {}

void Font::setMaterial(const std::string& filename)
{
	std::string materialname = filename;
	mMaterial = static_cast<Material*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_RENDER_MATERIAL, materialname));
}

void Font::setMaterial(Material* material)
{
	if (material == NULL)
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
