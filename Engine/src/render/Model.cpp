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

#include <render/Model.h>
#include <render/MeshData.h>
#include <render/Material.h>
#include <resource/ResourceEvent.h>
#include <resource/ResourceManager.h>
#include <core/Utils.h>

namespace render
{

unsigned int Model::msNextGeneratedModelIndex = 0;

Model::Model(MeshData* meshData): Renderable("Model_" + core::intToString(msNextGeneratedModelIndex++))
{
	mRenderableType = RENDERABLE_TYPE_MODEL;

	// Init matrix
	mWorldMatrix = core::matrix4::IDENTITY;

	mModifiedWorldTransform = true;

	mMaterial = NULL;

	assert(meshData != NULL);
	if (meshData == NULL)
		return;
	
	mMeshData = meshData;
	mMeshData->addResourceEventReceiver(this);
}

Model::Model(const std::string& name, MeshData* meshData): Renderable(name)
{
	mRenderableType = RENDERABLE_TYPE_MODEL;

	// Init matrix
	mWorldMatrix = core::matrix4::IDENTITY;

	mModifiedWorldTransform = true;

	mMaterial = NULL;

	assert(meshData != NULL);
	if (meshData == NULL)
		return;

	mMeshData = meshData;
	mMeshData->addResourceEventReceiver(this);
}

Model::~Model()
{
	if (mMeshData != NULL)
	{
		mMeshData->removeResourceEventReceiver(this);
	}

	uninitialize();
}

void Model::setMeshData(const std::string& filename)
{
	MeshData* newMeshData = static_cast<MeshData*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_MESH_DATA, filename));
	assert(newMeshData != NULL);
	if (newMeshData == NULL)
		return;

	if (mMeshData != NULL)
	{
		mMeshData->removeResourceEventReceiver(this);
	}

	uninitialize();

	mMeshData = newMeshData;
	mMeshData->addResourceEventReceiver(this);
}

void Model::setMeshData(MeshData* meshData)
{
	if (meshData == NULL)
		return;

	if (mMeshData != NULL)
	{
		mMeshData->removeResourceEventReceiver(this);
	}

	uninitialize();

	mMeshData = meshData;
	mMeshData->addResourceEventReceiver(this);
}

MeshData* Model::getMeshData() const
{
	return mMeshData;
}

void Model::setMaterial(const std::string& filename)
{
	std::string materialname = filename;
	mMaterial = static_cast<Material*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_RENDER_MATERIAL, materialname));

	if (mMaterial != NULL)
	{
		mMaterial->addResourceEventReceiver(this);
	}
}

void Model::setMaterial(Material* material)
{
	if (material == NULL)
		return;

	mMaterial = material;

	if (mMaterial != NULL)
	{
		mMaterial->addResourceEventReceiver(this);
	}
}

Material* Model::getMaterial() const
{
	return mMaterial;
}

void Model::resourceLoaded(const resource::ResourceEvent& evt)
{
	if (evt.source != NULL)
	{
		if (evt.source == mMeshData)
		{
			if (mMaterial == NULL && mMeshData->getMaterial() != NULL)
			{
				mMaterial = mMeshData->getMaterial();
				
				if (mMaterial != NULL)
				{
					mMaterial->addResourceEventReceiver(this);
				}
			}

			mRenderOperationType = mMeshData->getRenderOperationType();
			mVertexData = mMeshData->getVertexData();
			mIndexData = mMeshData->getIndexData();
		
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
		mVertexData = NULL;
		mIndexData = NULL;

		uninitialize();
	}
}

void Model::updateImpl(float elapsedTime)
{
	mModifiedWorldTransform = mModifiedAbsoluteTransform;
	
	Node::updateImpl(elapsedTime);

	// Update bounding box
	mBoundingBox = mMeshData->getBoundingBox();
	// Scale and translate the Bounding Box.
	core::matrix4 trans = core::matrix4::IDENTITY;
	core::matrix4 rot = mAbsoluteOrientation.toRotationMatrix();
	core::matrix4 scale;
	scale.setScale(mAbsoluteScale);
	trans = rot * scale;
	trans.setTranslation(mAbsolutePosition);

	trans.transformBox(mBoundingBox);

	// Update bounding sphere
	float maxscale = core::max(mAbsoluteScale.X, mAbsoluteScale.Y, mAbsoluteScale.Z);
	mBoundingSphere.Radius = mMeshData->getBoundingSphereRadius();
	mBoundingSphere.Radius *= maxscale;

	mBoundingSphere.Center = mAbsolutePosition;

#ifdef _DEBUG
	//std::cout<<"Center: "<<mBoundingSphere.Center<<std::endl;
	//std::cout<<"Radius: "<<mBoundingSphere.Radius<<std::endl;
#endif
	
	updateWorldTransform();
}

void Model::updateWorldTransform()
{
	if(mModifiedWorldTransform)
	{
		updateTransformImpl();

		mWorldMatrix = core::matrix4::IDENTITY;
		// Ordering:
		//    1. Scale
		//    2. Rotate
		//    3. Translate

		// Own scale is applied before rotation
		core::matrix4 rot = mAbsoluteOrientation.toRotationMatrix();
		core::matrix4 scale;
		scale.setScale(mAbsoluteScale);
		mWorldMatrix = rot * scale;
		mWorldMatrix.setTranslation(mAbsolutePosition);
		
#ifdef _DEBUG
		//std::cout<<"Pos: "<<mPosition<<std::endl;
		//std::cout<<"Orientation: "<<mOrientation<<std::endl;
		//std::cout<<"Scale: "<<mScale<<std::endl;
#endif
		mModifiedWorldTransform = false;
	}
}

} //namespace render
