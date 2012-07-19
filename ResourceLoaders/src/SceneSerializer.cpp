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

#include <Win32SceneSerializer.h>
#include <resource/DataStream.h>
#include <scene/Node.h>
#include <scene/Scene.h>
#include <render/Light.h>
#include <render/Camera.h>
#include <render/Model.h>
#include <render/RenderManager.h>
#include <sound/Sound.h>
#include <sound/SoundManager.h>
#include <physics/Body.h>
#include <physics/PhysicsManager.h>
#include <engine/LogManager.h>
#include <core/Utils.h>

namespace resource
{

bool parseScene(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Scene"<<std::endl;
#endif
	// update section
	context.section = SSS_SCENE;

	// Return true because this must be followed by a {
	return true;
}

bool parseNodes(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Nodes"<<std::endl;
#endif
	context.node = NULL;

	// update section
	context.section = SSS_NODES;

	// Return true because this must be followed by a {
	return true;
}

bool parseLight(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Shape"<<std::endl;
#endif
		
	// update section
	context.section = SSS_LIGHT;

	// Return true because this must be followed by a {
	return true;
}

bool parseCamera(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Camera"<<std::endl;
#endif

	// update section
	context.section = SSS_CAMERA;

	// Return true because this must be followed by a {
	return true;
}

bool parseModel(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Model"<<std::endl;
#endif

	// update section
	context.section = SSS_MODEL;

	// Return true because this must be followed by a {
	return true;
}

bool parseSound(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Sound"<<std::endl;
#endif

	// update section
	context.section = SSS_SOUND;

	// Return true because this must be followed by a {
	return true;
}

bool parseBody(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Body"<<std::endl;
#endif

	// update section
	context.section = SSS_BODY;

	// Return true because this must be followed by a {
	return true;
}

bool parseNodePosition(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Node Position"<<std::endl;
#endif

	switch(context.section)
	{	
	case SSS_LIGHT:
		{
			context.tempLight->setPosition(parseVector3d(params));
		}
		break;
	case SSS_CAMERA:
		{
			context.tempCamera->setPosition(parseVector3d(params));
		}
		break;
	case SSS_MODEL:
		{
			context.tempModel->setPosition(parseVector3d(params));
		}
		break;
	case SSS_SOUND:
		{
			context.tempSound->setPosition(parseVector3d(params));
		}
		break;
	case SSS_BODY:
		{
			context.tempBody->setPosition(parseVector3d(params));
		}
		break;
	}

	return false;
}

bool parseNodeOrientation(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Node Orientation"<<std::endl;
#endif

	switch(context.section)
	{	
	case SSS_LIGHT:
		{
			context.tempLight->setOrientation(parseQuaternion(params));
		}
		break;
	case SSS_CAMERA:
		{
			context.tempCamera->setOrientation(parseQuaternion(params));
		}
		break;
	case SSS_MODEL:
		{
			context.tempModel->setOrientation(parseQuaternion(params));
		}
		break;
	case SSS_SOUND:
		{
			context.tempSound->setOrientation(parseQuaternion(params));
		}
		break;
	case SSS_BODY:
		{
			context.tempBody->setOrientation(parseQuaternion(params));
		}
		break;
	}

	return false;
}

bool parseNodeScale(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Node Scale"<<std::endl;
#endif

	switch(context.section)
	{	
	case SSS_LIGHT:
		{
			context.tempLight->setScale(parseVector3d(params));
		}
		break;
	case SSS_CAMERA:
		{
			context.tempCamera->setScale(parseVector3d(params));
		}
		break;
	case SSS_MODEL:
		{
			context.tempModel->setScale(parseVector3d(params));
		}
		break;
	case SSS_SOUND:
		{
			context.tempSound->setScale(parseVector3d(params));
		}
		break;
	case SSS_BODY:
		{
			context.tempBody->setScale(parseVector3d(params));
		}
		break;
	}

	return false;
}

bool parseNodeName(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse NodeName"<<std::endl;
#endif

	context.nodeName = params;
	
	return false;
}

bool parseParentName(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse ParentName"<<std::endl;
#endif

	context.parentName = params;

	return false;
}

bool parseResourceName(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse ResourceName"<<std::endl;
#endif

	context.resourceName = params;

	return false;
}

render::LightType convertLightType(const std::string& param)
{
	if (param == "point")
		return render::LT_POINT;
	else if (param == "directional")
		return render::LT_DIRECTIONAL;
	else if (param == "spotlight")
		return render::LT_SPOTLIGHT;
	else
	{
		engine::LogManager::getInstance()->logMessage("Win32SceneSerializer", "Invalid light type", engine::LL_ERROR);

		return render::LT_POINT;
	}
}

bool parseLightType(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Light Type"<<std::endl;
#endif

	context.tempLight->setLightType(convertLightType(params));

	return false;
}

bool parseLightAmbient(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Light AmbientColor"<<std::endl;
#endif

	context.tempLight->setAmbientColor(parseColor(params));

	return false;
}

bool parseLightSpecular(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Light SpecuralColor"<<std::endl;
#endif

	context.tempLight->setSpecularColor(parseColor(params));

	return false;
}

bool parseLightDiffuse(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Light DiffuseColor"<<std::endl;
#endif

	context.tempLight->setDiffuseColor(parseColor(params));

	return false;
}

bool parseLightAttenuationRange(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Light AttenuationRange"<<std::endl;
#endif

	float attenuationRange = core::stringToFloat(params);

	context.tempLight->setAttenuationRange(attenuationRange);

	return false;
}

bool parseLightAttenuationConstant(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Light AttenuationConstant"<<std::endl;
#endif

	float attenuationConstant = core::stringToFloat(params);

	context.tempLight->setAttenuationConstant(attenuationConstant);

	return false;
}

bool parseLightAttenuationLinear(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Light AttenuationLinear"<<std::endl;
#endif

	float attenuationLinear = core::stringToFloat(params);

	context.tempLight->setAttenuationLinear(attenuationLinear);

	return false;
}

bool parseLightAttenuationQuadric(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Light AttenuationQuadric"<<std::endl;
#endif

	float attenuationQuadric = core::stringToFloat(params);

	context.tempLight->setAttenuationQuadric(attenuationQuadric);

	return false;
}

bool parseLightSpotlightInnerAngle(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Light SpotlightInnerAngle"<<std::endl;
#endif

	float spotlightInnerAngle = core::stringToFloat(params);

	context.tempLight->setSpotlightInnerAngle(spotlightInnerAngle);

	return false;
}

bool parseLightSpotlightOuterAngle(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Light SpotlightOuterAngle"<<std::endl;
#endif

	float spotlightOuterAngle = core::stringToFloat(params);

	context.tempLight->setSpotlightOuterAngle(spotlightOuterAngle);

	return false;
}

bool parseLightSpotlightFalloff(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Light SpotlightFalloff"<<std::endl;
#endif

	float spotlightFalloff = core::stringToFloat(params);

	context.tempLight->setSpotlightFalloff(spotlightFalloff);

	return false;
}

bool parseLightPowerScale(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Light PowerScale"<<std::endl;
#endif

	float powerScale = core::stringToFloat(params);

	context.tempLight->setPowerScale(powerScale);

	return false;
}

render::ProjectionType convertCameraProjectionType(const std::string& param)
{
	if (param == "orthographic")
		return render::PT_ORTHOGRAPHIC;
	else if (param == "perspective")
		return render::PT_PERSPECTIVE;
	else
	{
		engine::LogManager::getInstance()->logMessage("Win32SceneSerializer", "Invalid camera projection type", engine::LL_ERROR);

		return render::PT_PERSPECTIVE;
	}
}

bool parseCameraProjectionType(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Camera ProjectionType"<<std::endl;
#endif

	context.tempCamera->setProjectionType(convertCameraProjectionType(params));

	return false;
}

bool parseCameraFOV(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Camera FOV"<<std::endl;
#endif

	float fov = core::stringToFloat(params);

	context.tempCamera->setFOV(fov);

	return false;
}

bool parseCameraNearDistance(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Camera Near Distance"<<std::endl;
#endif

	float nearDistance = core::stringToFloat(params);

	context.tempCamera->setNearClipDistance(nearDistance);

	return false;
}

bool parseCameraFarDistance(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Camera Far Distance"<<std::endl;
#endif

	float farDistance = core::stringToFloat(params);

	context.tempCamera->setFarClipDistance(farDistance);

	return false;
}

bool parseCameraAspectRatio(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Camera Aspect Ratio"<<std::endl;
#endif

	float aspectRatio = core::stringToFloat(params);

	context.tempCamera->setAspectRatio(aspectRatio);

	return false;
}

bool parseModelMaterial(std::string& params, SceneScriptContext& context)
{
#ifdef _DEBUG
	std::cout<<"Parse Model Material"<<std::endl;
#endif

	context.tempModel->setMaterial(params);

	return false;
}

void createNode(SceneScriptContext& context)
{
	switch(context.section)
	{	
	case SSS_LIGHT:
		{
			if (context.nodeName != core::STRING_BLANK)
				context.light = render::RenderManager::getInstance()->createLight(context.nodeName);
			else
				context.light = render::RenderManager::getInstance()->createLight();
			
			//Set light properties
			*context.light = *context.tempLight;

			context.scene->addNode(context.light);
			context.sceneNodes[context.light->getName()] = context.light;
			context.sceneParents[context.light->getName()] = context.parentName;
		}
		break;
	case SSS_CAMERA:
		{
			if (context.nodeName != core::STRING_BLANK)
				context.camera = render::RenderManager::getInstance()->createCamera(context.nodeName);
			else
				context.camera = render::RenderManager::getInstance()->createCamera();
			//////////////////////////////////////////////////////////////////////////
			//Set camera properties
			context.camera->setPosition(context.tempCamera->getPosition());
			context.camera->setOrientation(context.tempCamera->getOrientation());
			context.camera->setScale(context.tempCamera->getScale());
			context.camera->setProjectionType(context.tempCamera->getProjectionType());
			context.camera->setFOV(context.tempCamera->getFOV());
			context.camera->setNearClipDistance(context.tempCamera->getNearClipDistance());
			context.camera->setFarClipDistance(context.tempCamera->getFarClipDistance());
			context.camera->setAspectRatio(context.tempCamera->getAspectRatio());
			//////////////////////////////////////////////////////////////////////////

			context.scene->addNode(context.camera);
			context.sceneNodes[context.camera->getName()] = context.camera;
			context.sceneParents[context.camera->getName()] = context.parentName;
		}
		break;
	case SSS_MODEL:
		{
			assert(context.resourceName != core::STRING_BLANK);

			if (context.nodeName != core::STRING_BLANK)
				context.model = render::RenderManager::getInstance()->createModel(context.nodeName, context.resourceName);
			else
				context.model = render::RenderManager::getInstance()->createModel(context.resourceName);
			//////////////////////////////////////////////////////////////////////////
			//Set model properties
			context.model->setPosition(context.tempModel->getPosition());
			context.model->setOrientation(context.tempModel->getOrientation());
			context.model->setScale(context.tempModel->getScale());
			context.model->setMaterial(context.tempModel->getMaterial());
			//////////////////////////////////////////////////////////////////////////

			context.scene->addNode(context.model);
			context.sceneNodes[context.model->getName()] = context.model;
			context.sceneParents[context.model->getName()] = context.parentName;
		}
		break;
	case SSS_SOUND:
		{
			assert(context.resourceName != core::STRING_BLANK);

			if (context.nodeName != core::STRING_BLANK)
				context.sound = sound::SoundManager::getInstance()->createSound(context.nodeName, context.resourceName);
			else
				context.sound = sound::SoundManager::getInstance()->createSound(context.resourceName);
			//////////////////////////////////////////////////////////////////////////
			//Set sound properties
			context.sound->setPosition(context.tempSound->getPosition());
			context.sound->setOrientation(context.tempSound->getOrientation());
			context.sound->setScale(context.tempSound->getScale());
			//////////////////////////////////////////////////////////////////////////

			context.scene->addNode(context.sound);
			context.sceneNodes[context.sound->getName()] = context.sound;
			context.sceneParents[context.sound->getName()] = context.parentName;
		}
		break;
	case SSS_BODY:
		{
			assert(context.resourceName != core::STRING_BLANK);

			if (context.nodeName != core::STRING_BLANK)
				context.body = physics::PhysicsManager::getInstance()->createBody(context.nodeName, context.resourceName);
			else
				context.body = physics::PhysicsManager::getInstance()->createBody(context.resourceName);
			//////////////////////////////////////////////////////////////////////////
			//Set actor properties
			context.body->setPosition(context.tempBody->getPosition());
			context.body->setOrientation(context.tempBody->getOrientation());
			context.body->setScale(context.tempBody->getScale());
			//////////////////////////////////////////////////////////////////////////

			context.scene->addNode(context.body);
			context.sceneNodes[context.body->getName()] = context.body;
			context.sceneParents[context.body->getName()] = context.parentName;
		}
		break;
	}
}

void setNodeParents(SceneScriptContext& context)
{
	hashmap<std::string, scene::Node*>::const_iterator i,j;

	for (i = context.sceneNodes.begin(); i != context.sceneNodes.end(); ++i)
	{
		hashmap<std::string, std::string>::const_iterator k = context.sceneParents.find(i->first);
		if (k != context.sceneParents.end() && k->second != "")
		{
			j = context.sceneNodes.find(k->second);
			if (j != context.sceneNodes.end())
				i->second->setParent(j->second);
		}
	}
}

Win32SceneSerializer::Win32SceneSerializer()
{
	// Version number
	mVersion = "[Win32SceneSerializer_v1.00]";

	// Set up root attribute parsers
	mRootAttribParsers["scene"] = (SCENE_ATTRIBUTE_PARSER)parseScene;

	// Set up scene attribute parsers
	mSceneAttribParsers["nodes"] = (SCENE_ATTRIBUTE_PARSER)parseNodes;

	// Set up nodes attribute parsers
	mNodesAttribParsers["light"] = (SCENE_ATTRIBUTE_PARSER)parseLight;
	mNodesAttribParsers["camera"] = (SCENE_ATTRIBUTE_PARSER)parseCamera;
	mNodesAttribParsers["model"] = (SCENE_ATTRIBUTE_PARSER)parseModel;
	mNodesAttribParsers["sound"] = (SCENE_ATTRIBUTE_PARSER)parseSound;
	mNodesAttribParsers["body"] = (SCENE_ATTRIBUTE_PARSER)parseBody;

	// Set up light attribute parsers
	mLightAttribParsers["name"] = (SCENE_ATTRIBUTE_PARSER)parseNodeName;
	mLightAttribParsers["parent"] = (SCENE_ATTRIBUTE_PARSER)parseParentName;
	mLightAttribParsers["position"] = (SCENE_ATTRIBUTE_PARSER)parseNodePosition;
	mLightAttribParsers["orientation"] = (SCENE_ATTRIBUTE_PARSER)parseNodeOrientation;
	mLightAttribParsers["type"] = (SCENE_ATTRIBUTE_PARSER)parseLightType;
	mLightAttribParsers["scale"] = (SCENE_ATTRIBUTE_PARSER)parseNodeScale;
	mLightAttribParsers["ambient_color"] = (SCENE_ATTRIBUTE_PARSER)parseLightAmbient;
	mLightAttribParsers["specular_color"] = (SCENE_ATTRIBUTE_PARSER)parseLightSpecular;
	mLightAttribParsers["diffuse_color"] = (SCENE_ATTRIBUTE_PARSER)parseLightDiffuse;
	mLightAttribParsers["attenuation_range"] = (SCENE_ATTRIBUTE_PARSER)parseLightAttenuationRange;
	mLightAttribParsers["attenuation_constant"] = (SCENE_ATTRIBUTE_PARSER)parseLightAttenuationConstant;
	mLightAttribParsers["attenuation_linear"] = (SCENE_ATTRIBUTE_PARSER)parseLightAttenuationLinear;
	mLightAttribParsers["attenuation_quadric"] = (SCENE_ATTRIBUTE_PARSER)parseLightAttenuationQuadric;
	mLightAttribParsers["spotlight_inner_angle"] = (SCENE_ATTRIBUTE_PARSER)parseLightSpotlightInnerAngle;
	mLightAttribParsers["spotlight_outer_angle"] = (SCENE_ATTRIBUTE_PARSER)parseLightSpotlightOuterAngle;
	mLightAttribParsers["spotlight_falloff"] = (SCENE_ATTRIBUTE_PARSER)parseLightSpotlightFalloff;
	mLightAttribParsers["power_scale"] = (SCENE_ATTRIBUTE_PARSER)parseLightPowerScale;

	// Set up camera attribute parsers
	mCameraAttribParsers["name"] = (SCENE_ATTRIBUTE_PARSER)parseNodeName;
	mCameraAttribParsers["parent"] = (SCENE_ATTRIBUTE_PARSER)parseParentName;
	mCameraAttribParsers["position"] = (SCENE_ATTRIBUTE_PARSER)parseNodePosition;
	mCameraAttribParsers["orientation"] = (SCENE_ATTRIBUTE_PARSER)parseNodeOrientation;
	mCameraAttribParsers["scale"] = (SCENE_ATTRIBUTE_PARSER)parseNodeScale;
	mCameraAttribParsers["projection_type"] = (SCENE_ATTRIBUTE_PARSER)parseCameraProjectionType;
	mCameraAttribParsers["fov"] = (SCENE_ATTRIBUTE_PARSER)parseCameraFOV;
	mCameraAttribParsers["near_distance"] = (SCENE_ATTRIBUTE_PARSER)parseCameraNearDistance;
	mCameraAttribParsers["far_distance"] = (SCENE_ATTRIBUTE_PARSER)parseCameraFarDistance;
	mCameraAttribParsers["aspect_ratio"] = (SCENE_ATTRIBUTE_PARSER)parseCameraAspectRatio;

	// Set up model attribute parsers
	mModelAttribParsers["name"] = (SCENE_ATTRIBUTE_PARSER)parseNodeName;
	mModelAttribParsers["parent"] = (SCENE_ATTRIBUTE_PARSER)parseParentName;
	mModelAttribParsers["resource"] = (SCENE_ATTRIBUTE_PARSER)parseResourceName;
	mModelAttribParsers["material"] = (SCENE_ATTRIBUTE_PARSER)parseModelMaterial;
	mModelAttribParsers["position"] = (SCENE_ATTRIBUTE_PARSER)parseNodePosition;
	mModelAttribParsers["orientation"] = (SCENE_ATTRIBUTE_PARSER)parseNodeOrientation;
	mModelAttribParsers["scale"] = (SCENE_ATTRIBUTE_PARSER)parseNodeScale;

	// Set up sound attribute parsers
	mSoundAttribParsers["name"] = (SCENE_ATTRIBUTE_PARSER)parseNodeName;
	mSoundAttribParsers["parent"] = (SCENE_ATTRIBUTE_PARSER)parseParentName;
	mSoundAttribParsers["resource"] = (SCENE_ATTRIBUTE_PARSER)parseResourceName;
	mSoundAttribParsers["position"] = (SCENE_ATTRIBUTE_PARSER)parseNodePosition;
	mSoundAttribParsers["orientation"] = (SCENE_ATTRIBUTE_PARSER)parseNodeOrientation;
	mSoundAttribParsers["scale"] = (SCENE_ATTRIBUTE_PARSER)parseNodeScale;

	// Set up body attribute parsers
	mBodyAttribParsers["name"] = (SCENE_ATTRIBUTE_PARSER)parseNodeName;
	mBodyAttribParsers["parent"] = (SCENE_ATTRIBUTE_PARSER)parseParentName;
	mBodyAttribParsers["resource"] = (SCENE_ATTRIBUTE_PARSER)parseResourceName;
	mBodyAttribParsers["position"] = (SCENE_ATTRIBUTE_PARSER)parseNodePosition;
	mBodyAttribParsers["orientation"] = (SCENE_ATTRIBUTE_PARSER)parseNodeOrientation;
	mBodyAttribParsers["scale"] = (SCENE_ATTRIBUTE_PARSER)parseNodeScale;
			
	mScriptContext.section = SSS_NONE;
	mScriptContext.scene = NULL;
	mScriptContext.node = NULL;
	mScriptContext.camera = NULL;
	mScriptContext.light = NULL;
	mScriptContext.model = NULL;
	mScriptContext.sound = NULL;
	mScriptContext.body = NULL;
	mScriptContext.nodeName = core::STRING_BLANK;
	mScriptContext.parentName = core::STRING_BLANK;
	mScriptContext.resourceName = core::STRING_BLANK;
	mScriptContext.tempCamera = NULL;
	mScriptContext.tempLight = NULL;
	mScriptContext.tempModel = NULL;
	mScriptContext.tempSound = NULL;
	mScriptContext.tempBody = NULL;

	mScriptContext.lineNo = 0;
}

Win32SceneSerializer::~Win32SceneSerializer()
{
	mScriptContext.scene = NULL;
	mScriptContext.node = NULL;
	mScriptContext.camera = NULL;
	mScriptContext.light = NULL;
	mScriptContext.model = NULL;
	mScriptContext.sound = NULL;
	mScriptContext.body = NULL;
	mScriptContext.tempCamera = NULL;
	mScriptContext.tempLight = NULL;
	mScriptContext.tempModel = NULL;
	mScriptContext.tempSound = NULL;
	mScriptContext.tempBody = NULL;
}

bool Win32SceneSerializer::importResource(Resource* dest, const std::string& filename)
{
	assert(dest != NULL);
	if (dest == NULL)
		return false;

	if (dest->getResourceType() != RT_RESOURCE_SCENE)
	{
		engine::LogManager::getInstance()->logMessage("Win32SceneSerializer", "Unable to load scene - invalid resource pointer.", engine::LL_ERROR);
		return false;
	}

	scene::Scene* scene = static_cast<scene::Scene*>(dest);
	assert(scene != NULL);
	if (scene == NULL)
		return false;

	mScriptContext.scene = scene;

	mScriptContext.tempCamera = render::RenderManager::getInstance()->createCamera();
	mScriptContext.tempLight = render::RenderManager::getInstance()->createLight();
	mScriptContext.tempModel = render::RenderManager::getInstance()->createModel("meshes/Mesh.mesh");
	mScriptContext.tempSound = sound::SoundManager::getInstance()->createSound("sounds/sound.wav");
	mScriptContext.tempBody = physics::PhysicsManager::getInstance()->createBody("bodies/Body.body");

	resource::DataStream* dataStream = resource::ResourceManager::getInstance()->openFile(filename);
	if (dataStream == NULL)
		return false;

	parseScript(dataStream);

	delete dataStream;

	render::RenderManager::getInstance()->removeCamera(mScriptContext.tempCamera);
	render::RenderManager::getInstance()->removeLight(mScriptContext.tempLight);
	render::RenderManager::getInstance()->removeModel(mScriptContext.tempModel);
	sound::SoundManager::getInstance()->removeSound(mScriptContext.tempSound);
	physics::PhysicsManager::getInstance()->removeBody(mScriptContext.tempBody);

	return true;
}

bool Win32SceneSerializer::exportResource(Resource* source, const std::string& filename)
{
	return false;
}

void Win32SceneSerializer::parseScript(DataStream* stream)
{
	std::string line;
	bool nextIsOpenBrace = false;

	assert(stream);

	if (!stream)
		return;

	while (!stream->eof())
	{
		line = readString(stream);
		mScriptContext.lineNo++;
		core::stringTrim(line);

		// Ignore comments & blanks
		if ((line.size() != 0) && (line.substr(0, 2) != "//"))
		{
			if (nextIsOpenBrace)
			{
				// parser will have changed context already
				if (line != "{")
				{
					std::string message = "Expecting '{' but got " + line + " instead.";

					engine::LogManager::getInstance()->logMessage("Win32SceneSerializer", message.c_str(), engine::LL_ERROR);

					return;
				}
				nextIsOpenBrace = false;
			}
			else
			{
				nextIsOpenBrace = parseScriptLine(line);
			}
		}
	}

	// Check all braces were closed
	if (mScriptContext.section != SSS_NONE)
	{
		engine::LogManager::getInstance()->logMessage("Win32SceneSerializer", "Unexpected end of file.", engine::LL_ERROR);

		return;
	}
}

bool Win32SceneSerializer::parseScriptLine(std::string& line)
{
	switch(mScriptContext.section)
	{
	case SSS_NONE:
		if (line == "}")
		{
			engine::LogManager::getInstance()->logMessage("Win32SceneSerializer", "Unexpected terminating brace.", engine::LL_ERROR);

			return false;
		}
		else
		{
			// find & invoke a parser
			return invokeParser(line, mRootAttribParsers); 
		}
		break;
	case SSS_SCENE:
		if (line == "}")
		{
			// End of scene
			mScriptContext.section = SSS_NONE;
			mScriptContext.scene = NULL;
		}
		else
		{
			// find & invoke a parser
			return invokeParser(line, mSceneAttribParsers); 
		}
		break;
	case SSS_NODES:
		if (line == "}")
		{
			setNodeParents(mScriptContext);
			// End of nodes
			mScriptContext.section = SSS_SCENE;
			mScriptContext.node = NULL;
		}
		else
		{
			// find & invoke a parser
			return invokeParser(line, mNodesAttribParsers);
		}
		break;
	case SSS_LIGHT:
		if (line == "}")
		{
			createNode(mScriptContext);
			// End of light
			mScriptContext.section = SSS_NODES;
			mScriptContext.light = NULL;
			mScriptContext.nodeName = core::STRING_BLANK;
			mScriptContext.parentName = core::STRING_BLANK;
			mScriptContext.resourceName = core::STRING_BLANK;
		}
		else
		{
			// find & invoke a parser
			return invokeParser(line, mLightAttribParsers);
		}
		break;
	case SSS_CAMERA:
		if (line == "}")
		{
			createNode(mScriptContext);
			// End of camera
			mScriptContext.section = SSS_NODES;
			mScriptContext.camera = NULL;
			mScriptContext.nodeName = core::STRING_BLANK;
			mScriptContext.parentName = core::STRING_BLANK;
			mScriptContext.resourceName = core::STRING_BLANK;
		}
		else
		{
			// find & invoke a parser
			return invokeParser(line, mCameraAttribParsers);
		}
		break;
	case SSS_MODEL:
		if (line == "}")
		{					
			createNode(mScriptContext);
			// End of mesh
			mScriptContext.section = SSS_NODES;
			mScriptContext.model = NULL;
			mScriptContext.nodeName = core::STRING_BLANK;
			mScriptContext.parentName = core::STRING_BLANK;
			mScriptContext.resourceName = core::STRING_BLANK;
		}
		else
		{
			// find & invoke a parser
			return invokeParser(line, mModelAttribParsers);
		}
		break;
	case SSS_SOUND:
		if (line == "}")
		{					
			createNode(mScriptContext);
			// End of mesh
			mScriptContext.section = SSS_NODES;
			mScriptContext.sound = NULL;
			mScriptContext.nodeName = core::STRING_BLANK;
			mScriptContext.parentName = core::STRING_BLANK;
			mScriptContext.resourceName = core::STRING_BLANK;
		}
		else
		{
			// find & invoke a parser
			return invokeParser(line, mSoundAttribParsers);
		}
		break;
	case SSS_BODY:
		if (line == "}")
		{					
			createNode(mScriptContext);
			// End of mesh
			mScriptContext.section = SSS_NODES;
			mScriptContext.body = NULL;
			mScriptContext.nodeName = core::STRING_BLANK;
			mScriptContext.parentName = core::STRING_BLANK;
			mScriptContext.resourceName = core::STRING_BLANK;
		}
		else
		{
			// find & invoke a parser
			return invokeParser(line, mBodyAttribParsers);
		}
		break;
	};

	return false;
}

bool Win32SceneSerializer::invokeParser(std::string& line, std::map<std::string, SCENE_ATTRIBUTE_PARSER>& parsers)
{	
	std::vector<std::string> splitLine = core::splitString(line, " \t", 1);
	std::map<std::string, SCENE_ATTRIBUTE_PARSER>::const_iterator i = parsers.find(splitLine[0]);
	if (i != parsers.end())
	{
		std::string params;
		if (splitLine.size() > 1)
			params = splitLine[1];

		return (*i->second)(params, mScriptContext);
	}
	else
	{
		std::string message = "Unrecognised attribute: " + splitLine[0] + ".";

		engine::LogManager::getInstance()->logMessage("Win32SceneSerializer", message, engine::LL_ERROR);

		return false;
	}

	return false;
}

}// end namespace resource
