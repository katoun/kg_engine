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
#include <render/VertexIndexData.h>
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

			resource->setNewVertexData();
		
			render::VertexData* pVertexData = resource->getVertexData();
			pVertexData->vertexStart = 0;

			unsigned int numVertices = 0;
			if (pElement->QueryIntAttribute("count", &ivalue) == tinyxml2::XML_SUCCESS)
			{
				numVertices = (unsigned int)ivalue;
			}
			pVertexData->vertexCount = numVertices;

			unsigned int bindIdx = 0;
			float *pFloat = nullptr;

			// float* pVertices (x, y, z order x numVertices)
			pVertexData->vertexDeclaration->addElement(bindIdx, 0, render::VET_FLOAT3, render::VES_POSITION);
			render::VertexBuffer* pVertexBuffer = render::RenderManager::getInstance()->createVertexBuffer(pVertexData->vertexDeclaration->getVertexSize(bindIdx), pVertexData->vertexCount, resource->getVertexBufferUsage());
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
				if (i >= pVertexData->vertexCount)
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
			pVertexData->vertexBufferBinding->setBinding(bindIdx, pVertexBuffer);
			++bindIdx;

			bool normals = false;
			svalue = pElement->Attribute("normals");
			if (svalue != nullptr)
			{
				normals = std::string(svalue) == "true" ? true : false;
			}

			if (normals)
			{
				// float* pNormals (x, y, z order x numVertices)
				pVertexData->vertexDeclaration->addElement(bindIdx, 0, render::VET_FLOAT3, render::VES_NORMAL);
				pVertexBuffer = render::RenderManager::getInstance()->createVertexBuffer(pVertexData->vertexDeclaration->getVertexSize(bindIdx), pVertexData->vertexCount, resource->getVertexBufferUsage());
				pFloat = (float*)(pVertexBuffer->lock(resource::BL_DISCARD));
				if (pFloat == nullptr)
					return false;

				unsigned int i = 0;
				pSubElement = pElement->FirstChildElement("vertex");
				while(pSubElement != nullptr)
				{
					if (i >= pVertexData->vertexCount)
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
				pVertexData->vertexBufferBinding->setBinding(bindIdx, pVertexBuffer);
				++bindIdx;
			}

			bool colours_diffuse = false;
			svalue = pElement->Attribute("colours_diffuse");
			if (svalue != nullptr)
			{
				colours_diffuse = std::string(svalue) == "true" ? true : false;
			}

			unsigned int* pRGBA = nullptr;
			if (colours_diffuse)
			{
				// unsigned int* pColors (RGBA 8888 format x numVertices)
				pVertexData->vertexDeclaration->addElement(bindIdx, 0, render::VET_COLOR, render::VES_DIFFUSE);
				pVertexBuffer = render::RenderManager::getInstance()->createVertexBuffer(pVertexData->vertexDeclaration->getVertexSize(bindIdx), pVertexData->vertexCount, resource->getVertexBufferUsage());
				pRGBA = (unsigned int*)(pVertexBuffer->lock(resource::BL_DISCARD));
				if (pRGBA == nullptr)
					return false;

				unsigned int i = 0;
				pSubElement = pElement->FirstChildElement("vertex");
				while(pSubElement != nullptr)
				{
					if (i >= pVertexData->vertexCount)
						break;

					pVertexSubElement = pSubElement->FirstChildElement("colour_diffuse");
					if (pVertexSubElement != nullptr)
					{
						std::string RGBA;
						svalue = pVertexSubElement->Attribute("value");
						if (svalue != nullptr)
						{
							RGBA = svalue;
						}

						std::vector<std::string> vecparams = core::splitString(RGBA, " \t");

						float R = vecparams.size() > 1 ? core::stringToFloat(vecparams[0]) : 0.0f;
						float G = vecparams.size() > 2 ? core::stringToFloat(vecparams[1]) : 0.0f;
						float B = vecparams.size() > 3 ? core::stringToFloat(vecparams[2]) : 0.0f;
						float A = vecparams.size() > 4 ? core::stringToFloat(vecparams[3]) : 0.0f;

						render::Color color(R,G,B,A);

						pRGBA[i] = color.getAsRGBA();
					}

					i++;

					pSubElement = pSubElement->NextSiblingElement("vertex");
				}

				pVertexBuffer->unlock();
				pVertexData->vertexBufferBinding->setBinding(bindIdx, pVertexBuffer);
				++bindIdx;
			}

			unsigned int texture_coords = 0;
			if (pElement->QueryIntAttribute("texture_coords", &ivalue) == tinyxml2::XML_SUCCESS)
			{
				texture_coords = (unsigned int)ivalue;
			}

			for (unsigned int t = 0; t < texture_coords; ++t)
			{
				// float* pTexCoords  (u v order, dimensions x numVertices)
				pVertexData->vertexDeclaration->addElement(bindIdx, 0, render::VertexElement::multiplyTypeCount(render::VET_FLOAT1, 2), render::VES_TEXTURE_COORDINATES, t);
				pVertexBuffer = render::RenderManager::getInstance()->createVertexBuffer(pVertexData->vertexDeclaration->getVertexSize(bindIdx), pVertexData->vertexCount, resource->getVertexBufferUsage());
				pFloat = (float*)(pVertexBuffer->lock(resource::BL_DISCARD));
				if (pFloat == nullptr)
					return false;

				unsigned int i = 0;
				pSubElement = pElement->FirstChildElement("vertex");
				while(pSubElement != nullptr)
				{
					if (i >= pVertexData->vertexCount)
						break;

					pVertexSubElement = pSubElement->FirstChildElement("texcoord");
					for (unsigned int j = 1; j < texture_coords; ++j)
					{
						if (pVertexSubElement == nullptr)
							break;
						pVertexSubElement = pVertexSubElement->NextSiblingElement("texcoord");
					}
					
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
				pVertexData->vertexBufferBinding->setBinding(bindIdx, pVertexBuffer);
				++bindIdx;
			}
		}
	
		pElement = pRoot->FirstChildElement("indexbuffer");
		if (pElement != nullptr)
		{
			resource->setNewIndexData();

			render::IndexData* pIndexData = resource->getIndexData();
			pIndexData->indexStart = 0;

			unsigned int numIndexes = 0;
			if (pElement->QueryIntAttribute("count", &ivalue) == tinyxml2::XML_SUCCESS)
			{
				numIndexes = (unsigned int)ivalue;
			}
			pIndexData->indexCount = numIndexes;

			render::IndexBuffer* pIndexBuffer = render::RenderManager::getInstance()->createIndexBuffer(render::IT_32BIT, pIndexData->indexCount, resource->getIndexBufferUsage());
			pIndexData->indexBuffer = pIndexBuffer;

			unsigned int* pIdx = (unsigned int*)(pIndexBuffer->lock(resource::BL_DISCARD));
			if (pIdx == nullptr)
				return false;

			unsigned int i = 0;
			pSubElement = pElement->FirstChildElement("index");
			while(pSubElement != nullptr)
			{
				if (i >= pIndexData->indexCount)
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
