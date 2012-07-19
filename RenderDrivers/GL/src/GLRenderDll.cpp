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
