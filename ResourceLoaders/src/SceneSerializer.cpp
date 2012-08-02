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
#include <resource/ResourceManager.h>
#include <SceneSerializer.h>

#include <Poco/AutoPtr.h>
#include <Poco/Path.h>
#include <Poco/Util/XMLConfiguration.h>

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
		core::Log::getInstance().logMessage("SceneSerializer", "Invalid component type, using default.", core::LOG_LEVEL_ERROR);

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
		core::Log::getInstance().logMessage("SceneSerializer", "Invalid light type, using default.", core::LOG_LEVEL_ERROR);

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
	assert(dest != NULL);
	if (dest == NULL)
		return false;

	if (dest->getResourceType() != RESOURCE_TYPE_SCENE)
	{
		core::Log::getInstance().logMessage("SceneSerializer", "Unable to load scene - invalid resource pointer.", core::LOG_LEVEL_ERROR);
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	Poco::Path path(resource::ResourceManager::getInstance().getDataPath());
	path.append(filename);
	std::string filePath = path.toString();

	Poco::AutoPtr<Poco::Util::XMLConfiguration> pConf(new Poco::Util::XMLConfiguration());
	try
	{
		pConf->load(filePath);
	}
	catch(...)
	{
		return false;
	}

	importGameObjects(pConf);

	//////////////////////////////////////////////////////////////////////////

	return true;
}

bool SceneSerializer::exportResource(Resource* source, const std::string& filename)
{
	return false;
}

