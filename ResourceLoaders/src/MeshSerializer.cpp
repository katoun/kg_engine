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

#include <core/Log.h>
#include <core/Math.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <resource/ResourceManager.h>
#include <render/MeshData.h>
#include <render/VertexBuffer.h>
#include <render/IndexBuffer.h>
#include <render/VertexIndexData.h>
#include <render/RenderManager.h>
#include <render/Color.h>
#include <MeshSerializer.h>

#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>

#include <string>

namespace resource
{

MeshSerializer::MeshSerializer()
{
	// Version number
	mVersion = "[MeshSerializer_v1.00]";
}

MeshSerializer::~MeshSerializer() {}

bool MeshSerializer::importResource(Resource* dest, const std::string& filename)
{
	assert(dest != NULL);
	if (dest == NULL)
		return false;

	if (dest->getResourceType() != RESOURCE_TYPE_MESH_DATA)
	{
		core::Log::getInstance().logMessage("MeshSerializer", "Unable to load mesh - invalid resource pointer.", core::LOG_LEVEL_ERROR);
		return false;
	}

	render::MeshData* resource = static_cast<render::MeshData*>(dest);
	assert(resource != NULL);
	if (resource == NULL)
		return false;

	//////////////////////////////////////////////////////////////////////////
	std::string filePath = ResourceManager::getInstance().getPath(filename);
	Poco::AutoPtr<Poco::Util::XMLConfiguration> pConf(new Poco::Util::XMLConfiguration());
	try
	{
		pConf->load(filePath);
	}
	catch(...)
	{
		return false;
	}

	// Vertex buffers
	if (pConf->has("vertexbuffer"))
	{
		bool positions = false;
		if (pConf->has("vertexbuffer[@positions]"))
			positions = pConf->getBool("vertexbuffer[@positions]");

		if (!positions)
			return false;

		resource->setNewVertexData();
		
		render::VertexData* vertexData = resource->getVertexData();
		vertexData->vertexStart = 0;

		// unsigned int numVertices
		unsigned int numVertices = 0;
		if (pConf->has("vertexbuffer[@count]"))
			numVertices = (unsigned int)pConf->getInt("vertexbuffer[@count]");
		vertexData->vertexCount = numVertices;

		unsigned int bindIdx = 0;
		float *pFloat = NULL;

		// float* pVertices (x, y, z order x numVertices)
		vertexData->vertexDeclaration->addElement(bindIdx, 0, render::VET_FLOAT3, render::VES_POSITION);
		render::VertexBuffer* vbuf = render::RenderManager::getInstance().createVertexBuffer(vertexData->vertexDeclaration->getVertexSize(bindIdx), vertexData->vertexCount, resource->getVertexBufferUsage());
		pFloat = (float*)(vbuf->lock(resource::BL_DISCARD));
		if (pFloat == NULL)
			return false;
		// Read direct into hardware buffer
		for (unsigned int i = 0; i < vertexData->vertexCount; i++)
		{
			std::string xkey = "vertexbuffer.vertex[" + core::intToString(i) + "].position[@x]";
			std::string ykey = "vertexbuffer.vertex[" + core::intToString(i) + "].position[@y]";
			std::string zkey = "vertexbuffer.vertex[" + core::intToString(i) + "].position[@z]";

			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;

			if (pConf->has(xkey))
				x = (float)pConf->getDouble(xkey);
			if (pConf->has(ykey))
				y = (float)pConf->getDouble(ykey);
			if (pConf->has(zkey))
				z = (float)pConf->getDouble(zkey);

			pFloat[i * 3 + 0] = x;
			pFloat[i * 3 + 1] = y;
			pFloat[i * 3 + 2] = z;
		}

		//////////////////////////////////////////////////////////////////////////
		core::aabox3d localBox;
		core::vector3d min = core::vector3d::ORIGIN_3D;
		core::vector3d max = core::vector3d::ORIGIN_3D;
		bool first = true;
		float maxSquaredRadius = -1.0f;

		for (unsigned int vert = 0; vert < vertexData->vertexCount; ++vert)
		{
			core::vector3d vec(pFloat[0], pFloat[1], pFloat[2]);

			// Update sphere bounds
			if (vec.getLengthSQ() > maxSquaredRadius)
				maxSquaredRadius = vec.getLengthSQ();

			// Update box
			if (vec.X < min.X) min.X = vec.X;
			if (vec.Y < min.Y) min.Y = vec.Y;
			if (vec.Z < min.Z) min.Z = vec.Z;

			if (vec.X > max.X) max.X = vec.X;
			if (vec.Y > max.Y) max.Y = vec.Y;
			if (vec.Z > max.Z) max.Z = vec.Z;

			pFloat += 3;
		}
		localBox.MinEdge = min;
		localBox.MaxEdge = max;

		resource->setBoundingBox(localBox);

		// Pad out the sphere a little too
		resource->setBoundingSphereRadius(core::sqrt(maxSquaredRadius) * 1.25f);
		//////////////////////////////////////////////////////////////////////////
		vbuf->unlock();
		vertexData->vertexBufferBinding->setBinding(bindIdx, vbuf);
		++bindIdx;

		bool normals = false;
		if (pConf->has("vertexbuffer[@normals]"))
			normals = pConf->getBool("vertexbuffer[@normals]");

		if (normals)
		{
			// float* pNormals (x, y, z order x numVertices)
			vertexData->vertexDeclaration->addElement(bindIdx, 0, render::VET_FLOAT3, render::VES_NORMAL);
			vbuf = render::RenderManager::getInstance().createVertexBuffer(vertexData->vertexDeclaration->getVertexSize(bindIdx), vertexData->vertexCount, resource->getVertexBufferUsage());
			pFloat = (float*)(vbuf->lock(resource::BL_DISCARD));
			if (pFloat == NULL)
				return false;
			// Read direct into hardware buffer
			for (unsigned int i = 0; i < vertexData->vertexCount; i++)
			{
				std::string xkey = "vertexbuffer.vertex[" + core::intToString(i) + "].normal[@x]";
				std::string ykey = "vertexbuffer.vertex[" + core::intToString(i) + "].normal[@y]";
				std::string zkey = "vertexbuffer.vertex[" + core::intToString(i) + "].normal[@z]";

				float x = 0.0f;
				float y = 0.0f;
				float z = 0.0f;

				if (pConf->has(xkey))
					x = (float)pConf->getDouble(xkey);
				if (pConf->has(ykey))
					y = (float)pConf->getDouble(ykey);
				if (pConf->has(zkey))
					z = (float)pConf->getDouble(zkey);

				pFloat[i * 3 + 0] = x;
				pFloat[i * 3 + 1] = y;
				pFloat[i * 3 + 2] = z;
			}
			vbuf->unlock();
			vertexData->vertexBufferBinding->setBinding(bindIdx, vbuf);
			++bindIdx;
		}

		bool colours_diffuse = false;
		if (pConf->has("vertexbuffer[@colours_diffuse]"))
			colours_diffuse = pConf->getBool("vertexbuffer[@colours_diffuse]");

		unsigned int* pRGBA = NULL;
		if (colours_diffuse)
		{
			// unsigned int* pColors (RGBA 8888 format x numVertices)
			vertexData->vertexDeclaration->addElement(bindIdx, 0, render::VET_COLOR, render::VES_DIFFUSE);
			vbuf = render::RenderManager::getInstance().createVertexBuffer(vertexData->vertexDeclaration->getVertexSize(bindIdx), vertexData->vertexCount, resource->getVertexBufferUsage());
			pRGBA = (unsigned int*)(vbuf->lock(resource::BL_DISCARD));
			if (pRGBA == NULL)
				return false;
			// Read direct into hardware buffer
			for (unsigned int i = 0; i < vertexData->vertexCount; i++)
			{
				std::string key = "vertexbuffer.vertex[" + core::intToString(i) + "].colour_diffuse[@value]";

				std::string RGBA;

				if (pConf->has(key))
					RGBA = pConf->getString(key);

				std::vector<std::string> vecparams = core::splitString(RGBA, " \t");

				float R = vecparams.size() > 1 ? core::stringToFloat(vecparams[0]) : 0.0f;
				float G = vecparams.size() > 2 ? core::stringToFloat(vecparams[1]) : 0.0f;
				float B = vecparams.size() > 3 ? core::stringToFloat(vecparams[2]) : 0.0f;
				float A = vecparams.size() > 4 ? core::stringToFloat(vecparams[3]) : 0.0f;

				render::Color color(R,G,B,A);

				pRGBA[i] = color.getAsRGBA();
			}
			vbuf->unlock();
			vertexData->vertexBufferBinding->setBinding(bindIdx, vbuf);
			++bindIdx;
		}

		unsigned int texture_coords = false;
		if (pConf->has("vertexbuffer[@texture_coords]"))
			texture_coords = (unsigned int)pConf->getInt("vertexbuffer[@texture_coords]");

		unsigned int texCoordSet = 0;
		if (texture_coords > 0)
		{
			for (int t = 0; t < texture_coords; ++t)
			{
				// unsigned int dimensions    (1 for 1D, 2 for 2D, 3 for 3D)
				unsigned int dim = 2;
				// float* pTexCoords  (u [v] [w] order, dimensions x numVertices)
				vertexData->vertexDeclaration->addElement(bindIdx, 0, render::VertexElement::multiplyTypeCount(render::VET_FLOAT1, dim), render::VES_TEXTURE_COORDINATES, texCoordSet);
				vbuf = render::RenderManager::getInstance().createVertexBuffer(vertexData->vertexDeclaration->getVertexSize(bindIdx), vertexData->vertexCount, resource->getVertexBufferUsage());
				pFloat = (float*)(vbuf->lock(resource::BL_DISCARD));
				if (pFloat == NULL)
					return false;
				// Read direct into hardware buffer
				for (unsigned int i = 0; i < vertexData->vertexCount; i++)
				{
					std::string ukey = "vertexbuffer.vertex[" + core::intToString(i) + "].texcoord[" + core::intToString(t) + "][@u]";
					std::string vkey = "vertexbuffer.vertex[" + core::intToString(i) + "].texcoord[" + core::intToString(t) + "][@v]";

					float u = 0.0f;
					float v = 0.0f;

					if (pConf->has(ukey))
						u = (float)pConf->getDouble(ukey);
					if (pConf->has(vkey))
						v = (float)pConf->getDouble(vkey);

					pFloat[i * 2 + 0] = u;
					pFloat[i * 2 + 1] = v;
				}
				vbuf->unlock();
				vertexData->vertexBufferBinding->setBinding(bindIdx, vbuf);
				++bindIdx;
				++texCoordSet;
			}
		}
	}

	if (pConf->has("indexbuffer"))
	{
		resource->setNewIndexData();

		render::IndexData* indexData = resource->getIndexData();
		indexData->indexStart = 0;

		// unsigned int numIndexes
		unsigned int numIndexes = 0;
		if (pConf->has("indexbuffer[@count]"))
			numIndexes = (unsigned int)pConf->getInt("indexbuffer[@count]");
		indexData->indexCount = numIndexes;

		render::IndexBuffer* ibuf = render::RenderManager::getInstance().createIndexBuffer(render::IT_32BIT, indexData->indexCount, resource->getIndexBufferUsage());
		indexData->indexBuffer = ibuf;

		unsigned int* pIdx = (unsigned int*)(ibuf->lock(resource::BL_DISCARD));
		if (pIdx == NULL)
			return false;
		// Read direct into hardware buffer
		for (unsigned int i = 0; i < indexData->indexCount; i++)
		{
			std::string key = "indexbuffer.index[" + core::intToString(i) + "][@value]";
			
			unsigned int index = 0;

			if (pConf->has(key))
				index = pConf->getInt(key);

			pIdx[i] = index;
		}
		ibuf->unlock();
	}

	return true;
}

bool MeshSerializer::exportResource(Resource* source, const std::string& filename)
{
	return true;
}

}// end namespace resource
