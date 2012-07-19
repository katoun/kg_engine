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

	static InputManager& getInstance();
	static InputManager* getInstancePtr();

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