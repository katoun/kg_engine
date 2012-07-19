/*
 * openinput_api.h : Main API
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

#ifndef _OPENINPUT_API_H_
#define _OPENINPUT_API_H_

#ifndef _OPENINPUT_H_
#error Do not include this file directly - use openinput.h
#endif

/* ******************************************************************** */

// Default initialization of all available devices (num_failed)
extern DECLSPEC int OICALL oi_init(char* window_id,
                                   unsigned int flags);

// Shutdown all available devices (num_failed)
extern DECLSPEC int OICALL oi_close();

/* ******************************************************************** */

// Get device information (errorcode)
extern DECLSPEC int OICALL oi_device_info(unsigned char index,
                                          char **name,
                                          char **desc,
                                          unsigned int *provides);

// Enable/disable device event pumping (state)
extern DECLSPEC oi_bool OICALL oi_device_enable(unsigned char index,
                                                oi_bool q);

/* ******************************************************************** */

// Look at event without removing it from queue (number_returned)
extern DECLSPEC int OICALL oi_events_peep(oi_event *evts,
                                          int num);

// Add an event to the tail of the queue (number_added)
extern DECLSPEC int OICALL oi_events_add(oi_event *evts,
                                         int num);

// Pump all device to transfer events into queue (n/a)
extern DECLSPEC void OICALL oi_events_pump();

// Poll events (more_pending)
extern DECLSPEC int OICALL oi_events_poll(oi_event *evt);

// Wait for an event (n/a)
extern DECLSPEC void OICALL oi_events_wait(oi_event *evt);

// Set event type filter mask (n/a)
extern DECLSPEC void OICALL oi_events_setmask(unsigned int mask);

// Get event type filter mask (event_mask)
extern DECLSPEC unsigned int OICALL oi_events_getmask();

/* ******************************************************************** */

// Send events for down-state keys (errorcode)
extern DECLSPEC int OICALL oi_key_repeat(int delay,
                                         int interval);

// Get key state table and set num to number of elements (pointer)
extern DECLSPEC char * OICALL oi_key_keystate(unsigned char index,
                                              int *num);

// Return modifier mask (modifier_mask)
extern DECLSPEC unsigned int OICALL oi_key_modstate(unsigned char index);

// Get name of key (string)
extern DECLSPEC char * OICALL oi_key_getname(oi_key key);

// Get key code given name (oi_key)
extern DECLSPEC oi_key OICALL oi_key_getcode(char *name);

/* ******************************************************************** */

// Get absolute position of mouse (button_mask)
extern DECLSPEC int OICALL oi_mouse_absolute(unsigned char index,
                                             int *x,
                                             int *y);

// Get relative motion of mouse (button_mask)
extern DECLSPEC int OICALL oi_mouse_relative(unsigned char index,
                                             int *x,
                                             int *y);

// Warp mouse cursor position (errorcode)
extern DECLSPEC int OICALL oi_mouse_warp(unsigned char index,
                                         int x,
                                         int y);

// Get name of mouse button (string)
extern DECLSPEC char * OICALL oi_mouse_getname(oi_mouse button);

// Get mouse-id given name (oi_mouse)
extern DECLSPEC oi_mouse OICALL oi_mouse_getcode(char *name);

/* ******************************************************************** */

// Get absolute position of joystick axis (button_mask)
extern DECLSPEC unsigned int OICALL oi_joy_absolute(unsigned char index,
                                                    unsigned char axis,
                                                    int *value,
                                                    int *second);

// Get relative motion of joystick axis (button_mask)
extern DECLSPEC unsigned int OICALL oi_joy_absolute(unsigned char index,
                                                    unsigned char axis,
                                                    int *value,
                                                    int *second);

// Get name of joystick button/axis (string)
extern DECLSPEC char * OICALL oi_joy_getname(unsigned int code);

// Get mouse-id given name (oi_mouse)
extern DECLSPEC unsigned int OICALL oi_joy_getcode(char *name);

// Get basic information about a joystick device (errorcode)
extern DECLSPEC int OICALL oi_joy_info(unsigned char index,
                                       char **name,
                                       int *buttons,
                                       int *axes);

// Get advanced axes information for a joystick device (errorcode)
extern DECLSPEC int OICALL op_joy_axessetup(unsigned char index,
                                            oi_joytype *type[],
                                            unsigned char *pair[],
                                            int *num);

/* ******************************************************************** */

// Get focus state of application (focus_mask)
extern DECLSPEC unsigned int OICALL oi_app_focus();

// Show/hide cursor (state)
extern DECLSPEC oi_bool OICALL oi_app_cursor(oi_bool q);

// Grab/ungrab input (state)
extern DECLSPEC oi_bool OICALL oi_app_grab(oi_bool q);

/* ******************************************************************** */

// Install actionmap (errorcode)
extern DECLSPEC int OICALL oi_action_install(oi_actionmap *map,
                                             int num);

// Check/validate single actionmap structure (errorcode)
extern DECLSPEC int OICALL oi_action_validate(oi_actionmap *map);

// Get action state table and set num to number of elements (pointer)
extern DECLSPEC char * OICALL oi_action_actionstate(int *num);

/* ******************************************************************** */

#endif
