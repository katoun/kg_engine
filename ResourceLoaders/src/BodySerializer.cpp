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

#include <core/Log.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <resource/ResourceManager.h>
#include <physics/BodyData.h>
#include <physics/Shape.h>
#include <physics/PhysicsManager.h>
#include <BodySerializer.h>

#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>

#include <string>

namespace resource
{

BodySerializer::BodySerializer()
{
	// Version number
	mVersion = "[BodySerializer_v1.00]";
}

BodySerializer::~BodySerializer() {}

bool BodySerializer::importResource(Resource* dest, const std::string& filename)
{
	assert(dest != nullptr);
	if (dest == nullptr)
		return false;

	if (dest->getResourceType() != RESOURCE_TYPE_BODY_DATA)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("BodySerializer", "Unable to load body - invalid resource pointer.", core::LOG_LEVEL_ERROR);
		return false;
	}

	physics::BodyData* pBodyData = static_cast<physics::BodyData*>(dest);
	assert(pBodyData != nullptr);
	if (pBodyData == nullptr) return false;

	if (resource::ResourceManager::getInstance() == nullptr)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("BodySerializer", "Unable to load body - resources data path not set.", core::LOG_LEVEL_ERROR);
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string filePath = resource::ResourceManager::getInstance()->getDataPath() + "/" + filename;

	Poco::AutoPtr<Poco::Util::XMLConfiguration> pConf(new Poco::Util::XMLConfiguration());
	try
	{
		pConf->load(filePath);
	}
	catch(...)
	{
		return false;
	}

