/*
-----------------------------------------------------------------------------
Copyright (c) 2006-2012 Catalin Alexandru Nastase

KG game engine (http://k-game.sourceforge.net) is made available under the LGPL License (http://www.gnu.org/copyleft/lgpl.html)

Under the LGPL you may use KG game engine for any purpose you wish, as long as you:
1. Release any modifications to the KG game engine source back to the community
2. Pass on the source to KG game engine with all the copyrights intact, or link back to a place where the source code can be obtained (http://k-game.sourceforge.net)
3. Make it clear where you have customised it.
The above is a precis, please do read the full license agreement.
-----------------------------------------------------------------------------
*/

#ifndef _GL_SHADER_H_
#define _GL_SHADER_H_

#include <GLConfig.h>
#include <render/Shader.h>

namespace render
{

struct CGShaderParameter: public ShaderParameter
{
	CGShaderParameter();

	CGparameter cgParameter;
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
	void setParameter(const std::string& name, const signed int* val, unsigned int count);

protected:

	bool loadImpl();

	void unloadImpl();

	ShaderParameter* createParameterImpl(const std::string& name);

	CGcontext mCgContext;
	CGprogram mCgProgram;// Program handle
	CGprofile mSelectedCgProfile;

	char** mCgArguments;//Cg uses char** for arguments

	static bool isSampler(CGtype type);
	static ShaderParameterType getShaderPrameterType(CGtype type);
};

} // end namespace render

#endif