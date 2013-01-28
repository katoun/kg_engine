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

#include <render/Model.h>
#include <render/MeshData.h>
#include <render/Material.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <game/MessageDefines.h>
#include <render/RenderDefines.h>
#include <resource/ResourceEvent.h>
#include <resource/ResourceManager.h>
#include <core/Utils.h>

namespace render
{

Model::Model(): game::Component()
{
	mType = game::COMPONENT_TYPE_MODEL;

	// Init matrix
	mWorldMatrix = core::matrix4::IDENTITY;

	mModelNeedsUpdate = true;

	mMeshData = nullptr;

	mMaterial = nullptr;

	mVisibleBoundingBox = false;
	mVisibleBoundingSphere = false;

	mWorldMatrix = core::matrix4::IDENTITY;

	mRenderOperationType = ROT_TRIANGLE_LIST;
}

Model::~Model()
{
	if (mMeshData != nullptr)
	{
		mMeshData->removeResourceEventReceiver(this);
	}

	if (mMaterial != nullptr)
	{
		mMaterial->removeResourceEventReceiver(this);
	}

	uninitialize();
}

void Model::setMeshData(const std::string& filename)
{
	if (resource::ResourceManager::getInstance() != nullptr)
	{
		MeshData* newMeshData = static_cast<MeshData*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_MESH_DATA, filename));
		assert(newMeshData != nullptr);
		if (newMeshData == nullptr)
			return;

		if (mMeshData != nullptr)
		{
			mMeshData->removeResourceEventReceiver(this);

			uninitialize();
		}

		mMeshData = newMeshData;
		mMeshData->addResourceEventReceiver(this);
	}
}

void Model::setMeshData(MeshData* meshData)
{
	if (meshData == nullptr)
		return;

	if (mMeshData != nullptr)
	{
		mMeshData->removeResourceEventReceiver(this);

		uninitialize();
	}

	mMeshData = meshData;
	mMeshData->addResourceEventReceiver(this);
}

MeshData* Model::getMeshData() const
{
	return mMeshData;
}

void Model::setMaterial(const std::string& filename)
{
	if (mMaterial != nullptr)
	{
		mMaterial->removeResourceEventReceiver(this);
	}

	if (resource::ResourceManager::getInstance() != nullptr)
		mMaterial = static_cast<Material*>(resource::ResourceManager::getInstance()->createResource(resource::RESOURCE_TYPE_RENDER_MATERIAL, filename));

	if (mMaterial != nullptr)
	{
		mMaterial->addResourceEventReceiver(this);
	}
}

void Model::setMaterial(Material* material)
{
	if (material == nullptr)
		return;

	if (mMaterial != nullptr)
	{
		mMaterial->removeResourceEventReceiver(this);
	}

	mMaterial = material;

	if (mMaterial != nullptr)
	{
		mMaterial->addResourceEventReceiver(this);
	}
}

Material* Model::getMaterial() const
{
	return mMaterial;
}

bool Model::getVisibleBoundingBox()
{
	return mVisibleBoundingBox;
}

void Model::setVisibleBoundingBox(bool visible)
{
	mVisibleBoundingBox = visible;
}

bool Model::getVisibleBoundingSphere()
{
	return mVisibleBoundingSphere;
}

void Model::setVisibleBoundingSphere(bool visible)
{
	mVisibleBoundingSphere = visible;
}

const core::matrix4& Model::getWorldMatrix()
{
	return mWorldMatrix;
}

const core::aabox3d& Model::getBoundingBox()
{
	return mBoundingBox;
}

const core::sphere3d& Model::getBoundingSphere()
{
	return mBoundingSphere;
}

const RenderOperationType& Model::getRenderOperationType()
{
	return mRenderOperationType;
}

VertexBuffer* Model::getVertexBuffer(VertexBufferType type)
{
	if (mMeshData == nullptr)
		return nullptr;

	return mMeshData->getVertexBuffer(type);
}

IndexBuffer* Model::getIndexBuffer()
{
	if (mMeshData == nullptr)
		return nullptr;

	return mMeshData->getIndexBuffer();
}

void Model::resourceLoaded(const resource::ResourceEvent& evt)
{
	if (evt.source != nullptr)
	{
		if (evt.source == mMeshData)
		{
			if (mMaterial == nullptr && mMeshData->getMaterial() != nullptr)
			{
				mMaterial = mMeshData->getMaterial();
				
				if (mMaterial != nullptr)
				{
					mMaterial->addResourceEventReceiver(this);
				}
			}

			mRenderOperationType = mMeshData->getRenderOperationType();
		
			initialize();
		}

		if (evt.source == mMaterial)
		{
			initialize();
		}
	}
}

void Model::resourceUnloaded(const resource::ResourceEvent& evt)
{
	if (evt.source && evt.source == mMeshData)
	{
		mRenderOperationType = ROT_TRIANGLE_LIST;

		uninitialize();
	}
}

void Model::updateImpl(float elapsedTime)
{
	if (mModelNeedsUpdate)
	{
		if (mGameObject != nullptr)
		{
			game::Transform* pTransform = static_cast<game::Transform*>(mGameObject->getComponent(game::COMPONENT_TYPE_TRANSFORM));
			if (pTransform != nullptr)
			{
				core::vector3d position = pTransform->getAbsolutePosition();
				core::quaternion orientation = pTransform->getAbsoluteOrientation();
				core::vector3d scale = pTransform->getAbsoluteScale();

				mWorldMatrix.buildWorldMatrix(position, scale, orientation);

				// Update bounding box
				if (mMeshData != nullptr)
				{
					mBoundingBox = mMeshData->getBoundingBox();

					mWorldMatrix.transformBox(mBoundingBox);

					// Update bounding sphere
					float maxscale = core::max(scale.X, scale.Y, scale.Z);
					mBoundingSphere.Radius = mMeshData->getBoundingSphereRadius();
					mBoundingSphere.Radius *= maxscale;

					mBoundingSphere.Center = position;

#ifdef _DEBUG
					//std::cout<<"Center: "<<mBoundingSphere.Center<<std::endl;
					//std::cout<<"Radius: "<<mBoundingSphere.Radius<<std::endl;
#endif	
				}
			}
		}
		
		mModelNeedsUpdate = false;
	}
}

void Model::onMessageImpl(unsigned int messageID)
{
	if (messageID == game::MESSAGE_TRANSFORM_NEEDS_UPDATE)
	{
		mModelNeedsUpdate = true;
	}
}

} //namespace render
