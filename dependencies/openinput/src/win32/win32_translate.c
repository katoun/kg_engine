/*
 * win32_translate.c : Win32 keyboard translators
 *
 * This file is a part of the OpenInput library.
 * Copyright (C) 2005  Jakob Kjaer <makob@makob.dk>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* ******************************************************************** */

// Includes
#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "openinput.h"
#include "internal.h"
#include "win32.h"

// Globals
static oi_key win32_keymap[DW32_KEYTABLE];

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Initialize the Win32 VirtualKey to OpenInput keymap table
 *
 * To be able to translate from VKs to OIKs in constant time,
 * we intialize a simply lookup-table.
 */
void win32_initkeymap() {
    int i;

    // Default to unknown key
    for(i=0; i<DW32_KEYTABLE; i++) {
        win32_keymap[i] = OIK_UNKNOWN;
    }

    // Ok, here we go
    win32_keymap[VK_BACK]         = OIK_BACKSPACE;
    win32_keymap[VK_TAB]          = OIK_TAB;
    win32_keymap[VK_CLEAR]        = OIK_CLEAR;
    win32_keymap[VK_RETURN]       = OIK_RETURN;
    win32_keymap[VK_PAUSE]        = OIK_PAUSE;
    win32_keymap[VK_ESCAPE]       = OIK_ESC;
    win32_keymap[VK_SPACE]        = OIK_SPACE;
    win32_keymap[VK_APOSTROPHE]   = OIK_QUOTE;
    win32_keymap[VK_COMMA]        = OIK_COMMA;
    win32_keymap[VK_MINUS]        = OIK_MINUS;
    win32_keymap[VK_PERIOD]       = OIK_PERIOD;
    win32_keymap[VK_SLASH]        = OIK_SLASH;

    for(i='0'; i<='9'; i++) {
        win32_keymap[i] = OIK_0 + i - '0';
    }

    win32_keymap[VK_SEMICOLON]    = OIK_SEMICOLON;
    win32_keymap[VK_EQUALS]       = OIK_EQUALS;
    win32_keymap[VK_LBRACKET]     = OIK_LEFTBRACKET;
    win32_keymap[VK_BACKSLASH]    = OIK_BACKSLASH;
    win32_keymap[VK_RBRACKET]     = OIK_RIGHTBRACKET;
    win32_keymap[VK_GRAVE]        = OIK_BACKQUOTE;
    win32_keymap[VK_BACKTICK]     = OIK_BACKQUOTE;
    win32_keymap[VK_DELETE]       = OIK_DELETE;

    for(i='A'; i<='Z'; i++) {
        win32_keymap[i] = OIK_A + i - 'A';
    }

    win32_keymap[VK_NUMPAD0]      = OIK_N_0;
    win32_keymap[VK_NUMPAD1]      = OIK_N_1;
    win32_keymap[VK_NUMPAD2]      = OIK_N_2;
    win32_keymap[VK_NUMPAD3]      = OIK_N_3;
    win32_keymap[VK_NUMPAD4]      = OIK_N_4;
    win32_keymap[VK_NUMPAD5]      = OIK_N_5;
    win32_keymap[VK_NUMPAD6]      = OIK_N_6;
    win32_keymap[VK_NUMPAD7]      = OIK_N_7;
    win32_keymap[VK_NUMPAD8]      = OIK_N_8;
    win32_keymap[VK_NUMPAD9]      = OIK_N_9;
    win32_keymap[VK_DECIMAL]      = OIK_N_PERIOD;
    win32_keymap[VK_DIVIDE]       = OIK_N_DIVIDE;
    win32_keymap[VK_MULTIPLY]     = OIK_N_MULTIPLY;
    win32_keymap[VK_SUBTRACT]     = OIK_N_MINUS;
    win32_keymap[VK_ADD]          = OIK_N_PLUS;

    win32_keymap[VK_UP]           = OIK_UP;
    win32_keymap[VK_DOWN]         = OIK_DOWN;
    win32_keymap[VK_RIGHT]        = OIK_RIGHT;
    win32_keymap[VK_LEFT]         = OIK_LEFT;
    win32_keymap[VK_INSERT]       = OIK_INSERT;
    win32_keymap[VK_HOME]         = OIK_HOME;
    win32_keymap[VK_END]          = OIK_END;
    win32_keymap[VK_PRIOR]        = OIK_PAGEUP;
    win32_keymap[VK_NEXT]         = OIK_PAGEDOWN;

    win32_keymap[VK_F1]           = OIK_F1;
    win32_keymap[VK_F2]           = OIK_F2;
    win32_keymap[VK_F3]           = OIK_F3;
    win32_keymap[VK_F4]           = OIK_F4;
    win32_keymap[VK_F5]           = OIK_F5;
    win32_keymap[VK_F6]           = OIK_F6;
    win32_keymap[VK_F7]           = OIK_F7;
    win32_keymap[VK_F8]           = OIK_F8;
    win32_keymap[VK_F9]           = OIK_F9;
    win32_keymap[VK_F10]          = OIK_F10;
    win32_keymap[VK_F11]          = OIK_F11;
    win32_keymap[VK_F12]          = OIK_F12;
    win32_keymap[VK_F13]          = OIK_F13;
    win32_keymap[VK_F14]          = OIK_F14;
    win32_keymap[VK_F15]          = OIK_F15;

    win32_keymap[VK_NUMLOCK]      = OIK_NUMLOCK;
    win32_keymap[VK_CAPITAL]      = OIK_CAPSLOCK;
    win32_keymap[VK_SCROLL]       = OIK_SCROLLOCK;
    win32_keymap[VK_RSHIFT]       = OIK_RSHIFT;
    win32_keymap[VK_LSHIFT]       = OIK_LSHIFT;
    win32_keymap[VK_RCONTROL]     = OIK_RCTRL;
    win32_keymap[VK_LCONTROL]     = OIK_LCTRL;
    win32_keymap[VK_RMENU]        = OIK_RALT;
    win32_keymap[VK_LMENU]        = OIK_LALT;
    win32_keymap[VK_RWIN]         = OIK_RMETA;
    win32_keymap[VK_LWIN]         = OIK_LMETA;

    win32_keymap[VK_HELP]         = OIK_HELP;
    win32_keymap[VK_PRINT]        = OIK_PRINT;
    win32_keymap[VK_SNAPSHOT]     = OIK_PRINT;
    win32_keymap[VK_CANCEL]       = OIK_BREAK;
    win32_keymap[VK_APPS]         = OIK_MENU;
#ifdef VK_SLEEP
    win32_keymap[VK_SLEEP]        = OIK_POWER;
#endif
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Perform a full keyboard state update
 *
 * @param dev pointer to device structure
 *
 * If the application using OpenInput loses keyboard focus,
 * the state of the keyboard may have changed once we get
 * back the focus (both locks and regular keys). If that
 * happens, we must do a full sync between the OpenInput
 * state table and the system...
 */
void win32_keystate(oi_device *dev) {
    unsigned int mod;
    char *curstate;
    char keyboard[DW32_KEYTABLE];
    int i;
    oi_key key;

    debug("win32_keystate");

    // Fetch win32 keystate or bail
    if(!GetKeyboardState(keyboard)) {
        return;
    }

    // Get current OpenInput state
    curstate = oi_key_keystate(dev->index, NULL);
    if(!curstate) {
        return;
    }
    mod = OIM_NONE;

    // Parse all virtual keys
    for(i=0; i<DW32_KEYTABLE; i++) {

        // Only check known keys
        key = win32_keymap[i];
        if(key == OIK_UNKNOWN) {
            continue;
        }

        // Set state (0x81 catches both keys and lock activated bits)
        if(keyboard[i] & 0x81) {
            curstate[key] = TRUE;

            // Update modifiers (altgr is not a modifier under win32)
            switch(key) {
            case OIK_LSHIFT:
                mod |= OIM_LSHIFT;
                break;

            case OIK_RSHIFT:
                mod |= OIM_RSHIFT;
                break;

            case OIK_LCTRL:
                mod |= OIM_LCTRL;
                break;

            case OIK_RCTRL:
                mod |= OIM_RCTRL;
                break;

            case OIK_LALT:
                mod |= OIM_LALT;
                break;

            case OIK_RALT:
                mod |= OIM_RALT;
                break;

            case OIK_LMETA:
                mod |= OIM_LMETA;
                break;

            case OIK_RMETA:
                mod |= OIM_RMETA;
                break;

            case OIK_NUMLOCK:
                mod |= OIM_RMETA;
                break;

            case OIM_CAPSLOCK:
                mod |= OIM_CAPSLOCK;
                break;

            case OIM_SCROLLOCK:
                mod |= OIM_SCROLLOCK;
                break;

            default:
                break;
            }
        }

        // Key was up, clear state
        else {
            curstate[key] = FALSE;
        }
    }

    keyboard_setmodifier(dev->index, mod);
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Translate Win32 VK keysym to OpenInput keysym
 *
 * @param priv pointer to private structure
 * @param wparam raw message word param
 * @param lparam raw message long param
 * @param down true (1) if key pressed, false (0) otherwise
 * @param keysym OpenInput keysym to be filled
 * @returns pointer to the keysym parameter
 *
 * Here we translate from the Win32 VirtualKey to the
 * OpenInput keysym. We use the win32_keymap table so
 * it's quite fast (constant time).
 */
oi_keysym *win32_translate(win32_private *priv, WPARAM wparam, LPARAM lparam,
                                  char down, oi_keysym *keysym) {
    unsigned int vkey;

    // Find the virtual key
    switch(wparam) {

        // Control
    case VK_CONTROL:
        if(lparam & DW32_EXTENDMASK) {
            vkey = VK_RCONTROL;
        }
        else {
            vkey = VK_LCONTROL;
        }
        break;


        // Meta
    case VK_MENU:
        if(lparam & DW32_EXTENDMASK) {
            vkey = VK_RMENU;
        }
        else {
            vkey = VK_LMENU;
        }
        break;


        // Shift
    case VK_SHIFT:
        if(((down && !priv->shiftleft) || (!down && priv->shiftleft)) &&
           (GetKeyState(VK_LSHIFT) & 0x8000)) {
            vkey = VK_LSHIFT;
            priv->shiftleft = down;
        }
        else if(((down && !priv->shiftright) || (!down && priv->shiftright)) &&
                (GetKeyState(VK_LSHIFT) & 0x8000)) {
            vkey = VK_RSHIFT;
            priv->shiftright = down;
        }
		else {
			vkey = VK_SHIFT;
		}
        break;


        // Regular key
    default:
        vkey = wparam;
    }

    // The rest is trivial
    keysym->scancode = (unsigned char)HIWORD(lparam);

    // For some reason, the vkey might be out of range
    if((vkey < 0) || (vkey >= DW32_KEYTABLE)) {
        keysym->sym = OIK_UNKNOWN;
        debug("win32_translate: vkey out of range");
    }
    else {
        keysym->sym = win32_keymap[vkey];
    }
    keysym->mod = OIM_NONE;
    return keysym;
}

/* ******************************************************************** */
