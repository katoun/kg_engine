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