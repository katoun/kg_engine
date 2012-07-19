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

#ifndef _MODEL_H_
#define _MODEL_H_

#include <core/Config.h>
#include <core/Matrix4.h>
#include <core/Aabox3d.h>
#include <core/Sphere3d.h>
#include <render/Color.h>
#include <render/Renderable.h>
#include <resource/ResourceEventReceiver.h>

#include <string>

namespace resource
{
struct ResourceEvent;
}

namespace render
{

class Material;
class MeshData;
class MeshProperties;

//! Representation of a model in the render world.
class ENGINE_PUBLIC_EXPORT Model: public Renderable, public resource::ResourceEventReceiver
{
public:

	Model(MeshData* meshData);
	Model(const std::string& name, MeshData* meshData);

	~Model();

	//! Sets the mesh data this mesh will use.
	void setMeshData(const std::string& filename);
	void setMeshData(MeshData* meshData);

	MeshData* getMeshData() const;

	//! Sets the name of the material which this mesh will use.
	void setMaterial(const std::string& filename);
	void setMaterial(Material* material);
	
	//! Gets the material which this mesh uses.
	Material* getMaterial() const;

	void resourceLoaded(const resource::ResourceEvent& evt);
	void resourceUnloaded(const resource::ResourceEvent& evt);

protected:

	// Incremented count for next Index
	static unsigned int msNextGeneratedModelIndex;

	MeshData* mMeshData;

	Material* mMaterial;

	void updateImpl(float elapsedTime);

	bool mModifiedWorldTransform;

	void updateWorldTransform();
};

} //namespace render

#endif