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
