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

#include <EngineConfig.h>
#include <render/GLConfig.h>
#include <resource/Resource.h>
#include <resource/ResourceEventReceiver.h>
#include <render/ShaderParameterDefines.h>
#include <render/RenderStateData.h>
#include <render/TextureDefines.h>
#include <render/VertexBufferDefines.h>
#include <render/Color.h>

#include <glm/glm.hpp>

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
//! To optimize rendering, changes in render state must be
//! minimized. One of the most frequent render state changes are
//! changes to materials, mostly to textures. An mesh can have 
//! only one material.
class ENGINE_PUBLIC_EXPORT Material: public resource::Resource, public resource::ResourceEventReceiver
{
public:

	Material(const std::string& name, resource::Serializer* serializer);
	virtual ~Material();

	void setAmbientColor(float red, float green, float blue);
	void setAmbientColor(const render::Color& color);
	const render::Color& getAmbientColor() const;

	void setDiffuseColor(float red, float green, float blue);
	void setDiffuseColor(const render::Color& color);
	const render::Color& getDiffuseColor() const;

	void setSpecularColor(float red, float green, float blue);
	void setSpecularColor(const render::Color& color);
	const render::Color& getSpecularColor() const;

	void setShininess(float shininess);
	float getShininess() const;

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

	void setParameter(const std::string& name, const float value);
	void setParameter(ShaderParameter* parameter, const float value);
	void setParameter(const std::string& name, const Color& col);
	void setParameter(ShaderParameter* parameter, const Color& col);
	void setParameter(const std::string& name, const glm::vec2& vec);
	void setParameter(ShaderParameter* parameter, const glm::vec2& vec);
	void setParameter(const std::string& name, const glm::vec3& vec);
	void setParameter(ShaderParameter* parameter, const glm::vec3& vec);
	void setParameter(const std::string& name, const glm::vec4& vec);
	void setParameter(ShaderParameter* parameter, const glm::vec4& vec);
	void setParameter(const std::string& name, const glm::mat4x4& m);
	void setParameter(ShaderParameter* parameter, const glm::mat4x4& m);

	void resourceLoaded(const resource::ResourceEvent& evt);
	void resourceUnloaded(const resource::ResourceEvent& evt);

	GLhandleARB getGLHandle() const;

protected:

	bool loadImpl();
	void unloadImpl();

	GLhandleARB mGLHandle;

	render::Color mAmbient;
	render::Color mDiffuse;
	render::Color mSpecular;
	float mShininess;
	
	Shader* mVertexShader;
	Shader* mFragmentShader;
	Shader* mGeometryShader;

	// Textures
	std::list<Texture*> mTextureUnits;

	ShaderParameter* createParameter(const std::string& name, ShaderParameterType type);

	ShaderParameter* findParameter(const std::string& name);
	ShaderTextureParameter* findTextureParameter(ShaderParameter* parameter);
	ShaderAutoParameter* findAutoParameter(ShaderParameter* parameter);

	void removeAllParameters();

	hashmap<std::string, ShaderParameter*> mParameters;
	std::vector<ShaderVertexParameter*> mVertexParameters;
	std::vector<ShaderTextureParameter*> mTextureParameters;
	std::list<ShaderAutoParameter*> mAutoParameters;
};

} //namespace render

#endif