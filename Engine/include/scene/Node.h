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

#ifndef _NODE_H_
#define _NODE_H_

#include <core/Config.h>
#include <core/Vector3d.h>
#include <core/Quaternion.h>
#include <core/Matrix4.h>
#include <engine/Object.h>
#include <scene/NodeDefines.h>
#include <game/TransformDefines.h>

#include <map>

namespace scene
{

//! Class representing a node in the scene graph.
//!	A node in the scene graph is a node in a large tree
//!	containing all objects in the scene. A node contains
//!	information about the transformation which will apply to
//!	it and all of it's children.
class ENGINE_PUBLIC_EXPORT Node: public engine::Object
{
	//friend class SceneManager; // Allow full access to scene manager

public:
	
	Node();
	Node(const std::string& name);

	virtual ~Node();

	//! Gets node type.
	const NodeType& getNodeType() const;

	bool getVisibleAxis();
	void setVisibleAxis(bool visible);

	//! Sets the position of the node relative to it's parent.
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(const core::vector3d& pos);

	//! Gets the position of the node relative to it's parent.
	const core::vector3d& getPosition();

	//!Sets the orientation applied to this node.
	virtual void setOrientation(const core::quaternion& q);

	//!Gets the orientation of this node.
	const core::quaternion& getOrientation();

	//!Sets the scaling factor applied to this node.
	void setScale(float x, float y, float z);
	void setScale(const core::vector3d& scale);

	//!Gets the scaling factor of this node.
	const core::vector3d& getScale();

	//! Tells the node whether it should inherit orientation from it's parent node.
	void setInheritOrientation(bool inherit);
	//! Returns true if this node is affected by orientation applied to the parent node.
	bool getInheritOrientation();

	//! Tells the node whether it should inherit scaling factors from it's parent node.
	void setInheritScale(bool inherit);
	//! Returns true if this node is affected by scaling factors applied to the parent node.
	bool getInheritScale();

	//Transformations on all coordinates
	virtual void scale(const core::vector3d& scale);
	virtual void translate(const core::vector3d& d, game::TransformSpace relativeTo = game::TRANSFORM_SPACE_LOCAL);
	virtual void rotate(const core::quaternion& q, game::TransformSpace relativeTo = game::TRANSFORM_SPACE_LOCAL);
	
	virtual void rotate(const float& degrees, const core::vector3d& axis, game::TransformSpace relativeTo = game::TRANSFORM_SPACE_LOCAL);
	virtual void rotateX(float degrees, game::TransformSpace relativeTo = game::TRANSFORM_SPACE_LOCAL);
	virtual void rotateY(float degrees, game::TransformSpace relativeTo = game::TRANSFORM_SPACE_LOCAL);
	virtual void rotateZ(float degrees, game::TransformSpace relativeTo = game::TRANSFORM_SPACE_LOCAL);

	//! Sets this node's parent.
	void setParent(Node* parent);

	//! Gets this node's parent.
	Node* getParent();

	//! Creates a new node as a child of this node.
	Node* createChild(const core::vector3d& pos = core::vector3d::ORIGIN_3D, const core::quaternion& rot = core::quaternion::IDENTITY, const core::vector3d& scale = core::vector3d::UNIT_SCALE);

	//! Adds a child node to this node.
	void addChild(Node* child);
	
	//! Gets a pointer to a child node.
	Node* getChild(const unsigned int& id) const;

	//! Gets the number of child nodes under this one.
	unsigned short int getNumberOfChildren();

	const std::map<unsigned int, Node*>& getChildren();

	//Drops the specified child from this node. Does not delete the node, just detaches it from
	//this parent, potentially to be reattached elsewhere.
	void removeChild(Node* child);
	Node* removeChild(const unsigned int& id);

	//!Removes all child Nodes attached to this node. Does not delete the node, just detaches it from
	//this parent, potentially to be reattached elsewhere.
	void removeAllChildren();

	//! Gets the position of the node as derived from all parents.
	const core::vector3d& getAbsolutePosition();

	//! Gets the orientation of the node as derived from all parents.
	const core::quaternion& getAbsoluteOrientation();	

	//! Gets the scaling factor of the node as derived from all parents.
	const core::vector3d& getAbsoluteScale();

protected:

	//! Incremented count for next index
	static unsigned int msNextGeneratedNodeIndex;

	NodeType mNodeType;

	Node* mParent;

	//! Flag indicating derived transform was modified.
	bool mModifiedAbsoluteTransform;

	virtual void updateTransformImpl();

	virtual void updateImpl(float elapsedTime);

	bool mVisibleAxis;

	//! Stores the position/translation of the node relative to its parent.
	core::vector3d mPosition;

	//! Stores the orientation of the node relative to it's parent.
	core::quaternion mOrientation;

	//! Stores the scaling factor applied to this node
	core::vector3d mScale;

	//! Cached combined orientation.
	//! This member is the orientation derived by combining the
	//! local transformations and those of it's parents.
	core::quaternion mAbsoluteOrientation;

	//! Cached combined position.
	core::vector3d mAbsolutePosition;

	//! Cached combined scale.
	core::vector3d mAbsoluteScale;

	//! Stores whether this node inherits orientation from it's parent
	bool mInheritOrientation;

	//! Stores whether this node inherits scale from it's parent
	bool mInheritScale;

	//! Central list of children.
	std::map<unsigned int, Node*> mChildren;
};

//! Class representing the root node in the scene graph.
class ENGINE_PRIVATE_EXPORT RootNode: public Node
{
public:

	RootNode();
	~RootNode();
};

//! Class representing a group node in the scene graph.
class ENGINE_PRIVATE_EXPORT GroupNode: public Node
{
public:

	GroupNode();
	GroupNode(const std::string& name);

	~GroupNode();

	//! Adds a node to this group.
	void addNode(Node* node);

	//! Gets a pointer to a node in this group.
	Node* getNode(const unsigned int& id) const;

	//! Gets the number of nodes in this group.
	unsigned short int getNumberOfNodes();

	//Drops the specified node from this group.Does not delete the node, just detaches it from
	//this group, potentially to be reattached elsewhere.
	void removeNode(Node* node);
	Node* removeNode(const unsigned int& id);

	//!Removes all nodes in this group. Does delete the nodes.
	void removeAllNodes();

protected:

	//! Incremented count for next index
	static unsigned int msNextGeneratedGroupNodeIndex;
};

} // end namespace scene

#endif