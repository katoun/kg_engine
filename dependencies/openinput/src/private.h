/*
 * private.h : Private per-device data for managers
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

#ifndef _OPENINPUT_PRIVATE_H_
#define _OPENINPUT_PRIVATE_H_

/* ******************************************************************** */


/**
 * @ingroup IPrivate
 * @brief Per-device joystick managment data
 *
 * Non-driver specific private joystick data used
 * for local axis-mapping and other information
 */
typedef struct oi_privjoy {
    unsigned int button;                               /**< Button state mask */
    int relaxes[OI_JOY_NUM_AXES];                      /**< Cummulative relative axes values */
    int absaxes[OI_JOY_NUM_AXES];                      /**< Absolute axes values */
    int insaxes[OI_JOY_NUM_AXES];                      /**< Instantaneous relative values */
    char update[OI_JOY_NUM_AXES];                      /**< Post event update */
} oi_privjoy;


/**
 * @ingroup IPrivate
 * @brief Per-device mouse management data
 *
 * Non-driver specific private mouse data used for
 * positon/movement state caching.
 */
typedef struct oi_privmouse {
    oi_mouse button;                                   /**< Button bitmask */
    int absx;                                          /**< Absolute horizontal position */
    int absy;                                          /**< Absolute vertical position */
    int relx;                                          /**< Relative horizontal movement */
    int rely;                                          /**< Relative vertical movement */
} oi_privmouse;


/**
 * @ingroup IPrivate
 * @brief Per-device keyboard management data
 *
 * Non-driver specific private keyboard data used for
 * key-down/modifier state caching.
 */
typedef struct oi_privkey {
    char keystate[OIK_LAST];                            /**< Button state table */
    unsigned int modstate;                              /**< Modifier bitmask */
    char rep_first;                                     /**< Repeat first event */
    unsigned int rep_time;                              /**< Repeat timestap */
    oi_event rep_ev;                                    /**< Repeat event */
} oi_privkey;


/**
 * @ingroup IPrivate
 * @brief Private per-device data for managers
 *
 * The managers (keyboard/mouse/joystick) usually need some
 * per-device specific data, for example to store the
 * mouse cursor position or to perform joystick axis mapping.
 * This structure is a placeholder/entry, which is tied
 * into the device interface (device structure).
 */
typedef struct oi_private {
    oi_privkey *key;                                    /**< Keyboard data */
    oi_privmouse *mouse;                                /**< Mouse data */
    oi_privjoy *joy;                                    /**< Joystick data */
} oi_private;

/* ******************************************************************** */

#endif
