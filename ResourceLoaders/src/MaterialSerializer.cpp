/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

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
#include <render/BlendMode.h>
#include <render/TextureUnit.h>
#include <render/Shader.h>
#include <render/Material.h>
#include <physics/Material.h>
#include <MaterialSerializer.h>

#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>

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
		core::Log::getInstance().logMessage("MaterialSerializer", "Invalid blend factor, using default.", core::LOG_LEVEL_ERROR);

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
		core::Log::getInstance().logMessage("MaterialSerializer", "Invalid blend function", core::LOG_LEVEL_ERROR);

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
		core::Log::getInstance().logMessage("MaterialSerializer", "Invalid blend source", core::LOG_LEVEL_ERROR);
	
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
		core::Log::getInstance().logMessage("MaterialSerializer", "Invalid auto parameter type, using default.", core::LOG_LEVEL_ERROR);

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
	assert(dest != NULL);
	if (dest == NULL)
		return false;

	if (dest->getResourceType() != RESOURCE_TYPE_RENDER_MATERIAL && dest->getResourceType() != RESOURCE_TYPE_PHYSICS_MATERIAL)
	{
		core::Log::getInstance().logMessage("MaterialSerializer", "Unable to load material - invalid resource pointer.", core::LOG_LEVEL_ERROR);
		return false;
	}

	render::Material* renderMaterial = NULL;
	physics::Material* physicsMaterial = NULL;
	if (dest->getResourceType() == RESOURCE_TYPE_RENDER_MATERIAL)
	{
		renderMaterial = static_cast<render::Material*>(dest);

		assert(renderMaterial != NULL);
		if (renderMaterial == NULL)
			return false;
	}

	if (dest->getResourceType() == RESOURCE_TYPE_PHYSICS_MATERIAL)
	{
		physicsMaterial = static_cast<physics::Material*>(dest);

		assert(physicsMaterial != NULL);
		if (physicsMaterial == NULL)
			return false;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string filePath = ResourceManager::getInstance().getPath(filename);
	Poco::AutoPtr<Poco::Util::XMLConfiguration> pConf(new Poco::Util::XMLConfiguration());
	try
	{
		pConf->load(filePath);
	}
	catch(...)
	{
		return false;
	}

	if (dest->getResourceType() == RESOURCE_TYPE_RENDER_MATERIAL)
	{
		//ambient
		if (pConf->has("ambient"))
		{
			float r = 1.0f;
			float g = 1.0f;
			float b = 1.0f;
			float a = 1.0f;
			if (pConf->has("ambient[@r]"))
				r = (float)pConf->getDouble("ambient[@r]");
			if (pConf->has("ambient[@g]"))
				g = (float)pConf->getDouble("ambient[@g]");
			if (pConf->has("ambient[@b]"))
				b = (float)pConf->getDouble("ambient[@b]");
			if (pConf->has("ambient[@a]"))
				a = (float)pConf->getDouble("ambient[@a]");
			renderMaterial->setAmbient(render::Color(r,g,b,a));
		}

		//diffuse
		if (pConf->has("diffuse"))
		{
			float r = 1.0f;
			float g = 1.0f;
			float b = 1.0f;
			float a = 1.0f;
			if (pConf->has("diffuse[@r]"))
				r = (float)pConf->getDouble("diffuse[@r]");
			if (pConf->has("diffuse[@g]"))
				g = (float)pConf->getDouble("diffuse[@g]");
			if (pConf->has("diffuse[@b]"))
				b = (float)pConf->getDouble("diffuse[@b]");
			if (pConf->has("diffuse[@a]"))
				a = (float)pConf->getDouble("diffuse[@a]");
			renderMaterial->setDiffuse(render::Color(r,g,b,a));
		}

		//specular
		if (pConf->has("specular"))
		{
			float r = 1.0f;
			float g = 1.0f;
			float b = 1.0f;
			float a = 1.0f;
			if (pConf->has("specular[@r]"))
				r = (float)pConf->getDouble("specular[@r]");
			if (pConf->has("specular[@g]"))
				g = (float)pConf->getDouble("specular[@g]");
			if (pConf->has("specular[@b]"))
				b = (float)pConf->getDouble("specular[@b]");
			if (pConf->has("specular[@a]"))
				a = (float)pConf->getDouble("specular[@a]");
			renderMaterial->setSpecular(render::Color(r,g,b,a));
		}

		//emissive
		if (pConf->has("emissive"))
		{
			float r = 1.0f;
			float g = 1.0f;
			float b = 1.0f;
			float a = 1.0f;
			if (pConf->has("emissive[@r]"))
				r = (float)pConf->getDouble("emissive[@r]");
			if (pConf->has("emissive[@g]"))
				g = (float)pConf->getDouble("emissive[@g]");
			if (pConf->has("emissive[@b]"))
				b = (float)pConf->getDouble("emissive[@b]");
			if (pConf->has("emissive[@a]"))
				a = (float)pConf->getDouble("emissive[@a]");
			renderMaterial->setEmissive(render::Color(r,g,b,a));
		}

		//shininess
		if (pConf->has("shininess"))
		{
			float shininess = 0.0f;
			if (pConf->has("shininess[@value]"))
				shininess = (float)pConf->getDouble("shininess[@value]");
			renderMaterial->setShininess(shininess);
		}

		//lighting
		if (pConf->has("lighting"))
		{
			bool lighting = false;
			if (pConf->has("lighting[@value]"))
				lighting = pConf->getBool("lighting[@value]");
			renderMaterial->setLightingEnabled(lighting);
		}

		//fog
		if (pConf->has("fog"))
		{
			bool fog = false;
			float r = 1.0f;
			float g = 1.0f;
			float b = 1.0f;
			float a = 1.0f;
			render::FogMode mFogtype = render::FM_NONE;
			float density = 0.0f;
			float start = 0.0f;
			float end = 0.0f;
			
			if (pConf->has("fog[@value]"))
				fog = pConf->getBool("fog[@value]");

			std::string type;
			if (pConf->has("fog[@type]"))
				type = pConf->getString("fog[@type]");

			if (type == "none")
				mFogtype = render::FM_NONE;
			else if (type == "linear")
				mFogtype = render::FM_LINEAR;
			else if (type == "exp")
				mFogtype = render::FM_EXP;
			else if (type == "exp2")
				mFogtype = render::FM_EXP2;
			else
				core::Log::getInstance().logMessage("MaterialSerializer", "Bad fog attribute, valid parameters are 'none', 'linear', 'exp', or 'exp2'.", core::LOG_LEVEL_ERROR);

			if (pConf->has("fog[@r]"))
				r = (float)pConf->getDouble("fog[@r]");
			if (pConf->has("fog[@g]"))
				g = (float)pConf->getDouble("fog[@g]");
			if (pConf->has("fog[@b]"))
				b = (float)pConf->getDouble("fog[@b]");
			if (pConf->has("fog[@a]"))
				a = (float)pConf->getDouble("fog[@a]");
		
			if (pConf->has("fog[@density]"))
				density = (float)pConf->getDouble("fog[@density]");
			if (pConf->has("fog[@start]"))
				start = (float)pConf->getDouble("fog[@start]");
			if (pConf->has("fog[@end]"))
				end = (float)pConf->getDouble("fog[@end]");

			renderMaterial->setFog(fog, mFogtype, render::Color(r,g,b,a), density, start, end);
		}

		//shading
		if (pConf->has("shading"))
		{
			std::string type;
			if (pConf->has("shading[@type]"))
				type = pConf->getString("shading[@type]");

			render::ShadeOptions shadingMode = render::SO_FLAT;
			if (type == "flat")
				shadingMode = render::SO_FLAT;
			else if (type == "gouraud")
				shadingMode = render::SO_GOURAUD;
			else if (type == "phong")
				shadingMode = render::SO_PHONG;
			else
				core::Log::getInstance().logMessage("MaterialSerializer", "Bad shading attribute, valid parameters are 'flat', 'gouraud' or 'phong'.", core::LOG_LEVEL_ERROR);
		
			renderMaterial->setShadingMode(shadingMode);
		}

		//scene_blend
		if (pConf->has("scene_blend"))
		{
			if (pConf->has("scene_blend[@type]"))
			{
				std::string type = pConf->getString("scene_blend[@type]");

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
					core::Log::getInstance().logMessage("MaterialSerializer", message, core::LOG_LEVEL_ERROR);
				}
			}
			else if (pConf->has("scene_blend[@src]") && pConf->has("scene_blend[@dest]"))
			{
				render::SceneBlendFactor blendSrc, blendDest;
				std::string src = pConf->getString("scene_blend[@src]");
				std::string dest = pConf->getString("scene_blend[@dest]");

				blendSrc = convertBlendFactor(src);
				blendDest = convertBlendFactor(dest);

				renderMaterial->setSceneBlending(blendSrc, blendDest);
			}
		}

		//depth_check
		if (pConf->has("depth_check"))
		{
			bool depth_check = false;
			if (pConf->has("depth_check[@value]"))
				depth_check = pConf->getBool("depth_check[@value]");
			renderMaterial->setDepthCheckEnabled(depth_check);
		}

		//depth_write
		if (pConf->has("depth_write"))
		{
			bool depth_write = false;
			if (pConf->has("depth_write[@value]"))
				depth_write = pConf->getBool("depth_write[@value]");
			renderMaterial->setDepthWriteEnabled(depth_write);
		}

		std::vector<std::string> shaderkeys(3);
		//shaderkeys.reserve(3);
		shaderkeys[0] = "vertex_shader";
		shaderkeys[1] = "fragment_shader";
		shaderkeys[2] = "geometry_shader";

		//shaders
		for (int i = 0; i < shaderkeys.size(); ++i)
		{
			std::string key = shaderkeys[i];

			//vertex_shader
			if (pConf->has(key))
			{
				std::string shader;
				if (pConf->has(key + ".shader"))
				{
					if (pConf->has(key + ".shader[@value]"))
						shader = pConf->getString(key + ".shader[@value]");
				}

				if (shader.empty())
					continue;
			
				render::Shader* pShader = NULL;
				
				if (key == "vertex_shader")
				{
					renderMaterial->setVertexShader(shader);
					pShader = renderMaterial->getVertexShader();
				}
				if (key == "fragment_shader")
				{
					renderMaterial->setFragmentShader(shader);
					pShader = renderMaterial->getFragmentShader();
				}
				if (key == "geometry_shader")
				{
					renderMaterial->setGeometryShader(shader);
					pShader = renderMaterial->getGeometryShader();
				}
				
				if (pShader != NULL)
				{
					if (pConf->has(key + ".entry_point"))
					{
						std::string entry_point; 
						if (pConf->has(key + ".entry_point[@value]"))
							entry_point = pConf->getString(key + ".entry_point[@value]");
						pShader->setEntryPoint(entry_point);
					}

					if (pConf->has(key + ".param_named"))
					{
						std::string name;
						if (pConf->has(key + ".param_named[@name]"))
							name = pConf->getString(key + ".param_named[@name]");
						std::string type;
						if (pConf->has(key + ".param_named[@type]"))
							type = pConf->getString(key + ".param_named[@type]");

						if (type == "matrix4x4")
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

							if (pConf->has(key + ".param_named[@m00]"))
								m00 = (float)pConf->getDouble(key + ".param_named[@m00]");
							if (pConf->has(key + ".param_named[@m01]"))
								m01 = (float)pConf->getDouble(key + ".param_named[@m01]");
							if (pConf->has(key + ".param_named[@m02]"))
								m02 = (float)pConf->getDouble(key + ".param_named[@m02]");
							if (pConf->has(key + ".param_named[@m03]"))
								m03 = (float)pConf->getDouble(key + ".param_named[@m03]");
							if (pConf->has(key + ".param_named[@m10]"))
								m10 = (float)pConf->getDouble(key + ".param_named[@m10]");
							if (pConf->has(key + ".param_named[@m11]"))
								m11 = (float)pConf->getDouble(key + ".param_named[@m11]");
							if (pConf->has(key + ".param_named[@m12]"))
								m12 = (float)pConf->getDouble(key + ".param_named[@m12]");
							if (pConf->has(key + ".param_named[@m13]"))
								m13 = (float)pConf->getDouble(key + ".param_named[@m13]");
							if (pConf->has(key + ".param_named[@m20]"))
								m20 = (float)pConf->getDouble(key + ".param_named[@m20]");
							if (pConf->has(key + ".param_named[@m21]"))
								m21 = (float)pConf->getDouble(key + ".param_named[@m21]");
							if (pConf->has(key + ".param_named[@m22]"))
								m22 = (float)pConf->getDouble(key + ".param_named[@m22]");
							if (pConf->has(key + ".param_named[@m23]"))
								m23 = (float)pConf->getDouble(key + ".param_named[@m23]");
							if (pConf->has(key + ".param_named[@m30]"))
								m30 = (float)pConf->getDouble(key + ".param_named[@m30]");
							if (pConf->has(key + ".param_named[@m31]"))
								m31 = (float)pConf->getDouble(key + ".param_named[@m31]");
							if (pConf->has(key + ".param_named[@m32]"))
								m32 = (float)pConf->getDouble(key + ".param_named[@m32]");
							if (pConf->has(key + ".param_named[@m33]"))
								m33 = (float)pConf->getDouble(key + ".param_named[@m33]");

							core::matrix4 m(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33);
							pShader->setParameter(name, m);
						}
						else if (type == "color")
						{
							float r = 1.0f;
							float g = 1.0f;
							float b = 1.0f;
							float a = 1.0f;

							if (pConf->has(key + ".param_named[@r]"))
								r = (float)pConf->getDouble(key + ".param_named[@r]");
							if (pConf->has(key + ".param_named[@g]"))
								g = (float)pConf->getDouble(key + ".param_named[@g]");
							if (pConf->has(key + ".param_named[@b]"))
								b = (float)pConf->getDouble(key + ".param_named[@b]");
							if (pConf->has(key + ".param_named[@a]"))
								a = (float)pConf->getDouble(key + ".param_named[@a]");

							pShader->setParameter(name, render::Color(r,g,b,a));
						}
						else if (type == "vector2d")
						{
							float x = 0.0f;
							float y = 0.0f;

							if (pConf->has(key + ".param_named[@x]"))
								x = (float)pConf->getDouble(key + ".param_named[@x]");
							if (pConf->has(key + ".param_named[@y]"))
								y = (float)pConf->getDouble(key + ".param_named[@y]");

							pShader->setParameter(name, core::vector2d(x,y));
			
						}
						else if (type == "vector3d")
						{
							float x = 0.0f;
							float y = 0.0f;
							float z = 0.0f;

							if (pConf->has(key + ".param_named[@x]"))
								x = (float)pConf->getDouble(key + ".param_named[@x]");
							if (pConf->has(key + ".param_named[@y]"))
								y = (float)pConf->getDouble(key + ".param_named[@y]");
							if (pConf->has(key + ".param_named[@z]"))
								z = (float)pConf->getDouble(key + ".param_named[@z]");

							pShader->setParameter(name, core::vector3d(x,y,z));
						}
						else if (type == "vector4d")
						{
							float x = 0.0f;
							float y = 0.0f;
							float z = 0.0f;
							float w = 0.0f;

							if (pConf->has(key + ".param_named[@x]"))
								x = (float)pConf->getDouble(key + ".param_named[@x]");
							if (pConf->has(key + ".param_named[@y]"))
								y = (float)pConf->getDouble(key + ".param_named[@y]");
							if (pConf->has(key + ".param_named[@z]"))
								z = (float)pConf->getDouble(key + ".param_named[@z]");
							if (pConf->has(key + ".param_named[@w]"))
								w = (float)pConf->getDouble(key + ".param_named[@w]");

							pShader->setParameter(name, core::vector4d(x,y,z,w));
						}
						else if (type == "float")
						{
							std::string value;
							if (pConf->has(key + ".param_named[@value]"))
								value = pConf->getString(key + ".param_named[@value]");

							std::vector<std::string> vecparams = core::splitString(value, " \t");

							unsigned int count = vecparams.size();
							float* floatBuffer = new float[count];

							for(unsigned int i = 0; i < count; ++i)
								floatBuffer[i] = core::stringToFloat(vecparams[i]);

							pShader->setParameter(name, floatBuffer, count);
						}
						else if (type == "int")
						{
							std::string value;
							if (pConf->has(key + ".param_named[@value]"))
								value = pConf->getString(key + ".param_named[@value]");

							std::vector<std::string> vecparams = core::splitString(value, " \t");

							unsigned int count = vecparams.size();
							signed int* intBuffer = new signed int[count];

							for(unsigned int i = 0; i < count; ++i)
								intBuffer[i] = core::stringToInt(vecparams[i]);

							pShader->setParameter(name, intBuffer, count);
						}
						else
						{
							std::string message = "Invalid param_named attribute - unrecognized parameter type " + type + " .";
							core::Log::getInstance().logMessage("MaterialSerializer", message, core::LOG_LEVEL_ERROR);
						}
					}

					if (pConf->has(key + ".param_auto"))
					{
						std::string name;
						if (pConf->has(key + ".param_auto[@name]"))
							name = pConf->getString(key + ".param_auto[@name]");

						std::string type;
						if (pConf->has(key + ".param_auto[@type]"))
							type = pConf->getString(key + ".param_auto[@type]");

						render::ShaderAutoParameterType paramAutoType =	convertAutoParameterType(type);

						pShader->setAutoParameter(name, paramAutoType);
					}
				}
			}
		}
		
		//texture units
		unsigned int i = 0;
		std::string key = "texture_unit[" + core::intToString(i) + "]";
		while (pConf->has(key))
		{
			std::string texture;
			if (pConf->has(key + ".texture[@value]"))
				texture = pConf->getString(key + ".texture[@value]");

			render::TextureUnit* pTextureUnit = new render::TextureUnit();
			pTextureUnit->setTexture(texture);
			renderMaterial->addTextureUnit(pTextureUnit);

			if (pConf->has("color_op"))
			{
				std::string operation;
				if (pConf->has(key + ".color_op[@operation]"))
					operation = pConf->getString(key + ".color_op[@operation]");
				std::string source1;
				if (pConf->has(key + ".color_op[@source1]"))
					source1 = pConf->getString(key + ".color_op[@source1]");
				std::string source2;
				if (pConf->has(key + ".color_op[@source2]"))
					source2 = pConf->getString(key + ".color_op[@source2]");

				render::LayerBlendOperation blendOperation = convertBlendOperation(operation);
				render::LayerBlendSource blendSource1 = convertBlendSource(source1);
				render::LayerBlendSource blendSource2 = convertBlendSource(source2);
				render::Color colorArg1 = render::Color::White;
				render::Color colorArg2 = render::Color::White;
				float manualBlend = 0.0f;

				if (blendSource1 == render::LBS_MANUAL)
				{
					float r = 1.0f;
					float g = 1.0f;
					float b = 1.0f;
					float a = 1.0f;

					if (pConf->has(key + ".color_op[@r1]"))
						r = (float)pConf->getDouble(key + ".color_op[@r1]");
					if (pConf->has(key + ".color_op[@g1]"))
						g = (float)pConf->getDouble(key + ".color_op[@g1]");
					if (pConf->has(key + ".color_op[@b1]"))
						b = (float)pConf->getDouble(key + ".color_op[@b1]");
					if (pConf->has(key + ".color_op[@a1]"))
						a = (float)pConf->getDouble(key + ".color_op[@a1]");

					colorArg1 = render::Color(r,g,b,a);
				}

				if (blendSource2 == render::LBS_MANUAL)
				{
					float r = 1.0f;
					float g = 1.0f;
					float b = 1.0f;
					float a = 1.0f;

					if (pConf->has(key + ".color_op[@r2]"))
						r = (float)pConf->getDouble(key + ".color_op[@r2]");
					if (pConf->has(key + ".color_op[@g2]"))
						g = (float)pConf->getDouble(key + ".color_op[@g2]");
					if (pConf->has(key + ".color_op[@b2]"))
						b = (float)pConf->getDouble(key + ".color_op[@b2]");
					if (pConf->has(key + ".color_op[@a2]"))
						a = (float)pConf->getDouble(key + ".color_op[@a2]");

					colorArg2 = render::Color(r,g,b,a);
				}

				if (blendOperation == render::LBX_BLEND_MANUAL)
				{
					if (pConf->has(key + ".color_op[@manualBlend]"))
						manualBlend = (float)pConf->getDouble(key + ".color_op[@manualBlend]");
				}

				pTextureUnit->setColorOperation(blendOperation, blendSource1, blendSource2, colorArg1, colorArg2, manualBlend);
			}

			if (pConf->has("alpha_op"))
			{
				std::string operation;
				if (pConf->has(key + ".alpha_op[@operation]"))
					operation = pConf->getString(key + ".alpha_op[@operation]");
				std::string source1;
				if (pConf->has(key + ".alpha_op[@source1]"))
					source1 = pConf->getString(key + ".alpha_op[@source1]");
				std::string source2;
				if (pConf->has(key + ".alpha_op[@source2]"))
					source2 = pConf->getString(key + ".alpha_op[@source2]");

				render::LayerBlendOperation blendOperation = convertBlendOperation(operation);
				render::LayerBlendSource blendSource1 = convertBlendSource(source1);
				render::LayerBlendSource blendSource2 = convertBlendSource(source2);
				float arg1 = 1.0f;
				float arg2 = 1.0f;
				float manualBlend = 0.0f;

				if (blendSource1 == render::LBS_MANUAL)
				{
					if (pConf->has(key + ".alpha_op[@arg1]"))
						arg1 = (float)pConf->getDouble(key + ".alpha_op[@arg1]");
				}

				if (blendSource2 == render::LBS_MANUAL)
				{
					if (pConf->has(key + ".alpha_op[@arg2]"))
						arg2 = (float)pConf->getDouble(key + ".alpha_op[@arg2]");
				}

				if (blendOperation == render::LBX_BLEND_MANUAL)
				{
					if (pConf->has(key + ".alpha_op[@manualBlend]"))
						manualBlend = (float)pConf->getDouble(key + ".alpha_op[@manualBlend]");
				}

				pTextureUnit->setAlphaOperation(blendOperation, blendSource1, blendSource2, arg1, arg2, manualBlend);
			}
		
			if (pConf->has("tex_coord_set"))
			{
				unsigned int tex_coord_set = 0;
				if (pConf->has(key + ".tex_coord_set[@value]"))
					tex_coord_set = (unsigned int)pConf->getInt(key + ".tex_coord_set[@value]");
				pTextureUnit->setTextureCoordSet(tex_coord_set);
			}


			key = "texture_unit[" + core::intToString(++i) + "]";
		}
	}

	if (dest->getResourceType() == RESOURCE_TYPE_PHYSICS_MATERIAL)
	{
		float resitution = 0.0f;
		float static_friction = 0.0f;
		float dynamic_friction = 0.0f;

		if (pConf->has("restitution[@value]"))
			resitution = (float)pConf->getDouble("restitution[@value]");
		if (pConf->has("static_friction[@value]"))
			static_friction = (float)pConf->getDouble("static_friction[@value]");
		if (pConf->has("dynamic_friction[@value]"))
			dynamic_friction = (float)pConf->getDouble("dynamic_friction[@value]");
		
		physicsMaterial->setRestitution(resitution);	
		physicsMaterial->setStaticFriction(static_friction);
		physicsMaterial->setDynamicFriction(dynamic_friction);
	}
	//////////////////////////////////////////////////////////////////////////

	return true;
}

bool MaterialSerializer::exportResource(Resource* source, const std::string& filename)
{
	return false;
}

}// end namespace resource
