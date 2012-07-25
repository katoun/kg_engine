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

#include <game/Scene.h>
#include <game/GameObject.h>
#include <game/GameManager.h>

namespace game
{

Scene::Scene(const std::string& filename, resource::Serializer* serializer): resource::Resource(filename, serializer)
{
	mResourceType = resource::RESOURCE_TYPE_SCENE;
}

Scene::~Scene() {}

void Scene::addGameObject(GameObject* gameObject)
{
	if (gameObject == NULL)
		return;

	mGameObjects.push_back(gameObject);
}

const std::list<GameObject*>& Scene::getGameObjects()
{
	return mGameObjects;
}

void Scene::removeAllGameObjects()
{
	mGameObjects.clear();
}

void Scene::unloadImpl()
{
	// Remove all GameObjects
	removeAllGameObjects();
}

} // end namespace game