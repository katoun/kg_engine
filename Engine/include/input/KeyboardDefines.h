/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

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

#ifndef _KEYBOARD_DEFINES_H_
#define _KEYBOARD_DEFINES_H_

namespace input
{

//! Keyboard scan codes.
enum KeyCode
{
	KEY_UNKNOWN,
	KEY_ESCAPE,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
	KEY_MINUS,			// - on main keyboard
	KEY_EQUALS,			// =
	KEY_BACK,			// backspace
	KEY_TAB,
	KEY_Q,
	KEY_W,
	KEY_E,
	KEY_R,
	KEY_T,
	KEY_Y,
	KEY_U,
	KEY_I,
	KEY_O,
	KEY_P,
	KEY_LBRACKET,		// [
	KEY_RBRACKET,		// ]
	KEY_RETURN,			// Enter on main keyboard
	KEY_LCONTROL,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_GRAVE,			// accent
	KEY_LSHIFT,
	KEY_BACKSLASH,		// \ on main keyboadr
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	KEY_N,
	KEY_M,
	KEY_COMMA,			// , on main keyboard
	KEY_PERIOD,			// . on main keyboard
	KEY_SLASH,			// / on main keyboard
	KEY_RSHIFT,
	KEY_NUMPADMULTIPLY,	// * on numeric keypad
	KEY_LMENU,			// left Alt
	KEY_SPACE,
	KEY_CAPITAL,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_NUMLOCK,
	KEY_SCROLL,			// Scroll Lock
	KEY_NUMPAD7,
	KEY_NUMPAD8,
	KEY_NUMPAD9,
	KEY_NUMPADSUBTRACT,	// - on numeric keypad
	KEY_NUMPAD4,
	KEY_NUMPAD5,
	KEY_NUMPAD6,
	KEY_NUMPADADD,		// + on numeric keypad
	KEY_NUMPAD1,
	KEY_NUMPAD2,
	KEY_NUMPAD3,
	KEY_NUMPAD0,
	KEY_NUMPADDECIMAL,	// . on numeric keypad
	KEY_OEM_102,		// < > | on UK/Germany keyboards
	KEY_F11,
	KEY_F12,
	KEY_F13,			//					 (NEC PC98)
	KEY_F14,			//					 (NEC PC98)
	KEY_F15,			//					 (NEC PC98)
	KEY_KANA,			// (Japanese keyboard)
	KEY_ABNT_C1,		// / ? on Portugese (Brazilian) keyboards
	KEY_CONVERT,		// (Japanese keyboard)
	KEY_NOCONVERT,		// (Japanese keyboard)
	KEY_YEN,			// (Japanese keyboard)
	KEY_ABNT_C2,		// Numpad . on Portugese (Brazilian) keyboards
	KEY_NUMPADEQUALS,	// = on numeric keypad (NEC PC98)
	KEY_PREVTRACK,		// Previous Track (KEY_CIRCUMFLEX on Japanese keyboard)
	KEY_AT,				//					 (NEC PC98)
	KEY_COLON,			//					 (NEC PC98)
	KEY_UNDERLINE,		//					 (NEC PC98)
	KEY_KANJI,			// (Japanese keyboard)
	KEY_STOP,			//					 (NEC PC98)
	KEY_AX,				//					 (Japan AX)
	KEY_UNLABELED,		//						(J3100)
	KEY_NEXTTRACK,		// Next Track
	KEY_NUMPADENTER,	// Enter on numeric keypad
	KEY_RCONTROL,
	KEY_MUTE,			// Mute
	KEY_CALCULATOR,		// Calculator
	KEY_PLAYPAUSE,		// Play / Pause
	KEY_MEDIASTOP,		// Media Stop
	KEY_VOLUMEDOWN,		// Volume -
	KEY_VOLUMEUP,		// Volume +
	KEY_WEBHOME,		// Web home
	KEY_NUMPADCOMMA,	// , on numeric keypad (NEC PC98)
	KEY_NUMPADDIVIDE,	// / on numeric keypad
	KEY_SYSRQ,
	KEY_RMENU,			// right Alt
	KEY_PAUSE,			// Pause
	KEY_HOME,			// Home on arrow keypad
	KEY_UP,				// UpArrow on arrow keypad
	KEY_PGUP,			// PgUp on arrow keypad
	KEY_LEFT,			// LeftArrow on arrow keypad
	KEY_RIGHT,			// RightArrow on arrow keypad
	KEY_END,			// End on arrow keypad
	KEY_DOWN,			// DownArrow on arrow keypad
	KEY_PGDOWN,			// PgDn on arrow keypad
	KEY_INSERT,			// Insert on arrow keypad
	KEY_DELETE,			// Delete on arrow keypad
	KEY_LWIN,			// Left Windows key
	KEY_RWIN,			// Right Windows key
	KEY_APPS,			// AppMenu key
	KEY_POWER,			// System Power
	KEY_SLEEP,			// System Sleep
	KEY_WAKE,			// System Wake
	KEY_WEBSEARCH,		// Web Search
	KEY_WEBFAVORITES,	// Web Favorites
	KEY_WEBREFRESH,		// Web Refresh
	KEY_WEBSTOP,		// Web Stop
	KEY_WEBFORWARD,		// Web Forward
	KEY_WEBBACK,		// Web Back
	KEY_MYCOMPUTER,		// My Computer
	KEY_MAIL,			// Mail
	KEY_MEDIASELECT,	// Media Select
	KEY_COUNT
};

//! Keyboard enum of bit position of modifer
enum KeyModifier
{
	KEY_MODIFIER_SHIFT,
	KEY_MODIFIER_CTRL,
	KEY_MODIFIER_ALT,
	KEY_MODIFIER_COUNT
};

} // end namespace input

#endif