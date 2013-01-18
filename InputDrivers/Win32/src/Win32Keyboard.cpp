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

#include <input/KeyEvent.h>
#include <input/KeyEventReceiver.h>
#include <Win32Keyboard.h>
#include <Win32InputDriver.h>

namespace input
{

Win32Keyboard::Win32Keyboard(bool buffered): Keyboard()
{
	mBuffered = buffered;
}

Win32Keyboard::~Win32Keyboard() {}

void Win32Keyboard::sendKeyDown(oi_key key)
{
	KeyCode kc = mKeyMap[key];

	//Turn on modifier
	if (kc == KEY_LCONTROL || kc == KEY_RCONTROL)
		mModifiers.set(KEY_MODIFIER_CTRL);
	else if (kc == KEY_LSHIFT || kc == KEY_RSHIFT)
		mModifiers.set(KEY_MODIFIER_SHIFT);
	else if (kc == KEY_LMENU || kc == KEY_RMENU)
		mModifiers.set(KEY_MODIFIER_ALT);

	mEvent->set(kc, mModifiers, 0, this);

	std::list<input::KeyEventReceiver*>::const_iterator j;
	for (j = mKeyEventReceivers.begin(); j != mKeyEventReceivers.end(); ++j)
	{
		(*j)->keyPressed(*mEvent);
	}
}

void Win32Keyboard::sendKeyUp(oi_key key)
{
	KeyCode kc = mKeyMap[key];

	//Turn off modifier
	if (kc == KEY_LCONTROL || kc == KEY_RCONTROL)
		mModifiers.reset(KEY_MODIFIER_CTRL);
	else if (kc == KEY_LSHIFT || kc == KEY_RSHIFT)
		mModifiers.reset(KEY_MODIFIER_SHIFT);
	else if (kc == KEY_LMENU || kc == KEY_RMENU)
		mModifiers.reset(KEY_MODIFIER_ALT);

	mEvent->set(kc, mModifiers, 0, this);

	std::list<input::KeyEventReceiver*>::const_iterator j;
	for (j = mKeyEventReceivers.begin(); j != mKeyEventReceivers.end(); ++j)
	{
		(*j)->keyReleased(*mEvent);
	}
}

void Win32Keyboard::initializeImpl()
{
	mModifiers.reset();

	mKeyMap[OIK_UNKNOWN]			= KEY_UNKNOWN;   /**< key_unknown */

    // Ascii-keys
	mKeyMap[OIK_BACKSPACE]			= KEY_BACK;   /**< key_backspace */
    mKeyMap[OIK_TAB]				= KEY_TAB;   /**< key_tab */
    mKeyMap[OIK_CLEAR]				= KEY_UNKNOWN;  /**< key_clear */
	mKeyMap[OIK_RETURN]				= KEY_RETURN;  /**< key_return */
    mKeyMap[OIK_PAUSE]				= KEY_UNKNOWN;  /**< key_pause */
    mKeyMap[OIK_ESC]				= KEY_ESCAPE;  /**< key_escape */
    mKeyMap[OIK_SPACE]				= KEY_SPACE;  /**< key_space */
    mKeyMap[OIK_EXCLAIM]			= KEY_UNKNOWN;  /**< key_exclaim*/
    mKeyMap[OIK_QUOTEDBL]			= KEY_UNKNOWN;  /**< key_dblquote */
    mKeyMap[OIK_HASH]				= KEY_UNKNOWN;  /**< key_hash */
    mKeyMap[OIK_DOLLAR]				= KEY_UNKNOWN;  /**< key_dollar */
    mKeyMap[OIK_AMPERSAND]			= KEY_UNKNOWN;  /**< key_and */
    mKeyMap[OIK_QUOTE]				= KEY_UNKNOWN;  /**< key_quote */
    mKeyMap[OIK_LEFTPAREN]			= KEY_UNKNOWN;  /**< key_leftparan */
    mKeyMap[OIK_RIGHTPAREN]			= KEY_UNKNOWN;  /**< key_rightparan */
    mKeyMap[OIK_ASTERISK]			= KEY_UNKNOWN;  /**< key_asterisk */
    mKeyMap[OIK_PLUS]				= KEY_UNKNOWN;  /**< key_plus */
    mKeyMap[OIK_COMMA]				= KEY_UNKNOWN;  /**< key_comma */
    mKeyMap[OIK_MINUS]				= KEY_MINUS;  /**< key_minus */
    mKeyMap[OIK_PERIOD]				= KEY_UNKNOWN;  /**< key_period */
    mKeyMap[OIK_SLASH]				= KEY_SLASH;  /**< key_slash */
    mKeyMap[OIK_0]					= KEY_0;  /**< key_0 */
    mKeyMap[OIK_1]					= KEY_1;  /**< key_1 */
    mKeyMap[OIK_2]					= KEY_2;  /**< key_2 */
    mKeyMap[OIK_3]					= KEY_3;  /**< key_3 */
    mKeyMap[OIK_4]					= KEY_4;  /**< key_4 */
    mKeyMap[OIK_5]					= KEY_5;  /**< key_5 */
    mKeyMap[OIK_6]					= KEY_6;  /**< key_6 */
    mKeyMap[OIK_7]					= KEY_7;  /**< key_7 */
    mKeyMap[OIK_8]					= KEY_8;  /**< key_8 */
    mKeyMap[OIK_9]					= KEY_9;  /**< key_9 */
    mKeyMap[OIK_COLON]				= KEY_UNKNOWN;  /**< key_colon */
    mKeyMap[OIK_SEMICOLON]			= KEY_SEMICOLON;  /**< key_semicolon */
    mKeyMap[OIK_LESS]				= KEY_UNKNOWN;  /**< key_less */
	mKeyMap[OIK_EQUALS]				= KEY_EQUALS;  /**< key_equal */
    mKeyMap[OIK_GREATER]			= KEY_UNKNOWN;  /**< key_greater */
    mKeyMap[OIK_QUESTION]			= KEY_Q;  /**< key_question */
    mKeyMap[OIK_AT]					= KEY_AT;  /**< key_at */
    // Here we skip uppercase letters
    mKeyMap[OIK_LEFTBRACKET]		= KEY_LBRACKET;  /**< key_leftbracket */
	mKeyMap[OIK_BACKSLASH]			= KEY_BACKSLASH;  /**< key_backslash */
    mKeyMap[OIK_RIGHTBRACKET]		= KEY_RBRACKET;  /**< key_rightbracket */
    mKeyMap[OIK_CARET]				= KEY_UNKNOWN;  /**< key_caret */
    mKeyMap[OIK_UNDERSCORE]			= KEY_UNDERLINE;  /**< key_underscore */
    mKeyMap[OIK_BACKQUOTE]			= KEY_UNKNOWN;  /**< key_backquote */
    mKeyMap[OIK_A]					= KEY_A;  /**< key_a */
    mKeyMap[OIK_B]					= KEY_B;  /**< key_b */
    mKeyMap[OIK_C]					= KEY_C;  /**< key_c */
    mKeyMap[OIK_D]					= KEY_D; /**< key_d */
    mKeyMap[OIK_E]					= KEY_E; /**< key_e */
    mKeyMap[OIK_F]					= KEY_F; /**< key_f */
    mKeyMap[OIK_G]					= KEY_G; /**< key_g */
    mKeyMap[OIK_H]					= KEY_H; /**< key_h */
    mKeyMap[OIK_I]					= KEY_I; /**< key_i */
    mKeyMap[OIK_J]					= KEY_J; /**< key_j */
    mKeyMap[OIK_K]					= KEY_K; /**< key_k */
    mKeyMap[OIK_L]					= KEY_L; /**< key_l */
    mKeyMap[OIK_M]					= KEY_M; /**< key_m */
    mKeyMap[OIK_N]					= KEY_N; /**< key_n */
    mKeyMap[OIK_O]					= KEY_O; /**< key_o */
    mKeyMap[OIK_P]					= KEY_P; /**< key_p */
    mKeyMap[OIK_Q]					= KEY_Q; /**< key_q */
    mKeyMap[OIK_R]					= KEY_R; /**< key_r */
    mKeyMap[OIK_S]					= KEY_S; /**< key_s */
    mKeyMap[OIK_T]					= KEY_T; /**< key_t */
    mKeyMap[OIK_U]					= KEY_U; /**< key_u */
    mKeyMap[OIK_V]					= KEY_V; /**< key_v */
    mKeyMap[OIK_W]					= KEY_W; /**< key_w */
    mKeyMap[OIK_X]					= KEY_X; /**< key_x */
    mKeyMap[OIK_Y]					= KEY_Y; /**< key_y */
    mKeyMap[OIK_Z]					= KEY_Z; /**< key_z */
    mKeyMap[OIK_DELETE]				= KEY_DELETE; /**< key_delete */

    // Numeric keypad
	mKeyMap[OIK_N_0]				= KEY_NUMPAD0; /**< key_num_0 */
    mKeyMap[OIK_N_1]				= KEY_NUMPAD1; /**< key_num_1 */
    mKeyMap[OIK_N_2]				= KEY_NUMPAD2; /**< key_num_2 */
    mKeyMap[OIK_N_3]				= KEY_NUMPAD3; /**< key_num_3 */
    mKeyMap[OIK_N_4]				= KEY_NUMPAD4; /**< key_num_4 */
    mKeyMap[OIK_N_5]				= KEY_NUMPAD5; /**< key_num_5 */
    mKeyMap[OIK_N_6]				= KEY_NUMPAD6; /**< key_num_6 */
    mKeyMap[OIK_N_7]				= KEY_NUMPAD7; /**< key_num_7 */
    mKeyMap[OIK_N_8]				= KEY_NUMPAD8; /**< key_num_8 */
    mKeyMap[OIK_N_9]				= KEY_NUMPAD9; /**< key_num_9 */
    mKeyMap[OIK_N_PERIOD]			= KEY_NUMPADDECIMAL; /**< key_num_period */
    mKeyMap[OIK_N_DIVIDE]			= KEY_NUMPADDIVIDE; /**< key_num_divide */
	mKeyMap[OIK_N_MULTIPLY]			= KEY_NUMPADMULTIPLY; /**< key_num_multiply */
    mKeyMap[OIK_N_MINUS]			= KEY_NUMPADSUBTRACT; /**< key_num_minus */
    mKeyMap[OIK_N_PLUS]				= KEY_NUMPADADD; /**< key_num_plus */
    mKeyMap[OIK_N_ENTER]			= KEY_NUMPADENTER; /**< key_num_enter */
    mKeyMap[OIK_N_EQUALS]			= KEY_NUMPADEQUALS; /**< key_num_equals */

    // Arrows + home/end pad
    mKeyMap[OIK_UP]					= KEY_UP; /**< key_up */
	mKeyMap[OIK_DOWN]				= KEY_DOWN;		/**< key_down */
    mKeyMap[OIK_RIGHT]				= KEY_RIGHT;	/**< key_right */
    mKeyMap[OIK_LEFT]				= KEY_LEFT;		/**< key_left */
    mKeyMap[OIK_INSERT]				= KEY_INSERT;	/**< key_insert */
    mKeyMap[OIK_HOME]				= KEY_HOME;		/**< key_home */
	mKeyMap[OIK_END]				= KEY_END;		/**< key_end */
	mKeyMap[OIK_PAGEUP]				= KEY_PGUP;		/**< key_pageup */
	mKeyMap[OIK_PAGEDOWN]			= KEY_PGDOWN;	/**< key_pagedown */

    // Function keys
    mKeyMap[OIK_F1]					= KEY_F1; /**< key_f1 */
    mKeyMap[OIK_F2]					= KEY_F2; /**< key_f2 */
    mKeyMap[OIK_F3]					= KEY_F3; /**< key_f3 */
    mKeyMap[OIK_F4]					= KEY_F4; /**< key_f4 */
    mKeyMap[OIK_F5]					= KEY_F5; /**< key_f5 */
    mKeyMap[OIK_F6]					= KEY_F6; /**< key_f6 */
    mKeyMap[OIK_F7]					= KEY_F7; /**< key_f7 */
    mKeyMap[OIK_F8]					= KEY_F8; /**< key_f8 */
    mKeyMap[OIK_F9]					= KEY_F9; /**< key_f9 */
    mKeyMap[OIK_F10]				= KEY_F10; /**< key_f10 */
    mKeyMap[OIK_F11]				= KEY_F11; /**< key_f11 */
    mKeyMap[OIK_F12]				= KEY_F12; /**< key_f12 */
    mKeyMap[OIK_F13]				= KEY_F13; /**< key_f13 */
    mKeyMap[OIK_F14]				= KEY_F14; /**< key_f14 */
    mKeyMap[OIK_F15]				= KEY_F15; /**< key_f15 */

    // Key state modifiers
    mKeyMap[OIK_NUMLOCK]			= KEY_UNKNOWN; /**< key_numlock */
    mKeyMap[OIK_CAPSLOCK]			= KEY_UNKNOWN; /**< key_capslock */
    mKeyMap[OIK_SCROLLOCK]			= KEY_UNKNOWN; /**< key_scrolllock */
    mKeyMap[OIK_RSHIFT]				= KEY_UNKNOWN; /**< key_shift_right */
    mKeyMap[OIK_LSHIFT]				= KEY_UNKNOWN; /**< key_shift_left */
    mKeyMap[OIK_RCTRL]				= KEY_UNKNOWN; /**< key_control_right */
    mKeyMap[OIK_LCTRL]				= KEY_UNKNOWN; /**< key_control_left */
    mKeyMap[OIK_RALT]				= KEY_UNKNOWN; /**< key_alt_right */
    mKeyMap[OIK_LALT]				= KEY_UNKNOWN; /**< key_alt_left */
    mKeyMap[OIK_RMETA]				= KEY_UNKNOWN; /**< key_meta_right */
    mKeyMap[OIK_LMETA]				= KEY_UNKNOWN; /**< key_meta_left */
    mKeyMap[OIK_RWINDOWS]			= KEY_UNKNOWN; /**< key_windows_right */
    mKeyMap[OIK_LWINDOWS]			= KEY_UNKNOWN; /**< key_windows_left */
    mKeyMap[OIK_ALTGR]				= KEY_UNKNOWN; /**< key_altgr */
    mKeyMap[OIK_COMPOSE]			= KEY_UNKNOWN; /**< key_compose */

    // Miscellaneous function keys
	mKeyMap[OIK_HELP]				= KEY_UNKNOWN; /**< key_help */
    mKeyMap[OIK_PRINT]				= KEY_UNKNOWN; /**< key_print */
    mKeyMap[OIK_SYSREQ]				= KEY_UNKNOWN; /**< key_sysrq */
    mKeyMap[OIK_BREAK]				= KEY_UNKNOWN; /**< key_break */
    mKeyMap[OIK_MENU]				= KEY_UNKNOWN; /**< key_menu (windows menu key) */
    mKeyMap[OIK_POWER]				= KEY_UNKNOWN; /**< key_power (mac power key) */
    mKeyMap[OIK_EURO]				= KEY_UNKNOWN; /**< key_euro */
    mKeyMap[OIK_UNDO]				= KEY_UNKNOWN; /**< key_undo (atari undo key) */
}

void Win32Keyboard::uninitializeImpl() {}

void Win32Keyboard::updateImpl(float elapsedTime) {}

} // end namespace input
