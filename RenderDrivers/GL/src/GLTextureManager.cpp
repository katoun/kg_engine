#include <GLTextureManager.h>
#include <GLTexture.h>

namespace game
{
namespace render
{
    
GLTextureManager::GLTextureManager():TextureManager()
{
}

GLTextureManager::~GLTextureManager()
{
}


Texture* GLTextureManager::createImpl(const core::string& filename)
{
	Texture* tex = new GLTexture(filename);

	return tex;
}

} // end namespace render
} // end namespace game
