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

#ifndef _COMPONENT_FACTORY_H_
#define _COMPONENT_FACTORY_H_

#include <core/Config.h>

namespace game
{

class Component;

class ENGINE_PUBLIC_EXPORT ComponentFactory
{
public:

	ComponentFactory();

	//! Creates a new component.
	virtual Component* createComponent() = 0;

	//! Destroys a component which was created by this factory.
	virtual void destroyComponent(Component* component) = 0;

	//! Adds component to the coresponding manager.
	virtual void addComponent(Component* component);

	//! Removes component from the coresponding manager.
	virtual void removeComponent(Component* component);

	//! Return the name of the type of component this factory creates.
	const std::string& getName();

protected:

	std::string mName;
};

} // end namespace game

#endif