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

#ifndef _WIN32_KEYBOARD_H_
#define _WIN32_KEYBOARD_H_

#include <Win32Config.h>
#include <input/Keyboard.h>
#include <input/KeyboardDefines.h>

#include <map>
#include <bitset>

namespace input
{

class Win32Keyboard : public Keyboard
{
public:

	//! Constructor
	Win32Keyboard(bool buffered);

	//! Destructor
	~Win32Keyboard();

	void sendKeyDown(oi_key key);

	void sendKeyUp(oi_key key);

protected:

	void initializeImpl();
	void uninitializeImpl();

	void updateImpl(float elapsedTime);

	std::map<oi_key, KeyCode> mKeyMap;
	std::bitset<KEY_MODIFIER_COUNT> mModifiers;
};

} // end namespace input

#endif