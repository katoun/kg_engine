/*
 * openinput_joystick.h : Definitions for joystick interface
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

#ifndef _OPENINPUT_JOYSTICK_H_
#define _OPENINPUT_JOYSTICK_H_

#ifndef _OPENINPUT_H_
#error Do not include this file directly - use openinput.h
#endif

/* ******************************************************************** */

/**
 * @ingroup PTypes
 * @defgroup PJoyHats Joystick hat positions
 * @brief Joystick hat positions
 * @{
 */
#define OI_HAT_CENTER      0                          /**< Center */
#define OI_HAT_UP          1                          /**< Up */
#define OI_HAT_DOWN        2                          /**< Down */
#define OI_HAT_LEFT        4                          /**< Left */
#define OI_HAT_RIGHT       8                          /**< Right */
#define OI_HAT_UPLEFT      (OI_HAT_UP|OI_HAT_LEFT)    /**< Up + left */
#define OI_HAT_UPRIGHT     (OI_HAT_UP|OI_HAT_RIGHT)   /**< Up + right */
#define OI_HAT_DOWNLEFT    (OI_HAT_DOWN|OI_HAT_LEFT)  /**< Down + left */
#define OI_HAT_DOWNRIGHT   (OI_HAT_DOWN|OI_HAT_RIGHT) /**< Down + right */
/** @} */


/**
 * @ingroup PJoyTypes
 * @brief Joystick entity classes
 *
 * Joysticks typically have different kinds ("types") of axes like
 * sticks, throttles and rudders. This enum is used both to
 * tell what kind an axis is, and to tell the difference between
 * axes and buttons in the unique joystick "code" used in
 * joystick events.
 */
typedef enum {
    OIJ_NONE                = 0,      /**< Not an axis/button */
    OIJ_GEN_BUTTON          = 1,      /**< Generic button */
    OIJ_GEN_AXIS            = 2,      /**< Generic axis */
    OIJ_STICK               = 3,      /**< A classic stick (axis) */
    OIJ_HAT                 = 4,      /**< Positional button (axis) */
    OIJ_RUDDER              = 5,      /**< Yaw-stick (axis) */
    OIJ_THROTTLE            = 6,      /**< Throttle/trimmer (axis) */
    OIJ_BALL                = 7,      /**< Trackball (relative events, 2-axis) */
    OIJ_LAST                          /**< Last joystick class type */
} oi_joytype;


/**
 * @ingroup PTypes
 * @defgroup PJoyTypes Joystick definitions
 * @brief Joystick buttons and constants
 * @{
 */
#define OI_JOY_NUM_DEVS      32       /**< Maximum number of joysticks */
#define OI_JOY_NUM_AXES      16       /**< Maximum buttons/axes */
#define OI_JOY_AXIS_MIN     -32768    /**< Minimum axis value */
#define OI_JOY_AXIS_MAX      326767   /**< Maximum axis value */
#define OI_JOY_ENCODE_TYPE(t)  (0x0000ffff & (t))        /**< Make the type-part of a joystick code */
#define OI_JOY_DECODE_TYPE(t)  (0x0000ffff & (t))        /**< Get the type-part of a joystick code */
#define OI_JOY_ENCODE_INDEX(i) (0xffff0000 & ((i) <<16)) /**< Make the index-part of a joystick code */
#define OI_JOY_DECODE_INDEX(i) ((0xffff0000 & (i)) >>16) /**< Get the index-part of a joystick code */
#define OI_JOY_MAKE_CODE(t,i)  (OI_JOY_ENCODE_TYPE((t)) + OI_JOY_ENCODE_INDEX((i))) /**< Make joystick code given type and index (in that order) */
#define OI_JOY_NONE_CODE OI_JOY_MAKE_CODE(OIJ_NONE, 0)   /**< The 'none' code that never matches */
/** @} */

/* ******************************************************************** */

#endif
