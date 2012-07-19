#ifndef _GLTEXTUREMANAGER_H_
#define _GLTEXTUREMANAGER_H_

#include <GLConfig.h>
#include <core\Config.h>
#include <core/Types.h>
#include <core/String.h>
#include <render/Texture.h>
#include <render/TextureManager.h>

namespace game
{
namespace render
{

class GLTextureManager: public TextureManager
{
public:
    
	GLTextureManager();
	~GLTextureManager();

	Texture* createImpl(const core::string& filename);
   
};
 

} // end namespace render
} // end namespace game

#endif // _GLTEXTUREMANAGER_H_
