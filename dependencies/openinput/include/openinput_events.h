/*
 * openinput_events.h : Event structure types
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

#ifndef _OPENINPUT_EVENTS_H_
#define _OPENINPUT_EVENTS_H_

#ifndef _OPENINPUT_H_
#error Do not include this file directly - use openinput.h
#endif

/**
 * @ingroup PTypes
 * @defgroup PEventStructs Event structures
 * @brief Event structures
 *
 * Depending on which device that generates an event, different
 * event structures are used. All events contain a "type" field,
 * but are otherwise different from each other.
 */
typedef enum {
    OI_NOEVENT                    = 0,  /**< No event */
    OI_KEYUP                      = 1,  /**< Key released */
    OI_KEYDOWN                    = 2,  /**< Key pressed */
    OI_MOUSEMOVE                  = 3,  /**< Mouse motion */
    OI_MOUSEBUTTONUP              = 4,  /**< Button released */
    OI_MOUSEBUTTONDOWN            = 5,  /**< Button pressed */
    OI_ACTIVE                     = 6,  /**< App. focus gain/loss */
    OI_RESIZE                     = 7,  /**< App. window resize */
    OI_EXPOSE                     = 8,  /**< App. needs redraw */
    OI_QUIT                       = 9,  /**< Quit requested */
    OI_DISCOVERY                  = 10, /**< Device driver available */
    OI_ACTION                     = 11, /**< Action event (actionmap) */
    OI_JOYAXIS                    = 12, /**< Joystick axis */
    OI_JOYBUTTONUP                = 13, /**< Joystick button released */
    OI_JOYBUTTONDOWN              = 14, /**< Joystick button pressed */
    OI_JOYBALL                    = 15  /**< Joystick trackball */
} oi_type;


/**
 * @ingroup PTypes
 * @defgroup PEventmask Event masks
 * @brief Definition of event masks
 * @{
 */
#define OI_EVENT_MASK(x) (1<<(x))                                 /**< Mask generator */
#define OI_MASK_ALL 0xffffffff                                    /**< Match all masks */
#define OI_MASK_KEYUP           OI_EVENT_MASK(OI_KEYUP)           /**< Key up */
#define OI_MASK_KEYDOWN         OI_EVENT_MASK(OI_KEYDOWN)         /**< Key down */
#define OI_MASK_MOUSEMOVE       OI_EVENT_MASK(OI_MOUSEMOVE)       /**< Mouse motion */
#define OI_MASK_MOUSEBUTTONUP   OI_EVENT_MASK(OI_MOUSEBUTTONUP)   /**< Mouse button up */
#define OI_MASK_MOUSEBUTTONDOWN OI_EVENT_MASK(OI_MOUSEBUTTONDOWN) /**< Mouse button down */
#define OI_MASK_ACTIVE          OI_EVENT_MASK(OI_ACTIVE)          /**< Window focus */
#define OI_MASK_RESIZE          OI_EVENT_MASK(OI_RESIZE)          /**< Window resize */
#define OI_MASK_EXPOSE          OI_EVENT_MASK(OI_EXPOSE)          /**< Window show/update */
#define OI_MASK_DISCOVERY       OI_EVENT_MASK(OI_DISCOVERY)       /**< Device discovery */
#define OI_MASK_ACTION          OI_EVENT_MASK(OI_ACTION)          /**< Action map */
#define OI_MASK_QUIT            OI_EVENT_MASK(OI_QUIT)            /**< Application quit */
#define OI_MASK_WINDOW          (OI_EVENT_MASK(OI_ACTIVE) | OI_EVENT_MASK(OI_RESIZE) | OI_EVENT_MASK(OI_EXPOSE)) /**< Focus, resize, expose */
#define OI_MASK_MOUSE           (OI_EVENT_MASK(OI_MOUSEMOVE) | OI_EVENT_MASK(OI_MOUSEBUTTONUP) | OI_EVENT_MASK(OI_MOUSEBUTTONDOWN)) /**< Mouse button and motion */
#define OI_MASK_JOYSTICK        (OI_EVENT_MASK(OI_JOYAXIS) | OI_EVENT_MASK(OI_JOYBUTTONUP) | OI_EVENT_MASK(OI_JOYBUTTONDOWN) | OI_EVENT_MASK(OI_JOYBALL)) /**< Joystick axes, buttons and trackballs */
/** @} */


/**
 * @ingroup PEventStructs
 * @brief Device discovery event
 *
 * Sent when device drivers are registered and ready for use
 */
typedef struct oi_discovery_event {
    unsigned char type;              /**< OI_DISCOVERY  */
    unsigned char device;            /**< Device index  */
    char *name;                      /**< Short name  */
    char *description;               /**< Long description  */
    unsigned int provides;           /**< Provide mask  */
} oi_discovery_event;


/**
 * @ingroup PEventStructs
 * @brief Application visibility event
 *
 * Sent when applicaiton loses or gains focus, see
 * @ref PFocus
 */
typedef struct oi_active_event {
    unsigned char type;              /**< OI_ACTIVE  */
    unsigned char device;            /**< Device index  */
    char gain;                       /**< Focus was 0:lost 1:gained */
    unsigned int state;              /**< Bitmask of focus state  */
} oi_active_event;


/**
 * @ingroup PEventStructs
 * @brief Keyboard event
 *
 * Sent when keyboard keys are pressed or released
 */
typedef struct oi_keyboard_event {
    unsigned char type;              /**< OI_KEYUP or OI_KEYDOWN  */
    unsigned char device;            /**< Device index  */
    oi_keysym keysym;                /**< Key symbol  */
} oi_keyboard_event;


