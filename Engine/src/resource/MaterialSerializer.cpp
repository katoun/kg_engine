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

#include <resource/MaterialSerializer.h>
#include <core/Log.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <resource/ResourceManager.h>
#include <render/Shader.h>
#include <render/Material.h>
#include <physics/Material.h>

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
	if (param == "model_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_MODEL_MATRIX;
	else if (param == "view_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_VIEW_MATRIX;
	else if (param == "projection_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_PROJECTION_MATRIX;
	else if (param == "view_projection_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_VIEW_PROJECTION_MATRIX;
	else if (param == "model_view_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_MODEL_VIEW_MATRIX;
	else if (param == "model_view_projection_matrix")
		return render::SHADER_AUTO_PARAMETER_TYPE_MODEL_VIEW_PROJECTION_MATRIX;

	else if (param == "material_ambient_colour")
		return render::SHADER_AUTO_PARAMETER_TYPE_MATERIAL_AMBIENT_COLOUR;
	else if (param == "material_diffuse_colour")
		return render::SHADER_AUTO_PARAMETER_TYPE_MATERIAL_DIFFUSE_COLOUR;
	else if (param == "material_specular_colour")
		return render::SHADER_AUTO_PARAMETER_TYPE_MATERIAL_SPECULAR_COLOUR;
	else if (param == "material_shininess")
		return render::SHADER_AUTO_PARAMETER_TYPE_MATERIAL_SHININESS;
		
	else if (param == "light_count")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_COUNT;
	else if (param == "light_position")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_POSITION;
	
	else if (param == "light_ambient_colour")
		return render::SHADER_AUTO_PARAMETER_TYPE_LIGHT_AMBIENT_COLOUR;
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
		render::Material* renderMaterial = static_cast<render::Material*>(dest);

		assert(renderMaterial != nullptr);
		if (renderMaterial == nullptr)
			return false;

		int ivalue = 0;
		double dvalue = 0.0;
		const char* svalue;
		std::string value;
		tinyxml2::XMLElement* pElement = nullptr;
		tinyxml2::XMLElement* pSubElement = nullptr;

		if (dest->getResourceType() == RESOURCE_TYPE_RENDER_MATERIAL)
		{
			pElement = pRoot->FirstChildElement("ambient_color");
			if (pElement != nullptr)
			{
				float r = 0.0f;
				float g = 0.0f;
				float b = 0.0f;
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

				renderMaterial->setAmbientColor(render::Color(r, g, b, a));
			}

			pElement = pRoot->FirstChildElement("diffuse_color");
			if (pElement != nullptr)
			{
				float r = 0.0f;
				float g = 0.0f;
				float b = 0.0f;
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

				renderMaterial->setDiffuseColor(render::Color(r, g, b, a));
			}

			pElement = pRoot->FirstChildElement("specular_color");
			if (pElement != nullptr)
			{
				float r = 0.0f;
				float g = 0.0f;
				float b = 0.0f;
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

				renderMaterial->setSpecularColor(render::Color(r, g, b, a));
			}

			pElement = pRoot->FirstChildElement("shininess");
			if (pElement != nullptr)
			{
				if (pElement->QueryDoubleAttribute("value", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					renderMaterial->setShininess((float)dvalue);
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
			physics::Material* physicsMaterial = static_cast<physics::Material*>(dest);

			assert(physicsMaterial != nullptr);
			if (physicsMaterial == nullptr)
				return false;
			
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
