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
#include <core/Math.h>
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <resource/ResourceManager.h>
#include <render/MeshData.h>
#include <render/VertexBuffer.h>
#include <render/IndexBuffer.h>
#include <render/RenderManager.h>
#include <render/Color.h>
#include <MeshSerializer.h>

#include <tinyxml2.h>

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
	assert(dest != nullptr);
	if (dest == nullptr)
		return false;

	if (dest->getResourceType() != RESOURCE_TYPE_MESH_DATA)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("MeshSerializer", "Unable to load mesh - invalid resource pointer.", core::LOG_LEVEL_ERROR);
		return false;
	}

	render::MeshData* resource = static_cast<render::MeshData*>(dest);
	assert(resource != nullptr);
	if (resource == nullptr)
		return false;

	if (resource::ResourceManager::getInstance() == nullptr)
	{
		if (core::Log::getInstance() != nullptr) core::Log::getInstance()->logMessage("MeshSerializer", "Unable to load mesh - resources data path not set.", core::LOG_LEVEL_ERROR);
		return false;
	}

	std::string filePath = resource::ResourceManager::getInstance()->getDataPath() + "/" + filename;

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS)
		return false;

	tinyxml2::XMLElement* pRoot = doc.FirstChildElement("mesh");
	if (pRoot != nullptr)
	{
		int ivalue = 0;
		double dvalue = 0.0;
		const char* svalue;
		tinyxml2::XMLElement* pElement = nullptr;
		tinyxml2::XMLElement* pSubElement = nullptr;
		tinyxml2::XMLElement* pVertexSubElement = nullptr;
		
		pElement = pRoot->FirstChildElement("vertexbuffer");
		if (pElement != nullptr)
		{
			svalue = pElement->Attribute("positions");
			if (svalue != nullptr)
			{
				if (std::string(svalue) != "true")
					return false;
			}

			unsigned int numVertices = 0;
			if (pElement->QueryIntAttribute("count", &ivalue) == tinyxml2::XML_SUCCESS)
			{
				numVertices = (unsigned int)ivalue;
			}

			float *pFloat = nullptr;

			// float* pVertices (x, y, z order x numVertices)
			render::VertexBuffer* pVertexBuffer = render::RenderManager::getInstance()->createVertexBuffer(render::VERTEX_BUFFER_TYPE_POSITION, render::VERTEX_ELEMENT_TYPE_FLOAT3, numVertices, resource->getVertexBufferUsage());
			pFloat = (float*)(pVertexBuffer->lock(resource::BL_DISCARD));
			if (pFloat == nullptr)
				return false;

			core::aabox3d localBox;
			core::vector3d min = core::vector3d::ORIGIN_3D;
			core::vector3d max = core::vector3d::ORIGIN_3D;
			bool first = true;
			float maxSquaredRadius = -1.0f;

			unsigned int i = 0;
			pSubElement = pElement->FirstChildElement("vertex");
			while(pSubElement != nullptr)
			{
				if (i >= numVertices)
					break;

				pVertexSubElement = pSubElement->FirstChildElement("position");
				if (pVertexSubElement != nullptr)
				{
					float x = 0.0f;
					float y = 0.0f;
					float z = 0.0f;

					if (pVertexSubElement->QueryDoubleAttribute("x", &dvalue) == tinyxml2::XML_SUCCESS)
					{
						x = (float)dvalue;
					}

					if (pVertexSubElement->QueryDoubleAttribute("y", &dvalue) == tinyxml2::XML_SUCCESS)
					{
						y = (float)dvalue;
					}

					if (pVertexSubElement->QueryDoubleAttribute("z", &dvalue) == tinyxml2::XML_SUCCESS)
					{
						z = (float)dvalue;
					}

					pFloat[i * 3 + 0] = x;
					pFloat[i * 3 + 1] = y;
					pFloat[i * 3 + 2] = z;

					core::vector3d vec(x, y, z);

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
				}

				i++;

				pSubElement = pSubElement->NextSiblingElement("vertex");
			}

			localBox.MinEdge = min;
			localBox.MaxEdge = max;

			resource->setBoundingBox(localBox);

			// Pad out the sphere a little too
			resource->setBoundingSphereRadius(core::sqrt(maxSquaredRadius) * 1.25f);

			pVertexBuffer->unlock();
			resource->setVertexBuffer(render::VERTEX_BUFFER_TYPE_POSITION, pVertexBuffer);

			bool normals = false;
			svalue = pElement->Attribute("normals");
			if (svalue != nullptr)
			{
				normals = std::string(svalue) == "true" ? true : false;
			}

			if (normals)
			{
				// float* pNormals (x, y, z order x numVertices)
				pVertexBuffer = render::RenderManager::getInstance()->createVertexBuffer(render::VERTEX_BUFFER_TYPE_NORMAL, render::VERTEX_ELEMENT_TYPE_FLOAT3, numVertices, resource->getVertexBufferUsage());
				pFloat = (float*)(pVertexBuffer->lock(resource::BL_DISCARD));
				if (pFloat == nullptr)
					return false;

				unsigned int i = 0;
				pSubElement = pElement->FirstChildElement("vertex");
				while(pSubElement != nullptr)
				{
					if (i >= numVertices)
						break;

					pVertexSubElement = pSubElement->FirstChildElement("normal");
					if (pVertexSubElement != nullptr)
					{
						float x = 0.0f;
						float y = 0.0f;
						float z = 0.0f;

						if (pVertexSubElement->QueryDoubleAttribute("x", &dvalue) == tinyxml2::XML_SUCCESS)
						{
							x = (float)dvalue;
						}

						if (pVertexSubElement->QueryDoubleAttribute("y", &dvalue) == tinyxml2::XML_SUCCESS)
						{
							y = (float)dvalue;
						}

						if (pVertexSubElement->QueryDoubleAttribute("z", &dvalue) == tinyxml2::XML_SUCCESS)
						{
							z = (float)dvalue;
						}

						pFloat[i * 3 + 0] = x;
						pFloat[i * 3 + 1] = y;
						pFloat[i * 3 + 2] = z;
					}

					i++;

					pSubElement = pSubElement->NextSiblingElement("vertex");
				}

				pVertexBuffer->unlock();
				resource->setVertexBuffer(render::VERTEX_BUFFER_TYPE_NORMAL, pVertexBuffer);
			}

			bool texture_coords = false;
			svalue = pElement->Attribute("texture_coords");
			if (svalue != nullptr)
			{
				texture_coords = std::string(svalue) == "true" ? true : false;
			}

			if (texture_coords)
			{
				// float* pTexCoords  (u v order, dimensions x numVertices)
				pVertexBuffer = render::RenderManager::getInstance()->createVertexBuffer(render::VERTEX_BUFFER_TYPE_TEXTURE_COORDINATES, render::VERTEX_ELEMENT_TYPE_FLOAT2, numVertices, resource->getVertexBufferUsage());
				pFloat = (float*)(pVertexBuffer->lock(resource::BL_DISCARD));
				if (pFloat == nullptr)
					return false;

				unsigned int i = 0;
				pSubElement = pElement->FirstChildElement("vertex");
				while(pSubElement != nullptr)
				{
					if (i >= numVertices)
						break;

					pVertexSubElement = pSubElement->FirstChildElement("texcoord");
					if (pVertexSubElement != nullptr)
					{
						float u = 0.0f;
						float v = 0.0f;

						if (pVertexSubElement->QueryDoubleAttribute("u", &dvalue) == tinyxml2::XML_SUCCESS)
						{
							u = (float)dvalue;
						}

						if (pVertexSubElement->QueryDoubleAttribute("v", &dvalue) == tinyxml2::XML_SUCCESS)
						{
							v = (float)dvalue;
						}

						pFloat[i * 2 + 0] = u;
						pFloat[i * 2 + 1] = v;
					}

					i++;

					pSubElement = pSubElement->NextSiblingElement("vertex");
				}

				pVertexBuffer->unlock();
				resource->setVertexBuffer(render::VERTEX_BUFFER_TYPE_TEXTURE_COORDINATES, pVertexBuffer);
			}
		}
	
		pElement = pRoot->FirstChildElement("indexbuffer");
		if (pElement != nullptr)
		{
			unsigned int numIndexes = 0;
			if (pElement->QueryIntAttribute("count", &ivalue) == tinyxml2::XML_SUCCESS)
			{
				numIndexes = (unsigned int)ivalue;
			}

			render::IndexBuffer* pIndexBuffer = render::RenderManager::getInstance()->createIndexBuffer(render::IT_32BIT, numIndexes, resource->getIndexBufferUsage());
			resource->setIndexBuffer(pIndexBuffer);

			unsigned int* pIdx = (unsigned int*)(pIndexBuffer->lock(resource::BL_DISCARD));
			if (pIdx == nullptr)
				return false;

			unsigned int i = 0;
			pSubElement = pElement->FirstChildElement("index");
			while(pSubElement != nullptr)
			{
				if (i >= numIndexes)
					break;
				
				unsigned int index = 0;
				if (pSubElement->QueryIntAttribute("value", &ivalue) == tinyxml2::XML_SUCCESS)
				{
					index = (unsigned int)ivalue;
				}

				pIdx[i++] = index;

				pSubElement = pSubElement->NextSiblingElement("index");
			}

			pIndexBuffer->unlock();
		}
	}

	return true;
}

bool MeshSerializer::exportResource(Resource* source, const std::string& filename)
{
	return true;
}

}// end namespace resource