/**
 * @ingroup PEventStructs
 * @brief Mouse move event
 *
 * Sent when the mouse moves
 */
typedef struct oi_mousemove_event {
    unsigned char type;              /**< OI_MOUSEMOVE */
    unsigned char device;            /**< Device index */
    unsigned int state;              /**< Button state bitmask */
    int x;                           /**< Absolute x coordinate */
    int y;                           /**< Absolute y coordinate */
    int relx;                        /**< Relative x movement */
    int rely;                        /**< Relative y movement */
} oi_mousemove_event;


/**
 * @ingroup PEventStructs
 * @brief Mouse button event
 *
 * Sent when mouse buttons are pressed or released
 */
typedef struct oi_mousebutton_event {
    unsigned char type;              /**< OI_MOUSEBUTTONUP or OI_MOUSEBUTTONDOWN */
    unsigned char device;            /**< Device index */
    unsigned char button;            /**< Mouse button index */
    unsigned int state;              /**< Button state bitmask */
    int x;                           /**< Absolute x coordinate at event time */
    int y;                           /**< Absolute y coordinate at event time */
} oi_mousebutton_event;


/**
 * @ingroup PEventStructs
 * @brief Application window resize event
 *
 * Sent when the size of the application window changes
 */
typedef struct oi_resize_event {
    unsigned char type;              /**< OI_RESIZE */
    unsigned char device;            /**< Device index */
    int width;                       /**< New window width */
    int height;                      /**< New window height */
} oi_resize_event;


/**
 * @ingroup PEventStructs
 * @brief Application window needs redraw
 *
 * Sent when application window is shown
 */
typedef struct oi_expose_event {
    unsigned char type;              /**< OI_EXPOSE */
} oi_expose_event;


/**
 * @ingroup PEventStructs
 * @brief Quit event
 *
 * Sent when your application should shutdown.
 * This usually happens when the user closes the
 * window, or when system error occurs
 */
typedef struct oi_quit_event {
    unsigned char type;              /**< OI_QUIT */
} oi_quit_event;


/**
 * @ingroup PEventStructs
 * @brief Action event
 *
 * Sent when an action map has been triggered
 */
typedef struct oi_action_event {
    unsigned char type;              /**< OI_ACTION */
    unsigned char device;            /**< Device index */
    unsigned int actionid;           /**< User-defined actionid */
    char state;                      /**< State (pressed:1/released:0) */
    int data1;                       /**< Default data slot   (1d device: x coord) */
    int data2;                       /**< Secondary data slot (2d device: y coord) */
    int data3;                       /**< Tertiary data slot  (3d device: z coord) */
} oi_action_event;


/**
 * @ingroup PEventStructs
 * @brief Joystick axis event
 *
 * Sent when an axis (stick/throttle/whatever)
 * changes position. Use the "code" to get the axis index and type.
 */
typedef struct oi_joyaxis_event {
    unsigned char type;              /**< OI_JOYAXIS */
    unsigned char device;            /**< Device index */
    unsigned int code;               /**< Joystick code see @ref PJoyTypes */
    int abs;                         /**< Absolute axis position */
    int rel;                         /**< Relative axis motion */
} oi_joyaxis_event;


/**
 * @ingroup PEventStructs
 * @brief Joystick button event
 *
 * Sent when a button on a joystick
 * is pressed or released. Use the "code" to get the button index.
 */
typedef struct oi_joybutton_event {
    unsigned char type;              /**< OI_JOYBUTTONUP or OI_JOYBUTTONDOWN */
    unsigned char device;            /**< Device index */
    unsigned int code;               /**< Joystick code see @ref PJoyTypes */
    unsigned int state;              /**< Buttons state bitmask */
} oi_joybutton_event;


/**
 * @ingroup PEventStructs
 * @brief Joystick trackball event
 *
 * Sent when an joystick trackball moves
 */
typedef struct oi_joyball_event {
    unsigned char type;              /**< OI_JOYBALL */
    unsigned char device;            /**< Device index */
    unsigned int code;               /**< Joystick code see @ref PJoyTypes */
    int relx;                        /**< Relative x movement */
    int rely;                        /**< Relative y movement */
} oi_joyball_event;


/**
 * @ingroup PEventStructs
 * @brief Event union
 *
 * The combined event union. Check the "type" variable
 * to determine what kind of event you're dealing with.
 */
typedef union {
    unsigned char type;               /**< Event type */
    oi_active_event active;           /**< OI_ACTIVE */
    oi_keyboard_event key;            /**< OI_KEYUP or OI_KEYDOWN */
    oi_mousemove_event move;          /**< OI_MOUSEMOVE */
    oi_mousebutton_event button;      /**< OI_MOUSEBUTTONUP or OI_MOUSEBUTTONDOWN */
    oi_resize_event resize;           /**< OI_RESIZE */
    oi_expose_event expose;           /**< OI_EXPOSE */
    oi_quit_event quit;               /**< OI_QUIT */
    oi_discovery_event discover;      /**< OI_DISCOVERY */
    oi_action_event action;           /**< OI_ACTION */
    oi_joyaxis_event joyaxis;         /**< OI_JOYAXIS */
    oi_joybutton_event joybutton;     /**< OI_JOYBUTTONUP or OI_JOYBUTTONDOWN */
    oi_joyball_event joyball;         /**< OI_JOYBALL */
} oi_event;


/* ******************************************************************** */

#endif
