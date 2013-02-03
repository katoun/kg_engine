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

#include <game/Transform.h>
#include <game/GameObject.h>
#include <game/MessageDefines.h>

#include <glm/gtc/matrix_transform.hpp>

namespace game
{

Transform::Transform(): Component()
{
	mType = COMPONENT_TYPE_TRANSFORM;

	mVisibleAxis = false;

	mPosition = mAbsolutePosition = glm::vec3(0, 0, 0);
	mOrientation = mAbsoluteOrientation = glm::quat(0, 0, 0, 1);
	mScale = mAbsoluteScale = glm::vec3(1, 1, 1);

	mInheritOrientation = true;
	mInheritScale = true;
	mTransformNeedsUpdate = true;
}

Transform::~Transform() {}

bool Transform::getVisibleAxis()
{
	return mVisibleAxis;
}

void Transform::setVisibleAxis(bool visible)
{
	mVisibleAxis = visible;
}

const glm::vec3& Transform::getPosition()
{
	return mPosition;
}

void Transform::setPosition(float x, float y, float z)
{
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
	mTransformNeedsUpdate = true;
}

void Transform::setPosition(const glm::vec3& pos)
{
	mPosition = pos;
	mTransformNeedsUpdate = true;
}

const glm::quat& Transform::getOrientation()
{
	return mOrientation;
}

void Transform::setOrientation(float x, float y, float z, float w)
{
	mOrientation.x = x;
	mOrientation.y = y;
	mOrientation.z = z;
	mOrientation.w = w;
	mTransformNeedsUpdate = true;
}

void Transform::setOrientation(const glm::quat& q)
{
	mOrientation = q;
	mTransformNeedsUpdate = true;
}

const glm::vec3& Transform::getScale()
{
	return mScale;
}

void Transform::setScale(float x, float y, float z)
{
	mScale.x = x;
	mScale.y = y;
	mScale.z = z;
	mTransformNeedsUpdate = true;
}

void Transform::setScale(const glm::vec3& scale)
{
	mScale = scale;
	mTransformNeedsUpdate = true;
}

bool Transform::getInheritOrientation()
{
	return mInheritOrientation;
}

void Transform::setInheritOrientation(bool inherit)
{
	mInheritOrientation = inherit;
}

bool Transform::getInheritScale()
{
	return mInheritScale;
}

void Transform::setInheritScale(bool inherit)
{
	mInheritScale = inherit;
}

const glm::vec3& Transform::getAbsolutePosition()
{
	return mAbsolutePosition;
}

const glm::quat& Transform::getAbsoluteOrientation()
{
	return mAbsoluteOrientation;
}

const glm::vec3& Transform::getAbsoluteScale()
{
	return mAbsoluteScale;
}

void Transform::scale(const glm::vec3& scale)
{
	mScale = mScale * scale;

	mTransformNeedsUpdate = true;
}

void Transform::translate(const glm::vec3& d,  TransformSpace relativeTo)
{
	switch (relativeTo)
	{
	case TRANSFORM_LOCAL_SPACE:
		// position is relative to parent so transform downwards
		mPosition += mOrientation * d;
		break;
	case TRANSFORM_PARENT_SPACE:
		mPosition += d;
		break;
	case TRANSFORM_WORLD_SPACE:
		// position is relative to parent so transform upwards
		GameObject* pParent = mGameObject->getParent();
		if (pParent != nullptr)
		{
			Transform* pParentTransform = static_cast<Transform*>(pParent->getComponent(COMPONENT_TYPE_TRANSFORM));
			if (pParentTransform != nullptr)
			{
				glm::vec3 offset = glm::inverse(pParentTransform->getAbsoluteOrientation()) * d;

				offset = offset / pParentTransform->getAbsoluteScale();

				mPosition += offset;
			}
			else
			{
				mPosition += d;
			}
		}
		else
		{
			mPosition += d;
		}
		break;		
	}

	mTransformNeedsUpdate = true;
}

void Transform::rotate(const glm::quat& q, TransformSpace relativeTo)
{
	// Normalise quaternion to avoid drift
	glm::quat qnorm = glm::normalize(q);

	switch (relativeTo)
	{
	case TRANSFORM_LOCAL_SPACE:
		// Note the order of the mult, i.e. q comes after
		mOrientation = mOrientation * qnorm;
		break;
	case TRANSFORM_PARENT_SPACE:
		// Rotations are normally relative to local axes, transform up
		mOrientation = qnorm * mOrientation;
		break;
	case TRANSFORM_WORLD_SPACE:
		// Rotations are normally relative to local axes, transform up
		mOrientation = mOrientation * glm::inverse(getAbsoluteOrientation()) * qnorm * getAbsoluteOrientation();
		break;		
	}

	mTransformNeedsUpdate = true;
}

void Transform::rotate(const float& degrees, const glm::vec3& axis, TransformSpace relativeTo)
{
	glm::quat q = glm::angleAxis(degrees, axis);
	rotate(q, relativeTo);
}

void Transform::rotateX(float degrees, TransformSpace relativeTo)
{
	rotate(degrees, glm::vec3(1, 0, 0), relativeTo);
}

void Transform::rotateY(float degrees, TransformSpace relativeTo)
{
	rotate(degrees, glm::vec3(0, 1, 0), relativeTo);
}

void Transform::rotateZ(float degrees, TransformSpace relativeTo)
{
	rotate(degrees, glm::vec3(0, 0, 1), relativeTo);
}

void Transform::updateImpl(float elapsedTime)
{
	if (mTransformNeedsUpdate)
	{
		if (mGameObject != nullptr)
		{
			mGameObject->sendMessage(this, MESSAGE_TRANSFORM_NEEDS_UPDATE);

			GameObject* pParent = mGameObject->getParent();
			if (pParent != nullptr)
			{
				Transform* pParentTransform = static_cast<Transform*>(pParent->getComponent(COMPONENT_TYPE_TRANSFORM));
				if (pParentTransform != nullptr)
				{
					// Combine orientation with that of parent
					const glm::quat& parentOrientation = pParentTransform->getAbsoluteOrientation();
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

					const glm::vec3& parentScale = pParentTransform->getAbsoluteScale();
					// Update scale
					if (mInheritScale)
					{
						// Set own scale, NB just combine as equivalent axes, no shearing
						mAbsoluteScale = mScale;

						mAbsoluteScale = mAbsoluteScale * parentScale;
					}
					else
					{
						// No inheritance
						mAbsoluteScale = mScale;
					}

					const glm::vec3& parentPosition = pParentTransform->getAbsolutePosition();
			
					// Change position vector based on parent's orientation & scale
					mAbsolutePosition = mPosition;

					mAbsoluteScale = mAbsoluteScale * parentScale;

					mAbsolutePosition = parentOrientation * mAbsolutePosition;
					// Add altered position vector to parents
					mAbsolutePosition += parentPosition;
				}
				else
				{
					// Parent has no transform
					mAbsoluteOrientation = mOrientation;
					mAbsolutePosition = mPosition;
					mAbsoluteScale = mScale;
				}
			}
			else
			{
				// Root node, no parent
				mAbsoluteOrientation = mOrientation;
				mAbsolutePosition = mPosition;
				mAbsoluteScale = mScale;
			}
		}

		mTransformNeedsUpdate = false;
	}
}

void Transform::onMessageImpl(unsigned int messageID)
{
	if (messageID == MESSAGE_PARENT_CHANGED)
	{
		mTransformNeedsUpdate = true;
	}
	else if (messageID == MESSAGE_TRANSFORM_NEEDS_UPDATE)
	{
		mTransformNeedsUpdate = true;
	}
}

} // end namespace game