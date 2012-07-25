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

#ifndef _MODEL_FACTORY_H_
#define _MODEL_FACTORY_H_

#include <core/Config.h>
#include <game/ComponentFactory.h>

namespace game
{
class Component;
}

namespace render
{

class ENGINE_PUBLIC_EXPORT ModelFactory: public game::ComponentFactory
{
public:

	ModelFactory();

	//! Creates a new model component.
	game::Component* createComponent();

	//! Destroys a model component which was created by this factory.
	void destroyComponent(game::Component* component);

	//! Adds model component to the render manager.
	void addComponent(game::Component* component);

	//! Removes model component from the render manager.
	void removeComponent(game::Component* component);
};

} // end namespace render

#endif