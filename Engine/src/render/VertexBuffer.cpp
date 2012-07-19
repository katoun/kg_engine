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
#include <core/Utils.h>
#include <core/LogDefines.h>
#include <render/VertexBuffer.h>

namespace render
{

VertexElement::VertexElement(unsigned short int source, unsigned int offset, VertexElementType theType, VertexElementSemantic semantic, unsigned short int index)
{
	mSource = source;
	mOffset = offset;
	mType = theType; 
	mSemantic = semantic;
	mIndex = index;
}

inline bool VertexElement::operator== (const VertexElement& rhs) const
{
	if (mType != rhs.mType || mIndex != rhs.mIndex || mOffset != rhs.mOffset || mSemantic != rhs.mSemantic || mSource != rhs.mSource)
		return false;
	else
		return true;
}

unsigned short int VertexElement::getSource() const
{
	return mSource;
}

unsigned int VertexElement::getOffset() const
{
	return mOffset;
}

VertexElementType VertexElement::getType() const
{
	return mType;
}

VertexElementSemantic VertexElement::getSemantic() const
{
	return mSemantic;
}

unsigned short int VertexElement::getIndex() const 
{
	return mIndex;
}

unsigned int VertexElement::getSize() const
{
	return getTypeSize(mType);
}

unsigned int VertexElement::getTypeSize(VertexElementType etype)
{
	switch(etype)
	{
	case VET_COLOR:
		return sizeof(unsigned int);
	case VET_FLOAT1:
		return sizeof(float);
	case VET_FLOAT2:
		return sizeof(float)*2;
	case VET_FLOAT3:
		return sizeof(float)*3;
	case VET_FLOAT4:
		return sizeof(float)*4;
	case VET_SHORT1:
		return sizeof(signed short int);
	case VET_SHORT2:
		return sizeof(signed short int)*2;
	case VET_SHORT3:
		return sizeof(signed short int)*3;
	case VET_SHORT4:
		return sizeof(signed short int)*4;
	}
	return 0;
}

unsigned short int VertexElement::getTypeCount(VertexElementType etype)
{
	switch (etype)
	{
	case VET_COLOR:
		return 1;
	case VET_FLOAT1:
		return 1;
	case VET_FLOAT2:
		return 2;
	case VET_FLOAT3:
		return 3;
	case VET_FLOAT4:
		return 4;
	case VET_SHORT1:
		return 1;
	case VET_SHORT2:
		return 2;
	case VET_SHORT3:
		return 3;
	case VET_SHORT4:
		return 4;
	default:
		return 0;
	}
}

VertexElementType VertexElement::multiplyTypeCount(VertexElementType baseType, unsigned short count)
{
	switch (baseType)
	{
	case VET_FLOAT1:
		switch(count)
		{
		case 1:
			return VET_FLOAT1;
		case 2:
			return VET_FLOAT2;
		case 3:
			return VET_FLOAT3;
		case 4:
			return VET_FLOAT4;
		default:
			return VET_FLOAT1;
		}
		break;
	case VET_SHORT1:
		switch(count)
		{
		case 1:
			return VET_SHORT1;
		case 2:
			return VET_SHORT2;
		case 3:
			return VET_SHORT3;
		case 4:
			return VET_SHORT4;
		default:
			return VET_SHORT1;
		}
		break;
	default:
		return VET_FLOAT1;
	}
}

//////////////////////////////////////////////////////////////////////////

VertexDeclaration::VertexDeclaration() {}

VertexDeclaration::~VertexDeclaration()
{
	// Remove all Elements
	removeAllElements();
}

inline bool VertexDeclaration::operator== (const VertexDeclaration& rhs) const
{
	if (mVertexElements.size() != rhs.mVertexElements.size())
		return false;

	std::list<VertexElement*>::const_iterator i, rhsi;
	rhsi = rhs.mVertexElements.begin();

	for (i = mVertexElements.begin(); i != mVertexElements.end() && rhsi != rhs.mVertexElements.end(); ++i, ++rhsi)
	{
		VertexElement* a = (*i);
		VertexElement* b = (*rhsi);
		if (!((*a) == (*b)))
			return false;
	}

	return true;
}

const std::list<VertexElement*>& VertexDeclaration::getElements() const
{
	return mVertexElements;
}

const VertexElement* VertexDeclaration::addElement(unsigned short int source, unsigned int offset, VertexElementType theType, VertexElementSemantic semantic, unsigned short int index)
{
	VertexElement* newVertexElement = new VertexElement(source, offset, theType, semantic, index);
	mVertexElements.push_back(newVertexElement);

	return newVertexElement;
}

void VertexDeclaration::removeElement(unsigned short int elem_index)
{
	assert(elem_index < mVertexElements.size());

	std::list<VertexElement*>::iterator i = mVertexElements.begin();
	std::advance(i, elem_index);

	mVertexElements.erase(i);
	delete (*i);
}

void VertexDeclaration::removeElement(VertexElementSemantic semantic, unsigned short int index)
{
	std::list<VertexElement*>::iterator i;
	for (i = mVertexElements.begin(); i != mVertexElements.end(); ++i)
	{
		if ((*i)->getSemantic() == semantic && (*i)->getIndex() == index)
		{
			mVertexElements.erase(i);
			delete (*i);
			break;
		}
	}
}

void VertexDeclaration::removeAllElements()
{
	std::list<VertexElement*>::iterator i;
	for (i = mVertexElements.begin(); i != mVertexElements.end(); ++i)
	{
		delete (*i);
	}
	mVertexElements.clear();
}

void VertexDeclaration::modifyElement(unsigned short int elem_index, unsigned short int source, unsigned int offset, VertexElementType theType, VertexElementSemantic semantic, unsigned short int index)
{
	assert(elem_index < mVertexElements.size());

	std::list<VertexElement*>::iterator i = mVertexElements.begin();
	std::advance(i, elem_index);
	
	if ((*i))
	{
		delete (*i);
		(*i) = NULL;
	}

	(*i) = new VertexElement(source, offset, theType, semantic, index);
}

const VertexElement* VertexDeclaration::findElementBySemantic(VertexElementSemantic sem, unsigned short int index)
{
	std::list<VertexElement*>::const_iterator i;
	for (i = mVertexElements.begin(); i != mVertexElements.end(); ++i)
	{
		if ((*i)->getSemantic() == sem && (*i)->getIndex() == index)
		{
			return (*i);
		}
	}

	return NULL;
}

std::list<VertexElement*> VertexDeclaration::findElementsBySource(unsigned short int source)
{
	std::list<VertexElement*> retList;
	std::list<VertexElement*>::const_iterator i;
	for (i = mVertexElements.begin(); i != mVertexElements.end(); ++i)
	{
		if ((*i)->getSource() == source)
		{
			retList.push_back((*i));
		}
	}
	return retList;
}

unsigned int VertexDeclaration::getVertexSize(unsigned short int source)
{
	unsigned int sz = 0;

	std::list<VertexElement*>::const_iterator i;
	for (i = mVertexElements.begin(); i != mVertexElements.end(); ++i)
	{
		if ((*i)->getSource() == source)
		{
			sz += (*i)->getSize();
		}
	}
	return sz;
}

//////////////////////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer(unsigned int vertexSize, unsigned int numVertices, resource::BufferUsage usage)
: resource::Buffer(usage)
{
	mNumVertices = numVertices;
	mVertexSize = vertexSize;

	// Calculate the size of the vertices
	mSizeInBytes = mVertexSize * numVertices;
}

VertexBuffer::~VertexBuffer() {}

unsigned int VertexBuffer::getNumVertices()
{
	return mNumVertices;
}

unsigned int VertexBuffer::getVertexSize()
{
	return mVertexSize;
}

//////////////////////////////////////////////////////////////////////////

VertexBufferBinding::VertexBufferBinding()
{
	mHighIndex = 0;
}

VertexBufferBinding::~VertexBufferBinding()
{
	unsetAllBindings();
}

const std::map<unsigned short int, VertexBuffer*>& VertexBufferBinding::getBindings() const
{
	return mBindingMap;
}

void VertexBufferBinding::setBinding(unsigned short int index, VertexBuffer* buffer)
{
	// will replace any existing buffer ptr at this index, and will thus cause
	// reference count to decrement on that buffer (possibly destroying it)
	mBindingMap[index] = buffer;
	if (index > mHighIndex)
		mHighIndex = index;
}

void VertexBufferBinding::unsetBinding(unsigned short int index)
{
	std::map<unsigned short int, VertexBuffer*>::iterator i = mBindingMap.find(index);
	if (i == mBindingMap.end())
	{		
		std::string message = "Cannot find buffer binding for index " + core::intToString(index) + ".";

		core::Log::getInstance().logMessage("VertexBufferBinding", message, core::LOG_LEVEL_ERROR);

		return;
	}

	mBindingMap.erase(i);
}

void VertexBufferBinding::unsetAllBindings()
{
	mBindingMap.clear();
}

VertexBuffer* VertexBufferBinding::getBuffer(unsigned short int index)
{
	std::map<unsigned short int, VertexBuffer*>::const_iterator i = mBindingMap.find(index);
	if (i != mBindingMap.end())
		return i->second;

	return NULL;
}

unsigned short int VertexBufferBinding::getNextIndex()
{
	return mHighIndex;
}

}// end namespace render
