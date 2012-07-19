/*
 * joystick.c : Generic joystick interface
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
static char *joynames[2][OIJ_LAST];

// Joystick string names
static char *joybases[] = {
    "joy_axis%u",
    "joy_button%u"
};

/* ******************************************************************** */

/**
 * @ingroup IJoystick
 * @brief Initialize joystick state manager
 *
 * @returns errorcode, see @ref PErrors
 *
 * Called on library initialization. The function prepares
 * the joystick manager for use.
 */
int joystick_init() {
    int i;
    char target[100];

    debug("joystick_init");

    // Fill symbolic joystick type class names
    for(i=0; i<OIJ_LAST; i++) {
        // Axes
        sprintf(target, joybases[OI_JOY_TAB_AXES], i);
        strcpy(joynames[OI_JOY_TAB_AXES][i], target);

        // Buttons
        sprintf(target ,joybases[OI_JOY_TAB_BTNS], i);
        strcpy(joynames[OI_JOY_TAB_BTNS][i], target);
    }

    // Done
    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup IJoystick
 * @brief Cleanup joystick manager
 *
 * @returns errorcode, see @ref PErrors
 *
 * This function must be called on library shutdown such that
 * the joystick symbolic name table can be freed.
 */
int joystick_close() {
    int i;

    // Free symbolic joystick type class names
    for(i=0; i<OIJ_LAST; i++) {
        free(joynames[OI_JOY_TAB_AXES][i]);
        free(joynames[OI_JOY_TAB_BTNS][i]);
    }

    // Done
    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup IJoystick
 * @brief Allocate and prepare private managment data
 *
 * @param joy pointer to the address of the joystick data
 * @param provide provide mask, see @ref PProvide
 *
 * @pre This function is called during "device_register" where
 * the base device->managment structure has been allocated and
 * initialized
 *
 * This function (possible) allocates and initializes the joystick
 * per-device private managment data, ie. axes positions and button
 * states
 *
 * The structure might also not be created, depending on whether
 * the device provides joystick as determined by the provide-mask
 */
void joystick_manage(oi_privjoy **joy, unsigned int provide) {
    // Only care about keyboard
    if(!(provide & OI_PRO_JOYSTICK)) {
        return;
    }

    // Allocate
    *joy = (oi_privjoy*)malloc(sizeof(oi_privjoy));

    // Clear states
    (*joy)->button = 0;
    memset((*joy)->relaxes, 0, TABLESIZE((*joy)->relaxes));
    memset((*joy)->absaxes, 0, TABLESIZE((*joy)->absaxes));
    memset((*joy)->insaxes, 0, TABLESIZE((*joy)->insaxes));
    memset((*joy)->update, 0, TABLESIZE((*joy)->update));

    debug("joystick_manage: manager data installed");
}

/* ******************************************************************** */

/**
 * @ingroup IJoystick
 * @brief Joystick axis update
 *
 * @param index device index
 * @param axis axis index
 * @param value value (position/motion) of axis
 * @param relative true or false, if set to OI_QUERY, set depending on axis type
 * @param post true (1) to post event, false (0) otherwise
 *
 * Send a joystick axis update into the library. This function
 * will handle the decoding of the axis index and determine
 * whether the axis is a stick-axes, hat, throttle, rudder
 * or trackball-axes.
 *
 * Note that this function does not inject events into the queue, but merely
 * tags exes for "update needed". This allows for the post-event generation
 * "joystick_pump" to combine multi-axes things (balls, sticks, etc.) into
 * a single event.
 */
void joystick_axis(unsigned char index, unsigned char axis, int value, oi_bool relative, char post) {
    oi_privjoy *priv;
    oi_joyconfig *conf;
    int corval;
    char rel;

    // Get private data or bail
    priv = device_priv(index, OI_PRO_JOYSTICK);
    if(!priv) {
        debug("joystick_axis: not a joystick");
        return;
    }

    // Get joystick config
    conf = device_get(index)->joyconfig;
    if(!conf) {
        debug("joystick_axis: no joystick config");
        return;
    }

    // Clip value
    if(value > OI_JOY_AXIS_MAX) {
        corval = OI_JOY_AXIS_MAX;
    }
    else if(value < OI_JOY_AXIS_MIN) {
        corval = OI_JOY_AXIS_MIN;
    }
    else {
        corval = value;
    }

    // Relative or absolute
    if(relative == OI_ENABLE) {
        rel = TRUE;
    }
    else if(relative == OI_DISABLE) {
        rel = FALSE;
    }
    // Detect relativiness
    else {
        // So far, only balls are relative
        rel = (conf->kind[axis] == OIJ_BALL);
    }

    // Store state for relative update
    if(rel) {
        priv->insaxes[axis] = corval;
        priv->relaxes[axis] += corval;
        priv->absaxes[axis] += corval;
    }
    // State for absolute update
    else {
        priv->insaxes[axis] = corval - priv->absaxes[axis];
        priv->relaxes[axis] = corval - priv->absaxes[axis];
        priv->absaxes[axis] = corval;
    }

    // Update flag
    priv->update[axis] |= post;
}

/* ******************************************************************** */

/**
 * @ingroup IJoystick
 * @brief Joystick button update
 *
 * @param index device index
 * @param btn button index
 * @param down true (1) on button press, false (0) otherwise
 * @param post true (1) to post event, false (0) otherwise
 *
 * Feed joystick button press/release into joystick state manager.
 */
void joystick_button(unsigned char index, unsigned char btn, char down, char post) {
    unsigned int newbut;
    unsigned char type;
    oi_privjoy *priv;

    // Get private data or bail
    priv = device_priv(index, OI_PRO_JOYSTICK);
    if(!priv) {
        return;
    }

    // Calculate button mask
    newbut = priv->button;
    if(down) {
        type = OI_JOYBUTTONDOWN;
        newbut |= OI_BUTTON_MASK(btn);
    }
    else {
        type = OI_JOYBUTTONUP;
        newbut &= ~OI_BUTTON_MASK(btn);
    }
    priv->button = newbut;

    // Postal services
    if(post) {
        oi_event ev;
        ev.type = type;
        ev.joybutton.device = index;
        ev.joybutton.code = OI_JOY_MAKE_CODE(OIJ_GEN_BUTTON, btn);
        ev.joybutton.state = newbut;
        queue_add(&ev);
    }
}

/* ******************************************************************** */

/**
 * @ingroup IJoystick
 * @brief Pump pending joystick events into queue
 *
 * Here, joystick axes with the 'update' flag set are analyzed,
 * possibly paired, and sent to the event queue. We need to do this as
 * the last step in the joystick frame in order to collect x- and
 * y-axes for trackballs, hats etc.
 */
void joystick_pump() {
    unsigned char index;
    oi_device *dev;
    oi_privjoy *priv;
    oi_joyconfig *conf;
    oi_event ev;
    unsigned char axis;
    int abs;
    int rel;

    // Parse all devices
    for(index=1; index<OI_MAX_DEVICES; index++) {
        // Devices are always ordered, so bail if we hit the end
        dev = device_get(index);
        if(!dev) {
            break;
        }

        // Get structures
        conf = dev->joyconfig;
        priv = device_priv(index, OI_PRO_JOYSTICK);

        // Only handle joysticks with valid thingies
        if(!(dev->provides & OI_PRO_JOYSTICK) || !conf || !priv) {
            continue;
        }

        // Ok, we have a valid joystick, parse each axis
        for(axis=0; axis<OI_JOY_NUM_AXES; axis++) {
            // Bail if no-axis or no update needed
            if((conf->kind[axis] == OIJ_NONE) || !(priv->update[axis])) {
                continue;
            }

            // Flag axis as handled as we may bail out anytime now
            priv->update[axis] = FALSE;

            // Trackballs
            if(conf->kind[axis] == OIJ_BALL) {

                // Get vertical movement (second axis)
                if(conf->pair[axis] != 0) {
                    rel = priv->relaxes[conf->pair[axis]];
                }
                else {
                    rel = 0;
                }

                // Send trackball event
                ev.type = OI_JOYBALL;
                ev.joyball.device = index;
                ev.joyball.code = OI_JOY_MAKE_CODE(OIJ_BALL, axis);
                ev.joyball.relx = priv->relaxes[axis];
                ev.joyball.rely = rel;
                queue_add(&ev);

                // We're done with this axis
                continue;
            }

            // Set absolute and instantaneous relative value
            abs = priv->absaxes[axis];
            rel = priv->insaxes[axis];

            // Hats
            if(conf->kind[axis] == OIJ_HAT) {
                // Two-axis hats
                if(conf->pair[axis] != 0) {
                    abs = joystick_hatpos(priv->absaxes[axis],
                                          priv->absaxes[conf->pair[axis]]);
                }

                // Hats have no relative movement
                rel = abs;
            }

            // Send absolute axis event
            ev.type = OI_JOYAXIS;
            ev.joyaxis.device = index;
            ev.joyaxis.code = OI_JOY_MAKE_CODE(conf->kind[axis], axis);
            ev.joyaxis.abs = abs;
            ev.joyaxis.rel = rel;
            queue_add(&ev);
        }
    }
}

/* ******************************************************************** */

/**
 * @ingroup IJoystick
 * @brief Hat position converter
 *
 * @param x horizontal axis
 * @param y vertival axis
 * @returns hat posistion, see @ref PJoyHats
 *
 * Under some OSes (like Linux), joystick hats are reported using
 * multiple axes.  This function converts two-axis hats to the
 * discrete joystick hat position values found as OI_HAT_*.
 */
int joystick_hatpos(int x, int y) {
    const int hatpos[3][3] = {
        {OI_HAT_UPLEFT, OI_HAT_UP, OI_HAT_UPRIGHT},
        {OI_HAT_LEFT, OI_HAT_CENTER, OI_HAT_RIGHT},
        {OI_HAT_DOWNLEFT, OI_HAT_DOWN, OI_HAT_DOWNRIGHT} };
    int cx;
    int cy;

    // Clip
    if(x < 0) {
        cx = 0;
    }
    else if(x == 0) {
        cx = 1;
    }
    else {
        cx = 2;
    }

    if(y < 0) {
        cy = 0;
    }
    else if(y == 0) {
        cy = 1;
    }
    else {
        cy = 2;
    }

    // Ok, now it's just a lookup
    return hatpos[cy][cx];
}

/* ******************************************************************** */

/**
 * @ingroup PJoystick
 * @brief Get relative position of a joystick axis
 *
 * @param index device index, 0 for default joystick
 * @param axis axis index (index, not code)
 * @param value pointer to relative motion of axis
 * @param second pointer to relative motioin of second axis (for trackballs etc)
 * @returns the current button mask
 *
 * Get the relative motion of a joystick axis since last call
 * to this function (ie. the cummulative, relative motion).
 * If device or axis is not found, the value returned is 0.
 */
unsigned int oi_joy_relative(unsigned char index, unsigned char axis, int *value, int *second) {
    oi_privjoy *priv;
    oi_joyconfig *conf;
    unsigned char i;

    // Default value
    if(value) {
        *value = 0;
    }
    if(second) {
        *second = 0;
    }

    // Dummy check
    if(axis >= OI_JOY_NUM_AXES) {
        return OI_BUTTON_MASK(0);
    }

    // Get device index
    i = index;
    if(i == 0) {
        for(i=1; i<OI_MAX_DEVICES; i++) {
            if(device_priv(i, OI_PRO_JOYSTICK)) {
                break;
            }
        }

        // End reached
        if(i==OI_MAX_DEVICES) {
            return OI_BUTTON_MASK(0);
        }
    }

    // Get device data
    priv = (oi_privjoy*)device_priv(i, OI_PRO_JOYSTICK);
    conf = device_get(i)->joyconfig;
    if(!priv || !conf) {
        return OI_BUTTON_MASK(0);
    }

    // Check axis
    if(conf->kind[axis] == OIJ_NONE) {
        return OI_BUTTON_MASK(0);
    }

    // Set data
    if(value) {
        *value = priv->relaxes[axis];
    }
    // Reset delta
    priv->relaxes[axis] = 0;

    // Paired axis
    if((conf->pair[axis] != 0) && second) {
        *second = priv->relaxes[conf->pair[axis]];
        priv->relaxes[conf->pair[axis]] = 0;
    }

    // Return button state
    return priv->button;
}

/* ******************************************************************** */

/**
 * @ingroup PJoystick
 * @brief Get absolute position of a joystick axis
 *
 * @param index device index, 0 for default joystick
 * @param axis axis index (index, not code)
 * @param value pointer to absolute position of axis
 * @param second pointer to absolute position of second axis (for trackballs etc)
 * @returns the current button mask
 *
 * Get the absolute position of a joystick axis and the current
 * button state (as a bitmask). If the device or axis can not be found
 * the value returned is zero.
 */
unsigned int oi_joy_absolute(unsigned char index, unsigned char axis, int *value, int *second) {
    oi_privjoy *priv;
    oi_joyconfig *conf;
    unsigned char i;

    // Default value
    if(value) {
        *value = 0;
    }
    if(second) {
        *second = 0;
    }

    // Dummy check
    if(axis >= OI_JOY_NUM_AXES) {
        return OI_BUTTON_MASK(0);
    }

    // Get device index
    i = index;
    if(i == 0) {
        for(i=1; i<OI_MAX_DEVICES; i++) {
            if(device_priv(i, OI_PRO_JOYSTICK)) {
                break;
            }
        }

        // End reached
        if(i==OI_MAX_DEVICES) {
            return OI_BUTTON_MASK(0);
        }
    }

    // Get device data
    priv = (oi_privjoy*)device_priv(i, OI_PRO_JOYSTICK);
    conf = device_get(i)->joyconfig;
    if(!priv || !conf) {
        return OI_BUTTON_MASK(0);
    }

    // Check axis
    if(conf->kind[axis] == OIJ_NONE) {
        return OI_BUTTON_MASK(0);
    }

    // Set data
    if(value) {
        *value = priv->absaxes[axis];
    }

    // Data for paired axis
    if((conf->pair[axis] != 0) && second) {
        *second = priv->absaxes[conf->pair[axis]];
    }

    // Return button state
    return priv->button;
}

/* ******************************************************************** */

/**
 * @ingroup PJoystick
 * @brief Get string from joystick event-code
 *
 * @param code joystick button/axis code
 * @returns joystick button/axis name as a string
 *
 * Get the symbolic string name of a joystick button/axis defined
 * by the given code.
 */
char *oi_joy_getname(unsigned int code) {
    oi_joytype t;
    unsigned char i;

    t = OI_JOY_DECODE_TYPE(code);
    i = (unsigned char)OI_JOY_DECODE_INDEX(code);
    debug("oi_joystick_getname: decoded type:%u index:%u", t, i);

    // Dummy checks (return joy_unknown on error)
    if((i >= OIJ_LAST) || (t == 0) || (t >= OI_JOY_NUM_AXES)) {
        return "joy_unknown";
    }

    // Axes (hats, sticks, rudders, etc.)
    if(t != OIJ_GEN_BUTTON) {
        return joynames[OI_JOY_TAB_AXES][i];
    }
    // Buttons
    else {
        return joynames[OI_JOY_TAB_BTNS][i];
    }
}

/* ******************************************************************** */

/**
 * @ingroup PJoystick
 * @brief Get joystick code from string
 *
 * @param name symbolic string name of button/axis
 * @returns the joystick code
 *
 * Translate symbolic string to joystick code.
 */
unsigned int oi_joy_getcode(char *name) {
    int i;

    // Dummy checks
    if(!name) {
        return OI_JOY_NONE_CODE;
    }
    if((strlen(name) < OI_MIN_KEYLENGTH) ||
       (strlen(name) > OI_MAX_KEYLENGTH)) {
        return OI_JOY_NONE_CODE;
    }

    // Check prefix
    if(strncmp(name, "joy_", 4) != 0) {
        return OI_JOY_NONE_CODE;
    }

    // Axes
    if(strncmp(name, "joy_axis", 8) == 0) {
        i = atoi(name+8);
        if((i >= 0) && (i<OI_JOY_NUM_AXES)) {
            return OI_JOY_MAKE_CODE(OIJ_GEN_AXIS, i);
        }
    }

    // Buttons
    if(strncmp(name, "joy_button", 10) == 0) {
        i = atoi(name+10);
        if((i >= 0) && (i<OI_JOY_NUM_AXES)) {
            return OI_JOY_MAKE_CODE(OIJ_GEN_BUTTON, i);
        }
    }

    // Fail
    return OI_JOY_NONE_CODE;
}

/* ******************************************************************** */

/**
 * @ingroup PJoystick
 * @brief Get basic joystick information
 *
 * @param index device index, 0 for default joystick
 * @param name pointer to string with joystick name
 * @param buttons pointer to number of buttons
 * @param axes pointer to number of axes
 * @returns errorcode, see @ref PErrors
 *
 * Get basic information about a joystick device such as the vendor-supplied
 * name, the number of buttons and axes.
 */
int oi_joy_info(unsigned char index, char **name, int *buttons, int *axes) {
    oi_device *dev;
    oi_joyconfig *conf;
    unsigned char i;
    unsigned char j;

    // Find first keyboard if index is zero
    i = index;
    if(i == 0) {
        for(i=1; i<OI_MAX_DEVICES; i++) {
            if(device_priv(i, OI_PRO_JOYSTICK)) {
                break;
            }
        }

        // No keyboard found
        return OI_ERR_NO_DEVICE;
    }

    // Get device pointers and dummy checking
    dev = device_get(i);
    if(!dev) {
        return OI_ERR_NO_DEVICE;
    }
    conf = dev->joyconfig;
    if(!conf) {
        return OI_ERR_NO_DEVICE;
    }

    // Count number of axis
    i = 0;
    for(j=0; j<OI_JOY_NUM_AXES; j++) {
        if((conf->kind[j] != OIJ_NONE) && (conf->kind[j] != OIJ_GEN_BUTTON)) {
            i++;
        }
    }

    // Safe filling of data
    if(name) {
        *name = conf->name;
    }
    if(buttons) {
        *buttons = conf->buttons;
    }
    if(axes) {
        *axes = i;
    }

    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup PJoystick
 * @brief Get axes mapping structures
 *
 * @param index device index, 0 for default joystick
 * @param type joystick type array
 * @param pair joystick axes pairing array
 * @param num pointer to number of elements (axes) in arrays
 * @returns errorcode, see @ref PErrors
 *
 * Get pointers to the axes mappings of a joystick device. The axes
 * mappings relate a generic axis index number on the joystick (such
 * as the horizontal or vertical axis of a stick or hat, a throttle, a
 * rudder etc.) to the aforementioned type of axis. This information
 * is in the 'type' parameter.
 *
 * The 'pair' parameter is used to link a single axis to it's second
 * (or pair) axis, for example on hats, which might be encoded as
 * double axes eventhough the usefull information is more like a
 * 9-positional button.
 *
 * @note You would normally not need this information. It can, however,
 * be used to setup a joystick if OpenInput does not know about
 * the specific joystick (or because your operating system does not
 * reveal enough information to fully support the different kinds of
 * axes etc.)
 */
int op_joy_axessetup(unsigned char index, oi_joytype *type[], unsigned char *pair[], int *num) {
    oi_device *dev;
    oi_joyconfig *conf;
    unsigned char i;

    // Find first keyboard if index is zero
    i = index;
    if(i == 0) {
        for(i=1; i<OI_MAX_DEVICES; i++) {
            if(device_priv(i, OI_PRO_JOYSTICK)) {
                break;
            }
        }

        // No keyboard found
        return OI_ERR_NO_DEVICE;
    }

    // Get private data
    dev = device_get(i);
    if(!dev) {
        return OI_ERR_NO_DEVICE;
    }
    conf = dev->joyconfig;
    if(!conf) {
        return OI_ERR_NO_DEVICE;
    }

    // Safe filling of data
    if(type) {
        *type = conf->kind;
    }
    if(pair) {
        *pair = conf->pair;
    }
    if(num) {
        *num = OI_JOY_NUM_AXES;
    }

    // Done
    return OI_ERR_OK;
}

/* ******************************************************************** */
