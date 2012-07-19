#include <physics/BodyData.h>
#include <physics/Material.h>
#include <physics/Shape.h>
#include <physics/PhysicsManager.h>

namespace physics
{

BodyData::BodyData(const std::string& name, resource::Serializer* serializer): resource::Resource(name, serializer)
{
	mResourceType = resource::RESOURCE_TYPE_BODY_DATA;

	mBodyType = BT_STATIC;

	mMaterial = NULL;

	mMass = 1.0f;
	mSleeping = false;
	mSleepiness = 0.0f;
	mLinearDamping = 0.0f;
	mAngularDamping = 0.0f;
	mLinearVelocity = core::vector3d::ORIGIN_3D;
	mAngularVelocity = core::vector3d::ORIGIN_3D;
}

BodyData::~BodyData() {}

void BodyData::setBodyType(BodyType type)
{
	mBodyType = type;
}

const BodyType& BodyData::getBodyType() const
{
	return mBodyType;
}

void BodyData::setSleeping(bool sleeping)
{
	mSleeping = sleeping;
}

bool BodyData::getSleeping()
{
	return mSleeping;
}

void BodyData::setSleepiness(float sleepiness)
{
	mSleepiness = sleepiness;
}

float BodyData::getSleepiness()
{
	return mSleepiness;
}

void BodyData::setMass(float mass)
{
	mMass = mass;
}

float BodyData::getMass()
{
	return mMass;
}

void BodyData::setLinearDamping(float linearDamping)
{
	assert(linearDamping >= 0.0f);

	mLinearDamping = linearDamping;
}

float BodyData::getLinearDamping()
{
	return mLinearDamping;
}

void BodyData::setAngularDamping(float angularDamping)
{
	assert(angularDamping >= 0.0f);

	mAngularDamping = angularDamping;
}

float BodyData::getAngularDamping()
{
	return mAngularDamping;
}

void BodyData::setLinearVelocity(const core::vector3d& velocity)
{
	mLinearVelocity = velocity;
}

const core::vector3d& BodyData::getLinearVelocity() const
{
	return mLinearVelocity;
}

void BodyData::setAngularVelocity(const core::vector3d& velocity)
{
	mAngularVelocity = velocity;
}

const core::vector3d& BodyData::getAngularVelocity() const
{
	return mAngularVelocity;
}

void BodyData::setMaterial(const std::string& filename)
{
	std::string materialname = filename;
	mMaterial = PhysicsManager::getInstance().createMaterial(materialname);
}

Material* BodyData::getMaterial()
{
	return mMaterial;
}

void BodyData::addShape(Shape* shape)
{
	if (shape == NULL)
		return;

	mShapes.push_back(shape);
}

std::list<Shape*>& BodyData::getShapes()
{
	return mShapes;
}

void BodyData::removeAllShapes()
{
	std::list<Shape*>::const_iterator i;
	for (i = mShapes.begin(); i != mShapes.end(); ++i)
		delete (*i);

	mShapes.clear();
}

void BodyData::unloadImpl()
{
	// Remove all Shapes
	removeAllShapes();

	mMass = 1.0f;
	mBodyType = BT_STATIC;
	mSleeping = false;
	mSleepiness = 0.0f;
	mLinearDamping = 0.0f;
	mAngularDamping = 0.0f;
	mLinearVelocity = core::vector3d::ORIGIN_3D;
	mAngularVelocity = core::vector3d::ORIGIN_3D;
	mMaterial = NULL;
}

} // end namespace physics
