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

#include <render/RenderDriver.h>
#include <render/Shader.h>

namespace render
{

RenderDriver::RenderDriver(const std::string& name): core::SystemDriver(name)
{
	mWorldMatrix = core::matrix4::IDENTITY;
	mViewMatrix = core::matrix4::IDENTITY;
	mProjMatrix = core::matrix4::IDENTITY;

	mCurrentVertexShader = nullptr;
	mCurrentFragmentShader = nullptr;
	mCurrentGeometryShader = nullptr;
}

RenderDriver::~RenderDriver() {}

void RenderDriver::setWorldMatrix(const core::matrix4& m)
{
	mWorldMatrix = m;
}

void RenderDriver::setViewMatrix(const core::matrix4& m)
{
	mViewMatrix = m;
}

void RenderDriver::setProjectionMatrix(const core::matrix4& m)
{	
	mProjMatrix = m;
}

void RenderDriver::updateShaderAutoParameters(Shader* shader, ShaderParamData& data)
{
	if (shader == nullptr)
		return;

	shader->updateAutoParameters(data);
}

} // end namespace render
