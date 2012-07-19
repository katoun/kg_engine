/*
 * keyboard.c : Keyboard state interface
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "openinput.h"
#include "internal.h"
#include "private.h"

// Globals
static char *keynames[OIK_LAST];
static int rep_interval;
static int rep_delay;

/* ******************************************************************** */

/**
 * @ingroup IKeyboard
 * @brief Initialize keyboard state manager
 *
 * @returns errorcode, see @ref PErrors
 *
 * Must be called on library initialization.
 */
int keyboard_init() {
    // Disable key-repeat
    oi_key_repeat(0, 0);

    // Fill keyboard names
    keyboard_fillnames(keynames);

    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup IKeyboard
 * @brief Allocate and prepare private managment data
 *
 * @param key pointer to the address of the keyboard data
 * @param provide provide mask, see @ref PProvide
 *
 * @pre This function is called during "device_register" where
 * the base device->managment structure has been allocated and
 * initialized
 *
 * This function (possible) allocates and initializes the keyboard
 * per-device private managment data, ie. the modifier and button-down
 * state tables.
 *
 * The structure might also not be created, depending on whether
 * the device provides keyboard as determined by the provide-mask
 */
void keyboard_manage(oi_privkey **key, unsigned int provide) {
    // Only care about keyboard
    if(!(provide & OI_PRO_KEYBOARD)) {
        return;
    }

    // Allocate
    *key = (oi_privkey*)malloc(sizeof(oi_privkey));

    // Clear state
    memset((*key)->keystate, FALSE, TABLESIZE((*key)->keystate));
    (*key)->modstate = OIM_NONE;
    (*key)->rep_first = FALSE;
    (*key)->rep_time = 0;

    debug("keyboard_manage: manager data installed");
}

/* ******************************************************************** */

/**
 * @ingroup IKeyboard
 * @brief Update keyboard button state
 *
 * @param index device index
 * @param keysym keyboard symbol structure - copied
 * @param down true (1) on button press, false (0) otherwise
 * @param post true (1) to post event, false (0) otherwise
 *
 * Device drivers should use this function to generate keyboard
 * events and update the internal keyboard state table.
 * The function correctly translates modifier buttons into
 * modifier-masks and handles the lock-button: CapsLock, NumLock
 * and ScrollLock.
 *
 * Note that the keysym parameter is copied by this function, so it is
 * ok to give pointers to a local function variable.
 */
void keyboard_update(unsigned char index, oi_keysym *keysym, char down, char post) {
    oi_privkey *priv;
    oi_type type;
    oi_event ev;
    unsigned int newmod;
    char repeat;

    // Get private per-device data
    priv = (oi_privkey*)device_priv(index, OI_PRO_KEYBOARD);
    if(!priv) {
        return;
    }

    // Temporary new modifier state
    newmod = priv->modstate;
    repeat = FALSE;

    // Check for modifier updates
    if(down) {
        // Add modifiers to the event
        keysym->mod = newmod;

        // Handle each modifier-key
        switch(keysym->sym) {

        case OIK_NUMLOCK:
            // Locks are special
            newmod ^= OIM_NUMLOCK;
            if(!(newmod & OIM_NUMLOCK)) {
                down = FALSE;
            }
            keysym->mod = newmod;
            break;

        case OIK_CAPSLOCK:
            // Locks are special
            newmod ^= OIM_CAPSLOCK;
            if(!(newmod & OIM_CAPSLOCK)) {
                down = FALSE;
            }
            keysym->mod = newmod;
            break;

        case OIK_SCROLLOCK:
            // Locks are special
            newmod ^= OIM_SCROLLOCK;
            if(!(newmod & OIM_SCROLLOCK)) {
                down = FALSE;
            }
            keysym->mod = newmod;
            break;

        case OIK_RSHIFT:
            newmod |= OIM_RSHIFT;
            break;

        case OIK_LSHIFT:
            newmod |= OIM_LSHIFT;
            break;

        case OIK_RCTRL:
            newmod |= OIM_RCTRL;
            break;

        case OIK_LCTRL:
            newmod |= OIM_LCTRL;
            break;

        case OIK_RALT:
            newmod |= OIM_RALT;
            break;

        case OIK_LALT:
            newmod |= OIM_LALT;
            break;

        case OIK_RMETA:
            newmod |= OIM_RMETA;
            break;

        case OIK_LMETA:
            newmod |= OIM_LMETA;
            break;

        case OIK_ALTGR:
            newmod |= OIM_ALTGR;
            break;

        default:
            // Not a modifier and key down - we can do repeats ;)
            repeat = TRUE;
            break;
        }
    }
    else {
        // Handle each modifier-key
        switch(keysym->sym) {

        case OIK_NUMLOCK:
        case OIK_CAPSLOCK:
        case OIK_SCROLLOCK:
            // Only send down-events on num and capslock
            return;

        case OIK_RSHIFT:
            newmod &= ~OIM_RSHIFT;
            break;

        case OIK_LSHIFT:
            newmod &= ~OIM_LSHIFT;
            break;

        case OIK_RCTRL:
            newmod &= ~OIM_RCTRL;
            break;

        case OIK_LCTRL:
            newmod &= ~OIM_LCTRL;
            break;

        case OIK_RALT:
            newmod &= ~OIM_RALT;
            break;

        case OIK_LALT:
            newmod &= ~OIM_LALT;
            break;

        case OIK_RMETA:
            newmod &= ~OIM_RMETA;
            break;

        case OIK_LMETA:
            newmod &= ~OIM_LMETA;
            break;

        case OIK_ALTGR:
            newmod &= ~OIM_ALTGR;
            break;

        default:
            break;
        }

        // We're removing modifiers, store event mod
        keysym->mod = newmod;
    }

    // Handle down and up type event
    if(down) {
        type = OI_KEYDOWN;
    }
    else {
        type = OI_KEYUP;

        // Disable repeat if key matches
        if(priv->rep_time &&
           (priv->rep_ev.key.keysym.sym == keysym->sym)) {
            priv->rep_time = 0;
        }
    }

    // If key state didn't change, bail out
    if(priv->keystate[keysym->sym] == down) {
        return;
    }

    // Store new states
    priv->keystate[keysym->sym] = down;
    priv->modstate = newmod;

    // Setup event
    ev.type = type;
    ev.key.device = index;
    ev.key.keysym = *keysym;

    // Update key-repeat if enabled and repeatable key
    if(repeat && (rep_delay > 0)) {
        priv->rep_ev = ev;
        priv->rep_first = TRUE;
        priv->rep_time = oi_getticks();
    }

    // Postal services
    if(post) {
        queue_add(&ev);
    }
}

/* ******************************************************************** */

/**
 * @ingroup IKeyboard
 * @brief Repeat keyboard events
 *
 * This function generates keyboard events depending, ie.
 * either pressed/released events. The user must setup
 * the delay and interval settings before events are
 * posted.
 *
 * The function is called by oi_events_pump, and
 * should not be invoked from elsewhere
 */
void keyboard_dorepeat() {
    unsigned char i;
    oi_privkey *priv;
    unsigned int now;
    int interval;

    // Perform repeating for all keyboards
    for(i=0; i<OI_MAX_DEVICES; i++) {

        // Speed-checking of keyboard
        priv = (oi_privkey*)device_priv(i, OI_PRO_KEYBOARD);
        if(priv) {

            // Available?
            if(priv->rep_time) {
                now = oi_getticks();
                interval = now - priv->rep_time;

                // New keypress?
                if(priv->rep_first) {

                    // Wait for delay to expire
                    if(interval > rep_delay) {
                        priv->rep_first = FALSE;
                        priv->rep_time = now;
                    }
                }

                // Really, do a repeat by sending an event
                else if(interval > rep_interval) {
                    priv->rep_time = now;
                    queue_add(&priv->rep_ev);
                }
            }
            // End timestamp

        }
        // End valid priv

    }
    // End for-loop
}

/* ******************************************************************** */

/**
 * @ingroup IKeyboard
 * @brief Set keyboard modifier
 *
 * @param index device index
 * @param newmod modifier mask
 *
 * Set new keyboard modifier mask.
 */
void keyboard_setmodifier(unsigned char index, unsigned int newmod) {
    oi_privkey *priv;

    // Only set modifier for known keyboard
    priv = (oi_privkey*)device_priv(index, OI_PRO_KEYBOARD);
    if(priv) {
        priv->modstate = newmod;
    }
}

/* ******************************************************************** */

/**
 * @ingroup PKeyboard
 * @brief Get keyboard modifier state
 *
 * @param index device index, 0 for default keyboard
 * @returns modifier mask
 *
 * Return the current keyboard modifier state, ie.
 * which modifiers (alt, shift, meta, etc.) are down.
 * See @ref PModname for modifier definitions.
 */
unsigned int oi_key_modstate(unsigned char index) {
    oi_privkey *priv;
    unsigned char i;

    // Find first keyboard if index is zero
    i = index;
    if(i == 0) {
        for(i=1; i<OI_MAX_DEVICES; i++) {
            if(device_priv(index, OI_PRO_KEYBOARD)) {
                break;
            }
        }

        // No keyboard found
        return OIM_NONE;
    }

    // Get private data, gracefull value return
    priv = (oi_privkey*)device_priv(i, OI_PRO_KEYBOARD);
    if(priv) {
        return priv->modstate;
    }
    else {
        return OIM_NONE;
    }
}

/* ******************************************************************** */

/**
 * @ingroup PKeyboard
 * @brief Get key state table
 *
 * @param index device index, 0 for default keyboard
 * @param num pointer to size of table
 * @returns pointer to keyboard state table or NULL if invalid device
 *
 * Return the internal keyboard state table, which can be
 * parsed to determine which keys are down using the keycode
 * as array index (eg. table[OIK_A]).
 *
 * Note that the returned table is internal for OpenInput, and
 * you may NOT free or alter it. If the "num" parameter is not
 * NULL, it will be filled with the number of available elements
 * in the state table.
 */
char *oi_key_keystate(unsigned char index, int *num) {
    oi_privkey *priv;
    unsigned char i;

    // Find first keyboard if index is zero
    i = index;
    if(i == 0) {
        for(i=1; i<OI_MAX_DEVICES; i++) {
            if(device_priv(index, OI_PRO_KEYBOARD)) {
                break;
            }
        }

        // No keyboard found
        return NULL;
    }

    // Set number of keys
    if(num != NULL) {
        *num = OIK_LAST;
    }

    // Get private data, gracefull value return
    priv = (oi_privkey*)device_priv(i, OI_PRO_KEYBOARD);
    if(priv) {
        return priv->keystate;
    }
    else {
        return NULL;
    }
}

/* ******************************************************************** */

/**
 * @ingroup PKeyboard
 * @brief Return string name for keycode
 *
 * @param key keycode
 * @returns string containing button name
 *
 * Get the "symbolic" keyboard button name given keycode.
 * You can use this in configuration files and other
 * user-semi-friendly environments.
 */
char *oi_key_getname(oi_key key) {
    // Simply boundary check
    if((key > OIK_FIRST) && (key < OIK_LAST)) {
        return keynames[key];
    }

    // Out of bounds
    return keynames[OIK_UNKNOWN];
}

/* ******************************************************************** */

/**
 * @ingroup IKeyboard
 * @brief Keycode to keyname translation for special keys
 *
 * @param name string containing key name
 * @param first keycode index to start search
 * @param last keycode index to end search
 * @returns keycode
 *
 * Scan the range between (both inclusive) "first" and "last"
 * for the key named "name", and return the keycode.
 *
 * This function should be used only when other, faster, means
 * are exhausted (such as calculation of offset).
 *
 * If no name matches, the keycode returned is OIK_UNKNOWN.
 */
oi_key keyboard_scangetkey(char *name, oi_key first, oi_key last) {
    oi_key k;

    for(k=first; k<=last; k++) {
        if(strcmp(name, oi_key_getname(k)) == 0) {
            return k;
        }
    }
    return OIK_UNKNOWN;
}

/* ******************************************************************** */

/**
 * @ingroup PKeyboard
 * @brief Get keycode for button string
 *
 * @param name string name of button
 * @returns keycode
 *
 * Perform a lookup of the keyboard button with "name" and
 * return the OpenInput keycode. You should not use this in
 * time critical regions, as some keycodes are found using
 * string comparisons (which is time-consuming).
 *
 * This function may be usable when converting from
 * user-semi-friendly configuration files to something which
 * can be used inside your application.
 *
 * If the name is not a known keyboard button name
 * OIK_UNKNOWN is returned.
 */
oi_key oi_key_getcode(char *name) {
    int i;
    oi_key k;

    // Dummies
    if(!name) {
        return OIK_UNKNOWN;
    }
    if((strlen(name) < OI_MIN_KEYLENGTH) ||
       (strlen(name) > OI_MAX_KEYLENGTH)) {
        return OIK_UNKNOWN;
    }

    // Check prefix
    if(strncmp(name, "key_", 4) != 0) {
        return OIK_UNKNOWN;
    }

    // Catch this one now
    if(strcmp(name, "key_unknown") == 0) {
        return OIK_UNKNOWN;
    }

    // Just to be sure...
    k = OIK_UNKNOWN;

    // Letter or digit
    if(strlen(name) == 5) {
        if((name[4] >= 'a') && (name[4] <= 'z')) {
            return OIK_A + (name[4]-'a');
        }
        if((name[4] >= '0') && (name[4] <= '9')) {
            return OIK_0 + (name[4]-'0');
        }

        return OIK_UNKNOWN;
    }

    // Function keys (only these and 'f' starts with 'f')
    if(name[4] == 'f') {
        i = atoi(name+5);
        if((i>=1) && (i<=15)) {
            return OIK_F1 + i - 1;
        }

        return OIK_UNKNOWN;
    }

    // International
    if(strncmp(name, "key_int", 7) == 0) {
        i = atoi(name+7);
        if((i>=0) && (i<=95)) {
            return OIK_INT_0 + i;
        }

        return OIK_UNKNOWN;
    }

    // Numeric keypad non-numbers (num_period -> num_equals)
    if((k = keyboard_scangetkey(name, OIK_N_PERIOD, OIK_N_EQUALS)) != OIK_UNKNOWN) {
        return k;
    }

    // Numeric keypad numbers
    if(strncmp(name, "key_num_", 8) == 0) {
        i = atoi(name+8);
        if((i>=0) && (i<=9)) {
            return OIK_N_0 + i;
        }
    }

    // Backspace -> Slash
    if((k = keyboard_scangetkey(name, OIK_BACKSPACE, OIK_SLASH)) != OIK_UNKNOWN) {
        return k;
    }

    // Colon -> Backquote
    if((k = keyboard_scangetkey(name, OIK_COLON, OIK_BACKQUOTE)) != OIK_UNKNOWN) {
        return k;
    }

    // Delete
    if(strcmp(name, "key_delete") == 0) {
        return OIK_DELETE;
    }

    // Up -> pagedown
    if((k = keyboard_scangetkey(name, OIK_UP, OIK_PAGEDOWN)) != OIK_UNKNOWN) {
        return k;
    }

    // Numlock -> undo
    if((k = keyboard_scangetkey(name, OIK_NUMLOCK, OIK_UNDO)) != OIK_UNKNOWN) {
        return k;
    }

    // No more keys left
    return OIK_UNKNOWN;
}

/* ******************************************************************** */

/**
 * @ingroup PKeyboard
 * @brief Setup key repeater
 *
 * @param delay delay in ms for first event
 * @param interval interval in ms between events
 * @returns errorcode, see @ref PErrors
 *
 * Set "delay" and "interval" to a number higer than
 * zero to enable generation of key-repeat events (which
 * are simply press/release events when a button is held
 * pressed). To disable the repeat-system, set both parameters
 * to 0 (zero).
 *
 * Delay is the time a button must be held down for the
 * repeat-system to be started.
 *
 * When the delay has expired, an event will be triggered
 * based on the interval time.
 *
 * Both delay and interval are "ms" (1/1000 second).
 */
int oi_key_repeat(int delay, int interval) {
    oi_privkey *priv;
    unsigned char i;

    // Dummy check
    if((delay < 0) || (interval < 0)) {
        return OI_ERR_PARAM;
    }

    // Setup
    rep_delay = delay;
    rep_interval = interval;

    // Reset all keyboard repeat states
    for(i=1; i<OI_MAX_DEVICES; i++) {
        priv = (oi_privkey*)device_priv(i, OI_PRO_KEYBOARD);
        if(priv) {
            priv->rep_first = FALSE;
            priv->rep_time = 0;
        }
    }

    return OI_ERR_OK;
}

/* ******************************************************************** */
