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

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <EngineConfig.h>
#include <engine/ObjectDefines.h>
#include <core/Singleton.h>

#include <string>

namespace engine
{

class ENGINE_PUBLIC_EXPORT Object
{
public:

	Object();
	Object(const std::string& name);

	~Object();

	//! Returns the id of the object.
	const unsigned int& getID() const;

	//! Gets object type.
	const ObjectType& getObjectType() const;

	void setName(const std::string& name);
	const std::string& getName() const;

	//! Initialize object.
	void initialize();

	//! Uninitialize object.
	void uninitialize();

	void update(float elapsedTime);

	//! Returns true if the Object has been initialized.
	bool isInitialized() const;

protected:

	unsigned int mID;

	bool mChangeableName;
	std::string mName;

	ObjectType mObjectType;

	//! Incremented count for next object index
	static unsigned int msNextGeneratedObjectIndex;

	static unsigned int mIndexCounter;

	virtual void initializeImpl();
	virtual void uninitializeImpl();
	virtual void updateImpl(float elapsedTime);

	bool mInitialized;
};

} // end namespace engine

#endif