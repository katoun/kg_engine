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

#include <core/Utils.h>
#include <resource/Serializer.h>
#include <core/Vector3d.h>
#include <core/Quaternion.h>
#include <render/Color.h>
#include <resource/Resource.h>

#include <vector>

namespace resource
{

Serializer::Serializer()
{
	// Version number
	mVersion = "[Serializer_v1.00]";
}

Serializer::~Serializer() {}

core::vector3d parseVector3d(std::string& params)
{
#ifdef _DEBUG
	std::cout<<"Parse Vector3d"<<std::endl;
	std::cout<<params<<std::endl;
#endif
	
	std::vector<std::string> vecparams = core::splitString(params, " \t", 2);
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	
	if (vecparams.size() > 0)
		x = core::stringToFloat(vecparams[0]);
	if (vecparams.size() > 1)
		y = core::stringToFloat(vecparams[1]);
	if (vecparams.size() > 2)
		z = core::stringToFloat(vecparams[2]);

	return core::vector3d(x, y, z);
}

core::quaternion parseQuaternion(std::string& params)
{
#ifdef _DEBUG
	std::cout<<"Parse Quaternion"<<std::endl;
	std::cout<<params<<std::endl;
#endif

	std::vector<std::string> vecparams = core::splitString(params, " \t", 3);
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	if (vecparams.size() > 0)
		x = core::stringToFloat(vecparams[0]);
	if (vecparams.size() > 1)
		y = core::stringToFloat(vecparams[1]);
	if (vecparams.size() > 2)
		z = core::stringToFloat(vecparams[2]);
	if (vecparams.size() > 3)
		w = core::stringToFloat(vecparams[3]);

	return core::quaternion(x, y, z, w);
}

render::Color parseColor(std::string& params)
{
#ifdef _DEBUG
	std::cout<<"Parse Color"<<std::endl;
	std::cout<<params<<std::endl;
#endif
	core::stringToLower(params);
	if (params == "black")
	{
		return render::Color::Black;
	}
	else if (params == "white")
	{
		return render::Color::White;
	}
	else if (params == "red")
	{
		return render::Color::Red;
	}
	else if (params == "green")
	{
		return render::Color::Green;
	}
	else if (params == "blue")
	{
		return render::Color::Blue;
	}
	else
	{
		std::vector<std::string> vecparams = core::splitString(params, " \t", 3);

		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 0.0f;

		if (vecparams.size() > 0)
			r = core::stringToFloat(vecparams[0]);
		if (vecparams.size() > 1)
			g = core::stringToFloat(vecparams[1]);
		if (vecparams.size() > 2)
			b = core::stringToFloat(vecparams[2]);
		if (vecparams.size() > 3)
			a = core::stringToFloat(vecparams[3]);

		return render::Color(r, g, b, a);
	}
}

}// end namespace resource
