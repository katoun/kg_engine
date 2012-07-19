/*
-----------------------------------------------------------------------------
Copyright (c) 2006-2012 Catalin Alexandru Nastase

KG game engine (http://k-game.sourceforge.net) is made available under the LGPL License (http://www.gnu.org/copyleft/lgpl.html)

Under the LGPL you may use KG game engine for any purpose you wish, as long as you:
1. Release any modifications to the KG game engine source back to the community
2. Pass on the source to KG game engine with all the copyrights intact, or link back to a place where the source code can be obtained (http://k-game.sourceforge.net)
3. Make it clear where you have customised it.
The above is a precis, please do read the full license agreement.
-----------------------------------------------------------------------------
*/

#include <BulletJointFactory.h>
#include <BulletJoint.h>

namespace physics
{

Joint* BulletJointFactory::createJoint(JointType type)
{
	Joint* newJoint = NULL;

	switch(type)
	{
	case JOINT_TYPE_UNDEFINED:
		newJoint = NULL;
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
		newJoint = NULL;
		break;	
	}

	return newJoint;
}

Joint* BulletJointFactory::createJoint(const std::string& name, JointType type)
{
	Joint* newJoint = NULL;

	switch(type)
	{
	case JOINT_TYPE_UNDEFINED:
		newJoint = NULL;
		break;
	case JOINT_TYPE_SPHERICAL:
		newJoint = new BulletSphericalJoint(name);
		break;
	case JOINT_TYPE_HINGE:
		newJoint = new BulletHingeJoint(name);
		break;
	case JOINT_TYPE_SLIDER:
		newJoint = new BulletSliderJoint(name);
		break;
	case JOINT_TYPE_GENERIC:
		newJoint = new BulletGenericJoint(name);
		break;
	case JOINT_TYPE_RIGID:
		newJoint = new BulletRigidJoint(name);
		break;
	default:
		newJoint = NULL;
		break;	
	}

	return newJoint;
}

void BulletJointFactory::destroyJoint(Joint* joint)
{
	if (joint == NULL)
		return;

	switch(joint->getJointType())
	{
	case JOINT_TYPE_UNDEFINED:
		{
			if (joint != NULL) delete joint;
		}
		break;
	case JOINT_TYPE_SPHERICAL:
		{
			BulletSphericalJoint* bulletJoint = static_cast<BulletSphericalJoint*>(joint);
			assert(bulletJoint != NULL);
			if (bulletJoint != NULL) delete bulletJoint;
		}
		break;
	case JOINT_TYPE_HINGE:
		{
			BulletHingeJoint* bulletJoint = static_cast<BulletHingeJoint*>(joint);
			assert(bulletJoint != NULL);
			if (bulletJoint != NULL) delete bulletJoint;
		}
		break;
	case JOINT_TYPE_SLIDER:
		{
			BulletSliderJoint* bulletJoint = static_cast<BulletSliderJoint*>(joint);
			assert(bulletJoint != NULL);
			if (bulletJoint != NULL) delete bulletJoint;
		}
		break;
	case JOINT_TYPE_GENERIC:
		{
			BulletGenericJoint* bulletJoint = static_cast<BulletGenericJoint*>(joint);
			assert(bulletJoint != NULL);
			if (bulletJoint != NULL) delete bulletJoint;
		}
		break;
	case JOINT_TYPE_RIGID:
		{
			BulletRigidJoint* bulletJoint = static_cast<BulletRigidJoint*>(joint);
			assert(bulletJoint != NULL);
			if (bulletJoint != NULL) delete bulletJoint;
		}
		break;
	default:
		{
			if (joint != NULL) delete joint;
		}
		break;	
	}
}

} // end namespace physics
