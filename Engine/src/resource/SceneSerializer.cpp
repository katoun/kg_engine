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

#include <resource/SceneSerializer.h>
#include <resource/ResourceManager.h>
#include <core/Log.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <game/Scene.h>
#include <game/GameObject.h>
#include <game/Component.h>
#include <game/ComponentDefines.h>
#include <game/Transform.h>
#include <game/GameManager.h>
#include <render/Light.h>
#include <render/Camera.h>
#include <render/Model.h>
#include <physics/Body.h>
#include <sound/Sound.h>
#include <sound/Listener.h>

#include <tinyxml2.h>
#include <string>

namespace resource
{

game::ComponentType convertComponentType(const std::string& param)
{
	if (param == "transform")
		return game::COMPONENT_TYPE_TRANSFORM;
	else if (param == "light")
		return game::COMPONENT_TYPE_LIGHT;
	else if (param == "camera")
		return game::COMPONENT_TYPE_CAMERA;
	else if (param == "model")
		return game::COMPONENT_TYPE_MODEL;
	else if (param == "body")
		return game::COMPONENT_TYPE_BODY;
	else if (param == "joint")
		return game::COMPONENT_TYPE_JOINT;
	else if (param == "sound")
		return game::COMPONENT_TYPE_SOUND;
	else if (param == "listener")
		return game::COMPONENT_TYPE_LISTENER;
	else
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("SceneSerializer", "Invalid component type, using default.", core::LOG_LEVEL_ERROR);

		return game::COMPONENT_TYPE_UNDEFINED;
	}
}

render::LightType convertLightType(const std::string& param)
{
	if (param == "point")
		return render::LIGHT_TYPE_POINT;
	else if (param == "directional")
		return render::LIGHT_TYPE_DIRECTIONAL;
	else if (param == "spotlight")
		return render::LIGHT_TYPE_SPOTLIGHT;
	else
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("SceneSerializer", "Invalid light type, using default.", core::LOG_LEVEL_ERROR);

		return render::LIGHT_TYPE_POINT;
	}
}

SceneSerializer::SceneSerializer()
{
	// Version number
	mVersion = "[SceneSerializer_v1.00]";
}

SceneSerializer::~SceneSerializer() {}

bool SceneSerializer::importResource(Resource* dest, const std::string& filename)
{
	assert(dest != nullptr);
	if (dest == nullptr)
		return false;

	if (dest->getResourceType() != RESOURCE_TYPE_SCENE)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("SceneSerializer", "Unable to load scene - invalid resource pointer.", core::LOG_LEVEL_ERROR);
		return false;
	}

	if (resource::ResourceManager::getInstance() == nullptr)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("SceneSerializer", "Unable to load scene - resources data path not set.", core::LOG_LEVEL_ERROR);
		return false;
	}

	std::string filePath = resource::ResourceManager::getInstance()->getDataPath() + "/" + filename;

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS)
		return false;

	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("scene");
	if (pRoot != nullptr)
	{
		importGameObjects(pRoot);
	}

	return true;
}

bool SceneSerializer::exportResource(Resource* source, const std::string& filename)
{
	return false;
}

