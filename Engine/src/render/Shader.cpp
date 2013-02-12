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

#include <core/Log.h>
#include <core/LogDefines.h>
#include <core/System.h>
#include <render/Shader.h>
#include <render/Light.h>
#include <render/Texture.h>
#include <resource/ResourceManager.h>

#include <stdio.h>
#include <limits>
#include <iostream>
#include <fstream>

namespace render
{

Shader::Shader(const std::string& name, resource::Serializer* serializer): resource::Resource(name, serializer)
{
	mResourceType	= resource::RESOURCE_TYPE_SHADER;
	mShaderType		= SHADER_TYPE_VERTEX;
	mGLHandle		= 0;
	mSource			= "";
}

Shader::~Shader() {}

void Shader::setShaderType(const ShaderType& type)
{
	mShaderType = type;
}

ShaderType Shader::getShaderType()
{
	return mShaderType;
}

void Shader::setSource(const std::string& source)
{
	mSource = source;
}

void Shader::setEntryPoint(const std::string& entry)
{
	mEntryPoint = entry;
}


GLhandleARB Shader::getGLHandle() const
{
	return mGLHandle;
}

bool Shader::loadImpl()
{
	if (resource::ResourceManager::getInstance() == nullptr)
		return false;

	std::string filePath = resource::ResourceManager::getInstance()->getDataPath() + "/" + mFilename;
	
	FILE* pFile = fopen(filePath.c_str(), "rb");
	
	if (pFile == nullptr)
		return false;

		
	fseek(pFile, 0, SEEK_END);
	mSize = (unsigned int)ftell(pFile);

	fseek(pFile, 0, SEEK_SET);

	char* pBuffer = new char[mSize];
	fread(pBuffer, mSize, 1, pFile);

	mSource.insert(0, (const char*)pBuffer, mSize);
	SAFE_DELETE_ARRAY(pBuffer);

	fclose(pFile);

	mGLHandle = glCreateShader(getShaderType(mShaderType));

	const char* source = mSource.c_str();
	glShaderSource(mGLHandle, 1, &source, NULL);

	int InfoLogLength;
	GLint compiled;
	glCompileShader(mGLHandle);
	// check for compile errors
	glGetShaderiv(mGLHandle, GL_COMPILE_STATUS, &compiled);
#ifdef _DEBUG
	glGetShaderiv(mGLHandle, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(mGLHandle, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}
#endif

	return (compiled == 1);
}

void Shader::unloadImpl()
{
	glDeleteShader(mGLHandle);
}

GLenum Shader::getShaderType(ShaderType type)
{
	switch(type)
	{
	case SHADER_TYPE_VERTEX:
		return GL_VERTEX_SHADER;
	case SHADER_TYPE_FRAGMENT:
		return GL_FRAGMENT_SHADER;
	case SHADER_TYPE_GEOMETRY:
		return GL_GEOMETRY_SHADER;
	default:
		return 0x0000;
	}

	return 0x0000;
}

} // end namespace render
