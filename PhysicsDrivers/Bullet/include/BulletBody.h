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

#ifndef _BULLET_BODY_H_
#define _BULLET_BODY_H_

#include <BulletConfig.h>
#include <physics/Body.h>

namespace physics
{
// Forward definition of references
class Shape;

class BulletBody: public Body
{
public:

	BulletBody();
	~BulletBody();

	void setEnabled(bool enabled);

	void setMaterial(const std::string& filename);

	void setMaterial(Material* material);

	void applyForce(const glm::vec3& force);

	void applyTorque(const glm::vec3& torque);

	void applyLinearImpulse(const glm::vec3& linearImpulse);

	void applyAngularImpulse(const glm::vec3& angularImpulse);

	void createBtRigitBody(btDynamicsWorld* world);

	btRigidBody* getBulletRigidBody();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void updateImpl(float elapsedTime);
	void onMessageImpl(unsigned int messageID);

	btCollisionShape* addBulletShape(Shape* shape);

	btRigidBody*			mRigidBody;
	btDefaultMotionState*	mMotionState;

	bool mBodyNeedsUpdate;
};

} // end namespace game

#endif