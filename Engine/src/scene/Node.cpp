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

#include <scene/Node.h>
#include <core/Utils.h>

namespace scene
{

unsigned int Node::msNextGeneratedNodeIndex = 0;

Node::Node(): engine::Object("Node_" + core::intToString(msNextGeneratedNodeIndex++))
{
	mObjectType = engine::OT_NODE;
	mNodeType = NT_UNDEFINED;

	mParent = NULL;

	mVisibleAxis = false;

	mPosition = mAbsolutePosition = core::vector3d::ORIGIN_3D;
	mOrientation = mAbsoluteOrientation = core::quaternion::IDENTITY;
	mScale = mAbsoluteScale = core::vector3d::UNIT_SCALE;
	mInheritOrientation = true;
	mInheritScale = true;
	mModifiedAbsoluteTransform = true;
}

Node::Node(const std::string& name): engine::Object(name)
{
	mObjectType = engine::OT_NODE;
	mNodeType = NT_UNDEFINED;

	mParent = NULL;

	mVisibleAxis = false;

	mPosition = mAbsolutePosition = core::vector3d::ORIGIN_3D;
	mOrientation = mAbsoluteOrientation = core::quaternion::IDENTITY;
	mScale = mAbsoluteScale = core::vector3d::UNIT_SCALE;
	mInheritOrientation = true;
	mInheritScale = true;
	mModifiedAbsoluteTransform = true;
}

Node::~Node()
{
	removeAllChildren();

	if (mParent)
		mParent->removeChild(this);
}

const NodeType& Node::getNodeType() const
{
	return mNodeType;
}

bool Node::getVisibleAxis()
{
	return mVisibleAxis;
}

void Node::setVisibleAxis(bool visible)
{
	mVisibleAxis = visible;
}

void Node::setPosition(float x, float y, float z)
{
	mPosition.X = x;
	mPosition.Y = y;
	mPosition.Z = z;
	mModifiedAbsoluteTransform = true;
}

void Node::setPosition(const core::vector3d& pos)
{
	mPosition = pos;
	mModifiedAbsoluteTransform = true;
}

const core::vector3d& Node::getPosition()
{
	return mPosition;
}

void Node::setOrientation(const core::quaternion& q)
{
	mOrientation = q;
	mModifiedAbsoluteTransform = true;
}

const core::quaternion& Node::getOrientation()
{
	return mOrientation;
}

void Node::setScale(float x, float y, float z)
{
	mScale.X = x;
	mScale.Y = y;
	mScale.Z = z;
	mModifiedAbsoluteTransform = true;
}

void Node::setScale(const core::vector3d& scale)
{
	mScale = scale;
	mModifiedAbsoluteTransform = true;
}

const core::vector3d& Node::getScale()
{
	return mScale;
}

void Node::setInheritOrientation(bool inherit)
{
	mInheritOrientation = inherit;
}

bool Node::getInheritOrientation()
{
	return mInheritOrientation;
}

void Node::setInheritScale(bool inherit)
{
	mInheritScale = inherit;
}

bool Node::getInheritScale()
{
	return mInheritScale;
}

void Node::scale(const core::vector3d &scale)
{
	core::matrix4 m;
	m.setScale(scale);
	m.transformVector(mScale);

	mModifiedAbsoluteTransform = true;
}

void Node::translate(const core::vector3d &d, game::TransformSpace relativeTo)
{
	switch (relativeTo)
	{
	case game::TRANSFORM_SPACE_LOCAL:
		// position is relative to parent so transform downwards
		mPosition += mOrientation * d;
		break;
	case game::TRANSFORM_SPACE_PARENT:
		mPosition += d;
		break;
	case game::TRANSFORM_SPACE_WORLD:
		// position is relative to parent so transform upwards
		if (mParent != NULL)
		{
			core::vector3d offset = mParent->getAbsoluteOrientation().getInverse() * d;
			core::matrix4 m;
			m.setInverseScale(mParent->getAbsoluteScale());
			m.transformVector(offset);
			mPosition += offset;
		}
		else
		{
			mPosition += d;
		}
		break;		
	}

	mModifiedAbsoluteTransform = true;
}

void Node::rotate(const core::quaternion &q, game::TransformSpace relativeTo)
{
	// Normalise quaternion to avoid drift
	core::quaternion qnorm = q;
	qnorm.normalize();

	switch (relativeTo)
	{
	case game::TRANSFORM_SPACE_LOCAL:
		// Note the order of the mult, i.e. q comes after
		mOrientation = mOrientation * qnorm;
		break;
	case game::TRANSFORM_SPACE_PARENT:
		// Rotations are normally relative to local axes, transform up
		mOrientation = qnorm * mOrientation;
		break;
	case game::TRANSFORM_SPACE_WORLD:
		// Rotations are normally relative to local axes, transform up
		mOrientation = mOrientation * getAbsoluteOrientation().getInverse()	* qnorm * getAbsoluteOrientation();
		break;		
	}

	mModifiedAbsoluteTransform = true;
}

void Node::rotate(const float& degrees, const core::vector3d &axis, game::TransformSpace relativeTo)
{
	core::quaternion q;
	q.fromDegreeAxis(degrees, axis);
	rotate(q, relativeTo);
}

void Node::rotateX(float degrees, game::TransformSpace relativeTo)
{
	rotate(degrees, core::vector3d::UNIT_X, relativeTo);
}

void Node::rotateY(float degrees, game::TransformSpace relativeTo)
{
	rotate(degrees, core::vector3d::UNIT_Y, relativeTo);
}

void Node::rotateZ(float degrees, game::TransformSpace relativeTo)
{
	rotate(degrees, core::vector3d::UNIT_Z, relativeTo);
}

void Node::setParent(Node* parent)
{
	if (parent == NULL)
		return;

	if (mParent)
		mParent->removeChild(this);

	mParent = parent;
	mParent->mChildren[mID] = this;
	updateTransformImpl();
}

Node* Node::getParent()
{
	return mParent;
}

Node* Node::createChild(const core::vector3d& pos, const core::quaternion& rot, const core::vector3d& scale)
{
	Node* newNode = new Node();
	newNode->translate(pos);
	newNode->rotate(rot);
	newNode->scale(scale);
	mChildren[newNode->getID()] = newNode;
	newNode->mParent = this;
	newNode->updateTransformImpl();

	return newNode;
}

void Node::addChild(Node* child)
{
	if (child == NULL)
		return;

	if (child->mParent)
		child->mParent->removeChild(child);

	mChildren[child->getID()] = child;
	child->mParent = this;
	child->updateTransformImpl();
}

Node* Node::getChild(const unsigned int& id) const
{
	std::map<unsigned int, Node*>::const_iterator i = mChildren.find(id);
	if (i != mChildren.end())
		return i->second;

	return NULL;
}

unsigned short int Node::getNumberOfChildren()
{
	return mChildren.size();
}

const std::map<unsigned int, Node*>& Node::getChildren()
{
	return mChildren;
}

void Node::removeChild(Node* child)
{
	if (child == NULL)
		return;

	std::map<unsigned int, Node*>::iterator i = mChildren.find(child->getID());
	if (i != mChildren.end())
	{
		i->second->mParent = NULL;
		mChildren.erase(i);
	}
}

Node* Node::removeChild(const unsigned int& id)
{
	std::map<unsigned int, Node*>::iterator i = mChildren.find(id);
	if (i != mChildren.end())
	{
		Node* node = i->second;
		i->second->mParent = NULL;
		mChildren.erase(i);

		return node;
	}

	return NULL;
}

void Node::removeAllChildren()
{
	std::map<unsigned int, Node*>::iterator i;
	for (i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		i->second->mParent = NULL;
	}
	
	mChildren.clear();
}

const core::vector3d& Node::getAbsolutePosition()
{
	return mAbsolutePosition;
}

const core::quaternion& Node::getAbsoluteOrientation()
{
	return mAbsoluteOrientation;
}

const core::vector3d& Node::getAbsoluteScale()
{
	return mAbsoluteScale;
}

void Node::updateTransformImpl()
{
	if (mModifiedAbsoluteTransform)
	{
		std::map<unsigned int, Node*>::const_iterator i;
		for (i = mChildren.begin(); i != mChildren.end(); ++i)
			i->second->mModifiedAbsoluteTransform = true;

		if (mParent != NULL)
		{
			// Combine orientation with that of parent
			const core::quaternion& parentOrientation = mParent->getAbsoluteOrientation();
			if (mInheritOrientation)
			{
				// Combine orientation with that of parent
				mAbsoluteOrientation = parentOrientation * mOrientation;
			}
			else
			{
				// No inheritance
				mAbsoluteOrientation = mOrientation;
			}

			const core::vector3d& parentScale = mParent->getAbsoluteScale();
			// Update scale
			if (mInheritScale)
			{
				// Set own scale, NB just combine as equivalent axes, no shearing
				mAbsoluteScale = mScale;

				core::matrix4 m;
				m.setScale(parentScale);
				m.transformVector(mAbsoluteScale);
			}
			else
			{
				// No inheritance
				mAbsoluteScale = mScale;
			}

			const core::vector3d& parentPosition = mParent->getAbsolutePosition();
			
			// Change position vector based on parent's orientation & scale
			mAbsolutePosition = mPosition;
			core::matrix4 m;
			m.setScale(parentScale);
			m.transformVector(mAbsoluteScale);

			mAbsolutePosition = parentOrientation * mAbsolutePosition;
			// Add altered position vector to parents
			mAbsolutePosition += parentPosition;
		}
		else
		{
			// Root node, no parent
			mAbsoluteOrientation = mOrientation;
			mAbsolutePosition = mPosition;
			mAbsoluteScale = mScale;
		}

		mModifiedAbsoluteTransform = false;
	}
}

void Node::updateImpl(float elapsedTime)
{
	updateTransformImpl();
}

RootNode::RootNode(): Node("Root")
{
	mNodeType = NT_ROOT;
}

RootNode::~RootNode() {}

unsigned int GroupNode::msNextGeneratedGroupNodeIndex = 0;

GroupNode::GroupNode(): Node("Group" + core::intToString(msNextGeneratedGroupNodeIndex++))
{
	mNodeType = NT_GROUP;
}

GroupNode::GroupNode(const std::string& name): Node(name)
{
	mNodeType = NT_GROUP;
}

GroupNode::~GroupNode() {}

void GroupNode::addNode(Node* node)
{
	addChild(node);
}

Node* GroupNode::getNode(const unsigned int& id) const
{
	return getChild(id);
}

unsigned short int GroupNode::getNumberOfNodes()
{
	return getNumberOfChildren();
}

void GroupNode::removeNode(Node* node)
{
	removeChild(node);
}

Node* GroupNode::removeNode(const unsigned int& id)
{
	return removeChild(id);
}

void GroupNode::removeAllNodes()
{
	removeAllChildren();
}

} // end namespace scene
