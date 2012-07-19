/*
 * internal.h : Header for internal functions
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

#ifndef _OPENINPUT_INTERNAL_H_
#define _OPENINPUT_INTERNAL_H_

/* ******************************************************************** */

// Forward definitions
struct oi_joyconfig;
struct oi_aclink;
struct oi_privmouse;
struct oi_privkey;
struct oi_privjoy;

/* ******************************************************************** */

/**
 * @ingroup IDevstructs
 * @brief Device driver abstraction interface
 *
 * This is the primary device abstraction layer and entry point for
 * OpenInput to access the drivers capabilities. This should be created
 * dynamically from the bootstrap create function.
 *
 * The following notations are used:
 * - F: Function pointer, device driver entry function
 * - D: Driver data, driver-supplied data
 * - M: Managment data, library data used by mouse/keyboard/etc. managers
 * - A: Automatically set during stage 2 bootstrap (before device->init)
 * - O: Optional, set to NULL to disable usage
 */
typedef struct oi_device {
    unsigned char index;                                              /**< Device index (M,A) */
    char *name;                                                       /**< Short device name (D,A) */
    char *desc;                                                       /**< Description of device (D,A) */
    unsigned int provides;                                            /**< Provide-flag (D,A) */
    void *private;                                                    /**< Private driver data (D,O) */
    struct oi_joyconfig *joyconfig;                                   /**< Joystick configuration (D,O) */
    int (*init)(struct oi_device *dev,
                char *window_id, unsigned int flags);                 /**< Initialize device (F) */
    int (*destroy)(struct oi_device *dev);                            /**< Shutdown device (F) */
    void (*process)(struct oi_device *dev);                           /**< Pump events into queue (F) */
    int (*grab)(struct oi_device *dec, int on);                       /**< Grab input focus (F,O) */
    int (*hide)(struct oi_device *dev, int on);                       /**< Hide/show cursor (F,O) */
    int (*warp)(struct oi_device *dev, int x, int y);                 /**< Warp mouse cursor (F,O) */
    int (*winsize)(struct oi_device *dev, int *w, int *h);            /**< Query window size (F,O) */
    int (*reset)(struct oi_device *dev);                              /**< Reset internal state (F,O) */
} oi_device;

/**
 * @ingroup IDevstructs
 * @brief Device driver bootstrap interface
 *
 * Static structure used to kickstart device drivers when the
 * library is initialized.
 *
 * The fields with a "fcnptr"-note are function pointers.
 */
typedef struct oi_bootstrap {
    char *name;                                                        /**< Short device name */
    char *desc;                                                        /**< Device description */
    unsigned int provides;                                             /**< Device provide-flag */
    int (*avail)(unsigned int flags);                                  /**< Is device available (fcnptr) */
    struct oi_device *(*create)();                                     /**< Return device structure (fcnptr) */
} oi_bootstrap;

/* ******************************************************************** */
// Special functions

char oi_runstate();

unsigned int oi_getticks();

/* ******************************************************************** */
// Internal queue functions

int queue_init();

int queue_lock();

int queue_unlock();

int queue_cut(unsigned int where);

int queue_add(oi_event *evt);

int queue_peep(oi_event *evts,
               unsigned int num,
               unsigned int mask,
               char remove);

/* ******************************************************************** */
// Device handling

int device_init();

void device_bootstrap(char *window_id,
                      unsigned int flags);

int device_register(oi_bootstrap *boot,
                    char *window_id,
                    unsigned int flags);

oi_device *device_get(unsigned char index);

void device_pumpall();

unsigned int device_windowid(char *str,
                             char tok);

int device_destroy(unsigned char index);

void device_moreavail(char more);

void *device_priv(unsigned char index,
                  unsigned int manager);

/* ******************************************************************** */
// Application state

int appstate_init();

void appstate_focus(unsigned char index,
                    char gain,
                    unsigned int state,
                    char post);

void appstate_resize(unsigned char index,
                     int w,
                     int h,
                     char post);

int appstate_width();

int appstate_height();

/* ******************************************************************** */
// Mouse state

int mouse_init();

void mouse_manage(struct oi_privmouse **mouse,
                  unsigned int provide);

