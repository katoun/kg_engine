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

#include <render/LightFactory.h>
#include <render/Light.h>
#include <render/RenderManager.h>

namespace render
{

LightFactory::LightFactory(): game::ComponentFactory()
{
	mName = "Light";
}

game::Component* LightFactory::createComponent()
{
	Light* pLight = new Light();

	if (RenderManager::getInstance() != nullptr)
		RenderManager::getInstance()->addLight(pLight);

	return pLight;
}

void LightFactory::destroyComponent(game::Component* component)
{
	Light* pLight = static_cast<Light*>(component);

	if (RenderManager::getInstance() != nullptr)
		RenderManager::getInstance()->removeLight(pLight);

	assert(pLight != nullptr);
	SAFE_DELETE(pLight);
}

} // end namespace game