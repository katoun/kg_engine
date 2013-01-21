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

#ifndef _GL_SHADER_H_
#define _GL_SHADER_H_

#include <GLConfig.h>
#include <render/Shader.h>

namespace render
{

struct GLShaderParameter: public ShaderParameter
{
	GLShaderParameter();

	//TODO!!!
};

class GL_PUBLIC_EXPORT GLShader: public Shader
{
public:

	GLShader(const std::string& name, resource::Serializer* serializer);
	~GLShader();

	void bind();

	void unbind();

	void setParameter(const std::string& name, const Color& col);
	void setParameter(const std::string& name, const core::vector2d& vec);
	void setParameter(const std::string& name, const core::vector3d& vec);
	void setParameter(const std::string& name, const core::vector4d& vec);
	void setParameter(const std::string& name, const core::matrix4& m);
	void setParameter(const std::string& name, const float* val, unsigned int count);
	void setParameter(const std::string& name, const int* val, unsigned int count);

	GLhandleARB getGLHandle() const;

protected:

	bool loadImpl();

	void unloadImpl();

	ShaderParameter* createParameterImpl(const std::string& name);

	GLhandleARB mGLHandle;

	static GLenum getShaderType(ShaderType type);
};

} // end namespace render

#endif