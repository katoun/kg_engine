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
