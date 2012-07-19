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