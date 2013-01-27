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

#ifndef _VERTEX_BUFFER_H_
#define _VERTEX_BUFFER_H_

#include <core/Config.h>
#include <resource/Buffer.h>
#include <render/VertexBufferDefines.h>

#include <list>
#include <map>

namespace render
{

/** This class declares the usage of a single vertex buffer as a component
of a complete VertexDeclaration. 
@remarks
Several vertex buffers can be used to supply the input geometry for a
rendering operation, and in each case a vertex buffer can be used in
different ways for different operations; the buffer itself does not
define the semantics (position, normal etc), the VertexElement
class does.
*/
class ENGINE_PUBLIC_EXPORT VertexElement
{
public:
	/// Constructor, should not be called directly, call VertexDeclaration::addElement
	VertexElement(unsigned short int source, VertexElementType type, VertexElementSemantic semantic);

	inline bool operator== (const VertexElement& rhs) const;
	
	//! Gets the vertex buffer index from where this element draws it's values.
	unsigned short int getSource() const;
	//! Gets the data format of this element.
	VertexElementType getType() const;
	//! Gets the meaning of this element
	VertexElementSemantic getSemantic() const;
	//! Gets the size of this element in bytes.
	unsigned int getSize() const;
	//! Gets the count of this element in bytes.
	unsigned short int getCount() const;
	//! Utility method for helping to calculate offsets.
	static unsigned int getTypeSize(VertexElementType etype);
	//! Utility method which returns the count of values in a given type.
	static unsigned short int getTypeCount(VertexElementType etype);

protected:

	//! The source vertex buffer, as bound to an index using VertexBufferBinding
	unsigned short int mSource;
	//! The type of element
	VertexElementType mType;
	//! The meaning of the element
	VertexElementSemantic mSemantic;
};

//! This class declares the format of a set of vertex inputs, which can be issued to the rendering API. 
//!
//! You should be aware that the ordering and structure of the 
//! VertexDeclaration can be very important on DirectX with older
//! cards,so if you want to maintain maximum compatibility with 
//! all render systems and all cards you should be careful to follow these rules:
//! VertexElements should be added in the following order, and the order of the
//! elements within a shared buffer should be as follows: 
//! position, blending weights, normals, diffuse colors, specular colors, 
//! texture coordinates (in order, with no gaps).
//! You must not have unused gaps in your buffers which are not referenced by any VertexElement.
//! You must not cause the buffer & offset settings of 2 VertexElements to overlap.
//!
//! Whilst GL and more modern graphics cards in D3D will allow you to defy these rules,
//! sticking to them will ensure that your buffers have the maximum compatibility. 
class ENGINE_PUBLIC_EXPORT VertexDeclaration
{
public:
	/// Standard constructor, not you should use HardwareBufferManager::createVertexDeclaration
	VertexDeclaration();
	virtual ~VertexDeclaration();

	inline bool operator== (const VertexDeclaration& rhs) const;
	
	//! Gets read-only access to the list of vertex elements.
	const std::list<VertexElement*>& getElements() const;

	//! Adds a new VertexElement to this declaration. 
	//!
	//!This method adds a single element (positions, normals etc) to the vertex declaration.
	//!@param source The binding index of HardwareVertexBuffer which will provide the source for this element.
	//!@param offset The offset in bytes where this element is located in the buffer
	//!@param theType The data format of the element (3 floats, a color etc)
	//!@param semantic The meaning of the data (position, normal, diffuse color etc)
	//!@param index Optional index for multi-input elements like texture coordinates
	//!@returns A reference to the VertexElement added.
	virtual const VertexElement* addElement(unsigned short int source, VertexElementType theType, VertexElementSemantic semantic);

	//! Remove the element at the given index from this declaration.
	virtual void removeElement(unsigned short int elem_index);

	//! Remove the element with the given semantic. 
	virtual void removeElement(VertexElementSemantic semantic);

	//! Remove all elements. 
	virtual void removeAllElements();

	//! Modify an element in-place, params as addElement. 
	virtual void modifyElement(unsigned short int elem_index, unsigned short int source, VertexElementType type, VertexElementSemantic semantic);

	//! Finds a VertexElement with the given semantic, and index if there is more than 
	virtual const VertexElement* findElementBySemantic(VertexElementSemantic sem);

	//! Gets a list of elements which use a given source. 
	virtual std::list<VertexElement*> findElementsBySource(unsigned short int source);

	//! Gets the vertex size defined by this declaration for a given source.
	virtual unsigned int getVertexSize(unsigned short int source);

protected:

	std::list<VertexElement*> mVertexElements;
};

//! Class for index buffers, still abstract.
class ENGINE_PUBLIC_EXPORT VertexBuffer: public resource::Buffer
{
public:

	VertexBuffer(unsigned int vertexSize, unsigned int numVertices, resource::BufferUsage usage);

	virtual ~VertexBuffer();

	//! Get the number of vertices in this buffer.
	unsigned int getNumVertices();
	//! Get the size in bytes of each vertex.
	unsigned int getVertexSize();

protected:
	
	unsigned int mNumVertices;
	unsigned int mVertexSize;
};

//! Records the state of all the vertex buffer bindings required to provide a vertex declaration
//! with the input data it needs for the vertex elements.
//!
//! In the underlying APIs, binding the vertex buffers to an index (or 'stream') is the way that vertex data
//! is linked, so this structure better reflects the realities of that. 
//! In addition, by separating the vertex declaration from the list of vertex buffer bindings, it becomes possible
//! to reuse bindings between declarations and vice versa, giving opportunities to reduce the state changes to perform rendering.
class ENGINE_PUBLIC_EXPORT VertexBufferBinding
{
public:

	VertexBufferBinding();
	virtual ~VertexBufferBinding();

	//!  Gets a read-only version of the buffer bindings.
	virtual const std::map<unsigned short int, VertexBuffer*>& getBindings() const;

	//!  Set a binding, associating a vertex buffer with a given index. 
	//! 
	//!  If the index is already associated with a vertex buffer, 
	//!  the association will be replaced. This may cause the old buffer
	//!  to be destroyed if nothing else is referring to it.
	//!  You should assign bindings from 0 and not leave gaps, although you can
	//!  bind them in any order.
	virtual void setBinding(unsigned short int index, VertexBuffer* buffer);
	//!  Removes an existing binding.
	virtual void unsetBinding(unsigned short int index);

	//!  Removes all the bindings.
	virtual void unsetAllBindings();	

	//!  Gets the buffer bound to the given source index.
	virtual VertexBuffer* getBuffer(unsigned short int index);

	//!  Gets the highest index which has already been set, plus 1.
	//! 
	//!  This is to assist in binding the vertex buffers such that there are
	//!  not gaps in the list.
	virtual unsigned short int getNextIndex();

protected:

	std::map<unsigned short int, VertexBuffer*> mBindingMap;
	unsigned short int mHighIndex;
};

}// end namespace render

#endif// _VERTEXBUFFER_H_