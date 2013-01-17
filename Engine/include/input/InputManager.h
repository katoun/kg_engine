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

#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <core/Config.h>
#include <core/System.h>
#include <core/Singleton.h>
#include <core/Position2d.h>

#include <map>

namespace input
{

class Cursor;
class InputDriver;
class InputDevice;
enum InputType;

class ENGINE_PUBLIC_EXPORT InputManager: public core::System, public core::Singleton<InputManager>
{
public:
	
	//! Default constructor.
	InputManager();

	virtual ~InputManager();

	//! Returns the number of joysticks found.
	signed int numJoySticks();

	//!	Returns the number of mice found.
	signed int numMice();

	//!	Returns the number of keyboards found.
	signed int numKeyboards();

	InputDevice* createInputDevice(InputType type, bool buffered);

	void removeInputDevice(InputDevice* device);

	void removeAllInputDevices();

	Cursor* getCursor();

	void setInputDriver(InputDriver* driver);
	void removeInputDriver();

	static InputManager* getInstance();

protected:

	void initializeImpl();
	void uninitializeImpl();
	void startImpl();
	void stopImpl();
	void updateImpl(float elapsedTime);
	void setSystemDriverImpl(core::SystemDriver* systemDriver);
	void removeSystemDriverImpl();

	std::list<InputDevice*> mInputDevices;

	Cursor* mCursor;

	core::position2d mCursorCenter;

	InputDriver* mInputDriver;
};

} // end namespace input

#endif