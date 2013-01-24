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
#include <render/Color.h>
#include <resource/ResourceEventReceiver.h>

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

	bool hasVertexShader();
	bool hasFragmentShader();
	bool hasGeometryShader();

protected:

	void unloadImpl();

	// Textures
	std::list<Texture*> mTextureUnits;

	Shader* mVertexShader;
	Shader* mFragmentShader;
	Shader* mGeometryShader;
};

} //namespace render

#endif