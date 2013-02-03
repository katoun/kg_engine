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

#ifndef _GL_RENDERMATERIAL_H_
#define _GL_RENDERMATERIAL_H_

#include <GLConfig.h>
#include <render/Material.h>
#include <render/ShaderParameter.h>

namespace render
{

struct GLShaderParameter: public ShaderParameter
{
	GLShaderParameter();

	GLuint ParameterID;
};

struct GLShaderVertexParameter: public ShaderVertexParameter
{
	GLShaderVertexParameter();

	GLuint ParameterID;
};

class GL_PUBLIC_EXPORT GLMaterial: public Material
{
public:

	GLMaterial(const std::string& name, resource::Serializer* serializer);
	~GLMaterial();

	void resourceLoaded(const resource::ResourceEvent& evt);
	void resourceUnloaded(const resource::ResourceEvent& evt);

	GLhandleARB getGLHandle() const;

protected:

	bool loadImpl();

	void unloadImpl();

	ShaderVertexParameter* createVertexParameterImpl();
	ShaderParameter* createParameterImpl();

	void setParameterImpl(ShaderParameter* parameter, const Color& col);
	void setParameterImpl(ShaderParameter* parameter, const glm::vec2& vec);
	void setParameterImpl(ShaderParameter* parameter, const glm::vec3& vec);
	void setParameterImpl(ShaderParameter* parameter, const glm::vec4& vec);
	void setParameterImpl(ShaderParameter* parameter, const glm::mat4x4& m);

	static GLenum getGLType(ShaderParameterType type);

	GLhandleARB mGLHandle;
};

} //namespace render

#endif