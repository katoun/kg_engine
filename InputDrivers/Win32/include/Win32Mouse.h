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

#ifndef _WIN32_MOUSE_H_
#define _WIN32_MOUSE_H_

#include <Win32Config.h>
#include <input/Mouse.h>

#include <map>

namespace input
{

enum MouseButton;

class Win32Mouse : public Mouse
{
public:

	//! Constructor
	Win32Mouse(bool buffered);

	//! Destructor
	~Win32Mouse();

	void sendButtonDown(oi_mouse button);

	void sendButtonUp(oi_mouse button);

	void sendMove(signed int axisX, signed int axisY);

	void setGrab(bool grabbed);

protected:

	void initializeImpl();
	void uninitializeImpl();

	void updateImpl(float elapsedTime);

	std::map<oi_mouse, MouseButton> mButtonMap;
};

} // end namespace input

#endif