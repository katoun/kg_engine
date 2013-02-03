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

#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <EngineConfig.h>
#include <game/Component.h>
#include <game/TransformDefines.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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
	const glm::vec3& getPosition();

	//! Sets the position.
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(const glm::vec3& pos);

	//!Gets the orientation.
	const glm::quat& getOrientation();

	//!Sets the orientation.
	virtual void setOrientation(float x, float y, float z, float w);
	virtual void setOrientation(const glm::quat& q);

	//!Gets the scaling factor.
	const glm::vec3& getScale();

	//!Sets the scaling factor.
	void setScale(float x, float y, float z);
	void setScale(const glm::vec3& scale);

	//! Returns true if is affected by orientation applied to the parent.
	bool getInheritOrientation();

	//! Tells it should inherit orientation from it's parent.
	void setInheritOrientation(bool inherit);

	//! Returns true if is affected by scaling factors applied to the parent.
	bool getInheritScale();

	//! Tells it should inherit scaling factors from it's parent.
	void setInheritScale(bool inherit);

	//! Gets the position as derived from all parents.
	const glm::vec3& getAbsolutePosition();

	//! Gets the orientation as derived from all parents.
	const glm::quat& getAbsoluteOrientation();

	//! Gets the scaling facto as derived from all parents.
	const glm::vec3& getAbsoluteScale();

	//Transformations on all coordinates
	virtual void scale(const glm::vec3& scale);
	virtual void translate(const glm::vec3& d, TransformSpace relativeTo = TRANSFORM_LOCAL_SPACE);
	virtual void rotate(const glm::quat& q, TransformSpace relativeTo = TRANSFORM_LOCAL_SPACE);
	
	virtual void rotate(const float& degrees, const glm::vec3& axis, TransformSpace relativeTo = TRANSFORM_LOCAL_SPACE);
	virtual void rotateX(float degrees, TransformSpace relativeTo = TRANSFORM_LOCAL_SPACE);
	virtual void rotateY(float degrees, TransformSpace relativeTo = TRANSFORM_LOCAL_SPACE);
	virtual void rotateZ(float degrees, TransformSpace relativeTo = TRANSFORM_LOCAL_SPACE);

protected:

	void updateImpl(float elapsedTime);
	void onMessageImpl(unsigned int messageID);

	bool mVisibleAxis;

	//! Stores the position/translation relative to its parent.
	glm::vec3 mPosition;

	//! Stores the orientation relative to it's parent.
	glm::quat mOrientation;

	//! Stores the scaling factor.
	glm::vec3 mScale;

	//! Cached combined position.
	glm::vec3 mAbsolutePosition;

	//! Cached combined orientation.
	glm::quat mAbsoluteOrientation;

	//! Cached combined scale.
	glm::vec3 mAbsoluteScale;

	//! Flag indicating transform needs an update.
	bool mTransformNeedsUpdate;

	//! Stores whether inherits orientation from it's parent
	bool mInheritOrientation;

	//! Stores whether inherits scale from it's parent
	bool mInheritScale;
};

} // end namespace game

#endif