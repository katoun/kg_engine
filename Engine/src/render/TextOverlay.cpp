/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

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

#include <render/TextOverlay.h>
#include <render/Font.h>
#include <render/VertexBuffer.h>
#include <render/IndexBuffer.h>
#include <render/RenderManager.h>
#include <resource/Resource.h>
#include <resource/ResourceManager.h>

namespace render
{

#define DEFAULT_INITIAL_CHARS 12

// vertex buffer bindings, set at compile time
#define POS_TEX_BINDING 0

#define UNICODE_NEL 0x0085
#define UNICODE_CR 0x000D
#define UNICODE_LF 0x000A
#define UNICODE_SPACE 0x0020
#define UNICODE_ZERO 0x0030

TextOverlay::TextOverlay(): Overlay()
{
	mFont = NULL;

	mCharHeight = 0.02f;
	mPixelCharHeight = 12.0f;
	mSpaceWidth = 0.0f;
	mPixelSpaceWidth = 0.0f;
	mAllocSize = 0;

	mAlignment = A_LEFT;

	mViewportAspectCoef = 1;

	initialize();
}

TextOverlay::TextOverlay(const std::string& name): Overlay(name)
{
	mFont = NULL;

	mCharHeight = 0.02f;
	mPixelCharHeight = 12.0f;
	mSpaceWidth = 0.0f;
	mPixelSpaceWidth = 0.0f;
	mAllocSize = 0;

	mAlignment = A_LEFT;

	mViewportAspectCoef = 1;

	initialize();
}

TextOverlay::~TextOverlay() {}

void TextOverlay::setFont(const std::string& filename)
{
	std::string fontname = filename;
	mFont = static_cast<Font*>(resource::ResourceManager::getInstance().createResource(resource::RESOURCE_TYPE_FONT, fontname));
	
	mGeomPositionsOutOfDate = true;
	mGeomUVsOutOfDate = true;
}

void TextOverlay::setFont(Font* font)
{
	if (font == NULL)
		return;

	mFont = font;

	mGeomPositionsOutOfDate = true;
	mGeomUVsOutOfDate = true;
}

Material* TextOverlay::getMaterial() const
{
	return mFont->getMaterial();
}

void TextOverlay::setCaption(const std::string& caption)
{
	mCaption = caption;

	mGeomPositionsOutOfDate = true;
	mGeomUVsOutOfDate = true;
}

const std::string& TextOverlay::getCaption() const
{
	return mCaption;
}

void TextOverlay::setAlignment(Alignment a)
{
	mAlignment = a;

	mGeomPositionsOutOfDate = true;
}

Alignment TextOverlay::getAlignment() const
{
	return mAlignment;
}

void TextOverlay::setCharHeight(float height)
{
	if (mMetricsMode == MM_PIXELS)
	{
		mPixelCharHeight = height;
	}
	else
	{
		mCharHeight = height;
	}

	mGeomPositionsOutOfDate = true;
}

float TextOverlay::getCharHeight() const
{
	return mCharHeight;
}

void TextOverlay::setSpaceWidth(float width)
{
	if (mMetricsMode == MM_PIXELS)
	{
		mPixelSpaceWidth = width;
	}
	else
	{
		mSpaceWidth = width;
	}

	mGeomPositionsOutOfDate = true;
}

float TextOverlay::getSpaceWidth() const
{
	return mSpaceWidth;
}

void TextOverlay::initializeImpl()
{
	mRenderOperationType = ROT_TRIANGLE_LIST;

	mVertexData = new VertexData();
	// Basic vertex data
	mVertexData->vertexStart = 0;

	unsigned int offset = 0;
	VertexDeclaration* decl = mVertexData->vertexDeclaration;
	decl->addElement(POS_TEX_BINDING, offset, VET_FLOAT3, VES_POSITION);
	offset += VertexElement::getTypeSize(VET_FLOAT3);
	decl->addElement(POS_TEX_BINDING, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);

	mIndexData = new IndexData();
	// Basic index data
	mIndexData->indexStart = 0;
	
	allocateMemory(DEFAULT_INITIAL_CHARS);

	mGeometryInitialized = true;
}

void TextOverlay::updateTransformImpl()
{
	float vpWidth, vpHeight;
	vpWidth = (float)(RenderManager::getInstance().getViewportWidth());
	vpHeight = (float)(RenderManager::getInstance().getViewportHeight());
	mViewportAspectCoef = (vpHeight / vpWidth);

	Overlay::updateTransformImpl();

	if (mGeomPositionsOutOfDate)
	{
		switch (mMetricsMode)
		{
		case MM_PIXELS :
			{
				// recalculate character size
				mCharHeight = mPixelCharHeight / vpHeight;
				mSpaceWidth = mPixelSpaceWidth / vpHeight;
				mGeomPositionsOutOfDate = true;
			}
			break;

		case MM_RELATIVE_ASPECT_ADJUSTED :
			{
				// recalculate character size
				mCharHeight = mPixelCharHeight / 10000.0f;
				mSpaceWidth = mPixelSpaceWidth / 10000.0f;
				mGeomPositionsOutOfDate = true;
			}
			break;

		default:
			break;
		}
	}
}

void TextOverlay::allocateMemory(unsigned int numChars)
{
	//	0-----2  4-----6  8-----10
	//	|    /|  |    /|  |    /|
	//	|  /  |  |  /  |  |  /  |  [...]
	//	|/    |  |/    |  |/    |
	//	1-----3  5-----7  9-----11

	if(mAllocSize < numChars)
	{
		VertexDeclaration* decl = mVertexData->vertexDeclaration;
		VertexBufferBinding* bind = mVertexData->vertexBufferBinding;

		// 4 verts per char since we're doing tri lists with indexes
		mVertexData->vertexCount = numChars * 4;

		// Vertex buffer #1, position & texcoords
		VertexBuffer* vbuf1 = RenderManager::getInstance().createVertexBuffer(decl->getVertexSize(POS_TEX_BINDING), mVertexData->vertexCount, resource::BU_DYNAMIC);
		// bind position & texcoords
		mVertexData->vertexBufferBinding->setBinding(POS_TEX_BINDING, vbuf1);	

		// 6 indexes per char since we're doing tri lists with indexes
		mIndexData->indexCount = numChars* 6;

		// Index buffer
		IndexBuffer* ibuf = RenderManager::getInstance().createIndexBuffer(IT_32BIT, mIndexData->indexCount, resource::BU_DYNAMIC);
		mIndexData->indexBuffer = ibuf;
		unsigned int* idx = (unsigned int*)(ibuf->lock(resource::BL_DISCARD));

		for (unsigned int i=0; i<numChars; ++i)
		{
			idx[i*6+0] = i*4+0;
			idx[i*6+1] = i*4+1;
			idx[i*6+2] = i*4+2;

			idx[i*6+3] = i*4+1;
			idx[i*6+4] = i*4+3;
			idx[i*6+5] = i*4+2;
		}

		ibuf->unlock();

		mAllocSize = numChars;
	}
}

void TextOverlay::updatePositionBinding()
{
	//	0-----2  4-----6  8-----10
	//	|    /|  |    /|  |    /|
	//	|  /  |  |  /  |  |  /  |  [...]
	//	|/    |  |/    |  |/    |
	//	1-----3  5-----7  9-----11

	if (mFont == NULL || mFont->getState() != resource::RESOURCE_STATE_LOADED)
		return;

	unsigned int charlen = mCaption.size();
	allocateMemory(charlen);
	mGeometryInitialized = true;

	mVertexData->vertexCount = charlen * 4;
	mIndexData->indexCount = charlen * 6;

	float left, top, largestWidth;

	// Convert positions into -1, 1 coordinate space (homogenous clip space).
	left = -mWidth;
	top = mHeight;

	largestWidth = 0;

	// Derive space with from a number 0
	mSpaceWidth = mFont->getCharAspectRatio(UNICODE_ZERO) * mCharHeight * 2.0f * mViewportAspectCoef;

	// Use the furthest away depth value, since materials should have depth-check off
	// This initialized the depth buffer for any 3D objects in front
	float zValue = RenderManager::getInstance().getMaximumDepthInputValue();

	VertexBuffer* vbuf = mVertexData->vertexBufferBinding->getBuffer(POS_TEX_BINDING);
	float* pVert = (float*)(vbuf->lock(resource::BL_DISCARD));

	bool newLine = true;

	for (unsigned int i=0; i<charlen; ++i)
	{
		if (newLine)
		{
			float len = 0.0f;
			for (unsigned int j = i; j < mCaption.size(); ++j)
			{
				if (mCaption[j] == UNICODE_CR || mCaption[j] == UNICODE_NEL || mCaption[j] == UNICODE_LF) 
				{
					break;
				}
				else if (mCaption[j] == UNICODE_SPACE)
				{
					len += mSpaceWidth;
				}
				else 
				{
					len += mFont->getCharAspectRatio(mCaption[j]) * mCharHeight * 2.0f * mViewportAspectCoef;
				}
			}

			if (mAlignment == A_RIGHT)
				left -= len;
			else if (mAlignment == A_CENTER)
				left -= len * 0.5f;

			newLine = false;
		}

		if (mCaption[i] == UNICODE_CR || mCaption[i] == UNICODE_NEL || mCaption[i] == UNICODE_LF) 
		{
			left = -mWidth;
			top -= mCharHeight * 2.0f;
			newLine = true;
			// Also reduce tri count
			mVertexData->vertexCount -= 4;
			// Also reduce index count
			mIndexData->indexCount -= 6;

			// consume CR/LF in one
			if (mCaption[i] == UNICODE_CR)
			{
				if ((i+1) < mCaption.size() && mCaption[i+1] == UNICODE_LF)
				{
					++i; // skip both as one newline
					// Also reduce tri count
					mVertexData->vertexCount -= 4;
					// Also reduce index count
					mIndexData->indexCount -= 6;
				}
			}
			continue;
		}
		else if (mCaption[i] == UNICODE_SPACE)
		{
			// Just leave a gap, no tris
			left += mSpaceWidth;
			// Also reduce tri count
			mVertexData->vertexCount -= 4;
			// Also reduce index count
			mIndexData->indexCount -= 6;

			continue;
		}

		float horiz_height = mFont->getCharAspectRatio(mCaption[i]) * mViewportAspectCoef;
		float u1, u2, v1, v2; 
		mFont->getCharTexCoords(mCaption[i], u1, v1, u2, v2 );

		// each vert is (x, y, z, u, v)
		
		// Upper left
		*pVert++ = left;
		*pVert++ = top;
		*pVert++ = zValue;
		*pVert++ = u1;
		*pVert++ = v1;

		top -= mCharHeight * 2.0f;

		// Bottom left
		*pVert++ = left;
		*pVert++ = top;
		*pVert++ = zValue;
		*pVert++ = u1;
		*pVert++ = v2;

		top += mCharHeight * 2.0f;
		left += horiz_height * mCharHeight * 2.0f;

		// Top right
		*pVert++ = left;
		*pVert++ = top;
		*pVert++ = zValue;
		*pVert++ = u2;
		*pVert++ = v1;
		
		top -= mCharHeight * 2.0f;
		
		// Bottom right
		*pVert++ = left;
		*pVert++ = top;
		*pVert++ = zValue;
		*pVert++ = u2;
		*pVert++ = v2;

		// Go back up with top
		top += mCharHeight * 2.0f;
		
		float currentWidth = (left + mWidth)/2.0f;
		if (currentWidth > largestWidth)
		{
			largestWidth = currentWidth;
		}
	}

	vbuf->unlock();
	if (mMetricsMode == MM_PIXELS)
	{
		// Derive parametric version of dimensions
		float vpWidth;
		vpWidth = (float)(RenderManager::getInstance().getViewportWidth());

		largestWidth *= vpWidth;
	}
	if (getWidth() < largestWidth)
		setDimension(largestWidth, mHeight);
}

void TextOverlay::updateTextureBinding() {}

} //namespace render
