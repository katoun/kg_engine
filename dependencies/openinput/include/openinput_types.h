/*
 * openinput_types.h : Basic type definitions
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

#ifndef _OPENINPUT_TYPES_H_
#define _OPENINPUT_TYPES_H_

#ifndef _OPENINPUT_H_
#error Do not include this file directly - use openinput.h
#endif

/* ******************************************************************** */

/**
 * @ingroup PTypes
 * @defgroup PBool Tri-state type
 * @brief Enable, disable or query
 * @{
 */
typedef enum {
    OI_DISABLE                     = 0,    /**< False/disable */
    OI_ENABLE                      = 1,    /**< True/enable */
    OI_QUERY                       = 2     /**< Don't change, return current */
} oi_bool;                                 /**< Tri-state (bool + query) */
/** @} */


/**
 * @ingroup PTypes
 * @defgroup PWindow Window hook parameters
 * @brief Init string parameters for window_id
 * @{
 */
#define OI_I_CONN                  'c' /**< Server connection handle */
#define OI_I_SCRN                  's' /**< Screen handle */
#define OI_I_WINID                 'w' /**< Window handle */
/** @} */


/**
 * @ingroup PTypes
 * @defgroup PProvide Device provide flags
 * @brief Device provide flags
 * @{
 */
#define OI_PRO_UNKNOWN              1 /**< Unknown/test type device */
#define OI_PRO_WINDOW               2 /**< Window stuff (move/state/size) */
#define OI_PRO_SYSTEM               4 /**< System events (segfault/interrupt/etc) */
#define OI_PRO_KEYBOARD             8 /**< Keyboard input device */
#define OI_PRO_MOUSE               16 /**< Pointer input device (mouse) */
#define OI_PRO_JOYSTICK            32 /**< Joystick input device */
/** @} */


/**
 * @ingroup PTypes
 * @defgroup PFocus Application focus flags
 * @brief Application activation
 * @{
 */
#define OI_FOCUS_MOUSE              1 /**< Window has mouse focus */
#define OI_FOCUS_INPUT              2 /**< Window has keyboard (input) focus */
#define OI_FOCUS_VISIBLE            4 /**< Window is visible */
/** @} */


/**
 * @ingroup PTypes
 * @defgroup PFlags Library initilization flags
 * @brief Initialization flags
 * @{
 */
#define OI_FLAG_NOWINDOW        1 /**< Do not hook into window */
/** @} */


/**
 * @ingroup PTypes
 * @defgroup PErrors Error codes
 * @brief Error codes
 * @{
 */
#define OI_ERR_OK               0 /**< All ok */
#define OI_ERR_NO_DEVICE        1 /**< Wrong device */
#define OI_ERR_INDEX            2 /**< Index query out of bounds */
#define OI_ERR_NOT_IMPLEM       4 /**< Not implemented */
#define OI_ERR_DEV_EXIST        5 /**< Device already initialized */
#define OI_ERR_PARAM            6 /**< Invalid parameter */
#define OI_ERR_NO_NAME          7 /**< No such event name exists */
#define OI_ERR_INTERNAL         8 /**< Fatal internal error */
#define OI_ERR_DEV_BEHAVE       9 /**< Device programming error */
/** @} */

/* ******************************************************************** */

#endif
