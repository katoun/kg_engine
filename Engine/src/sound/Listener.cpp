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

#include <sound/Listener.h>
#include <core/Utils.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>

namespace sound
{

Listener::Listener(): game::Component()
{
	mType = game::COMPONENT_TYPE_LISTENER;

	mLastPosition = core::vector3d::ORIGIN_3D;
	mVelocity = core::vector3d::ORIGIN_3D;
}

Listener::~Listener() {}

const core::vector3d& Listener::getVelocity() const
{
	return mVelocity;
}

void Listener::updateImpl(float elapsedTime)
{
	if (elapsedTime == 0.0f)
		return;

	if (mGameObject != nullptr)
	{
		game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
		if (pTransform != nullptr)
		{
			core::vector3d delta = pTransform->getAbsolutePosition() - mLastPosition;
			mVelocity = delta / elapsedTime;
			mLastPosition = pTransform->getAbsolutePosition();
		}
	}
}

} // end namespace sound
