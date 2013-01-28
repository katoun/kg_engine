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

#include <Config.h>
#include <resource/Resource.h>
#include <resource/ResourceEventReceiver.h>
#include <render/ShaderParameterDefines.h>
#include <render/RenderStateData.h>
#include <render/TextureDefines.h>
#include <render/VertexBufferDefines.h>
#include <render/Color.h>
#include <core/Vector2d.h>
#include <core/Vector3d.h>
#include <core/Vector4d.h>
#include <core/Matrix4.h>

#include <string>
#include <list>
#include <vector>

namespace resource
{
class Serializer;
}

namespace render
{

class Texture;
class Shader;
struct ShaderParameter;
struct ShaderVertexParameter;
struct ShaderTextureParameter;
struct ShaderAutoParameter;

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

	std::vector<ShaderVertexParameter*>& getVertexParameters();
	std::vector<ShaderTextureParameter*>& getTextureParameters();
	std::list<ShaderAutoParameter*>& getAutoParameters();

	void addVertexParameter(const std::string& name, VertexBufferType type);
	void addTextureParameter(const std::string& name, ShaderParameterType type);
	void addAutoParameter(const std::string& name, ShaderAutoParameterType type);

	void addParameter(const std::string& name, ShaderParameterType type);

	void setParameter(const std::string& name, const Color& col);
	void setParameter(ShaderParameter* parameter, const Color& col);
	void setParameter(const std::string& name, const core::vector2d& vec);
	void setParameter(ShaderParameter* parameter, const core::vector2d& vec);
	void setParameter(const std::string& name, const core::vector3d& vec);
	void setParameter(ShaderParameter* parameter, const core::vector3d& vec);
	void setParameter(const std::string& name, const core::vector4d& vec);
	void setParameter(ShaderParameter* parameter, const core::vector4d& vec);
	void setParameter(const std::string& name, const core::matrix4& m);
	void setParameter(ShaderParameter* parameter, const core::matrix4& m);

protected:

	void unloadImpl();

	// Textures
	std::list<Texture*> mTextureUnits;

	Shader* mVertexShader;
	Shader* mFragmentShader;
	Shader* mGeometryShader;

	ShaderParameter* createParameter(const std::string& name, ShaderParameterType type);

	virtual ShaderVertexParameter* createVertexParameterImpl();
	virtual ShaderParameter* createParameterImpl();

	ShaderParameter* findParameter(const std::string& name);
	ShaderTextureParameter* findTextureParameter(ShaderParameter* parameter);
	ShaderAutoParameter* findAutoParameter(ShaderParameter* parameter);

	void removeAllParameters();

	virtual void setParameterImpl(ShaderParameter* parameter, const Color& col);
	virtual void setParameterImpl(ShaderParameter* parameter, const core::vector2d& vec);
	virtual void setParameterImpl(ShaderParameter* parameter, const core::vector3d& vec);
	virtual void setParameterImpl(ShaderParameter* parameter, const core::vector4d& vec);
	virtual void setParameterImpl(ShaderParameter* parameter, const core::matrix4& m);

	static ShaderParameterType getType(ShaderAutoParameterType type);
	static ShaderParameterType getType(TextureType type);

	hashmap<std::string, ShaderParameter*> mParameters;
	std::vector<ShaderVertexParameter*> mVertexParameters;
	std::vector<ShaderTextureParameter*> mTextureParameters;
	std::list<ShaderAutoParameter*> mAutoParameters;
};

} //namespace render

#endif