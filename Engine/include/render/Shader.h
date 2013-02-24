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

#include <EngineConfig.h>
#include <render/GLConfig.h>
#include <resource/Resource.h>
#include <render/ShaderDefines.h>
#include <render/TextureDefines.h>

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

	GLhandleARB getGLHandle() const;

protected:

	bool loadImpl();
	void unloadImpl();

	GLhandleARB mGLHandle;

	ShaderType mShaderType;

	std::string mSource;		// The assembler source of the program (may be blank until file loaded)
	std::string mEntryPoint;	// Entry point eg. main_vp, main_fp etc
};

} // end namespace render

#endif