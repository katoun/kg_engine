/*
 * appstate.c : Application state (focus/grab/cursor) interface
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
#include "openinput.h"
#include "internal.h"

// Globals
static oi_device *windowdev;
static unsigned int focus;
static oi_bool grab;
static oi_bool cursor;
static int win_width;
static int win_height;

/* ******************************************************************** */

/**
 * @ingroup IAppstate
 * @brief Initialize application state manager
 *
 * Must be called on library initialization.
 */
int appstate_init() {
    unsigned char i;

    // Ok, our focus is complete
    focus = OI_FOCUS_MOUSE | OI_FOCUS_INPUT | OI_FOCUS_VISIBLE;

    // Not grabbed, and cursor is visible
    grab = FALSE;
    cursor = TRUE;

    // Find default/first window device which we assume is "the root"
    i = 1;
    while((windowdev = device_get(i)) != NULL) {
        if((windowdev->provides & OI_PRO_WINDOW) == OI_PRO_WINDOW) {
            break;
        }
        i++;
    }

    // We really want a window
    if(windowdev == NULL) {
        return OI_ERR_NO_DEVICE;
    }

    // Get window metrics
    if(windowdev->winsize) {
        windowdev->winsize(windowdev, &win_width, &win_height);
    }
    else {
        // No winsize function, very bad
        return OI_ERR_NOT_IMPLEM;
    }

    debug("appstate_init: window device is '%s'", windowdev->name);

    // Done
    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup IAppstate
 * @brief Update focus state
 *
 * @param index device index
 * @param gain focus gained:1 or lost:0
 * @param state type of focus gained/lost
 * @param post true (1) to send event, false (0) otherwise
 *
 * Update the focus state of the application (ie. window).
 * Rhree types of focus exists: #OI_FOCUS_MOUSE #OI_FOCUS_INPUT and
 * #OI_FOCUS_VISIBLE
 */
void appstate_focus(unsigned char index, char gain, unsigned int state, char post) {
    unsigned int newfocus;

    // Loose or gain
    newfocus = focus;
    if(gain) {
        newfocus |= state;
    }
    else {
        newfocus = focus - (focus & state);
    }

    // If nothing changed, bail out
    if(newfocus == focus) {
        return;
    }

    // Store state
    focus = newfocus;

    // Postal services
    if(post) {
        oi_event ev;
        ev.type = OI_ACTIVE;
        ev.active.device = index;
        ev.active.gain = gain & TRUE;
        ev.active.state = focus;
        queue_add(&ev);
    }
}

/* ******************************************************************** */

/**
 * @ingroup IAppstate
 * @brief Update application window size state
 *
 * @param index device index
 * @param w width of window
 * @param h height of window
 * @param post true (1) to send event, false (0) otherwise
 *
 * Update the window size state.
 */
void appstate_resize(unsigned char index, int w, int h, char post) {
    // Store state
    win_width = w;
    win_height = h;

    // Postal services
    if(post) {
        oi_event ev;
        ev.type = OI_RESIZE;
        ev.resize.device = index;
        ev.resize.width = w;
        ev.resize.height = h;
        queue_add(&ev);
    }
}

/* ******************************************************************** */

/**
 * @ingroup IAppstate
 * @brief Get application window width
 *
 * @returns window width (pixels)
 */
int appstate_width() {
    return win_width;
}

/* ******************************************************************** */

/**
 * @ingroup IAppstate
 * @brief Get application window height
 *
 * @returns window height (pixels)
 */
int appstate_height() {
    return win_height;
}

/* ******************************************************************** */

/**
 * @ingroup PAppstate
 * @brief Get application focus state
 *
 * @returns focus bitmask
 *
 * A bitmask comprised of #OI_FOCUS_MOUSE #OI_FOCUS_INPUT or
 * #OI_FOCUS_VISIBLE
 */
unsigned int oi_app_focus() {
    return focus;
}

/* ******************************************************************** */

/**
 * @ingroup PAppstate
 * @brief Hide or display the mouse cursor
 *
 * @param q new cursor state or query, see @ref PBool
 * @returns state of cursor visiblity, OI_QUERY on error
 *
 * Show or hide the mouse cursor. The state can also
 * be queried using #OI_QUERY
 */
oi_bool oi_app_cursor(oi_bool q) {
    char hide;
    unsigned char i;
    oi_device *dev;

    switch(q) {
    case OI_ENABLE:
        hide = FALSE;
        break;

    case OI_DISABLE:
        hide = TRUE;
        break;

    case OI_QUERY:
        if(cursor) {
            return OI_ENABLE;
        }
        else {
            return OI_DISABLE;
        }
    }

    // Set cursor mode on all devices
    i = 1;
    while((dev = device_get(i)) != NULL) {
        // Hide is an optional function
        if(dev->hide) {
            dev->hide(dev, hide);
        }

        // When unhiding, warp the mouse due to fudge
        if(!hide && dev->warp) {
            int x;
            int y;
            oi_mouse_absolute(0, &x, &y);
            dev->warp(dev, x, y);
        }
        i++;
    }

    // Remember mode
    cursor = hide;
    return q;
}

/* ******************************************************************** */

/**
 * @ingroup PAppstate
 * @brief Grab or release the mouse cursor
 *
 * @param q enable/disable/query grabbing, see @ref PBool
 * @returns state of grabbing, OI_QUERY on error
 *
 * Grab or release the mouse cursor. The grab state can
 * also be queried using #OI_QUERY
 */
oi_bool oi_app_grab(oi_bool q) {
    char eat;
    unsigned char i;
    oi_device *dev;

    switch(q) {
    case OI_ENABLE:
        eat = TRUE;
        break;

    case OI_DISABLE:
        eat = FALSE;
        break;

    case OI_QUERY:
        if(grab) {
            return OI_ENABLE;
        }
        else {
            return OI_DISABLE;
        }
    }

    // Set cursor mode on all devices
    i = 1;
    while((dev = device_get(i)) != NULL) {
        // Grab is an optional function
        if(dev->grab) {
            dev->grab(dev, eat);
        }
        i++;
    }

    // Remember mode
    grab = eat;
    return q;
}

/* ******************************************************************** */
