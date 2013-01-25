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

#ifndef _RENDERMATERIAL_H_
#define _RENDERMATERIAL_H_

#include <core/Config.h>
#include <resource/Resource.h>
#include <resource/ResourceEventReceiver.h>
#include <render/ShaderDefines.h>
#include <render/ShaderParamData.h>
#include <render/TextureDefines.h>
#include <render/Color.h>
#include <core/Vector2d.h>
#include <core/Vector3d.h>
#include <core/Vector4d.h>
#include <core/Matrix4.h>

#include <string>
#include <list>

namespace resource
{
class Serializer;
}

namespace render
{

class Shader;
class Texture;
class VertexBuffer;

struct ENGINE_PUBLIC_EXPORT ShaderParameter
{
	ShaderParameter();

	ShaderParameterType paramType;			// Param type
	ShaderAutoParameterType autoParamType;	// Auto Param type
	unsigned int index;						// Start index in buffer (either float, int or texture buffer)
	unsigned int elemCount;					// Number of elements
};

//! Class encapsulates surface properties of a mesh.
//!
//! To optimise rendering, changes in render state must be
//! minimised. One of the most frequent render state changes are
//! changes to materials, mostly to textures. An mesh can have 
//! only one material.
class ENGINE_PUBLIC_EXPORT Material: public resource::Resource, public resource::ResourceEventReceiver
{
public:

	Material(const std::string& name, resource::Serializer* serializer);
	virtual ~Material();

	//! Inserts a new texture unit in the material.
	void addTextureUnit(const std::string& filename);
	void addTextureUnit(Texture* texture);

	//! Retrieves a pointer to a texture unit so it may be modified.
	Texture* getTextureUnit(unsigned int index) const;

	//! Returns the number of texture unit.
	unsigned int getNumTextureUnits() const;	
	
	//! Removes a texture unit.
	void removeTextureUnit(unsigned int index);

	//! Removes all texture unit.
	void removeAllTextureUnits();

	void setVertexShader(const std::string& filename);
	void setVertexShader(Shader* shader);
	Shader* getVertexShader();

	void setFragmentShader(const std::string& filename);
	void setFragmentShader(Shader* shader);
	Shader* getFragmentShader();

	void setGeometryShader(const std::string& filename);
	void setGeometryShader(Shader* shader);
	Shader* getGeometryShader();

	virtual void updateAutoParameters(ShaderParamData& data);

	virtual void setParameter(const std::string& name, const Color& col);
	virtual void setParameter(const std::string& name, const core::vector2d& vec);
	virtual void setParameter(const std::string& name, const core::vector3d& vec);
	virtual void setParameter(const std::string& name, const core::vector4d& vec);
	virtual void setParameter(const std::string& name, const core::matrix4& m);
	virtual void setParameter(const std::string& name, const float* val, unsigned int count);
	virtual void setParameter(const std::string& name, const int* val, unsigned int count);

	virtual void setParameter(const std::string& name, VertexBuffer* vertexBuffer);

	virtual void addParameter(const std::string& name, ShaderParameterType type);

	void setAutoParameter(const std::string& name, ShaderAutoParameterType type);

protected:

	void unloadImpl();

	// Textures
	std::list<Texture*> mTextureUnits;

	Shader* mVertexShader;
	Shader* mFragmentShader;
	Shader* mGeometryShader;

	ShaderParameter* createParameter(const std::string& name, ShaderParameterType type, unsigned int index, unsigned int elemCount);
	virtual ShaderParameter* createParameterImpl(const std::string& name, ShaderParameterType type);

	ShaderParameter* findParameter(const std::string& name);
	void writedParameterData(unsigned int index, const float* val, unsigned int count);
	void writedParameterData(unsigned int index, const int* val, unsigned int count);

	float* getFloatPrameterData(unsigned int index);
	const float* getFloatPrameterData(unsigned int index) const;

	int* getIntPrameterData(unsigned int index);
	const int* getIntPrameterData(unsigned int index) const;

	void removeAllParameters();

	static bool isFloat(ShaderParameterType type);
	static bool isFloat(ShaderAutoParameterType type);
	static bool isSampler(ShaderParameterType type);
	static unsigned int getElementCount(ShaderParameterType type);
	static unsigned int getElementCount(ShaderAutoParameterType type);
	static ShaderParameterType getType(ShaderAutoParameterType type);
	static ShaderParameterType getType(TextureType type);

	std::vector<float> mFloatParameterData;
	std::vector<int> mIntParameterData;
	hashmap<std::string, ShaderParameter*> mParameters;
};

} //namespace render

#endif