void mouse_move(unsigned char index,
                int x,
                int y,
                char relative,
                char post);

void mouse_button(unsigned char index,
                  oi_mouse btn,
                  char down,
                  char post);

/* ******************************************************************** */
// Keyboard state

int keyboard_init();

void keyboard_manage(struct oi_privkey **key,
                     unsigned int provide);

int keyboard_fillnames(char **kn);

void keyboard_update(unsigned char index,
                     oi_keysym *keysym,
                     char down,
                     char post);

void keyboard_dorepeat();

void keyboard_setmodifier(unsigned char index,
                          unsigned int newmod);

oi_key keyboard_scangetkey(char *name,
                           oi_key first,
                           oi_key last);

/* ******************************************************************** */
// Joystick state

int joystick_init();

int joystick_close();

void joystick_manage(struct oi_privjoy **joy,
                     unsigned int provide);

void joystick_axis(unsigned char index,
                   unsigned char axis,
                   int value,
                   oi_bool relative,
                   char post);

void joystick_button(unsigned char index,
                     unsigned char btn,
                     char down,
                     char post);

int joystick_hatpos(int x,
                    int y);

void joystick_pump();

/**
 * @ingroup IJoystick
 * @brief Joystick device configuration
 *
 * Joystick configuration structure. This struct is used
 * for setting up axes mappings and for general information
 * about joysticks like number of buttonts etc.
 *
 * The "kind[index]" number should be looked up in as oi_joyaxis
 * to determine which type of axis it is. Counting these
 * axis-ids is also the correct way  to determine number of
 * sticks/throttles/etc. There may be OI_AXIS_NONE types anywere
 * in the array.
 *
 * To pair two-axes sticks, trackballs and hats, the "pair[index]"
 * number can be used to find the twin-brother. A pair-number of
 * zero denotes a no-pair. Not all platforms may need pairing.
 * So far, Linux does as balls and hats are two-axis
 */
typedef struct oi_joyconfig {
    char *name;                                                       /**< Name of joystick */
    unsigned char buttons;                                            /**< Number of buttons */
    oi_joytype kind[OI_JOY_NUM_AXES];                                 /**< Axis "type" mapping */
    unsigned char pair[OI_JOY_NUM_AXES];                              /**< Ball/stick/hat pairing */
} oi_joyconfig;

/* ******************************************************************** */
// Action state

int action_init();

void action_clearreal();

void action_process(oi_event *evt);

void action_cleartable(struct oi_aclink *tab[],
                       int num);

struct oi_aclink *action_tail(struct oi_aclink **head,
                              char alloc);

void action_statepost(oi_event *evt);

/**
 * @ingroup IAction
 * @brief Action mapper linked list item
 *
 * Action map link list item for event. This is used to determine
 * which devices that generate specific events or to allow
 * keypresses to generate multiple events (or even a combo).
 */
typedef struct oi_aclink {
    unsigned int action;                                               /**< Action id */
    unsigned char device;                                              /**< Device index */
    struct oi_aclink *next;                                            /**< Next pointer */
} oi_aclink;

/* ******************************************************************** */

// Debug macro
#ifdef DEBUG
void debug(char *format, ...);
#elif __GNUC__
#define debug(format, args...) ((void)0)
#else
void debug(char *format, ...);
#endif

// Table size helper
#define TABLESIZE(table) (sizeof(table)/sizeof(table[0]))

// True and false
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/**
 * @ingroup ITypes
 * @defgroup IConsts Internal constants
 * @brief Various constants for internal library use
 * @{
 */
#define OI_MAX_DEVICES 64                                              /**< Max number of attached devices */
#define OI_MAX_EVENTS 128                                              /**< Size of event queue */
#define OI_SLEEP 1                                                     /**< Ms to sleep in busy wait-loop */
#define OI_MIN_KEYLENGTH 5                                             /**< Min symbolic event name */
#define OI_MAX_KEYLENGTH 20                                            /**< Max symbolic event name */

#define OI_JOY_TAB_AXES 0                                              /**< Lookup table offset for joystick axes */
#define OI_JOY_TAB_BTNS 1                                              /**< Lookup table offset for joystick buttons */
/**
 * @}
 */

/* ******************************************************************** */

#endif
