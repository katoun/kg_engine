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

#ifndef _SHADER_H_
#define _SHADER_H_

#include <core/Config.h>
#include <resource/Resource.h>
#include <render/ShaderDefines.h>
#include <render/ShaderParamData.h>
#include <render/TextureDefines.h>
#include <render/Color.h>
#include <core/Vector2d.h>
#include <core/Vector3d.h>
#include <core/Vector4d.h>
#include <core/Matrix4.h>

#include <string>
#include <vector>
#include <map>

namespace resource
{
class Serializer;
}

namespace render
{

class Light;
class Texture;

struct ENGINE_PUBLIC_EXPORT ShaderParameter
{
	ShaderParameter();

	ShaderParameterType paramType;			// Param type
	ShaderAutoParameterType autoParamType;	// Auto Param type
	unsigned int index;								// Start index in buffer (either float, int or texture buffer)
	unsigned int elemCount;							// Number of elements
};

//! Abstract class representing a Shader resource.
//!
//! This class defines the low-level program in assembler code,
//! the sort used to directly assemble into machine instructions for the GPU to execute.
class ENGINE_PUBLIC_EXPORT Shader: public resource::Resource
{
public:

	Shader(const std::string& name, resource::Serializer* serializer);
	virtual ~Shader();

	//! Sets the type of texture,
	void setShaderType(const ShaderType& type);

	//! Gets the type of shader.
	ShaderType getShaderType();

	void setSource(const std::string& source);

	void setEntryPoint(const std::string& entry);

	virtual void bind();

	virtual void unbind();

	virtual void updateAutoParameters(ShaderParamData& data);

	virtual void setParameter(const std::string& name, const Color& col);
	virtual void setParameter(const std::string& name, const core::vector2d& vec);
	virtual void setParameter(const std::string& name, const core::vector3d& vec);
	virtual void setParameter(const std::string& name, const core::vector4d& vec);
	virtual void setParameter(const std::string& name, const core::matrix4& m);
	virtual void setParameter(const std::string& name, const float* val, unsigned int count);
	virtual void setParameter(const std::string& name, const signed int* val, unsigned int count);

	virtual void addParameter(const std::string& name, ShaderParameterType type);

	void setAutoParameter(const std::string& name, ShaderAutoParameterType type);

protected:

	virtual bool loadImpl();
	virtual void unloadImpl();

	ShaderType mShaderType;

	std::string mSource;		// The assembler source of the program (may be blank until file loaded)
	std::string mEntryPoint;	// Entry point eg. main_vp, main_fp etc

	ShaderParameter* createParameter(const std::string& name, ShaderParameterType type, unsigned int index, unsigned int elemCount);
	virtual ShaderParameter* createParameterImpl(const std::string& name);

	ShaderParameter* findParameter(const std::string& name);
	void writedParameterData(unsigned int index, const float* val, unsigned int count);
	void writedParameterData(unsigned int index, const signed int* val, unsigned int count);

	float* getFloatPrameterData(unsigned int index);
	const float* getFloatPrameterData(unsigned int index) const;

	signed int* getIntPrameterData(unsigned int index);
	const signed int* getIntPrameterData(unsigned int index) const;

	void removeAllParameters();

	static bool isFloat(ShaderParameterType type);
	static bool isFloat(ShaderAutoParameterType type);
	static bool isSampler(ShaderParameterType type);
	static unsigned int getElementCount(ShaderParameterType type);
	static unsigned int getElementCount(ShaderAutoParameterType type);
	static ShaderParameterType getType(ShaderAutoParameterType type);
	static ShaderParameterType getType(TextureType type);

	std::vector<float> mFloatParameterData;
	std::vector<signed int> mIntParameterData;
	hashmap<std::string, ShaderParameter*> mParameters;
};

} // end namespace render

#endif