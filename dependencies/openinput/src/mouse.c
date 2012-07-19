/*
 * mouse.c : Mouse button and movement state interface
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
#include <string.h>
#include <stdlib.h>
#include "openinput.h"
#include "internal.h"
#include "private.h"

/* ******************************************************************** */

/**
 * @ingroup IMouse
 * @brief Initialize mouse state manager
 *
 * @returns errorcode, see @ref PErrors
 *
 * Called on library initialization. The function prepares
 * the mouse manager for use.
 */
int mouse_init() {
    debug("mouse_init");

    // Done
    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup IMouse
 * @brief Allocate and prepare private managment data
 *
 * @param mouse pointer to the address of the private mouse data
 * @param provide provide mask, see @ref PProvide
 *
 * @pre This function is called during "device_register" where
 * the base device->managment structure has been allocated and
 * initialized
 *
 * This function (possible) allocates and initializes the mouse
 * per-device private managment data, ie. absolute and relative
 * motion of the mouse and the button state
 *
 * The structure might also not be created, depending on whether
 * the device provides a mice as determined by the provide-mask
 */
void mouse_manage(oi_privmouse **mouse, unsigned int provide) {
    // Only care about keyboard
    if(!(provide & OI_PRO_MOUSE)) {
        return;
    }

    // Allocate
    *mouse = (oi_privmouse*)malloc(sizeof(oi_privmouse));

    // Clear state
    (*mouse)->button = OI_BUTTON_MASK(OIP_UNKNOWN);
    (*mouse)->absx = 0;
    (*mouse)->absy = 0;
    (*mouse)->relx = 0;
    (*mouse)->rely = 0;

    debug("mouse_manage: manager data installed");
}

/* ******************************************************************** */

/**
 * @ingroup IMouse
 * @brief Mouse motion update
 *
 * @param index device index
 * @param x horizontal position/movement
 * @param y vertical position/movement
 * @param relative false (0) if x,y is absolute coordinate, true
 *   if x,y is relative movement
 * @param post trur (1) to send event, false (0) otherwise
 *
 * Inject absolute and relative mouse movement into the mouse
 * state manager. The x,y parameters should both be in pixels.
 *
 * Device drivers which resembles pointer devices probably wants
 * to use this function to inject events into the library.
 */
void mouse_move(unsigned char index, int x, int y, char relative, char post) {
    oi_privmouse *priv;
    int nx;
    int ny;
    int rx;
    int ry;

    // Get private per-device data
    priv = (oi_privmouse*)device_priv(index, OI_PRO_MOUSE);
    if(!priv) {
        return;
    }

    // Always make x,y absolute
    rx = x;
    ry = y;
    if(relative) {
        x = priv->absx + x;
        y = priv->absy + y;
    }

    // Check absolute position of mouse wrt. window size
    if(x < 0) {
        nx = 0;
    }
    else if(x >= appstate_width()) {
        nx = appstate_width()-1;
    }
    else {
        nx = x;
    }

    if(y < 0) {
        ny = 0;
    }
    else if(y >= appstate_height()) {
        ny = appstate_height()-1;
    }
    else {
        ny = y;
    }

    // Calculate relative motion, but only if inside window
    if(!relative && (priv->absx >= 0) && (priv->absy >= 0)) {
        rx = nx - priv->absx;
        ry = ny - priv->absy;
    }

    // Ok, everything calculated - set global state
    priv->absx = nx;
    priv->absy = ny;
    priv->relx += rx;
    priv->rely += ry;

    // Postal services
    if(post) {
        oi_event ev;
        ev.type = OI_MOUSEMOVE;
        ev.move.device = index;
        ev.move.state = priv->button;
        ev.move.x = priv->absx;
        ev.move.y = priv->absy;
        ev.move.relx = rx;
        ev.move.rely = ry;

        queue_add(&ev);
    }
}

/* ******************************************************************** */

/**
 * @ingroup IMouse
 * @brief Mouse button update
 *
 * @param index device index
 * @param btn button index
 * @param down true (1) if button is pressed false (0) otherwise
 * @param post true (1) to send event, false (0) otherwise
 *
 * Feed mouse button press/release into mouse state manager.
 */
void mouse_button(unsigned char index, oi_mouse btn, char down, char post) {
    oi_privmouse *priv;
    oi_mouse newbutton;
    unsigned char type;

    // Get per-device private data
    priv = (oi_privmouse*)device_priv(index, OI_PRO_MOUSE);
    if(!priv) {
        return;
    }

    newbutton = priv->button;

    if(down) {
        // Down
        type = OI_MOUSEBUTTONDOWN;
        newbutton |= OI_BUTTON_MASK(btn);
    }
    else {
        // Up
        type = OI_MOUSEBUTTONUP;
        newbutton = newbutton - (newbutton & OI_BUTTON_MASK(btn));
    }

    // If nothing changed, bail out
    if(newbutton == priv->button) {
        return;
    }

    // Store state
    priv->button = newbutton;

    // Postal services
    if(post) {
        oi_event ev;
        ev.type = type;
        ev.button.device = index;
        ev.button.button = btn;
        ev.button.state = priv->button;
        ev.button.x = priv->absx;
        ev.button.y = priv->absy;
        queue_add(&ev);
    }
}

/* ******************************************************************** */

/**
 * @ingroup PMouse
 * @brief Get absolute position of mouse pointer
 *
 * @param index device index, 0 for default mouse
 * @param x pointer to horizontal position
 * @param y pointer to vertical position
 * @returns mouse button mask, see @ref PMouseMask
 *
 * Get the absolute position of the mouse cursor
 * (stored in the integer pointers) and the current
 * button state as a button mask.
 */
int oi_mouse_absolute(unsigned char index, int *x, int *y) {
    oi_privmouse *priv;
    unsigned char i;

    // Default coordinates
    if(x) {
        *x = 0;
    }
    if(y) {
        *y = 0;
    }

    // Get device index
    i = index;
    if(i == 0) {
        for(i=1; i<OI_MAX_DEVICES; i++) {
            if(device_priv(i, OI_PRO_MOUSE)) {
                break;
            }
        }

        // End reached
        if(i==OI_MAX_DEVICES) {
            return OIP_UNKNOWN;
        }
    }

    // Get device data
    priv = (oi_privmouse*)device_priv(i, OI_PRO_MOUSE);
    if(!priv) {
        return OIP_UNKNOWN;
    }

    // Check current position
    if(priv->absx < 0) {
        priv->absx = 0;
    }
    if(priv->absy < 0) {
        priv->absy = 0;
    }

    // Set data
    if(x) {
        *x = priv->absx;
    }
    if(y) {
        *y = priv->absy;
    }

    // Return button state
    return priv->button;
}

/* ******************************************************************** */

/**
 * @ingroup PMouse
 * @brief Get relative mouse motion
 *
 * @param index device index, 0 for default mouse
 * @param x pointer to horizontal motion
 * @param y pointer to vertical motion
 * @returns mouse button mask, see @ref PMouseMask
 *
 * Get the relative mouse motion since last call
 * to this function (ie. it is cummulative).
 */
int oi_mouse_relative(unsigned char index, int *x, int *y) {
    oi_privmouse *priv;
    unsigned char i;

    // Default coordinates
    if(x) {
        *x = 0;
    }
    if(y) {
        *y = 0;
    }

    // Get device index
    i = index;
    if(i == 0) {
        for(i=1; i<OI_MAX_DEVICES; i++) {
            if(device_priv(i, OI_PRO_MOUSE)) {
                break;
            }
        }

        // End reached
        if(i==OI_MAX_DEVICES) {
            return OIP_UNKNOWN;
        }
    }

    // Get device data
    priv = (oi_privmouse*)device_priv(i, OI_PRO_MOUSE);
    if(!priv) {
        return OIP_UNKNOWN;
    }

    // Set data
    if(x) {
        *x = priv->relx;
    }
    if(y) {
        *y = priv->rely;
    }

    // Reset deltas
    priv->relx = 0;
    priv->rely = 0;

    // Return button state
    return priv->button;
}

/* ******************************************************************** */

/**
 * @ingroup PMouse
 * @brief Warp (move) mouse pointer
 *
 * @param index device index, use 0 to warp all mice
 * @param x absolute horizontal position of cursor
 * @param y absolute vertical position of cursor
 * @returns errorcode, see @ref PErrors
 *
 * Move the mouse cursor to the defined absolute
 * position. The function generates a mouse
 * movement event.
 */
int oi_mouse_warp(unsigned char index, int x, int y) {
    oi_device *dev;
    unsigned char i;
    int e;

    // Clip coordinates
    if(x < 0) {
        x = 0;
    }
    else if(x > appstate_width()) {
        x = appstate_width() -1;
    }
    if(y < 0) {
        y = 0;
    }
    else if(y > appstate_height()) {
        y = appstate_height() - 1;
    }
    e = OI_ERR_NO_DEVICE;

    // Parse all devices
    for(i=1; i<OI_MAX_DEVICES; i++) {

        // Only touch a single device?
        if(index != 0) {
            i = index;
        }

        // Get device
        dev = device_get(i);
        if(dev && (dev->provides & OI_PRO_MOUSE)) {

            // If mouse is hidden and grabbed, don't physically move it
            if((oi_app_cursor(OI_QUERY) == OI_DISABLE) &&
               (oi_app_grab(OI_QUERY) == OI_ENABLE)) {
                mouse_move(i, x, y, FALSE, FALSE);
            }

            // Warp default mouse device - driver must generate motion event!
            else if(dev->warp) {
                e = dev->warp(dev, x, y);
            }
        }

        // Only touch a single device?
        if(index != 0) {
            return e;
        }
    }

    // Done, return whatever
    return e;
}

/* ******************************************************************** */

/**
 * @ingroup PMouse
 * @brief Return string from event-code
 *
 * @param button Button or mouse motion code
 * @returns string containing button or motion name
 *
 * Get the "symbolic" name for a mouse event like
 * buttons and motion.
 */
char *oi_mouse_getname(oi_mouse button) {
    // Dead simple
    switch(button) {
    case OIP_BUTTON_LEFT:
        return "mouse_button_left";

    case OIP_BUTTON_RIGHT:
        return "mouse_button_right";

    case OIP_BUTTON_MIDDLE:
        return "mouse_button_middle";

    case OIP_WHEEL_UP:
        return "mouse_wheel_up";

    case OIP_WHEEL_DOWN:
        return "mouse_wheel_down";

    case OIP_MOTION:
        return "mouse_motion";

    default:
        return "mouse_unknown";
    }
}

/* ******************************************************************** */

/**
 * @ingroup PMouse
 * @brief Get mouse button/motion code given string
 *
 * @param name string name of button/motion
 * @returns mouse button/motion code
 *
 * Translate symbolic string into mouse code.
 */
oi_mouse oi_mouse_getcode(char *name) {
    // Dummies
    if(!name) {
        return OIP_UNKNOWN;
    }
    if((strlen(name) < OI_MIN_KEYLENGTH) ||
       (strlen(name) > OI_MAX_KEYLENGTH)) {
        return OIP_UNKNOWN;
    }

    // Check prefix
    if(strncmp(name, "mouse_", 6) != 0) {
        return OIP_UNKNOWN;
    }

    // Do the comparisons
    if(strcmp(name, "mouse_button_left") == 0) {
        return OIP_BUTTON_LEFT;
    }
    if(strcmp(name, "mouse_button_right") == 0) {
        return OIP_BUTTON_RIGHT;
    }
    if(strcmp(name, "mouse_button_middle") == 0) {
        return OIP_BUTTON_MIDDLE;
    }
    if(strcmp(name, "mouse_wheel_up") == 0) {
        return OIP_WHEEL_UP;
    }
    if(strcmp(name, "mouse_wheel_down") == 0) {
        return OIP_WHEEL_DOWN;
    }
    if(strcmp(name, "mouse_motion") == 0) {
        return OIP_MOTION;
    }

    // We shouldn't reach this point
    return OIP_UNKNOWN;
}

/* ******************************************************************** */
