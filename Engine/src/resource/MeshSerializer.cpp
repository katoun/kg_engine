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

#include <resource/MeshSerializer.h>
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

#include <glm/glm.hpp>
#include <tinyxml2.h>

#include <string>
#include <vector>

struct PositionAndUV
{
	glm::vec3 position;
	glm::vec2 uv;
};

struct TangentAndBinormal
{
	glm::vec3 tangent;
	glm::vec3 binormal;
};

TangentAndBinormal calculateTangentAndBinormal(const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec3& pos3,
											const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec2& uv3)
{
	TangentAndBinormal rez;

	glm::vec3 side0 = pos1 - pos2;
	glm::vec3 side1 = pos3 - pos1;

	glm::vec3 normal = glm::normalize(glm::cross(side1, side0));

	float deltaV0 = uv1.y - uv2.y;
	float deltaV1 = uv3.y - uv1.y;

	glm::vec3 tangent = glm::normalize(side0 * deltaV1 - side1 * deltaV0);

	float deltaU0 = uv1.x - uv2.x;
	float deltaU1 = uv3.x - uv1.x;

	glm::vec3 binormal = glm::normalize(side0 * deltaU1 - side1 * deltaU0);

	glm::vec3 tangentCross = glm::cross(tangent, binormal);
	if (glm::dot(tangentCross, normal) < 0.0f)
	{
		tangent = -tangent;
		binormal = -binormal;
	}

	rez.tangent = tangent;
	rez.binormal = binormal;

	return rez;
}

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
		
		unsigned int numVertices = 0;
		unsigned int numIndexes = 0;
		std::vector<PositionAndUV> vertexArray;
		std::vector<unsigned int> indexArray;
		pElement = pRoot->FirstChildElement("vertexbuffer");
		if (pElement != nullptr)
		{
			svalue = pElement->Attribute("positions");
			if (svalue != nullptr)
			{
				if (std::string(svalue) != "true")
					return false;
			}

			if (pElement->QueryIntAttribute("count", &ivalue) == tinyxml2::XML_SUCCESS)
			{
				numVertices = (unsigned int)ivalue;
			}

			vertexArray.reserve(numVertices);
			vertexArray.resize(numVertices, PositionAndUV());

			render::VertexBuffer* pPositionVertexBuffer = nullptr;
			render::VertexBuffer* pNormalVertexBuffer = nullptr;
			render::VertexBuffer* pTexcoordVertexBuffer = nullptr;
			float *pPositionFloat = nullptr;
			float *pNormalFloat = nullptr;
			float *pTexcoordFloat = nullptr;

			pPositionVertexBuffer = render::RenderManager::getInstance()->createVertexBuffer(render::VERTEX_BUFFER_TYPE_POSITION, render::VERTEX_ELEMENT_TYPE_FLOAT3, numVertices, resource::BU_STATIC_WRITE_ONLY);
			pPositionFloat = (float*)(pPositionVertexBuffer->lock(resource::BL_DISCARD));
			if (pPositionFloat == nullptr)
				return false;

			pNormalVertexBuffer = render::RenderManager::getInstance()->createVertexBuffer(render::VERTEX_BUFFER_TYPE_NORMAL, render::VERTEX_ELEMENT_TYPE_FLOAT3, numVertices, resource::BU_STATIC_WRITE_ONLY);
			pNormalFloat = (float*)(pNormalVertexBuffer->lock(resource::BL_DISCARD));
			if (pNormalFloat == nullptr)
				return false;

			pTexcoordVertexBuffer = render::RenderManager::getInstance()->createVertexBuffer(render::VERTEX_BUFFER_TYPE_TEXTURE_COORDINATES, render::VERTEX_ELEMENT_TYPE_FLOAT2, numVertices, resource::BU_STATIC_WRITE_ONLY);
			pTexcoordFloat = (float*)(pTexcoordVertexBuffer->lock(resource::BL_DISCARD));
			if (pTexcoordFloat == nullptr)
				return false;

			core::aabox3d localBox;
			glm::vec3 min = glm::vec3(0, 0, 0);
			glm::vec3 max = glm::vec3(0, 0, 0);
			float maxSquaredRadius = -1.0f;

			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			float u = 0.0f;
			float v = 0.0f;
			unsigned int i = 0;
			pSubElement = pElement->FirstChildElement("vertex");
			while(pSubElement != nullptr)
			{
				if (i >= numVertices)
					break;

				///Position///
				pVertexSubElement = pSubElement->FirstChildElement("position");
				if (pVertexSubElement == nullptr)
				{
					if (core::Log::getInstance() != nullptr)
					{
						std::string msg =  "Unable to load mesh - position data not set at index:" + core::intToString(i) + ".";
						core::Log::getInstance()->logMessage("MeshSerializer", msg, core::LOG_LEVEL_ERROR);
					}
					return false;
				}
				
				x = 0.0f;
				y = 0.0f;
				z = 0.0f;

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

				pPositionFloat[i * 3 + 0] = x;
				pPositionFloat[i * 3 + 1] = y;
				pPositionFloat[i * 3 + 2] = z;

				glm::vec3 vec(x, y, z);

				vertexArray[i].position = vec;

				// Update sphere bounds
				float lengthSQ = vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
				if (lengthSQ > maxSquaredRadius)
					maxSquaredRadius = lengthSQ;

				// Update box
				if (vec.x < min.x) min.x = vec.x;
				if (vec.y < min.y) min.y = vec.y;
				if (vec.z < min.z) min.z = vec.z;

				if (vec.x > max.x) max.x = vec.x;
				if (vec.y > max.y) max.y = vec.y;
				if (vec.z > max.z) max.z = vec.z;
				///Position///

				///Normal///
				pVertexSubElement = pSubElement->FirstChildElement("normal");
				if (pVertexSubElement == nullptr)
				{
					if (core::Log::getInstance() != nullptr)
					{
						std::string msg =  "Unable to load mesh - normal data not set at index:" + core::intToString(i) + ".";
						core::Log::getInstance()->logMessage("MeshSerializer", msg, core::LOG_LEVEL_ERROR);
					}
					return false;
				}

				x = 0.0f;
				y = 0.0f;
				z = 0.0f;

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

				pNormalFloat[i * 3 + 0] = x;
				pNormalFloat[i * 3 + 1] = y;
				pNormalFloat[i * 3 + 2] = z;
				///Normal///

				///Texcoord///
				pVertexSubElement = pSubElement->FirstChildElement("texcoord");
				if (pVertexSubElement == nullptr)
				{
					if (core::Log::getInstance() != nullptr)
					{
						std::string msg =  "Unable to load mesh - texcoord data not set at index:" + core::intToString(i) + ".";
						core::Log::getInstance()->logMessage("MeshSerializer", msg, core::LOG_LEVEL_ERROR);
					}
					return false;
				}

				u = 0.0f;
				v = 0.0f;

				if (pVertexSubElement->QueryDoubleAttribute("u", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					u = (float)dvalue;
				}

				if (pVertexSubElement->QueryDoubleAttribute("v", &dvalue) == tinyxml2::XML_SUCCESS)
				{
					v = (float)dvalue;
				}

				pTexcoordFloat[i * 2 + 0] = u;
				pTexcoordFloat[i * 2 + 1] = v;

				vertexArray[i].uv = glm::vec2(u, v);
				///Texcoord///

				i++;

				pSubElement = pSubElement->NextSiblingElement("vertex");
			}

			localBox.MinEdge = min;
			localBox.MaxEdge = max;

			resource->setBoundingBox(localBox);

			// Pad out the sphere a little too
			resource->setBoundingSphereRadius(core::sqrt(maxSquaredRadius) * 1.25f);

			pPositionVertexBuffer->unlock();
			resource->setVertexBuffer(render::VERTEX_BUFFER_TYPE_POSITION, pPositionVertexBuffer);

			pNormalVertexBuffer->unlock();
			resource->setVertexBuffer(render::VERTEX_BUFFER_TYPE_NORMAL, pNormalVertexBuffer);

			pTexcoordVertexBuffer->unlock();
			resource->setVertexBuffer(render::VERTEX_BUFFER_TYPE_TEXTURE_COORDINATES, pTexcoordVertexBuffer);
		}
	
		pElement = pRoot->FirstChildElement("indexbuffer");
		if (pElement != nullptr)
		{
			if (pElement->QueryIntAttribute("count", &ivalue) == tinyxml2::XML_SUCCESS)
			{
				numIndexes = (unsigned int)ivalue;
			}

			indexArray.reserve(numIndexes);
			indexArray.resize(numIndexes,0);

			render::IndexBuffer* pIndexBuffer = render::RenderManager::getInstance()->createIndexBuffer(render::IT_32BIT, numIndexes, resource::BU_STATIC_WRITE_ONLY);
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

				indexArray[i] = index;
				pIdx[i++] = index;

				pSubElement = pSubElement->NextSiblingElement("index");
			}

			pIndexBuffer->unlock();
		}

		///Tangents and Binormals///
		std::vector<TangentAndBinormal> tangentArray;
		tangentArray.reserve(numVertices);
		tangentArray.resize(numVertices, TangentAndBinormal());

		for (unsigned int i = 0; i < numIndexes; i+=3)
		{
			unsigned int index1 = indexArray[i + 0];
			unsigned int index2 = indexArray[i + 1];
			unsigned int index3 = indexArray[i + 2];
			glm::vec3 pos1 = vertexArray[index1].position;
			glm::vec3 pos2 = vertexArray[index2].position;
			glm::vec3 pos3 = vertexArray[index3].position;

			glm::vec2 uv1 = vertexArray[index1].uv;
			glm::vec2 uv2 = vertexArray[index2].uv;
			glm::vec2 uv3 = vertexArray[index3].uv;

			TangentAndBinormal tangentAndBinormal = calculateTangentAndBinormal(pos1, pos2, pos3, uv1, uv2, uv3);

			tangentArray[index1].tangent += tangentAndBinormal.tangent;
			tangentArray[index1].binormal += tangentAndBinormal.binormal;

			tangentArray[index2].tangent += tangentAndBinormal.tangent;
			tangentArray[index2].binormal += tangentAndBinormal.binormal;

			tangentArray[index3].tangent += tangentAndBinormal.tangent;
			tangentArray[index3].binormal += tangentAndBinormal.binormal;
		}

		render::VertexBuffer* pTangentVertexBuffer = nullptr;
		render::VertexBuffer* pBinormalVertexBuffer = nullptr;
		float *pTangentFloat = nullptr;
		float *pBinormalFloat = nullptr;

		pTangentVertexBuffer = render::RenderManager::getInstance()->createVertexBuffer(render::VERTEX_BUFFER_TYPE_TANGENT, render::VERTEX_ELEMENT_TYPE_FLOAT3, numVertices, resource::BU_STATIC_WRITE_ONLY);
		pTangentFloat = (float*)(pTangentVertexBuffer->lock(resource::BL_DISCARD));
		if (pTangentFloat == nullptr)
			return false;

		pBinormalVertexBuffer = render::RenderManager::getInstance()->createVertexBuffer(render::VERTEX_BUFFER_TYPE_BINORMAL, render::VERTEX_ELEMENT_TYPE_FLOAT3, numVertices, resource::BU_STATIC_WRITE_ONLY);
		pBinormalFloat = (float*)(pBinormalVertexBuffer->lock(resource::BL_DISCARD));
		if (pBinormalFloat == nullptr)
			return false;

		for (unsigned int i = 0; i < numVertices; ++i)
		{
			tangentArray[i].tangent = glm::normalize(tangentArray[i].tangent);
			tangentArray[i].binormal = glm::normalize(tangentArray[i].binormal);

			pTangentFloat[i * 3 + 0] = tangentArray[i].tangent.x;
			pTangentFloat[i * 3 + 1] = tangentArray[i].tangent.y;
			pTangentFloat[i * 3 + 2] = tangentArray[i].tangent.z;

			pBinormalFloat[i * 3 + 0] = tangentArray[i].binormal.x;
			pBinormalFloat[i * 3 + 1] = tangentArray[i].binormal.y;
			pBinormalFloat[i * 3 + 2] = tangentArray[i].binormal.z;
		}

		pTangentVertexBuffer->unlock();
		resource->setVertexBuffer(render::VERTEX_BUFFER_TYPE_TANGENT, pTangentVertexBuffer);

		pBinormalVertexBuffer->unlock();
		resource->setVertexBuffer(render::VERTEX_BUFFER_TYPE_BINORMAL, pBinormalVertexBuffer);
		///Tangents and Binormals///
	}

	return true;
}

bool MeshSerializer::exportResource(Resource* source, const std::string& filename)
{
	return true;
}

}// end namespace resource
