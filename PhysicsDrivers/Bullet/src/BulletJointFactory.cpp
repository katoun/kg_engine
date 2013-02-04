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

#include <BulletJointFactory.h>
#include <BulletJoint.h>

namespace physics
{

Joint* BulletJointFactory::createJoint(JointType type)
{
	Joint* newJoint = nullptr;

	switch(type)
	{
	case JOINT_TYPE_UNDEFINED:
		newJoint = nullptr;
		break;
	case JOINT_TYPE_SPHERICAL:
		newJoint = new BulletSphericalJoint();
		break;
	case JOINT_TYPE_HINGE:
		newJoint = new BulletHingeJoint();
		break;
	case JOINT_TYPE_SLIDER:
		newJoint = new BulletSliderJoint();
		break;
	case JOINT_TYPE_GENERIC:
		newJoint = new BulletGenericJoint();
		break;
	case JOINT_TYPE_RIGID:
		newJoint = new BulletRigidJoint();
		break;
	default:
		newJoint = nullptr;
		break;	
	}

	return newJoint;
}

void BulletJointFactory::destroyJoint(Joint* joint)
{
	if (joint == nullptr)
		return;

	switch(joint->getJointType())
	{
	case JOINT_TYPE_UNDEFINED:
		{
			SAFE_DELETE(joint);
		}
		break;
	case JOINT_TYPE_SPHERICAL:
		{
			BulletSphericalJoint* bulletJoint = static_cast<BulletSphericalJoint*>(joint);
			assert(bulletJoint != nullptr);
			SAFE_DELETE(bulletJoint);
		}
		break;
	case JOINT_TYPE_HINGE:
		{
			BulletHingeJoint* bulletJoint = static_cast<BulletHingeJoint*>(joint);
			assert(bulletJoint != nullptr);
			SAFE_DELETE(bulletJoint);
		}
		break;
	case JOINT_TYPE_SLIDER:
		{
			BulletSliderJoint* bulletJoint = static_cast<BulletSliderJoint*>(joint);
			assert(bulletJoint != nullptr);
			SAFE_DELETE(bulletJoint);
		}
		break;
	case JOINT_TYPE_GENERIC:
		{
			BulletGenericJoint* bulletJoint = static_cast<BulletGenericJoint*>(joint);
			assert(bulletJoint != nullptr);
			SAFE_DELETE(bulletJoint);
		}
		break;
	case JOINT_TYPE_RIGID:
		{
			BulletRigidJoint* bulletJoint = static_cast<BulletRigidJoint*>(joint);
			assert(bulletJoint != nullptr);
			SAFE_DELETE(bulletJoint);
		}
		break;
	default:
		{
			SAFE_DELETE(joint);
		}
		break;	
	}
}

} // end namespace physics
