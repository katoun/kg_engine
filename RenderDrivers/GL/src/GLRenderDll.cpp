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

#include <GLConfig.h>
#include <render/RenderManager.h>
#include <resource/ResourceDefines.h>
#include <resource/ResourceFactory.h>
#include <resource/ResourceManager.h>
#include <GLRenderDriver.h>
#include <GLTextureFactory.h>
#include <GLShaderFactory.h>

namespace render
{

RenderDriver* glRenderDriver = NULL;
resource::ResourceFactory* glTextureFactory = NULL;
resource::ResourceFactory* glShaderFactory = NULL;

extern "C" void GL_PUBLIC_EXPORT loadPlugin() throw()
{
	glRenderDriver = GLRenderDriver::getInstancePtr();
	RenderManager::getInstance().setSystemDriver(glRenderDriver);

	glTextureFactory = new GLTextureFactory();
	glShaderFactory = new GLShaderFactory();
	resource::ResourceManager::getInstance().registerResourceFactory(resource::RESOURCE_TYPE_TEXTURE, glTextureFactory);
	resource::ResourceManager::getInstance().registerResourceFactory(resource::RESOURCE_TYPE_SHADER, glShaderFactory);
}

extern "C" void GL_PUBLIC_EXPORT unloadPlugin()
{
	RenderManager::getInstance().removeSystemDriver();

	resource::ResourceManager::getInstance().removeResourceFactory(resource::RESOURCE_TYPE_TEXTURE);
	resource::ResourceManager::getInstance().removeResourceFactory(resource::RESOURCE_TYPE_SHADER);

	if (glTextureFactory != NULL)
		delete glTextureFactory;
	if (glShaderFactory != NULL)
		delete glShaderFactory;
}

} // end namespace render