void SceneSerializer::importGameObjects(tinyxml2::XMLElement* rootElement, game::GameObject* parent)
{
	if (rootElement == nullptr)
		return;

	if (game::GameManager::getInstance() == nullptr)
		return;

	double dvalue = 0.0;
	const char* svalue;
	tinyxml2::XMLElement* pElement = nullptr;
	tinyxml2::XMLElement* pSubElement = nullptr;
	tinyxml2::XMLElement* pComponentSubElement = nullptr;
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 1;
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;

	pElement = rootElement->FirstChildElement("game_object");
	while (pElement != nullptr)
	{
		game::GameObject* pGameObject = nullptr;

		svalue = pElement->Attribute("name");
		if (svalue != nullptr)
		{
			pGameObject = game::GameManager::getInstance()->createGameObject(svalue);
		}
		else
		{
			pGameObject = game::GameManager::getInstance()->createGameObject();
		}

		if (pGameObject != nullptr)
		{
			pGameObject->setParent(parent);

			pSubElement = pElement->FirstChildElement("component");
			while (pSubElement != nullptr)
			{
				svalue = pSubElement->Attribute("type");
				if (svalue != nullptr)
				{
					game::ComponentType componentType = convertComponentType(svalue);

					game::Component* pComponent = game::GameManager::getInstance()->createComponent(componentType);
					pGameObject->attachComponent(pComponent);

					switch(componentType)
					{
					case game::COMPONENT_TYPE_TRANSFORM:
						{
							game::Transform* pTransform = static_cast<game::Transform*>(pComponent);
							if (pTransform == nullptr)
								break;

							pComponentSubElement = pSubElement->FirstChildElement("position");
							if (pComponentSubElement != nullptr)
							{
								x = 0;
								y = 0;
								z = 0;

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

								pTransform->setPosition(x, y, z);
							}

							pComponentSubElement = pSubElement->FirstChildElement("orientation");
							if (pComponentSubElement != nullptr)
							{
								x = 1;
								y = 1;
								z = 1;
								w = 1;

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

								if (pElement->QueryDoubleAttribute("w", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									w = (float)dvalue;
								}

								pTransform->setOrientation(x, y, z, w);
							}

							pComponentSubElement = pSubElement->FirstChildElement("scale");
							if (pComponentSubElement != nullptr)
							{
								x = 1;
								y = 1;
								z = 1;

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

								pTransform->setScale(x, y, z);
							}
						}
						break;
					case game::COMPONENT_TYPE_LIGHT:
						{
							render::Light* pLight = static_cast<render::Light*>(pComponent);
							if (pLight == nullptr)
								break;

							pComponentSubElement = pSubElement->FirstChildElement("type");
							if (pComponentSubElement != nullptr)
							{
								svalue = pComponentSubElement->Attribute("value");
								if (svalue != nullptr)
								{
									render::LightType lightType = convertLightType(svalue);
									pLight->setLightType(lightType);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("ambient_color");
							if (pComponentSubElement != nullptr)
							{
								r = 0;
								g = 0;
								b = 0;
								a = 1;

								if (pComponentSubElement->QueryDoubleAttribute("r", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									r = (float)dvalue;
								}

								if (pComponentSubElement->QueryDoubleAttribute("g", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									g = (float)dvalue;
								}

								if (pComponentSubElement->QueryDoubleAttribute("b", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									b = (float)dvalue;
								}

								if (pComponentSubElement->QueryDoubleAttribute("a", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									a = (float)dvalue;
								}

								pLight->setAmbientColor(r,g,b,a);
							}

							pComponentSubElement = pSubElement->FirstChildElement("diffuse_color");
							if (pComponentSubElement != nullptr)
							{
								r = 0;
								g = 0;
								b = 0;
								a = 1;

								if (pComponentSubElement->QueryDoubleAttribute("r", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									r = (float)dvalue;
								}

								if (pComponentSubElement->QueryDoubleAttribute("g", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									g = (float)dvalue;
								}

								if (pComponentSubElement->QueryDoubleAttribute("b", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									b = (float)dvalue;
								}

								if (pComponentSubElement->QueryDoubleAttribute("a", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									a = (float)dvalue;
								}

								pLight->setDiffuseColor(r,g,b,a);
							}

							pComponentSubElement = pSubElement->FirstChildElement("specular_color");
							if (pComponentSubElement != nullptr)
							{
								r = 0;
								g = 0;
								b = 0;
								a = 1;

								if (pComponentSubElement->QueryDoubleAttribute("r", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									r = (float)dvalue;
								}

								if (pComponentSubElement->QueryDoubleAttribute("g", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									g = (float)dvalue;
								}

								if (pComponentSubElement->QueryDoubleAttribute("b", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									b = (float)dvalue;
								}

								if (pComponentSubElement->QueryDoubleAttribute("a", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									a = (float)dvalue;
								}

								pLight->setSpecularColor(r,g,b,a);
							}

							pComponentSubElement = pSubElement->FirstChildElement("attenuation_range");
							if (pComponentSubElement != nullptr)
							{
								if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									pLight->setAttenuationRange((float)dvalue);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("attenuation_constant");
							if (pComponentSubElement != nullptr)
							{
								if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									pLight->setAttenuationConstant((float)dvalue);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("attenuation_linear");
							if (pComponentSubElement != nullptr)
							{
								if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									pLight->setAttenuationLinear((float)dvalue);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("attenuation_quadratic");
							if (pComponentSubElement != nullptr)
							{
								if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									pLight->setAttenuationQuadric((float)dvalue);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("spotlight_inner_angle");
							if (pComponentSubElement != nullptr)
							{
								if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									pLight->setSpotlightInnerAngle((float)dvalue);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("spotlight_outer_angle");
							if (pComponentSubElement != nullptr)
							{
								if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									pLight->setSpotlightOuterAngle((float)dvalue);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("spotlight_falloff");
							if (pComponentSubElement != nullptr)
							{
								if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									pLight->setSpotlightFalloff((float)dvalue);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("power_scale");
							if (pComponentSubElement != nullptr)
							{
								if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									pLight->setPowerScale((float)dvalue);
								}
							}
						}
						break;
					case game::COMPONENT_TYPE_CAMERA:
						{
							render::Camera* pCamera = static_cast<render::Camera*>(pComponent);
							if (pCamera == nullptr)
								break;

							pComponentSubElement = pSubElement->FirstChildElement("fov");
							if (pComponentSubElement != nullptr)
							{
								if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									pCamera->setFOV((float)dvalue);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("near_clip_distance");
							if (pComponentSubElement != nullptr)
							{
								if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									pCamera->setNearClipDistance((float)dvalue);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("far_clip_distance");
							if (pComponentSubElement != nullptr)
							{
								if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									pCamera->setFarClipDistance((float)dvalue);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("aspect_ratio");
							if (pComponentSubElement != nullptr)
							{
								if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
								{
									pCamera->setAspectRatio((float)dvalue);
								}
							}
						}
						break;
					case game::COMPONENT_TYPE_MODEL:
						{
							render::Model* pModel = static_cast<render::Model*>(pComponent);
							if (pModel == nullptr)
								break;

							pComponentSubElement = pSubElement->FirstChildElement("resource");
							if (pComponentSubElement != nullptr)
							{
								svalue = pComponentSubElement->Attribute("value");
								if (svalue != nullptr)
								{
									pModel->setMeshData(svalue);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("material");
							if (pComponentSubElement != nullptr)
							{
								svalue = pComponentSubElement->Attribute("value");
								if (svalue != nullptr)
								{
									pModel->setMaterial(svalue);
								}
							}
						}
						break;
					case game::COMPONENT_TYPE_BODY:
						{
							physics::Body* pBody = static_cast<physics::Body*>(pComponent);
							if (pBody == nullptr)
								break;

							pComponentSubElement = pSubElement->FirstChildElement("resource");
							if (pComponentSubElement != nullptr)
							{
								svalue = pComponentSubElement->Attribute("value");
								if (svalue != nullptr)
								{
									pBody->setBodyData(svalue);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("material");
							if (pComponentSubElement != nullptr)
							{
								svalue = pComponentSubElement->Attribute("value");
								if (svalue != nullptr)
								{
									pBody->setMaterial(svalue);
								}
							}

							pComponentSubElement = pSubElement->FirstChildElement("enabled");
							if (pComponentSubElement != nullptr)
							{
								svalue = pComponentSubElement->Attribute("value");
								if (svalue != nullptr)
								{
									pBody->setEnabled((std::string(svalue) == "true") ? true : false);
								}
							}
						}
						break;
					case game::COMPONENT_TYPE_JOINT:
						{
						}
						break;
					case game::COMPONENT_TYPE_SOUND:
						{
						}
						break;
					case game::COMPONENT_TYPE_LISTENER:
						{
						}
						break;
					}
				}

				pSubElement = pSubElement->NextSiblingElement("component");
			}

			//child game objects
			importGameObjects(pElement, pGameObject);
		}

		pElement = pElement->NextSiblingElement("game_object");
	}
}

}// end namespace resource
