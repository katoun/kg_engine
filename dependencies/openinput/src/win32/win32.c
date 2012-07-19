/*
 * win32.c: Microsoft Windows input device driver
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
#include <windows.h>
#include "openinput.h"
#include "internal.h"
#include "bootstrap.h"
#include "win32.h"

/**
 * @ingroup Drivers
 * @defgroup DWin32 Microsoft Windows input driver
 *
 * The Windows input driver handles mouse and keyboard
 * under Microsoft Windows using the standard Windows
 * message API, which is supported on all Win32 platforms
 * (starting with Windows95).
 */

// Bootstrap global
oi_bootstrap win32_bootstrap = {
    "win32",
    "Microsoft Windows input system",
    OI_PRO_MOUSE | OI_PRO_KEYBOARD,
    win32_avail,
    win32_device
};

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Check availablity of win32 device
 *
 * @param flags initialization flags, see @ref PFlags
 * @returns true (1) if device is present, false (0) otherwise
 *
 * This is a bootstrap function.
 *
 * We're available if we're allowed to hook into an exisiting
 * window and that the Windows version is running on an NT
 * kernel. The latter is a requirement due to the mouse
 * tracking handling which is not available on all version
 * of Win95.
 */
int win32_avail(unsigned int flags) {
    DWORD ver;

    debug("win32_avail");

    // Check flags
    if(flags & OI_FLAG_NOWINDOW) {
        return FALSE;
    }

    // Require WinNT kernel
    ver = GetVersion();
    if(ver >= 0x80000000) {
        return FALSE;
    }

    // Version is ok
    return TRUE;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Create win32 device structure
 *
 * @returns pointer to allocated device interface
 *
 * This is a bootstrap function.
 *
 * Create the private data structure and device interface.
 */
oi_device *win32_device() {
    oi_device *dev;
    win32_private *priv;

    debug("win32_device");

    // Alloc device and private data
    dev = (oi_device*)malloc(sizeof(oi_device));
    priv = (win32_private*)malloc(sizeof(win32_private));
    if((dev == NULL) || (priv == NULL)) {
        debug("win32_device: device creation failed");
        if(dev) {
            free(dev);
        }
        if(priv) {
            free(priv);
        }
        return NULL;
    }

    // Clear structures
    memset(dev, 0, sizeof(oi_device));
    memset(priv, 0, sizeof(win32_private));

    // Set members
    dev->init = win32_init;
    dev->destroy = win32_destroy;
    dev->process = win32_process;
    dev->grab = win32_grab;
    dev->hide = win32_hidecursor;
    dev->warp = win32_warp;
    dev->winsize = win32_winsize;
    dev->reset = NULL;
    dev->private = priv;

    // Done
    return dev;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Initialize the win32 device driver
 *
 * @param dev pointer to device interface
 * @param window_id window hook paramaters, see @ref PWindow
 * @param flags initialization flags, see @ref PFlags
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * Initialize the Windows driver. This includes setting
 * up most of the private data block members and installing
 * a new windows procedure, which is how we 'hook' into
 * an existing/pre-created window.
 */
int win32_init(oi_device *dev, char *window_id, unsigned int flags) {
    win32_private *priv;

    // Set some stupid private values
    priv = (win32_private*)dev->private;
    debug("win32_init");

    // Get the window handle or bail
    priv->hwnd = (HWND)device_windowid(window_id, OI_I_WINID);
    if(!priv->hwnd) {
        debug("win32_init: winid (w) parameter required");
        return OI_ERR_NO_DEVICE;
    }

    // Get current window-procedure
#ifdef GWLP_WNDPROC
    priv->old_wndproc = (WNDPROC)GetWindowLongPtr(priv->hwnd, GWLP_WNDPROC);
#else
    priv->old_wndproc = (WNDPROC)GetWindowLong(priv->hwnd, GWL_WNDPROC);
#endif
    if(!priv->old_wndproc) {
        debug("win32_init: invalid window handle");
        return OI_ERR_NO_DEVICE;
    }

    // Set device pointer for callback functions
    win32_setdevhook(dev);

    // Initialize keymap table, size/position state and track mouse
    win32_initkeymap();
    win32_keystate(dev);
    win32_movesize();
    win32_trackmouse();

    // Lastly, install our own window handle
#ifdef GWLP_WNDPROC
    SetWindowLongPtr(priv->hwnd, GWLP_WNDPROC, (LONG)win32_wndproc);
#else
	SetWindowLong(priv->hwnd, GWL_WNDPROC, (LONG)win32_wndproc);
#endif
    debug("win32_init: initialized");

    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Shutdown the win32 device
 *
 * @param dev pointer to device interface
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * Shutdown the win32 driver by installing the
 * old windows-procedure (message handler) and
 * freeing all private data.
 */
int win32_destroy(oi_device *dev) {
    win32_private *priv;

    debug("win32_destroy");

    if(dev) {
        priv = (win32_private*)dev->private;

        // Private data
        if(priv) {
            // Set old window-procedure
            if(priv->old_wndproc) {
#ifdef GWLP_WNDPROC
                SetWindowLongPtr(priv->hwnd, GWLP_WNDPROC, (LONG)priv->old_wndproc);
#else
				SetWindowLong(priv->hwnd, GWL_WNDPROC, (LONG)priv->old_wndproc);
#endif
            }
            free(dev->private);
        }

        // Device struct
        free(dev);
        dev = NULL;
    }

    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Process events
 *
 * @param dev pointer to device interface
 *
 * This is a device interface function.
 *
 * Process any pending events in the queue. This is simply
 * done by fetching the events, translating any keypresses
 * etc. to something readible and finally dispatching the message
 * the the window procedures. Windows uses the wndproc-callback
 * interface - see the win32_wndproc function in win32_events.c
 */
void win32_process(oi_device *dev) {
    MSG msg;
    win32_private *priv;

    priv = (win32_private*)dev->private;

    // Process
    if(!oi_runstate()) {

        // Peekaboo and dispatch messages to the window-procedure
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Grab/release pointer
 *
 * @param dev pointer to device interface
 * @param on true (1) turns on grab, false (0) releases grab
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * Grab the mouse pointer, and focus the window
 * to gain keyboard focus aswell.
 */
int win32_grab(oi_device *dev, int on) {
    win32_private *priv;
    POINT pt;

    priv = (win32_private*)dev->private;
    debug("win32_grab");

    if(on) {
        // Center mouse
        pt.x = priv->width/2;
        pt.y = priv->height/2;
        ClientToScreen(priv->hwnd, &pt);
        SetCursorPos(pt.x, pt.y);

        // Grab mouse and focus keyboard
        ClipCursor(&priv->rect);
        SetFocus(priv->hwnd);

        priv->relative |= DW32_GRAB;
    }
    else {
        // Ungrab mouse
        ClipCursor(NULL);
        priv->relative &= ~DW32_GRAB;
    }

    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Show/hide pointer
 *
 * @param dev pointer to device interface
 * @param on true (1) hides cursor, false (0) shows cursor
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * Simply hide/show the mouse pointer.
 */
int win32_hidecursor(oi_device *dev, int on) {
    win32_private *priv;
    priv = (win32_private*)dev->private;
    debug("win32_hidecursor");

    // Hide
    if(on) {
        ShowCursor(FALSE);
        priv->relative |= DW32_HIDE;
    }
    else {
        ShowCursor(TRUE);
        priv->relative &= ~DW32_HIDE;
    }

    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Warp pointer
 *
 * @param dev pointer to device interface
 * @param x pointer to horizontal position
 * @param y pointer to vertical position
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * Move (warp) the mouse pointer given coordinates
 * relative to the window.
 */
int win32_warp(oi_device *dev, int x, int y) {
    win32_private *priv;
    priv = (win32_private*)dev->private;
    debug("win32_warp");

    // Post event if we're in relative mode
    if(priv->relative == (DW32_GRAB | DW32_HIDE)) {
        mouse_move(dev->index, x, y, FALSE, FALSE);
    }
    else {
        // Absolute mode, update position
        POINT pt;
        pt.x = x;
        pt.y = y;
        ClientToScreen(priv->hwnd, &pt);
        SetCursorPos(pt.x, pt.y);
    }

    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup DWin32
 * @brief Get window size
 *
 * @param dev pointer to device interface
 * @param w pointer to horizontal size
 * @param h pointer to vertical size
 * @returns errorcode, see @ref PErrors
 *
 * This is a device interface function.
 *
 * Simply fill out the width and height of the
 * window. Both values are cached in the private
 * structure.
 */
int win32_winsize(oi_device *dev, int *w, int *h) {
    win32_private *priv;
    priv = (win32_private*)dev->private;
    debug("win32_winsize");

    // Update driver width/height/position
    win32_movesize();

    // Safely store values
    if(w) {
        *w = priv->width;
    }
    if(h) {
        *h = priv->height;
    }

    return OI_ERR_OK;
}

/* ******************************************************************** */
