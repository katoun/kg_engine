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

PhysicsDriver* bulletPhysicsDriver = NULL;
BodyFactory* bulletBodyFactory = NULL;
ShapeFactory* bulletShapeFactory = NULL;
JointFactory* bulletJointFactory = NULL;
resource::ResourceFactory* bulletMaterialFactory = NULL;

extern "C" void BULLET_PUBLIC_EXPORT loadPlugin() throw()
{
	bulletPhysicsDriver = BulletPhysicsDriver::getInstancePtr();
	bulletBodyFactory = new BulletBodyFactory();
	bulletShapeFactory = new BulletShapeFactory();
	bulletJointFactory = new BulletJointFactory();
	PhysicsManager::getInstance().setSystemDriver(bulletPhysicsDriver);
	PhysicsManager::getInstance().setBodyFactory(bulletBodyFactory);
	PhysicsManager::getInstance().setShapeFactory(bulletShapeFactory);
	PhysicsManager::getInstance().setJointFactory(bulletJointFactory);

	bulletMaterialFactory = new BulletMaterialFactory();
	resource::ResourceManager::getInstance().registerResourceFactory(resource::RESOURCE_TYPE_PHYSICS_MATERIAL, bulletMaterialFactory);

}

extern "C" void BULLET_PUBLIC_EXPORT unloadPlugin()
{
	PhysicsManager::getInstance().removeSystemDriver();
	PhysicsManager::getInstance().removeBodyFactory();
	PhysicsManager::getInstance().removeShapeFactory();
	PhysicsManager::getInstance().removeJointFactory();

	resource::ResourceManager::getInstance().removeResourceFactory(resource::RESOURCE_TYPE_PHYSICS_MATERIAL);

	if (bulletBodyFactory != NULL)
		delete bulletBodyFactory;
	if (bulletShapeFactory != NULL)
		delete bulletShapeFactory;
	if (bulletJointFactory != NULL)
		delete bulletJointFactory;
	if (bulletMaterialFactory != NULL)
		delete bulletMaterialFactory;
}

} // end namespace game