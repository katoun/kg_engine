/*
 * win32_events.c : Win32 event dispatcher
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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "openinput.h"
#include "internal.h"
#include "win32.h"

// Globals
static oi_device *device;
static win32_private *private;

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Set global device hook
 *
 * @param dev pointer to device structure
 *
 * Since Windows uses those ugly callbacks, we need to
 * have a hook to the device structure in order to access
 * device and private structures. This doesn't matter much
 * as Win32 isn't network-transparent nor client-server oriented...
 * Actually, Win32 is fscking lame!
 */
void win32_setdevhook(oi_device *dev) {
    device = dev;
    private = (win32_private*)dev->private;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief The window procedure
 *
 * @param hwnd window handle
 * @param msg message id
 * @param wparam word-length paramter for message
 * @param lparam long-length paramter for message
 * @returns 0 if message was handled, otherwise undefined
 *
 * This is the main Windows callback procedure, which is called
 * whenever the application dispatches messages. This is where
 * we handle all the nitty-gritty inputs!
 *
 * The events we handle are eaten. Neither the default nor the
 * old application handler is called if we catch an event.
 */
LONG CALLBACK win32_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    // Handle
    switch(msg) {


        // Receive/loose focus
    case WM_ACTIVATE:
        {
            unsigned short minimize;
            char gain;
            unsigned int mask;

            minimize = HIWORD(wparam);
            gain = !minimize && (LOWORD(wparam) != WA_INACTIVE);
            mask = OI_FOCUS_INPUT;
            debug("win32_wndproc: activate - minimize:%u gain:%u", minimize, gain);

            // Visibility changes on gain and loose+minimize
            if(gain || (!gain && minimize)) {
                mask |= OI_FOCUS_VISIBLE;
            }

            // Update keyboard state on focus gain
            if(gain) {
                win32_keystate(device);
            }

            // Post (index, gain, mask, post)
            appstate_focus(device->index, gain, mask, TRUE);
        }
        return 0;


        // Mouse leaves window
#ifdef WM_MOUSELEAVE
    case WM_MOUSELEAVE:
        {
            debug("win32_wndproc: mouse leave");
            appstate_focus(device->index, FALSE, OI_FOCUS_MOUSE, TRUE);
        }
        return 0;
#endif

        // Window moved or resized
    case WM_SIZE:
    case WM_MOVE:
        {
            debug("win32_wndproc: size/move");
            win32_movesize();
        }
        return 0;


        // Keyboard presses and releases event
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYUP:
        {
            oi_keysym keysym;
            char state;

            state = (msg == WM_SYSKEYDOWN) || (msg == WM_KEYDOWN);
            debug("win32_wndproc: key up/down - down:%u", state);

            // Don't post repeated keys
            if(state && (lparam & DW32_REPKEYMASK)) {
                return 0;
            }

            // Translate and post
            win32_translate(private, wparam, lparam, state, &keysym);
            keyboard_update(device->index, &keysym, state, TRUE);
        }
        return 0;


        // Mouse motion
    case WM_MOUSEMOVE:
        {
            int x;
            int y;
            debug("win32_wndproc: mouse move");

            // Did the mouse enter the window?
            if(!(oi_app_focus() & OI_FOCUS_MOUSE)) {
                win32_trackmouse();
                appstate_focus(device->index, TRUE, OI_FOCUS_MOUSE, TRUE);
            }

            // Standard movement handling
            x = LOWORD(lparam);
            y = HIWORD(lparam);
            if(private->relative == (DW32_GRAB|DW32_HIDE)) {
                win32_relative_mouse(x, y);
            }
            else {
                mouse_move(device->index, x, y, FALSE, TRUE);
            }
        }
        return 0;


        // Left mouse button
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        {
            debug("win32_wndproc: left mouse button");
            mouse_button(device->index, OIP_BUTTON_LEFT,
                         (char)(msg == WM_LBUTTONDOWN), TRUE);
        }
        return 0;


        // Middle mouse button
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
        {
            debug("win32_wndproc: middle mouse button");
            mouse_button(device->index, OIP_BUTTON_MIDDLE,
                         (char)(msg == WM_MBUTTONDOWN), TRUE);
        }
        return 0;


        // Right mouse button
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        {
            debug("win32_wndproc: right mouse button");
            mouse_button(device->index, OIP_BUTTON_RIGHT,
                         (char)(msg == WM_RBUTTONDOWN), TRUE);
        }
        return 0;


#ifdef WM_MOUSEWHEEL
        // Mouse wheel
    case WM_MOUSEWHEEL:
        {
            oi_mouse btn;
            debug("win32_wndproc: mouse wheel");
            if((short)HIWORD(wparam) > 0) {
                btn = OIP_WHEEL_UP;
            }
            else {
                btn = OIP_WHEEL_DOWN;
            }
            mouse_button(device->index, btn, TRUE, TRUE);
        }
        return 0;
#endif

        // Close
    case WM_CLOSE:
    case WM_DESTROY:
        {
            oi_event ev;
            debug("win32_wndproc: close/destroy");
            ev.type = OI_QUIT;
            queue_add(&ev);
            PostQuitMessage(0);
        }
        return 0;


        // Erase background
    case WM_ERASEBKGND:
        {
            oi_event ev;
            debug("win32_wndproc: erase background");
            ev.type = OI_EXPOSE;
            queue_add(&ev);
        }
        // Fall through here to allow standard handler to repaint
        break;
    }

    // Call old window-proc to handle redraw etc.
    if(private->old_wndproc) {
        return CallWindowProc(private->old_wndproc, hwnd, msg,
                              wparam, lparam);
    }
    else {
        // The catch-all default handler
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Enable mouse trakcing
 *
 * In order to catch mouse leave events, we
 * must enable mouse tracking.
 */
void win32_trackmouse() {
#ifdef TRACKMOUSEEVENT
    TRACKMOUSEEVENT tme;

    tme.cbSize = sizeof(tme);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = private->hwnd;

    TrackMouseEvent(&tme);
#endif
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Handle relative mouse movement
 *
 * @param x horizontal position
 * @param y vertical position
 *
 * When the mouse is grabbed and the cursor is hidden
 * we are in relative mode - the cursor may hit
 * the window boundary, but we still have to fire
 * mouse movement events, as the user can't see the cursor.
 * We do this by warping the mouse to the window center
 */
void win32_relative_mouse(int x, int y) {
    POINT cen;
    int rx;
    int ry;

    // Get center and relative motion
    cen.x = private->width/2;
    cen.y = private->height/2;
    rx = x - cen.x;
    ry = y - cen.y;

    // Movement did happen
    if(rx || ry) {
        // Center cursor
        ClientToScreen(private->hwnd, &cen);
        SetCursorPos(cen.x, cen.y);
        mouse_move(device->index, rx, ry, TRUE, TRUE);
    }
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Window was moved or resized
 *
 * When the user moved or resized the window, query
 * for new settings, such that the mouse grab boundary
 * can be updated.
 */
void win32_movesize() {
    RECT rc;

    // Get "outer" position
    GetWindowRect(private->hwnd, &rc);
    private->winx = rc.left;
    private->winy = rc.top;

    // Store new boundaries for mouse grabbing
    GetClientRect(private->hwnd, &private->rect);

    // Did anything change?
    if((private->width == private->rect.right) &&
       (private->height == private->rect.bottom)) {
        return;
    }

    // Update and post event
    private->width = rc.right;
    private->height = rc.bottom;
    appstate_resize(device->index, private->width, private->height, TRUE);
}

/* ******************************************************************** */
