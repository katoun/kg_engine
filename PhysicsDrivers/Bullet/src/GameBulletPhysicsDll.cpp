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

#include <BulletConfig.h>
#include <physics/PhysicsManager.h>
#include <resource/ResourceManager.h>
#include <BulletPhysicsDriver.h>
#include <BulletBodyFactory.h>
#include <BulletShapeFactory.h>
#include <BulletJointFactory.h>
#include <BulletMaterialFactory.h>

namespace physics
{

PhysicsDriver* bulletPhysicsDriver = nullptr;
BodyFactory* bulletBodyFactory = nullptr;
ShapeFactory* bulletShapeFactory = nullptr;
JointFactory* bulletJointFactory = nullptr;
resource::ResourceFactory* bulletMaterialFactory = nullptr;

extern "C" void BULLET_PUBLIC_EXPORT loadPlugin() throw()
{
	bulletPhysicsDriver = new BulletPhysicsDriver();
	bulletBodyFactory = new BulletBodyFactory();
	bulletShapeFactory = new BulletShapeFactory();
	bulletJointFactory = new BulletJointFactory();
	if (PhysicsManager::getInstance() != nullptr)
	{
		PhysicsManager::getInstance()->setSystemDriver(bulletPhysicsDriver);
		PhysicsManager::getInstance()->setDefaultBodyFactory(bulletBodyFactory);
		PhysicsManager::getInstance()->setJointFactory(bulletJointFactory);
		PhysicsManager::getInstance()->setShapeFactory(bulletShapeFactory);
	}

	bulletMaterialFactory = new BulletMaterialFactory();
	if (resource::ResourceManager::getInstance() != nullptr)
		resource::ResourceManager::getInstance()->registerResourceFactory(resource::RESOURCE_TYPE_PHYSICS_MATERIAL, bulletMaterialFactory);

}

extern "C" void BULLET_PUBLIC_EXPORT unloadPlugin()
{
	if (PhysicsManager::getInstance() != nullptr)
	{
		PhysicsManager::getInstance()->removeSystemDriver();
		PhysicsManager::getInstance()->removeDefaultBodyFactory();
		PhysicsManager::getInstance()->removeJointFactory();
		PhysicsManager::getInstance()->removeShapeFactory();
	}

	if (resource::ResourceManager::getInstance() != nullptr)
		resource::ResourceManager::getInstance()->removeResourceFactory(resource::RESOURCE_TYPE_PHYSICS_MATERIAL);

	SAFE_DELETE(bulletBodyFactory);
	SAFE_DELETE(bulletShapeFactory);
	SAFE_DELETE(bulletJointFactory);
	SAFE_DELETE(bulletMaterialFactory);
	SAFE_DELETE(bulletPhysicsDriver);
}

} // end namespace game