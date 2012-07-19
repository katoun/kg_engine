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

#include <game/Transform.h>
#include <game/GameObject.h>
#include <core/Matrix4.h>

namespace game
{

Transform::Transform(): Component()
{
	mType = COMPONENT_TYPE_TRANSFORM;

	mVisibleAxis = false;

	mPosition = mAbsolutePosition = core::vector3d::ORIGIN_3D;
	mOrientation = mAbsoluteOrientation = core::quaternion::IDENTITY;
	mScale = mAbsoluteScale = core::vector3d::UNIT_SCALE;
	mInheritOrientation = true;
	mInheritScale = true;
	mModifiedAbsoluteTransform = true;
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

const core::vector3d& Transform::getPosition()
{
	return mPosition;
}

void Transform::setPosition(float x, float y, float z)
{
	mPosition.X = x;
	mPosition.Y = y;
	mPosition.Z = z;
	mModifiedAbsoluteTransform = true;
}

void Transform::setPosition(const core::vector3d& pos)
{
	mPosition = pos;
	mModifiedAbsoluteTransform = true;
}

const core::quaternion& Transform::getOrientation()
{
	return mOrientation;
}

void Transform::setOrientation(const core::quaternion& q)
{
	mOrientation = q;
	mModifiedAbsoluteTransform = true;
}

const core::vector3d& Transform::getScale()
{
	return mScale;
}

void Transform::setScale(float x, float y, float z)
{
	mScale.X = x;
	mScale.Y = y;
	mScale.Z = z;
	mModifiedAbsoluteTransform = true;
}

void Transform::setScale(const core::vector3d& scale)
{
	mScale = scale;
	mModifiedAbsoluteTransform = true;
}

bool Transform::isAbsoluteTransformModified()
{
	return mModifiedAbsoluteTransform;
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

const core::vector3d& Transform::getAbsolutePosition()
{
	return mAbsolutePosition;
}

const core::quaternion& Transform::getAbsoluteOrientation()
{
	return mAbsoluteOrientation;
}

const core::vector3d& Transform::getAbsoluteScale()
{
	return mAbsoluteScale;
}

void Transform::scale(const core::vector3d &scale)
{
	core::matrix4 m;
	m.setScale(scale);
	m.transformVector(mScale);

	mModifiedAbsoluteTransform = true;
}

void Transform::translate(const core::vector3d &d,  TransformSpace relativeTo)
{
	switch (relativeTo)
	{
	case TRANSFORM_SPACE_LOCAL:
		// position is relative to parent so transform downwards
		mPosition += mOrientation * d;
		break;
	case TRANSFORM_SPACE_PARENT:
		mPosition += d;
		break;
	case TRANSFORM_SPACE_WORLD:
		// position is relative to parent so transform upwards
		GameObject* pParent = mGameObject->getParent();
		if (pParent != NULL)
		{
			Transform* pParentTransform = static_cast<Transform*>(pParent->getComponent(COMPONENT_TYPE_TRANSFORM));
			if (pParentTransform != NULL)
			{
				core::vector3d offset = pParentTransform->getAbsoluteOrientation().getInverse() * d;
				core::matrix4 m;
				m.setInverseScale(pParentTransform->getAbsoluteScale());
				m.transformVector(offset);
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

	mModifiedAbsoluteTransform = true;
}

void Transform::rotate(const core::quaternion &q, TransformSpace relativeTo)
{
	// Normalise quaternion to avoid drift
	core::quaternion qnorm = q;
	qnorm.normalize();

	switch (relativeTo)
	{
	case TRANSFORM_SPACE_LOCAL:
		// Note the order of the mult, i.e. q comes after
		mOrientation = mOrientation * qnorm;
		break;
	case TRANSFORM_SPACE_PARENT:
		// Rotations are normally relative to local axes, transform up
		mOrientation = qnorm * mOrientation;
		break;
	case TRANSFORM_SPACE_WORLD:
		// Rotations are normally relative to local axes, transform up
		mOrientation = mOrientation * getAbsoluteOrientation().getInverse()	* qnorm * getAbsoluteOrientation();
		break;		
	}

	mModifiedAbsoluteTransform = true;
}

void Transform::rotate(const float& degrees, const core::vector3d &axis, TransformSpace relativeTo)
{
	core::quaternion q;
	q.fromDegreeAxis(degrees, axis);
	rotate(q, relativeTo);
}

void Transform::rotateX(float degrees, TransformSpace relativeTo)
{
	rotate(degrees, core::vector3d::UNIT_X, relativeTo);
}

void Transform::rotateY(float degrees, TransformSpace relativeTo)
{
	rotate(degrees, core::vector3d::UNIT_Y, relativeTo);
}

void Transform::rotateZ(float degrees, TransformSpace relativeTo)
{
	rotate(degrees, core::vector3d::UNIT_Z, relativeTo);
}

void Transform::updateImpl(float elapsedTime)
{
	if (mModifiedAbsoluteTransform)
	{
		if (mGameObject != NULL)
		{
			std::map<unsigned int, GameObject*> children = mGameObject->getChildren();
			std::map<unsigned int, GameObject*>::const_iterator i;
			for (i = children.begin(); i != children.end(); ++i)
			{
				GameObject* pChild = i->second;
				if (pChild != NULL)
				{
					Transform* pChildTransform = static_cast<Transform*>(pChild->getComponent(COMPONENT_TYPE_TRANSFORM));
					if (pChildTransform != NULL)
					{
						pChildTransform->mModifiedAbsoluteTransform = true;
					}
				}
			}

			GameObject* pParent = mGameObject->getParent();
			if (pParent != NULL)
			{
				Transform* pParentTransform = static_cast<Transform*>(pParent->getComponent(COMPONENT_TYPE_TRANSFORM));
				if (pParentTransform != NULL)
				{
					// Combine orientation with that of parent
					const core::quaternion& parentOrientation = pParentTransform->getAbsoluteOrientation();
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

					const core::vector3d& parentScale = pParentTransform->getAbsoluteScale();
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

					const core::vector3d& parentPosition = pParentTransform->getAbsolutePosition();
			
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

		mModifiedAbsoluteTransform = false;
	}
}

void Transform::onParentChangedImpl(GameObject* gameObject)
{
	mModifiedAbsoluteTransform = true;
}

} // end namespace game