void SceneSerializer::importGameObjects(Poco::AutoPtr<Poco::Util::XMLConfiguration>& config, std::string key, game::GameObject* parent)
{
	//game objects
	unsigned int i = 0;
	std::string gkey = key == core::STRING_BLANK ? "game_object[" + core::intToString(i) + "]" : key + ".game_object[" + core::intToString(i) + "]";
	while (config->has(gkey))
	{
		game::GameObject* pGameObject = NULL;
		if (config->has(gkey + "[@name]"))
		{
			std::string name = config->getString(gkey + "[@name]");
			pGameObject = game::GameManager::getInstance().createGameObject(name);
		}
		else
		{
			pGameObject = game::GameManager::getInstance().createGameObject();
		}

		if (pGameObject != NULL)
		{
			if (parent != NULL)
			{
				pGameObject->setParent(parent);
			}
			
			//components
			unsigned int j = 0;
			std::string ckey = gkey + ".component[" + core::intToString(j) + "]";
			while (config->has(ckey))
			{
				if (config->has(ckey + "[@type]"))
				{
					std::string type = config->getString(ckey + "[@type]");

					game::ComponentType componentType = convertComponentType(type);

					game::Component* pComponent = game::GameManager::getInstance().createComponent(componentType);
					pGameObject->attachComponent(pComponent);

					switch(componentType)
					{
					case game::COMPONENT_TYPE_TRANSFORM:
						{
							game::Transform* pTransform = static_cast<game::Transform*>(pComponent);
							if (pTransform == NULL)
								break;

							if (config->has(ckey + ".position"))
							{
								std::string xkey = ckey + ".position[@x]";
								std::string ykey = ckey + ".position[@y]";
								std::string zkey = ckey + ".position[@z]";

								float x = 0.0f;
								float y = 0.0f;
								float z = 0.0f;

								if (config->has(xkey))
									x = (float)config->getDouble(xkey);
								if (config->has(ykey))
									y = (float)config->getDouble(ykey);
								if (config->has(zkey))
									z = (float)config->getDouble(zkey);

								pTransform->setPosition(x, y, z);
							}

							if (config->has(ckey + ".orientation"))
							{
								std::string xkey = ckey + ".orientation[@x]";
								std::string ykey = ckey + ".orientation[@y]";
								std::string zkey = ckey + ".orientation[@z]";
								std::string wkey = ckey + ".orientation[@w]";

								float x = 0.0f;
								float y = 0.0f;
								float z = 0.0f;
								float w = 0.0f;

								if (config->has(xkey))
									x = (float)config->getDouble(xkey);
								if (config->has(ykey))
									y = (float)config->getDouble(ykey);
								if (config->has(zkey))
									z = (float)config->getDouble(zkey);
								if (config->has(wkey))
									w = (float)config->getDouble(wkey);

								pTransform->setOrientation(x, y, z, w);
							}

							if (config->has(ckey + ".scale"))
							{
								std::string xkey = ckey + ".scale[@x]";
								std::string ykey = ckey + ".scale[@y]";
								std::string zkey = ckey + ".scale[@z]";

								float x = 0.0f;
								float y = 0.0f;
								float z = 0.0f;

								if (config->has(xkey))
									x = (float)config->getDouble(xkey);
								if (config->has(ykey))
									y = (float)config->getDouble(ykey);
								if (config->has(zkey))
									z = (float)config->getDouble(zkey);

								pTransform->setScale(x, y, z);
							}
						}
						break;
					case game::COMPONENT_TYPE_LIGHT:
						{
							render::Light* pLight = static_cast<render::Light*>(pComponent);
							if (pLight == NULL)
								break;

							if (config->has(ckey + ".type[@value]"))
							{
								std::string type = config->getString(ckey + ".type[@value]");

								render::LightType lightType = convertLightType(type);

								pLight->setLightType(lightType);
								
								if (config->has(ckey + ".ambient_color"))
								{
									std::string rkey = ckey + ".ambient_color[@r]";
									std::string gkey = ckey + ".ambient_color[@g]";
									std::string bkey = ckey + ".ambient_color[@b]";
									std::string akey = ckey + ".ambient_color[@a]";
									
									float r = 1.0f;
									float g = 1.0f;
									float b = 1.0f;
									float a = 1.0f;
									if (config->has(rkey))
										r = (float)config->getDouble(rkey);
									if (config->has(gkey))
										g = (float)config->getDouble(gkey);
									if (config->has(bkey))
										b = (float)config->getDouble(bkey);
									if (config->has(akey))
										a = (float)config->getDouble(akey);
									
									pLight->setAmbientColor(render::Color(r,g,b,a));
								}

								if (config->has(ckey + ".diffuse_color"))
								{
									std::string rkey = ckey + ".diffuse_color[@r]";
									std::string gkey = ckey + ".diffuse_color[@g]";
									std::string bkey = ckey + ".diffuse_color[@b]";
									std::string akey = ckey + ".diffuse_color[@a]";
									
									float r = 1.0f;
									float g = 1.0f;
									float b = 1.0f;
									float a = 1.0f;
									if (config->has(rkey))
										r = (float)config->getDouble(rkey);
									if (config->has(gkey))
										g = (float)config->getDouble(gkey);
									if (config->has(bkey))
										b = (float)config->getDouble(bkey);
									if (config->has(akey))
										a = (float)config->getDouble(akey);
									
									pLight->setDiffuseColor(render::Color(r,g,b,a));
								}

								if (config->has(ckey + ".specular_color"))
								{
									std::string rkey = ckey + ".specular_color[@r]";
									std::string gkey = ckey + ".specular_color[@g]";
									std::string bkey = ckey + ".specular_color[@b]";
									std::string akey = ckey + ".specular_color[@a]";
									
									float r = 1.0f;
									float g = 1.0f;
									float b = 1.0f;
									float a = 1.0f;
									if (config->has(rkey))
										r = (float)config->getDouble(rkey);
									if (config->has(gkey))
										g = (float)config->getDouble(gkey);
									if (config->has(bkey))
										b = (float)config->getDouble(bkey);
									if (config->has(akey))
										a = (float)config->getDouble(akey);
									
									pLight->setSpecularColor(render::Color(r,g,b,a));
								}

								if (config->has(ckey + ".attenuation_range[@value]"))
								{
									float range = (float)config->getDouble(ckey + ".attenuation_range[@value]");

									pLight->setAttenuationRange(range);
								}

								if (config->has(ckey + ".attenuation_constant[@value]"))
								{
									float constant = (float)config->getDouble(ckey + ".attenuation_constant[@value]");

									pLight->setAttenuationConstant(constant);
								}

								if (config->has(ckey + ".attenuation_linear[@value]"))
								{
									float linear = (float)config->getDouble(ckey + ".attenuation_linear[@value]");

									pLight->setAttenuationLinear(linear);
								}

								if (config->has(ckey + ".attenuation_quadratic[@value]"))
								{
									float quadratic = (float)config->getDouble(ckey + ".attenuation_quadratic[@value]");

									pLight->setAttenuationQuadric(quadratic);
								}

								if (config->has(ckey + ".spotlight_inner_angle[@value]"))
								{
									float innerAngle = (float)config->getDouble(ckey + ".spotlight_inner_angle[@value]");

									pLight->setSpotlightInnerAngle(innerAngle);
								}

								if (config->has(ckey + ".spotlight_outer_angle[@value]"))
								{
									float outerAngle = (float)config->getDouble(ckey + ".spotlight_outer_angle[@value]");

									pLight->setSpotlightOuterAngle(outerAngle);
								}

								if (config->has(ckey + ".spotlight_falloff[@value]"))
								{
									float falloff = (float)config->getDouble(ckey + ".spotlight_falloff[@value]");

									pLight->setSpotlightFalloff(falloff);
								}

								if (config->has(ckey + ".power_scale[@value]"))
								{
									float scale = (float)config->getDouble(ckey + ".power_scale[@value]");

									pLight->setPowerScale(scale);
								}
							}
						}
						break;
					case game::COMPONENT_TYPE_CAMERA:
						{
							render::Camera* pCamera = static_cast<render::Camera*>(pComponent);
							if (pCamera == NULL)
								break;

							if (config->has(ckey + ".fov[@value]"))
							{
								float fov = (float)config->getDouble(ckey + ".fov[@value]");
								pCamera->setFOV(fov);
							}

							if (config->has(ckey + ".near_clip_distance[@value]"))
							{
								float distance = (float)config->getDouble(ckey + ".near_clip_distance[@value]");
								pCamera->setNearClipDistance(distance);
							}

							if (config->has(ckey + ".far_clip_distance[@value]"))
							{
								float distance = (float)config->getDouble(ckey + ".far_clip_distance[@value]");
								pCamera->setFarClipDistance(distance);
							}

							if (config->has(ckey + ".aspect_ratio[@value]"))
							{
								float ratio = (float)config->getDouble(ckey + ".aspect_ratio[@value]");
								pCamera->setAspectRatio(ratio);
							}
						}
						break;
					case game::COMPONENT_TYPE_MODEL:
						{
							render::Model* pModel = static_cast<render::Model*>(pComponent);
							if (pModel == NULL)
								break;

							if (config->has(ckey + ".resource[@value]"))
							{
								std::string resource = config->getString(ckey + ".resource[@value]");

								pModel->setMeshData(resource);
							}

							if (config->has(ckey + ".material[@value]"))
							{
								std::string material = config->getString(ckey + ".material[@value]");

								pModel->setMaterial(material);
							}
						}
						break;
					case game::COMPONENT_TYPE_BODY:
						{
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

				ckey = gkey + "component[" + core::intToString(++j) + "]";
			}

			//child game objects
			importGameObjects(config, gkey, pGameObject);
		}

		gkey = key == core::STRING_BLANK ? "game_object[" + core::intToString(++i) + "]" : key + ".game_object[" + core::intToString(++i) + "]";
	}
}

}// end namespace resource
