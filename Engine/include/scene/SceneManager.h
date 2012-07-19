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

#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include <core/Config.h>
#include <core/System.h>
#include <core/Singleton.h>

#include <map>

namespace resource
{
class Resource;
class Serializer;
}

namespace scene
{

class Node;

//! Scene Manager.
//! Defines the functionality of a 3D Scene.
class ENGINE_PUBLIC_EXPORT SceneManager: public core::System, public core::Singleton<SceneManager>
{
public:

	SceneManager();
	~SceneManager();

	//! Gets the Node at the root of the scene hierarchy.
	//!	The entire scene is held as a hierarchy of nodes, which
	//!	allows things like relative transforms.
	Node* getRootNode();

	//! Creates a node to be managed by scene manager.
	Node* createNode(Node* parent = NULL);
	Node* createNode(const std::string& name, Node* parent = NULL);

	//! Adds a node to be managed by scene manager.
	void addNode(Node* node, Node* parent = NULL);

	//! Retrieves a pointer to a node by id.
	Node* getNode(const unsigned int& id);

	//! Retrieves the total number of created nodes.
	unsigned int getNumberOfNodes() const;

	//! Removes a node.
	void removeNode(Node* node);
	//! Removes a node.
	void removeNode(const unsigned int& id);

	void removeAllNodes();

	static SceneManager& getInstance();
	static SceneManager* getInstancePtr();

private:

protected:

	void initializeImpl();
	void uninitializeImpl();
	void updateImpl(float elapsedTime);

	//! Central list of Nodes - for easy memory management.
	std::map<unsigned int, Node*> mNodes;

	//! Root node
	Node* mRoot;
};

} // end namespace scene

#endif