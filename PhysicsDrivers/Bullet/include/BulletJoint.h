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

#ifndef _BULLET_JOINT_H_
#define _BULLET_JOINT_H_

#include <BulletConfig.h>
#include <physics/Joint.h>

namespace physics
{

//////////////////////////////////////////////////////////////////////////
//Ball Joint
class BULLET_PUBLIC_EXPORT BulletSphericalJoint: public SphericalJoint
{
public:

	BulletSphericalJoint();
	BulletSphericalJoint(const std::string& name);
	~BulletSphericalJoint();

	void setLimits(float coneLimit, float twistLimit);

protected:

	void initializeImpl();

	btGeneric6DofConstraint *mSphericalConstraint;
};

//////////////////////////////////////////////////////////////////////////
//Hinge Joint
class BULLET_PUBLIC_EXPORT BulletHingeJoint: public HingeJoint
{
public:

	BulletHingeJoint();
	BulletHingeJoint(const std::string& name);
	~BulletHingeJoint();

	void setLimits(float lowerLimit, float upperLimit);

protected:

	void initializeImpl();

	btHingeConstraint *mHingeConstraint;
};

//////////////////////////////////////////////////////////////////////////
//Slider Joint
class BULLET_PUBLIC_EXPORT BulletSliderJoint: public SliderJoint
{
public:

	BulletSliderJoint();
	BulletSliderJoint(const std::string& name);
	~BulletSliderJoint();

protected:

	void initializeImpl();

	btSliderConstraint* mSliderConstraint;
};

//////////////////////////////////////////////////////////////////////////
//Generic Joint
class BULLET_PUBLIC_EXPORT BulletGenericJoint: public GenericJoint
{
public:

	BulletGenericJoint();
	BulletGenericJoint(const std::string& name);
	~BulletGenericJoint();

protected:

	void initializeImpl();

	btGeneric6DofConstraint* mGenericConstraint;
};

//////////////////////////////////////////////////////////////////////////
//Rigid Joint
class BULLET_PUBLIC_EXPORT BulletRigidJoint: public RigidJoint
{
public:

	BulletRigidJoint();
	BulletRigidJoint(const std::string& name);
	~BulletRigidJoint();

protected:

	void initializeImpl();

	btHingeConstraint* mRigidConstraint;
};

} // end namespace game

#endif
