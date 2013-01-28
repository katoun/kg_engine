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
#include <render/Shader.h>
#include <render/Material.h>
#include <physics/Material.h>
#include <MaterialSerializer.h>

#include <tinyxml2.h>

#include <string>

namespace resource
{

render::VertexBufferType convertVertexParameterType(const std::string& param)
{
	if (param == "vertex_position")
		return render::VERTEX_BUFFER_TYPE_POSITION;
	else if (param == "vertex_normal")
		return render::VERTEX_BUFFER_TYPE_NORMAL;
	else if (param == "vertex_binormal")
		return render::VERTEX_BUFFER_TYPE_BINORMAL;
	else if (param == "vertex_tangent")
		return render::VERTEX_BUFFER_TYPE_TANGENT;
	else if (param == "vertex_texture_coordinates")
		return render::VERTEX_BUFFER_TYPE_TEXTURE_COORDINATES;
	else
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("MaterialSerializer", "Invalid vertex parameter type, using default.", core::LOG_LEVEL_ERROR);

		return render::VERTEX_BUFFER_TYPE_POSITION;
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
	else
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

					pSubElement = pElement->FirstChildElement("param_vertex");
					while (pSubElement != nullptr)
					{
						render::VertexBufferType vertexType = render::VERTEX_BUFFER_TYPE_POSITION;

						svalue = pSubElement->Attribute("type");
						if (svalue != nullptr)
						{
							value = svalue;
							vertexType = convertVertexParameterType(value);
						}

						svalue = pSubElement->Attribute("name");
						if (svalue != nullptr)
						{
							value = svalue;
						}

						renderMaterial->addVertexParameter(value, vertexType);

						pSubElement = pSubElement->NextSiblingElement("param_vertex");
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

						renderMaterial->addAutoParameter(value, paramAutoType);

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
					renderMaterial->addTextureUnit(texture);
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
