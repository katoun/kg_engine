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

#ifndef _RENDERMATERIAL_H_
#define _RENDERMATERIAL_H_

#include <core/Config.h>
#include <resource/Resource.h>
#include <render/Color.h>
#include <render/BlendMode.h>

#include <string>
#include <list>

namespace resource
{
class Serializer;
}

namespace render
{

//! Light shading modes.
enum ShadeOptions
{
	SO_FLAT,
	SO_GOURAUD,
	SO_PHONG
};

//! Fog modes.
enum FogMode
{
	FM_NONE,	/// No fog. Duh.
	FM_EXP,		/// Fog density increases  exponentially from the camera (fog = 1/e^(distance * density))
	FM_EXP2,	/// Fog density increases at the square of FOG_EXP, i.e. even quicker (fog = 1/e^(distance * density)^2)
	FM_LINEAR	/// Fog density increases linearly between the start and end distances
};


class Shader;
class TextureUnit;

//! Class encapsulates surface properties of a mesh.
//!
//! To optimise rendering, changes in render state must be
//! minimised. One of the most frequent render state changes are
//! changes to materials, mostly to textures. An mesh can have 
//! only one material.
class ENGINE_PUBLIC_EXPORT Material : public resource::Resource
{
public:

	Material(const std::string& name, resource::Serializer* serializer);
	~Material();

	//! Sets the ambient color reflectance properties of this material.
	void setAmbient(float red, float green, float blue);
	void setAmbient(const Color& ambient);

	//! Sets the diffuse color reflectance properties of this material.
	void setDiffuse(float red, float green, float blue);
	void setDiffuse(const Color& diffuse);

	//! Sets the specular color reflectance properties of this material.
	void setSpecular(float red, float green, float blue);
	void setSpecular(const Color& specular);

	//! Sets the emissive color reflectance properties of this material.
	void setEmissive(float red, float green, float blue);
	void setEmissive(const Color& emissive);

	//! Sets the shininess of the material, affecting the size of specular highlights.
	void setShininess(float shininess);

	//! Gets the ambient color reflectance of the material.
	const Color& getAmbient() const;

	//! Gets the diffuse color reflectance of the material.
	const Color& getDiffuse() const;

	//! Gets the specular color reflectance of the material.
	const Color& getSpecular() const;

	//! Gets the emissive color of the material.
	const Color& getEmissive() const;

	//! Gets the 'shininess' property of the material (affects specular highlights).
	float getShininess() const;

	//! Sets whether or not dynamic lighting is enabled.
	void setLightingEnabled(bool enabled);

	//! Returns whether or not dynamic lighting is enabled.
	bool getLightingEnabled() const;

	//! Sets the type of light shading required.
	void setShadingMode(ShadeOptions mode);

	//! Returns the type of light shading to be used.
	ShadeOptions getShadingMode() const;

	//! Sets the fogging mode applied for this material.
	void setFog(bool overrideScene = true, FogMode mode = FM_NONE, const Color& color = Color::White, float density = 0.001f, float start = 0.0f, float end = 1.0f);
	
	//! Returns true if this pass is to override the scene fog settings.
	bool getFogOverride() const;

	//! Returns the fog mode for this material.
	FogMode getFogMode() const;

	//! Returns the fog color for the scene.
	const Color& getFogColor() const;

	//! Returns the fog density for this pass.
	float getFogDensity() const;

	//! Returns the fog start distance for this pass.
	float getFogStart() const;

	//! Returns the fog end distance for this pass.
	float getFogEnd() const;

	//! Sets the kind of blending this material has with the existing contents of the scene.
	void setSceneBlending(const SceneBlendType sbt);
	
	//! Allows very fine control of blending this material with the existing contents of the scene.
	void setSceneBlending(const SceneBlendFactor sourceFactor, const SceneBlendFactor destFactor);

	//! Retrieves the source blending factor for the material.
	SceneBlendFactor getSourceBlendFactor() const;

	//! Retrieves the destination blending factor for the material.
	SceneBlendFactor getDestBlendFactor() const;

	//! Returns true if this material has some element of transparency.
	bool isTransparent() const;

	//! Sets whether or not this material renders with depth-buffer checking on or not.
	void setDepthCheckEnabled(bool enabled);

	//! Returns whether or not this material renders with depth-buffer checking on or not.
	bool getDepthCheckEnabled() const;

	//! Sets whether or not this material renders with depth-buffer writing on or not.
	void setDepthWriteEnabled(bool enabled);

	//! Returns whether or not this material renders with depth-buffer writing on or not.
	bool getDepthWriteEnabled() const;

	//! Inserts a new texture unit in the material.
	void addTextureUnit(TextureUnit* tu);

	//! Retrieves a pointer to a texture unit so it may be modified.
	TextureUnit* getTextureUnit(unsigned int index) const;

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

	bool isProgrammable();
	bool hasVertexShader();
	bool hasFragmentShader();
	bool hasGeometryShader();

protected:

	void unloadImpl();

	// Color properties
	Color mAmbient;
	Color mDiffuse;
	Color mSpecular;
	Color mEmissive;
	float mShininess;

	// Blending factors
	SceneBlendFactor mSourceBlendFactor;
	SceneBlendFactor mDestBlendFactor;

	// Depth buffer settings
	bool mDepthCheck;
	bool mDepthWrite;

	bool mLightingEnabled;

	ShadeOptions mShadeOptions;

	// Fog
	bool mFogOverride;
	FogMode mFogMode;
	Color mFogColor;
	float mFogDensity;
	float mFogStart;
	float mFogEnd;

	// Textures
	std::list<TextureUnit*> mTextureUnits;

	Shader* mVertexShader;
	Shader* mFragmentShader;
	Shader* mGeometryShader;
};

} //namespace render

#endif