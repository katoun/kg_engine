/*
 * openinput_mouse.h : Definitions for mouse interface
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

#ifndef _OPENINPUT_MOUSE_H_
#define _OPENINPUT_MOUSE_H_

#ifndef _OPENINPUT_H_
#error Do not include this file directly - use openinput.h
#endif

/* ******************************************************************** */

/**
 * @ingroup PTypes
 * @defgroup PMousename Mouse button definitions
 * @brief Mouse buttons
 *
 * Mice typically have three buttons and a scroll-wheel.
 * Each of these are defined here with their human readable
 * string-counterpart.
 *
 * @{
 */
typedef enum {
    OIP_UNKNOWN            = 0, /**< mouse_unknown */
    OIP_FIRST              = 0, /**< mouse_unknown */
    OIP_BUTTON_LEFT        = 1, /**< mouse_left */
    OIP_BUTTON_MIDDLE      = 2, /**< mouse_middle */
    OIP_BUTTON_RIGHT       = 3, /**< mouse_right */
    OIP_WHEEL_UP           = 4, /**< mouse_wheel_up */
    OIP_WHEEL_DOWN         = 5, /**< mouse_wheel_down */
    OIP_MOTION             = 6, /**< mouse_motion */
    OIP_LAST                    /**< mouse_unknown */
} oi_mouse;                     /**< Mouse buttons */
/** @} */

/**
 * @ingroup PTypes
 * @defgroup PMouseMask Mouse button masks
 * @brief Definition of mouse button masks
 *
 * Mouse button bits used in state parameters.
 *
 * @{
 */
#define OI_BUTTON_MASK(x) (1<<(x))                                /**< Button bitmask generator */
#define OI_BUTTON_LEFTMASK      OI_BUTTON_MASK(OIP_BUTTON_LEFT)   /**< Bitmask for left button */
#define OI_BUTTON_MIDMASK       OI_BUTTON_MASK(OIP_BUTTON_MIDDLE) /**< Bitmask for middle button */
#define OI_BUTTON_RIGHTMASK     OI_BUTTON_MASK(OIP_BUTTON_RIGHT)  /**< Bitmask for right button */
/** @} */

/* ******************************************************************** */

#endif
