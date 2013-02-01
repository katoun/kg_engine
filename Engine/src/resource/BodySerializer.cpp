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

#include <core/Log.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <resource/ResourceManager.h>
#include <physics/BodyData.h>
#include <physics/Shape.h>
#include <physics/PhysicsManager.h>
#include <resource/BodySerializer.h>

#include <tinyxml2.h>

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

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS)
		return false;

	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("body");
	if (pRoot != nullptr)
	{
		double dvalue = 0.0;
		const char* svalue;
		tinyxml2::XMLElement* pElement = nullptr;

		std::string type = "static";
		pElement = pRoot->FirstChildElement("type");
		if (pElement != nullptr)
		{
			svalue = pElement->Attribute("value");
			if (svalue != nullptr)
			{
				type = svalue;
			}
		}

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

		pElement = pRoot->FirstChildElement("mass");
		if (pElement != nullptr)
		{
			if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
			{
				pBodyData->setMass((float)dvalue);
			}
		}

		pElement = pRoot->FirstChildElement("linear_damping");
		if (pElement != nullptr)
		{
			if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
			{
				pBodyData->setLinearDamping((float)dvalue);
			}
		}

		pElement = pRoot->FirstChildElement("angular_damping");
		if (pElement != nullptr)
		{
			if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
			{
				pBodyData->setAngularDamping((float)dvalue);
			}
		}

		pElement = pRoot->FirstChildElement("linear_velocity");
		if (pElement != nullptr)
		{
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;

			if (pElement->QueryDoubleAttribute("x", &dvalue) == tinyxml2::XML_SUCCESS)
			{
				x = (float)dvalue;
			}

			if (pElement->QueryDoubleAttribute("y", &dvalue) == tinyxml2::XML_SUCCESS)
			{
				y = (float)dvalue;
			}

			if (pElement->QueryDoubleAttribute("z", &dvalue) == tinyxml2::XML_SUCCESS)
			{
				z = (float)dvalue;
			}

			pBodyData->setLinearVelocity(core::vector3d(x, y, z));
		}

		pElement = pRoot->FirstChildElement("angular_velocity");
		if (pElement != nullptr)
		{
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;

			if (pElement->QueryDoubleAttribute("x", &dvalue) == tinyxml2::XML_SUCCESS)
			{
				x = (float)dvalue;
			}

			if (pElement->QueryDoubleAttribute("y", &dvalue) == tinyxml2::XML_SUCCESS)
			{
				y = (float)dvalue;
			}

			if (pElement->QueryDoubleAttribute("z", &dvalue) == tinyxml2::XML_SUCCESS)
			{
				z = (float)dvalue;
			}

			pBodyData->setAngularVelocity(core::vector3d(x, y, z));
		}

		pElement = pRoot->FirstChildElement("material");
		if (pElement != nullptr)
		{
			svalue = pElement->Attribute("value");
			if (svalue != nullptr)
			{
				std::string material = svalue;
				pBodyData->setMaterial(material);
			}
		}

		pElement = pRoot->FirstChildElement("shape");
		if (pElement != nullptr)
		{
			tinyxml2::XMLElement* pSubElement = nullptr;

			pSubElement = pElement->FirstChildElement("type");
			if (pSubElement != nullptr)
			{
				svalue = pSubElement->Attribute("value");
				if (svalue != nullptr)
				{
					type = svalue;
				}
			}

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

				if (pShape != nullptr)
				{
					pBodyData->addShape(pShape);

					pSubElement = pElement->FirstChildElement("position");
					if (pSubElement != nullptr)
					{
						float x = 0.0f;
						float y = 0.0f;
						float z = 0.0f;

						if (pSubElement->QueryDoubleAttribute("x", &dvalue) == tinyxml2::XML_SUCCESS)
						{
							x = (float)dvalue;
						}

						if (pSubElement->QueryDoubleAttribute("y", &dvalue) == tinyxml2::XML_SUCCESS)
						{
							y = (float)dvalue;
						}

						if (pSubElement->QueryDoubleAttribute("z", &dvalue) == tinyxml2::XML_SUCCESS)
						{
							z = (float)dvalue;
						}

						pShape->setPosition(core::vector3d(x, y, z));
					}

					pSubElement = pElement->FirstChildElement("orientation");
					if (pSubElement != nullptr)
					{
						float x = 0.0f;
						float y = 0.0f;
						float z = 0.0f;

						if (pSubElement->QueryDoubleAttribute("x", &dvalue) == tinyxml2::XML_SUCCESS)
						{
							x = (float)dvalue;
						}

						if (pSubElement->QueryDoubleAttribute("y", &dvalue) == tinyxml2::XML_SUCCESS)
						{
							y = (float)dvalue;
						}

						if (pSubElement->QueryDoubleAttribute("z", &dvalue) == tinyxml2::XML_SUCCESS)
						{
							z = (float)dvalue;
						}

						pShape->setOrientation(core::quaternion(x, y, z));
					}

					pSubElement = pElement->FirstChildElement("dimension");
					if (pSubElement != nullptr)
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

									if (pSubElement->QueryDoubleAttribute("x", &dvalue) == tinyxml2::XML_SUCCESS)
									{
										x = (float)dvalue;
									}

									if (pSubElement->QueryDoubleAttribute("y", &dvalue) == tinyxml2::XML_SUCCESS)
									{
										y = (float)dvalue;
									}

									if (pSubElement->QueryDoubleAttribute("z", &dvalue) == tinyxml2::XML_SUCCESS)
									{
										z = (float)dvalue;
									}

									if (pSubElement->QueryDoubleAttribute("d", &dvalue) == tinyxml2::XML_SUCCESS)
									{
										d = (float)dvalue;
									}

									planeShape->setDimension(core::vector3d(x, y, z), d);
								}
							}
							break;
						case physics::SHAPE_TYPE_SPHERE:
							{
								physics::SphereShape* sphereShape = static_cast<physics::SphereShape*>(pShape);
								if (sphereShape != nullptr)
								{
									if (pSubElement->QueryDoubleAttribute("radius", &dvalue) == tinyxml2::XML_SUCCESS)
									{
										sphereShape->setDimension((float)dvalue);
									}
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

									if (pSubElement->QueryDoubleAttribute("x", &dvalue) == tinyxml2::XML_SUCCESS)
									{
										x = (float)dvalue;
									}

									if (pSubElement->QueryDoubleAttribute("y", &dvalue) == tinyxml2::XML_SUCCESS)
									{
										y = (float)dvalue;
									}

									if (pSubElement->QueryDoubleAttribute("z", &dvalue) == tinyxml2::XML_SUCCESS)
									{
										z = (float)dvalue;
									}

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
	}

	return true;
}

bool BodySerializer::exportResource(Resource* source, const std::string& filename)
{
	return false;
}

}// end namespace resource
