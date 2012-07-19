/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2012 Catalin Alexandru Nastase

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