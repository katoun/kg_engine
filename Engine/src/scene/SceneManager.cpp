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

#include <scene/SceneManager.h>
#include <scene/Node.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>

template<> scene::SceneManager& core::Singleton<scene::SceneManager>::ms_Singleton = scene::SceneManager();

namespace scene
{

SceneManager::SceneManager(): core::System("SceneManager")
{
	mRoot = new RootNode();
}

SceneManager::~SceneManager()
{
	if (mRoot != NULL)
	{
		delete mRoot;
	}
}

Node* SceneManager::getRootNode()
{
	assert(mRoot != NULL);

	return mRoot;
}

Node* SceneManager::createNode(Node* parent)
{
	Node* newNode = new Node();
	mNodes[newNode->getID()] = newNode;

	if (parent)
		parent->addChild(newNode);
	else
		mRoot->addChild(newNode);

	return newNode;
}

Node* SceneManager::createNode(const std::string& name, Node* parent)
{
	Node* newNode = new Node(name);
	mNodes[newNode->getID()] = newNode;

	if (parent != NULL)
		parent->addChild(newNode);
	else
		mRoot->addChild(newNode);

	return newNode;
}

void SceneManager::addNode(Node* node, Node* parent)
{
	if (node == NULL)
		return;

	mNodes[node->getID()] = node;

	if (parent != NULL)
		parent->addChild(node);
	else
		mRoot->addChild(node);
}

Node* SceneManager::getNode(const unsigned int& id)
{
	std::map<unsigned int, Node*>::const_iterator i = mNodes.find(id);
	if (i != mNodes.end())
		return i->second;

	return NULL;
}

unsigned int SceneManager::getNumberOfNodes() const
{
	return mNodes.size();
}

void SceneManager::removeNode(Node* node)
{
	if (node == NULL)
		return;

	removeNode(node->getID());
}

void SceneManager::removeNode(const unsigned int& id)
{
	std::map<unsigned int, Node*>::iterator i = mNodes.find(id);
	if (i != mNodes.end())
	{
		delete i->second;
		mNodes.erase(i);
	}
}

void SceneManager::removeAllNodes()
{
	std::map<unsigned int, Node*>::iterator i;
	for (i = mNodes.begin(); i != mNodes.end(); ++i)
	{
		delete i->second;
		i->second = NULL;
	}

	mNodes.clear();
}

void SceneManager::initializeImpl() {}

void SceneManager::uninitializeImpl()
{
	// Remove all Nodes
	removeAllNodes();
}

void SceneManager::updateImpl(float elapsedTime)
{
	mRoot->update(elapsedTime);

	std::map<unsigned int, Node*>::iterator i;
	for (i = mNodes.begin(); i != mNodes.end(); ++i)
	{
		Node* node = i->second;
		if (node) node->update(elapsedTime);
	}
}

SceneManager& SceneManager::getInstance()
{
	return core::Singleton<SceneManager>::getInstance();
}

SceneManager* SceneManager::getInstancePtr()
{
	return core::Singleton<SceneManager>::getInstancePtr();
}

} // end namespace scene
