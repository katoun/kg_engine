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

#include <sound/Listener.h>
#include <core/Vector3d.h>
#include <core/Utils.h>

namespace sound
{

unsigned int Listener::msNextGeneratedListenerIndex = 0;

Listener::Listener(): scene::Node("Listener_" + core::intToString(msNextGeneratedListenerIndex++))
{
	mNodeType = scene::NT_LISTENER;

	mVelocity = core::vector3d::ORIGIN_3D;
}

Listener::Listener(const std::string& name): scene::Node(name)
{
	mNodeType = scene::NT_LISTENER;

	mVelocity = core::vector3d::ORIGIN_3D;
}

Listener::~Listener() {}

void Listener::setVelocity(float x, float y, float z)
{
	mVelocity.X = x;
	mVelocity.Y = y;
	mVelocity.Z = z;
	mModifiedAbsoluteTransform = true;
}

void Listener::setVelocity(const core::vector3d& vec)
{
	mVelocity = vec;
	mModifiedAbsoluteTransform = true;
}

const core::vector3d& Listener::getVelocity() const
{
	return mVelocity;
}

} // end namespace sound
