/*
 * win32.h : Microsoft Windows input device driver
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

#ifndef _OPENINPUT_WIN32_H_
#define _OPENINPUT_WIN32_H_

/* ******************************************************************** */

// Bootstrap
int win32_avail(unsigned int flags);
oi_device *win32_device();

// Device
int win32_init(oi_device *dev, char *window_id, unsigned int flags);
int win32_destroy(oi_device *dev);
void win32_process(oi_device *dev);
int win32_grab(oi_device *dev, int on);
int win32_hidecursor(oi_device *dev, int on);
int win32_warp(oi_device *dev, int x, int y);
int win32_winsize(oi_device *dev, int *w, int *h);

/* ******************************************************************** */

// Misc local functions
struct win32_private;
void win32_setdevhook(oi_device *dev);
void win32_initkeymap();
void win32_keystate(oi_device *dev);
oi_keysym *win32_translate(struct win32_private *priv,
                                  WPARAM wparam, LPARAM lparam,
                                  char down, oi_keysym *keysym);
LONG CALLBACK win32_wndproc(HWND hwnd, unsigned int msg,
                            WPARAM wparam, LPARAM lparam);
void win32_trackmouse();
void win32_relative_mouse(int x, int y);
void win32_movesize();

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Foo driver private instance data
 *
 * Private data is private for the particular
 * instance of the device driver.
 */
typedef struct win32_private {
    HWND hwnd;                    /**< Window handle */
    WNDPROC old_wndproc;          /**< Old window procedure handle */
    RECT rect;                    /**< Window bounds for mouse clipping */
    unsigned int relative;        /**< Relative mouse movement bitmask */
    int winx;                     /**< Window x coordinate */
    int winy;                     /**< Window y coordinate */
    int width;                    /**< Window width */
    int height;                   /**< Window height */
    char shiftleft;               /**< Previous left shift button state */
    char shiftright;              /**< Previous right shift button state */
} win32_private;

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @{
 */
#define VK_SEMICOLON     0xBA    /**< key_semicolon */
#define VK_EQUALS        0xBB    /**< key_equals */
#define VK_COMMA         0xBC    /**< key_comma */
#define VK_MINUS         0xBD    /**< key_minus */
#define VK_PERIOD        0xBE    /**< key_period */
#define VK_SLASH         0xBF    /**< key_slash */
#define VK_GRAVE         0xC0    /**< key_backquote */
#define VK_LBRACKET      0xDB    /**< key_leftbracket */
#define VK_BACKSLASH     0xDC    /**< key_backslash */
#define VK_RBRACKET      0xDD    /**< key_rightbracket */
#define VK_APOSTROPHE    0xDE    /**< key_quote */
#define VK_BACKTICK      0xDF    /**< key_backquote */

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL    0x020A  /**< Mouse wheel event */
#endif

#define DW32_KEYTABLE    256     /**< Keytable size */
#define DW32_REPKEYMASK  (1<<30) /**< Repeated key bit */
#define DW32_EXTENDMASK  (1<<24) /**< Extended key bit */
#define DW32_GRAB        1       /**< Mouse grabbed */
#define DW32_HIDE        2       /**< Mouse hidden */
/** @} */

/* ******************************************************************** */

#endif
