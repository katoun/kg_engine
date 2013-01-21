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
#include <render/BlendMode.h>
#include <render/TextureUnit.h>
#include <render/Shader.h>
#include <render/Material.h>
#include <physics/Material.h>
#include <MaterialSerializer.h>

#include <tinyxml2.h>

#include <string>

namespace resource
{

render::SceneBlendFactor convertBlendFactor(const std::string& param)
{
	if (param == "one")
		return render::SBF_ONE;
	else if (param == "zero")
		return render::SBF_ZERO;
	else if (param == "dest_color")
		return render::SBF_DEST_COLOR;
	else if (param == "src_color")
		return render::SBF_SOURCE_COLOR;
	else if (param == "one_minus_dest_color")
		return render::SBF_ONE_MINUS_DEST_COLOR;
	else if (param == "one_minus_src_color")
		return render::SBF_ONE_MINUS_SOURCE_COLOR;
	else if (param == "dest_alpha")
		return render::SBF_DEST_ALPHA;
	else if (param == "src_alpha")
		return render::SBF_SOURCE_ALPHA;
	else if (param == "one_minus_dest_alpha")
		return render::SBF_ONE_MINUS_DEST_ALPHA;
	else if (param == "one_minus_src_alpha")
		return render::SBF_ONE_MINUS_SOURCE_ALPHA;
	else
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("MaterialSerializer", "Invalid blend factor, using default.", core::LOG_LEVEL_ERROR);

		return render::SBF_ONE;
	}
}

render::LayerBlendOperation convertBlendOperation(const std::string& param)
{
	if (param == "source1")
		return render::LBX_SOURCE1;
	else if (param == "source2")
		return render::LBX_SOURCE2;
	else if (param == "modulate")
		return render::LBX_MODULATE;
	else if (param == "modulate_x2")
		return render::LBX_MODULATE_X2;
	else if (param == "modulate_x4")
		return render::LBX_MODULATE_X4;
	else if (param == "add")
		return render::LBX_ADD;
	else if (param == "add_signed")
		return render::LBX_ADD_SIGNED;
	else if (param == "add_smooth")
		return render::LBX_ADD_SMOOTH;
	else if (param == "subtract")
		return render::LBX_SUBTRACT;
	else if (param == "blend_diffuse_color")
		return render::LBX_BLEND_DIFFUSE_COLOR;
	else if (param == "blend_diffuse_alpha")
		return render::LBX_BLEND_DIFFUSE_ALPHA;
	else if (param == "blend_texture_alpha")
		return render::LBX_BLEND_TEXTURE_ALPHA;
	else if (param == "blend_current_alpha")
		return render::LBX_BLEND_CURRENT_ALPHA;
	else if (param == "blend_manual")
		return render::LBX_BLEND_MANUAL;
	else if (param == "dotproduct")
		return render::LBX_DOTPRODUCT;
	else
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("MaterialSerializer", "Invalid blend function", core::LOG_LEVEL_ERROR);

		return render::LBX_MODULATE;
	}
}

render::LayerBlendSource convertBlendSource(const std::string& param)
{
	if (param == "src_current")
		return render::LBS_CURRENT;
	else if (param == "src_texture")
		return render::LBS_TEXTURE;
	else if (param == "src_diffuse")
		return render::LBS_DIFFUSE;
	else if (param == "src_specular")
		return render::LBS_SPECULAR;
	else if (param == "src_manual")
		return render::LBS_MANUAL;
	else
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("MaterialSerializer", "Invalid blend source", core::LOG_LEVEL_ERROR);
	
		return render::LBS_CURRENT;
	}
}