	if (pConf->has("type"))
	{
		std::string type;
		if (pConf->has("type[@value]"))
			type = pConf->getString("type[@value]");

		if (type == "dynamic")
			pBodyData->setBodyType(physics::BT_DYNAMIC);
		else if (type == "static")
			pBodyData->setBodyType(physics::BT_STATIC);
		else if (type == "kinematic")
			pBodyData->setBodyType(physics::BT_KINEMATIC);
		else
		{
			if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("BodySerializer", "Bad body type attribute, valid parameters are 'dynamic', 'static' or 'kinematic'.", core::LOG_LEVEL_ERROR);
			return false;
		}

		if (pConf->has("mass"))
		{
			float mass = 0.0f;
			if (pConf->has("mass[@value]"))
				mass = (float)pConf->getDouble("mass[@value]");
			pBodyData->setMass(mass);
		}

		if (pConf->has("linear_damping"))
		{
			float linearDamping = 0.0f;
			if (pConf->has("linearDamping[@value]"))
				linearDamping = (float)pConf->getDouble("linearDamping[@value]");

			pBodyData->setLinearDamping(linearDamping);
		}

		if (pConf->has("angular_damping"))
		{
			float angularDamping = 0.0f;
			if (pConf->has("angularDamping[@value]"))
				angularDamping = (float)pConf->getDouble("angularDamping[@value]");

			pBodyData->setAngularDamping(angularDamping);
		}

		if (pConf->has("linear_velocity"))
		{
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;

			if (pConf->has("linear_velocity[@x]"))
				x = (float)pConf->getDouble("linear_velocity[@x]");
			if (pConf->has("linear_velocity[@y]"))
				y = (float)pConf->getDouble("linear_velocity[@y]");
			if (pConf->has("linear_velocity[@z]"))
				z = (float)pConf->getDouble("linear_velocity[@z]");

			pBodyData->setLinearVelocity(core::vector3d(x, y, z));
		}

		if (pConf->has("angular_velocity"))
		{
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;

			if (pConf->has("angular_velocity[@x]"))
				x = (float)pConf->getDouble("angular_velocity[@x]");
			if (pConf->has("angular_velocity[@y]"))
				y = (float)pConf->getDouble("angular_velocity[@y]");
			if (pConf->has("angular_velocity[@z]"))
				z = (float)pConf->getDouble("angular_velocity[@z]");

			pBodyData->setAngularVelocity(core::vector3d(x, y, z));
		}

		if (pConf->has("material"))
		{
			std::string material;
			if (pConf->has("material[@value]"))
				material = pConf->getString("material[@value]");

			pBodyData->setMaterial(material);
		}

		if (pConf->has("shape.type"))
		{
			std::string type;
			if (pConf->has("shape.type[@value]"))
				type = pConf->getString("shape.type[@value]");

			physics::ShapeType shapeType = physics::SHAPE_TYPE_UNDEFINED;
			if (type == "plane")
				shapeType = physics::SHAPE_TYPE_PLANE;
			else if (type == "sphere")
				shapeType = physics::SHAPE_TYPE_SPHERE;
			else if (type == "box")
				shapeType = physics::SHAPE_TYPE_BOX;
			else if (type == "capsule")
				shapeType = physics::SHAPE_TYPE_CAPSULE;
			else if (type == "convex")
				shapeType = physics::SHAPE_TYPE_CONVEX;
			else if (type == "mesh")
				shapeType = physics::SHAPE_TYPE_MESH;
			else
				if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("BodySerializer", "Bad shape type attribute, valid parameters are 'plane', 'sphere', 'box', 'capsule', 'convex', or 'mesh'.", core::LOG_LEVEL_ERROR);
	
			if (shapeType != physics::SHAPE_TYPE_UNDEFINED)
			{
				physics::Shape* pShape = nullptr;
				if (physics::PhysicsManager::getInstance() != nullptr)
					pShape = physics::PhysicsManager::getInstance()->createShape(shapeType);

				pBodyData->addShape(pShape);

				if (pConf->has("shape.position"))
				{
					float x = 0.0f;
					float y = 0.0f;
					float z = 0.0f;

					if (pConf->has("shape.position[@x]"))
						x = (float)pConf->getDouble("shape.position[@x]");
					if (pConf->has("shape.position[@y]"))
						y = (float)pConf->getDouble("shape.position[@y]");
					if (pConf->has("shape.position[@z]"))
						z = (float)pConf->getDouble("shape.position[@z]");

					pShape->setPosition(core::vector3d(x, y, z));
				}

				if (pConf->has("shape.orientation"))
				{
					float x = 0.0f;
					float y = 0.0f;
					float z = 0.0f;

					if (pConf->has("shape.position[@x]"))
						x = (float)pConf->getDouble("shape.position[@x]");
					if (pConf->has("shape.position[@y]"))
						y = (float)pConf->getDouble("shape.position[@y]");
					if (pConf->has("shape.position[@z]"))
						z = (float)pConf->getDouble("shape.position[@z]");

					pShape->setOrientation(core::quaternion(x, y, z));
				}

				
				if (pConf->has("shape.dimension"))
				{
					switch(shapeType)
					{
					case physics::SHAPE_TYPE_PLANE:
						{
							physics::PlaneShape* planeShape = static_cast<physics::PlaneShape*>(pShape);
							if (planeShape != nullptr)
							{
								float x = 0.0f;
								float y = 0.0f;
								float z = 0.0f;
								float d = 0.0f;
								if (pConf->has("shape.dimension[@x]"))
									x = (float)pConf->getDouble("shape.dimension[@x]");
								if (pConf->has("shape.dimension[@y]"))
									y = (float)pConf->getDouble("shape.dimension[@y]");
								if (pConf->has("shape.dimension[@z]"))
									z = (float)pConf->getDouble("shape.dimension[@z]");
								if (pConf->has("shape.dimension[@d]"))
									d = (float)pConf->getDouble("shape.dimension[@d]");

								planeShape->setDimension(core::vector3d(x, y, z), d);
							}
						}
						break;
					case physics::SHAPE_TYPE_SPHERE:
						{
							physics::SphereShape* sphereShape = static_cast<physics::SphereShape*>(pShape);
							if (sphereShape != nullptr)
							{
								float radius = 0.0f;
								if (pConf->has("shape.dimension[@radius]"))
									radius = (float)pConf->getDouble("shape.dimension[@radius]");

								sphereShape->setDimension(radius);
							}
						}
						break;
					case physics::SHAPE_TYPE_BOX:
						{
							physics::BoxShape* boxShape = static_cast<physics::BoxShape*>(pShape);
							if (boxShape != nullptr)
							{
								float x = 0.0f;
								float y = 0.0f;
								float z = 0.0f;
								float d = 0.0f;
								if (pConf->has("shape.dimension[@x]"))
									x = (float)pConf->getDouble("shape.dimension[@x]");
								if (pConf->has("shape.dimension[@y]"))
									y = (float)pConf->getDouble("shape.dimension[@y]");
								if (pConf->has("shape.dimension[@z]"))
									z = (float)pConf->getDouble("shape.dimension[@z]");
								if (pConf->has("shape.dimension[@d]"))
									d = (float)pConf->getDouble("shape.dimension[@d]");

								boxShape->setDimension(core::vector3d(x, y, z));
							}
						}
						break;
					case physics::SHAPE_TYPE_CAPSULE:
						{
							//katoun TODO
						}
						break;
					case physics::SHAPE_TYPE_CONVEX:
						{
							//katoun TODO
						}
						break;
					case physics::SHAPE_TYPE_MESH:
						{
							//katoun TODO
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	return true;
}

bool BodySerializer::exportResource(Resource* source, const std::string& filename)
{
	return false;
}

}// end namespace resource
