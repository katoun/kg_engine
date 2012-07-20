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

#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <core/Config.h>
#include <core/Vector3d.h>
#include <core/Quaternion.h>
#include <game/Component.h>
#include <game/TransformDefines.h>

#include <string>

namespace game
{

class ENGINE_PUBLIC_EXPORT Transform: public Component
{
public:

	Transform();
	~Transform();

	//! Gets if the axis are visible.
	bool getVisibleAxis();

	//! Sets is the axis are visible.
	void setVisibleAxis(bool visible);

	//! Gets the position.
	const core::vector3d& getPosition();

	//! Sets the position.
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(const core::vector3d& pos);

	//!Gets the orientation.
	const core::quaternion& getOrientation();

	//!Sets the orientation.
	virtual void setOrientation(float x, float y, float z, float w);
	virtual void setOrientation(const core::quaternion& q);

	//!Gets the scaling factor.
	const core::vector3d& getScale();

	//!Sets the scaling factor.
	void setScale(float x, float y, float z);
	void setScale(const core::vector3d& scale);

	//! Returns true if is affected by orientation applied to the parent.
	bool getInheritOrientation();

	//! Tells it should inherit orientation from it's parent.
	void setInheritOrientation(bool inherit);

	//! Returns true if is affected by scaling factors applied to the parent.
	bool getInheritScale();

	//! Tells it should inherit scaling factors from it's parent.
	void setInheritScale(bool inherit);

	//! Gets the position as derived from all parents.
	const core::vector3d& getAbsolutePosition();

	//! Gets the orientation as derived from all parents.
	const core::quaternion& getAbsoluteOrientation();

	//! Gets the scaling facto as derived from all parents.
	const core::vector3d& getAbsoluteScale();

	//Transformations on all coordinates
	virtual void scale(const core::vector3d& scale);
	virtual void translate(const core::vector3d& d, TransformSpace relativeTo = TRANSFORM_SPACE_LOCAL);
	virtual void rotate(const core::quaternion& q, TransformSpace relativeTo = TRANSFORM_SPACE_LOCAL);
	
	virtual void rotate(const float& degrees, const core::vector3d& axis, TransformSpace relativeTo = TRANSFORM_SPACE_LOCAL);
	virtual void rotateX(float degrees, TransformSpace relativeTo = TRANSFORM_SPACE_LOCAL);
	virtual void rotateY(float degrees, TransformSpace relativeTo = TRANSFORM_SPACE_LOCAL);
	virtual void rotateZ(float degrees, TransformSpace relativeTo = TRANSFORM_SPACE_LOCAL);

protected:

	void updateImpl(float elapsedTime);
	void onMessageImpl(unsigned int messageID);

	bool mVisibleAxis;

	//! Stores the position/translation relative to its parent.
	core::vector3d mPosition;

	//! Stores the orientation relative to it's parent.
	core::quaternion mOrientation;

	//! Stores the scaling factor.
	core::vector3d mScale;

	//! Cached combined position.
	core::vector3d mAbsolutePosition;

	//! Cached combined orientation.
	core::quaternion mAbsoluteOrientation;

	//! Cached combined scale.
	core::vector3d mAbsoluteScale;

	//! Flag indicating transform needs an update.
	bool mTransformNeedsUpdate;

	//! Stores whether inherits orientation from it's parent
	bool mInheritOrientation;

	//! Stores whether inherits scale from it's parent
	bool mInheritScale;
};

} // end namespace game

#endif