render::ShaderAutoParameterType convertAutoParameterType(const std::string& param)
{
	if (param == "world_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_WORLD_MATRIX;
	else if (param == "inverse_world_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLD_MATRIX;
	else if (param == "transpose_world_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLD_MATRIX;
	else if (param == "inverse_transpose_world_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLD_MATRIX;
	else if (param == "view_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_VIEW_MATRIX;
	else if (param == "inverse_view_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEW_MATRIX;
	else if (param == "transpose_view_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_VIEW_MATRIX;
	else if (param == "inverse_transpose_view_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_VIEW_MATRIX;
	else if (param == "projection_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_PROJECTION_MATRIX;
	else if (param == "inverse_projection_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_INVERSE_PROJECTION_MATRIX;
	else if (param == "transpose_projection_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_PROJECTION_MATRIX;
	else if (param == "inverse_transpose_projection_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_PROJECTION_MATRIX;
	else if (param == "viewproj_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_VIEWPROJ_MATRIX;
	else if (param == "inverse_viewproj_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_INVERSE_VIEWPROJ_MATRIX;
	else if (param == "transpose_viewproj_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_VIEWPROJ_MATRIX;
	else if (param == "inverse_transpose_viewproj_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_VIEWPROJ_MATRIX;
	else if (param == "worldview_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_WORLDVIEW_MATRIX;
	else if (param == "inverse_worldview_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLDVIEW_MATRIX;
	else if (param == "transpose_worldview_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLDVIEW_MATRIX;
	else if (param == "inverse_transpose_worldview_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLDVIEW_MATRIX;
	else if (param == "worldviewproj_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_WORLDVIEWPROJ_MATRIX;
	else if (param == "inverse_worldviewproj_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_INVERSE_WORLDVIEWPROJ_MATRIX;
	else if (param == "transpose_worldviewproj_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_TRANSPOSE_WORLDVIEWPROJ_MATRIX;
	else if (param == "inverse_transpose_worldviewproj_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_INVERSE_TRANSPOSE_WORLDVIEWPROJ_MATRIX;
		
	else if (param == "light_count")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_COUNT;
	else if (param == "light_position")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION;
	else if (param == "light_position_object_space")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_OBJECT_SPACE;
	else if (param == "light_position_view_space")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION_VIEW_SPACE;
	else if (param == "light_direction")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION;
	else if (param == "light_direction_object_space")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_OBJECT_SPACE;
	else if (param == "light_direction_view_space")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIRECTION_VIEW_SPACE;
		
	else if (param == "ambient_light_colour")
		return render::SHADER_AUTO_PARAMETER_TYPE_AMBIENT_LIGHT_COLOUR;
	else if (param == "light_diffuse_colour")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_DIFFUSE_COLOUR;
	else if (param == "light_specular_colour")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_SPECULAR_COLOUR;
	else if (param == "light_attenuation")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_ATTENUATION;
	else if (param == "light_power")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_POWER_SCALE;

	else if (param == "camera_position")
		return render::SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION;
	else if (param == "camera_position_object_space")
		return render::SHADER_AUTO_PARAMETER_TYPE_CAMERA_POSITION_OBJECT_SPACE;
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("MaterialSerializer", "Invalid auto parameter type, using default.", core::LOG_LEVEL_ERROR);

		return render::SHADER_AUTO_PARAMETER_TYPE_NONE;
	}
}

MaterialSerializer::MaterialSerializer()
{
	// Version number
	mVersion = "[MaterialSerializer_v1.00]";
}

MaterialSerializer::~MaterialSerializer() {}

bool MaterialSerializer::importResource(Resource* dest, const std::string& filename)
{
	assert(dest != nullptr);
	if (dest == nullptr)
		return false;

	if (dest->getResourceType() != RESOURCE_TYPE_RENDER_MATERIAL && dest->getResourceType() != RESOURCE_TYPE_PHYSICS_MATERIAL)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("MaterialSerializer", "Unable to load material - invalid resource pointer.", core::LOG_LEVEL_ERROR);
		return false;
	}

	render::Material* renderMaterial = nullptr;
	physics::Material* physicsMaterial = nullptr;
	if (dest->getResourceType() == RESOURCE_TYPE_RENDER_MATERIAL)
	{
		renderMaterial = static_cast<render::Material*>(dest);

		assert(renderMaterial != nullptr);
		if (renderMaterial == nullptr)
			return false;
	}

	if (dest->getResourceType() == RESOURCE_TYPE_PHYSICS_MATERIAL)
	{
		physicsMaterial = static_cast<physics::Material*>(dest);

		assert(physicsMaterial != nullptr);
		if (physicsMaterial == nullptr)
			return false;
	}

	if (resource::ResourceManager::getInstance() == nullptr)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("MaterialSerializer", "Unable to load material - resources data path not set.", core::LOG_LEVEL_ERROR);
		return false;
	}

	std::string filePath = resource::ResourceManager::getInstance()->getDataPath() + "/" + filename;

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS)
		return false;

	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("material");
	if (pRoot != nullptr)
	{
		int ivalue = 0;
		double dvalue = 0.0;
		const char* svalue;
		std::string value;
		tinyxml2::XMLElement* pElement = nullptr;
		tinyxml2::XMLElement* pSubElement = nullptr;

		if (dest->getResourceType() == RESOURCE_TYPE_RENDER_MATERIAL)
		{
			pElement = pRoot->FirstChildElement("ambient");
			if (pElement != nullptr)
			{
				float r = 1.0f;
				float g = 1.0f;
				float b = 1.0f;
				float a = 1.0f;

				if (pElement->QueryDoubleAttribute("r", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					r = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("g", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					g = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("b", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					b = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("a", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					a = (float)dvalue;
				}

				renderMaterial->setAmbient(render::Color(r,g,b,a));
			}

			pElement = pRoot->FirstChildElement("diffuse");
			if (pElement != nullptr)
			{
				float r = 1.0f;
				float g = 1.0f;
				float b = 1.0f;
				float a = 1.0f;

				if (pElement->QueryDoubleAttribute("r", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					r = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("g", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					g = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("b", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					b = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("a", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					a = (float)dvalue;
				}

				renderMaterial->setDiffuse(render::Color(r,g,b,a));
			}

			pElement = pRoot->FirstChildElement("specular");
			if (pElement != nullptr)
			{
				float r = 1.0f;
				float g = 1.0f;
				float b = 1.0f;
				float a = 1.0f;

				if (pElement->QueryDoubleAttribute("r", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					r = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("g", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					g = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("b", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					b = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("a", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					a = (float)dvalue;
				}

				renderMaterial->setSpecular(render::Color(r,g,b,a));
			}

			pElement = pRoot->FirstChildElement("emissive");
			if (pElement != nullptr)
			{
				float r = 1.0f;
				float g = 1.0f;
				float b = 1.0f;
				float a = 1.0f;

				if (pElement->QueryDoubleAttribute("r", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					r = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("g", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					g = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("b", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					b = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("a", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					a = (float)dvalue;
				}

				renderMaterial->setEmissive(render::Color(r,g,b,a));
			}

			pElement = pRoot->FirstChildElement("shininess");
			if (pElement != nullptr)
			{
				if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					renderMaterial->setShininess((float)dvalue);
				}
			}

			pElement = pRoot->FirstChildElement("lighting");
			if (pElement != nullptr)
			{
				svalue = pElement->Attribute("value");
				if (svalue != nullptr)
				{
					renderMaterial->setLightingEnabled((std::string(svalue) == "true") ? true : false);
				}
			}

			pElement = pRoot->FirstChildElement("fog");
			if (pElement != nullptr)
			{
				render::FogMode mFogtype = render::FM_NONE;
				bool fog = false;
				float r = 1.0f;
				float g = 1.0f;
				float b = 1.0f;
				float a = 1.0f;
				float density = 0.0f;
				float start = 0.0f;
				float end = 0.0f;

				svalue = pElement->Attribute("value");
				if (svalue != nullptr)
				{
					fog = (std::string(svalue) == "true") ? true : false;
				}

				std::string type;
				svalue = pElement->Attribute("type");
				if (svalue != nullptr)
				{
					type = svalue;
				}

				if (type == "none")
					mFogtype = render::FM_NONE;
				else if (type == "linear")
					mFogtype = render::FM_LINEAR;
				else if (type == "exp")
					mFogtype = render::FM_EXP;
				else if (type == "exp2")
					mFogtype = render::FM_EXP2;
				else
					if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("MaterialSerializer", "Bad fog attribute, valid parameters are 'none', 'linear', 'exp', or 'exp2'.", core::LOG_LEVEL_ERROR);

				if (pElement->QueryDoubleAttribute("r", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					r = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("g", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					g = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("b", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					b = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("a", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					a = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("density", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					density = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("start", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					start = (float)dvalue;
				}

				if (pElement->QueryDoubleAttribute("end", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					end = (float)dvalue;
				}

				renderMaterial->setFog(fog, mFogtype, render::Color(r,g,b,a), density, start, end);
			}

			pElement = pRoot->FirstChildElement("shading");
			if (pElement != nullptr)
			{
				std::string type;
				svalue = pElement->Attribute("type");
				if (svalue != nullptr)
				{
					type = svalue;
				}

				render::ShadeOptions shadingMode = render::SO_FLAT;
				if (type == "flat")
					shadingMode = render::SO_FLAT;
				else if (type == "gouraud")
					shadingMode = render::SO_GOURAUD;
				else if (type == "phong")
					shadingMode = render::SO_PHONG;
				else
					if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("MaterialSerializer", "Bad shading attribute, valid parameters are 'flat', 'gouraud' or 'phong'.", core::LOG_LEVEL_ERROR);
		
				renderMaterial->setShadingMode(shadingMode);
			}

			pElement = pRoot->FirstChildElement("scene_blend");
			if (pElement != nullptr)
			{
				if (pElement->Attribute("type") != nullptr)
				{
					std::string type;
					svalue = pElement->Attribute("type");
					if (svalue != nullptr)
					{
						type = svalue;
					}

					if (type == "add")
						renderMaterial->setSceneBlending(render::SBT_ADD);
					else if (type == "modulate")
						renderMaterial->setSceneBlending(render::SBT_MODULATE);
					else if (type == "color_blend")
						renderMaterial->setSceneBlending(render::SBT_TRANSPARENT_COLOR);
					else if (type == "alpha_blend")
						renderMaterial->setSceneBlending(render::SBT_TRANSPARENT_ALPHA);
					else
					{
						std::string message = "Bad scene_blend attribute, unrecognised parameter '" + type + "'.";
						if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("MaterialSerializer", message, core::LOG_LEVEL_ERROR);
					}
				}
				else if (pElement->Attribute("src") != nullptr && pElement->Attribute("dest") != nullptr)
				{
					std::string src;
					std::string dest;
					svalue = pElement->Attribute("src");
					if (svalue != nullptr)
					{
						src = svalue;
					}
					svalue = pElement->Attribute("dest");
					if (svalue != nullptr)
					{
						dest = svalue;
					}

					renderMaterial->setSceneBlending(convertBlendFactor(src), convertBlendFactor(dest));
				}
			}

			pElement = pRoot->FirstChildElement("depth_check");
			if (pElement != nullptr)
			{
				svalue = pElement->Attribute("value");
				if (svalue != nullptr)
				{
					renderMaterial->setDepthCheckEnabled((std::string(svalue) == "true") ? true : false);
				}
			}

			pElement = pRoot->FirstChildElement("depth_write");
			if (pElement != nullptr)
			{
				svalue = pElement->Attribute("value");
				if (svalue != nullptr)
				{
					renderMaterial->setDepthWriteEnabled((std::string(svalue) == "true") ? true : false);
				}
			}


			std::vector<std::string> shaderkeys(3);

			shaderkeys[0] = "vertex_shader";
			shaderkeys[1] = "fragment_shader";
			shaderkeys[2] = "geometry_shader";

			for (unsigned int i = 0; i < shaderkeys.size(); ++i)
			{
				pElement = pRoot->FirstChildElement(shaderkeys[i].c_str());
				if (pElement != nullptr)
				{
					std::string shader;
					pSubElement = pElement->FirstChildElement("shader");
					if (pSubElement != nullptr)
					{
						svalue = pSubElement->Attribute("value");
						if (svalue != nullptr)
						{
							shader = svalue;
						}
					}
					
					if (shader.empty())
						continue;

					render::Shader* pShader = nullptr;
				
					if (shaderkeys[i] == "vertex_shader")
					{
						renderMaterial->setVertexShader(shader);
						pShader = renderMaterial->getVertexShader();
					}
					if (shaderkeys[i] == "fragment_shader")
					{
						renderMaterial->setFragmentShader(shader);
						pShader = renderMaterial->getFragmentShader();
					}
					if (shaderkeys[i] == "geometry_shader")
					{
						renderMaterial->setGeometryShader(shader);
						pShader = renderMaterial->getGeometryShader();
					}

					if (pShader == nullptr)
						continue;

					pSubElement = pElement->FirstChildElement("entry_point");
					if (pSubElement != nullptr)
					{
						svalue = pSubElement->Attribute("value");
						if (svalue != nullptr)
						{
							value = svalue;
							pShader->setEntryPoint(value);
						}
					}

					pSubElement = pElement->FirstChildElement("param_named");
					while (pSubElement != nullptr)
					{
						std::string name;
						std::string type;

						svalue = pSubElement->Attribute("name");
						if (svalue != nullptr)
						{
							name = svalue;
						}

						svalue = pSubElement->Attribute("type");
						if (svalue != nullptr)
						{
							type = svalue;
						}

						if (name.empty() && type.empty())
							continue;

						if (type == "color")
						{
							float r = 1.0f;
							float g = 1.0f;
							float b = 1.0f;
							float a = 1.0f;

							if (pSubElement->QueryDoubleAttribute("r", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								r = (float)dvalue;
							}

							if (pSubElement->QueryDoubleAttribute("g", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								g = (float)dvalue;
							}

							if (pSubElement->QueryDoubleAttribute("b", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								b = (float)dvalue;
							}

							if (pSubElement->QueryDoubleAttribute("a", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								a = (float)dvalue;
							}

							pShader->setParameter(name, render::Color(r,g,b,a));
						}
						else if (type == "vector2d")
						{
							float x = 0.0f;
							float y = 0.0f;

							if (pSubElement->QueryDoubleAttribute("x", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								x = (float)dvalue;
							}

							if (pSubElement->QueryDoubleAttribute("y", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								y = (float)dvalue;
							}

							pShader->setParameter(name, core::vector2d(x,y));
						}
						else if (type == "vector3d")
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

							pShader->setParameter(name, core::vector3d(x,y,z));
						}
						else if (type == "vector4d")
						{
							float x = 0.0f;
							float y = 0.0f;
							float z = 0.0f;
							float w = 0.0f;

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

							if (pSubElement->QueryDoubleAttribute("w", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								w = (float)dvalue;
							}

							pShader->setParameter(name, core::vector4d(x,y,z,w));
						}
						else if (type == "matrix4x4")
						{
							float m00 = 0.0f;
							float m01 = 0.0f;
							float m02 = 0.0f;
							float m03 = 0.0f;
							float m10 = 0.0f;
							float m11 = 0.0f;
							float m12 = 0.0f;
							float m13 = 0.0f;
							float m20 = 0.0f;
							float m21 = 0.0f;
							float m22 = 0.0f;
							float m23 = 0.0f;
							float m30 = 0.0f;
							float m31 = 0.0f;
							float m32 = 0.0f;
							float m33 = 0.0f;

							tinyxml2::XMLElement* pRowSubElement = nullptr;
							pRowSubElement = pSubElement->FirstChildElement("row0");
							if (pRowSubElement != nullptr)
							{
								svalue = pRowSubElement->Attribute("value");
								if (svalue != nullptr)
								{
									value = svalue;
								}

								std::vector<std::string> vecparams = core::splitString(value, " \t");

								if (vecparams.size() > 0)
									m00 = core::stringToFloat(vecparams[0]);
								if (vecparams.size() > 1)
									m01 = core::stringToFloat(vecparams[1]);
								if (vecparams.size() > 2)
									m02 = core::stringToFloat(vecparams[2]);
								if (vecparams.size() > 3)
									m03 = core::stringToFloat(vecparams[3]);
							}

							pRowSubElement = pSubElement->FirstChildElement("row1");
							if (pRowSubElement != nullptr)
							{
								svalue = pRowSubElement->Attribute("value");
								if (svalue != nullptr)
								{
									value = svalue;
								}

								std::vector<std::string> vecparams = core::splitString(value, " \t");

								if (vecparams.size() > 0)
									m10 = core::stringToFloat(vecparams[0]);
								if (vecparams.size() > 1)
									m11 = core::stringToFloat(vecparams[1]);
								if (vecparams.size() > 2)
									m12 = core::stringToFloat(vecparams[2]);
								if (vecparams.size() > 3)
									m13 = core::stringToFloat(vecparams[3]);
							}

							pRowSubElement = pSubElement->FirstChildElement("row2");
							if (pRowSubElement != nullptr)
							{
								svalue = pRowSubElement->Attribute("value");
								if (svalue != nullptr)
								{
									value = svalue;
								}

								std::vector<std::string> vecparams = core::splitString(value, " \t");

								if (vecparams.size() > 0)
									m20 = core::stringToFloat(vecparams[0]);
								if (vecparams.size() > 1)
									m21 = core::stringToFloat(vecparams[1]);
								if (vecparams.size() > 2)
									m22 = core::stringToFloat(vecparams[2]);
								if (vecparams.size() > 3)
									m23 = core::stringToFloat(vecparams[3]);
							}

							pRowSubElement = pSubElement->FirstChildElement("row3");
							if (pRowSubElement != nullptr)
							{
								svalue = pRowSubElement->Attribute("value");
								if (svalue != nullptr)
								{
									value = svalue;
								}

								std::vector<std::string> vecparams = core::splitString(value, " \t");

								if (vecparams.size() > 0)
									m30 = core::stringToFloat(vecparams[0]);
								if (vecparams.size() > 1)
									m31 = core::stringToFloat(vecparams[1]);
								if (vecparams.size() > 2)
									m32 = core::stringToFloat(vecparams[2]);
								if (vecparams.size() > 3)
									m33 = core::stringToFloat(vecparams[3]);
							}

							pShader->setParameter(name, core::matrix4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33));
						}
						else if (type == "float")
						{
							svalue = pElement->Attribute("value");
							if (svalue != nullptr)
							{
								value = svalue;
							}

							std::vector<std::string> vecparams = core::splitString(value, " \t");

							unsigned int count = vecparams.size();
							float* floatBuffer = new float[count];

							for(unsigned int i = 0; i < count; ++i)
								floatBuffer[i] = core::stringToFloat(vecparams[i]);

							pShader->setParameter(name, floatBuffer, count);
						}
						else if (type == "int")
						{
							svalue = pElement->Attribute("value");
							if (svalue != nullptr)
							{
								value = svalue;
							}

							std::vector<std::string> vecparams = core::splitString(value, " \t");

							unsigned int count = vecparams.size();
							int* intBuffer = new int[count];

							for(unsigned int i = 0; i < count; ++i)
								intBuffer[i] = core::stringToInt(vecparams[i]);

							pShader->setParameter(name, intBuffer, count);
						}
						else
						{
							std::string message = "Invalid param_named attribute - unrecognized parameter type " + type + " .";
							if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("MaterialSerializer", message, core::LOG_LEVEL_ERROR);
						}

						pSubElement = pSubElement->NextSiblingElement("param_named");
					}

					pSubElement = pElement->FirstChildElement("param_auto");
					while (pSubElement != nullptr)
					{
						render::ShaderAutoParameterType paramAutoType = render::SHADER_AUTO_PARAMETER_TYPE_NONE;

						svalue = pSubElement->Attribute("type");
						if (svalue != nullptr)
						{
							value = svalue;
							paramAutoType =	convertAutoParameterType(value);
						}

						svalue = pSubElement->Attribute("name");
						if (svalue != nullptr)
						{
							value = svalue;
						}

						pShader->setAutoParameter(value, paramAutoType);

						pSubElement = pSubElement->NextSiblingElement("param_auto");
					}
				}
			}
			
			pElement = pRoot->FirstChildElement("texture_unit");
			while (pElement != nullptr)
			{
				std::string texture;
				pSubElement = pElement->FirstChildElement("texture");
				if (pSubElement != nullptr)
				{
					svalue = pSubElement->Attribute("value");
					if (svalue != nullptr)
					{
						texture = svalue;
					}
				}

				if (!texture.empty())
				{
					render::TextureUnit* pTextureUnit = new render::TextureUnit();
					pTextureUnit->setTexture(texture);
					renderMaterial->addTextureUnit(pTextureUnit);

					pSubElement = pElement->FirstChildElement("color_op");
					if (pSubElement != nullptr)
					{
						render::LayerBlendOperation blendOperation = render::LBX_ADD;
						render::LayerBlendSource blendSource1 = render::LBS_TEXTURE;
						render::LayerBlendSource blendSource2 = render::LBS_CURRENT;
						render::Color colorArg1 = render::Color::White;
						render::Color colorArg2 = render::Color::White;
						float manualBlend = 0.0f;

						svalue = pSubElement->Attribute("operation");
						if (svalue != nullptr)
						{
							value = svalue;
							blendOperation = convertBlendOperation(value);
						}

						svalue = pSubElement->Attribute("source1");
						if (svalue != nullptr)
						{
							value = svalue;
							blendSource1 = convertBlendSource(value);
						}

						svalue = pSubElement->Attribute("source2");
						if (svalue != nullptr)
						{
							value = svalue;
							blendSource2 = convertBlendSource(value);
						}

						if (blendSource1 == render::LBS_MANUAL)
						{
							float r = 1.0f;
							float g = 1.0f;
							float b = 1.0f;
							float a = 1.0f;

							if (pSubElement->QueryDoubleAttribute("r1", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								r = (float)dvalue;
							}

							if (pSubElement->QueryDoubleAttribute("g1", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								g = (float)dvalue;
							}

							if (pSubElement->QueryDoubleAttribute("b1", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								b = (float)dvalue;
							}

							if (pSubElement->QueryDoubleAttribute("a1", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								a = (float)dvalue;
							}

							colorArg1 = render::Color(r,g,b,a);
						}

						if (blendSource2 == render::LBS_MANUAL)
						{
							float r = 1.0f;
							float g = 1.0f;
							float b = 1.0f;
							float a = 1.0f;

							if (pSubElement->QueryDoubleAttribute("r2", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								r = (float)dvalue;
							}

							if (pSubElement->QueryDoubleAttribute("g2", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								g = (float)dvalue;
							}

							if (pSubElement->QueryDoubleAttribute("b2", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								b = (float)dvalue;
							}

							if (pSubElement->QueryDoubleAttribute("a2", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								a = (float)dvalue;
							}

							colorArg2 = render::Color(r,g,b,a);
						}

						if (blendOperation == render::LBX_BLEND_MANUAL)
						{
							if (pSubElement->QueryDoubleAttribute("manualBlend", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								manualBlend = (float)dvalue;
							}
						}

						pTextureUnit->setColorOperation(blendOperation, blendSource1, blendSource2, colorArg1, colorArg2, manualBlend);
					}

					pSubElement = pElement->FirstChildElement("alpha_op");
					if (pSubElement != nullptr)
					{
						render::LayerBlendOperation blendOperation = render::LBX_ADD;
						render::LayerBlendSource blendSource1 = render::LBS_TEXTURE;
						render::LayerBlendSource blendSource2 = render::LBS_CURRENT;
						float arg1 = 1.0f;
						float arg2 = 1.0f;
						float manualBlend = 0.0f;

						std::string value;
						svalue = pSubElement->Attribute("operation");
						if (svalue != nullptr)
						{
							value = svalue;
							blendOperation = convertBlendOperation(value);
						}

						svalue = pSubElement->Attribute("source1");
						if (svalue != nullptr)
						{
							value = svalue;
							blendSource1 = convertBlendSource(value);
						}

						svalue = pSubElement->Attribute("source2");
						if (svalue != nullptr)
						{
							value = svalue;
							blendSource2 = convertBlendSource(value);
						}

						if (blendSource1 == render::LBS_MANUAL)
						{
							if (pSubElement->QueryDoubleAttribute("arg1", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								arg1 = (float)dvalue;
							}
						}

						if (blendSource2 == render::LBS_MANUAL)
						{
							if (pSubElement->QueryDoubleAttribute("arg2", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								arg2 = (float)dvalue;
							}
						}

						if (blendOperation == render::LBX_BLEND_MANUAL)
						{
							if (pSubElement->QueryDoubleAttribute("manualBlend", &dvalue) == tinyxml2::XML_SUCCESS)
							{
								manualBlend = (float)dvalue;
							}
						}

						pTextureUnit->setAlphaOperation(blendOperation, blendSource1, blendSource2, arg1, arg2, manualBlend);
					}

					pSubElement = pElement->FirstChildElement("tex_coord_set");
					if (pSubElement != nullptr)
					{
						if (pSubElement->QueryIntAttribute("manualBlend", &ivalue) == tinyxml2::XML_SUCCESS)
						{
							pTextureUnit->setTextureCoordSet((unsigned int)ivalue);
						}
					}
				}

				pElement = pElement->NextSiblingElement("texture_unit");
			}
		}

		if (dest->getResourceType() == RESOURCE_TYPE_PHYSICS_MATERIAL)
		{
			pElement = pRoot->FirstChildElement("restitution");
			if (pElement != nullptr)
			{
				if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					physicsMaterial->setRestitution((float)dvalue);
				}
			}

			pElement = pRoot->FirstChildElement("static_friction");
			if (pElement != nullptr)
			{
				if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					physicsMaterial->setStaticFriction((float)dvalue);
				}
			}

			pElement = pRoot->FirstChildElement("dynamic_friction");
			if (pElement != nullptr)
			{
				if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					physicsMaterial->setDynamicFriction((float)dvalue);
				}
			}
		}
	}

	return true;
}

bool MaterialSerializer::exportResource(Resource* source, const std::string& filename)
{
	return false;
}

}// end namespace resource
