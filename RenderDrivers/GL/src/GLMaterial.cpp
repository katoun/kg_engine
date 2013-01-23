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

#include <GLMaterial.h>
#include <GLShader.h>
#include <render/Shader.h>
#include <resource/ResourceEvent.h>

namespace render
{

GLMaterial::GLMaterial(const std::string& name, resource::Serializer* serializer): Material(name, serializer)
{
	mGLHandle = 0;
}

GLMaterial::~GLMaterial() {}

void GLMaterial::resourceLoaded(const resource::ResourceEvent& evt)
{
	if (evt.source != nullptr)
	{
		if (evt.source == mVertexShader)
		{
			GLShader* pGLShader = static_cast<GLShader*>(mVertexShader);
			if (pGLShader != nullptr)
			{
				glAttachShader(mGLHandle, pGLShader->getGLHandle());
			}
		}

		if (evt.source == mFragmentShader)
		{
			GLShader* pGLShader = static_cast<GLShader*>(mFragmentShader);
			if (pGLShader != nullptr)
			{
				glAttachShader(mGLHandle, pGLShader->getGLHandle());
			}
		}

		if (evt.source == mGeometryShader)
		{
			GLShader* pGLShader = static_cast<GLShader*>(mGeometryShader);
			if (pGLShader != nullptr)
			{
				glAttachShader(mGLHandle, pGLShader->getGLHandle());
			}
		}

		if ((mVertexShader != nullptr && mVertexShader->getState() == resource::RESOURCE_STATE_LOADED) &&
			(mFragmentShader != nullptr && mFragmentShader->getState() == resource::RESOURCE_STATE_LOADED) &&
			(mGeometryShader != nullptr && mGeometryShader->getState() == resource::RESOURCE_STATE_LOADED))
		{

			GLint linked;
			glLinkProgram(mGLHandle);

			glGetProgramiv(mGLHandle, GL_LINK_STATUS, &linked);
		}
	}
}

void GLMaterial::resourceUnloaded(const resource::ResourceEvent& evt)
{
}

bool GLMaterial::loadImpl()
{
	if (!Material::loadImpl()) return false;

	mGLHandle = glCreateProgram();

	return true;
}

void GLMaterial::unloadImpl()
{
	glDeleteProgram(mGLHandle);
}


} //namespace render
