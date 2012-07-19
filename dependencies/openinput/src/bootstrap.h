/*
 * bootstrap.h : Header for platform bootstrap functions
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

#ifndef _OPENINPUT_BOOTSTRAP_H_
#define _OPENINPUT_BOOTSTRAP_H_

/* ******************************************************************** */

// Bootstrap function forward definitions
#ifdef ENABLE_FOO
extern oi_bootstrap foo_bootstrap;
#endif
#ifdef ENABLE_X11
extern oi_bootstrap x11_bootstrap;
#endif
#ifdef ENABLE_UNIXSIGNAL
extern oi_bootstrap unixsignal_bootstrap;
#endif
#ifdef ENABLE_LINUXJOY
extern oi_bootstrap linuxjoy_bootstrap;
#endif
#ifdef ENABLE_WIN32
extern oi_bootstrap win32_bootstrap;
#endif
#ifdef ENABLE_DX9
extern oi_bootstrap dx9_bootstrap;
#endif

/* ******************************************************************** */

// Global variable for "device.c" only! All drivers must fill in an entry
#ifdef _DEVICE_FILLER_

/**
 * @ingroup ITypes
 * @brief The bootstrap table
 *
 * This array of bootstrap structures is parsed when OpenInput is
 * initialized. Each driver is investigated and the device initialized
 * if the driver successfully detects the device.
 */
static oi_bootstrap *bootstrap[] = {

    // Begin list
#ifdef ENABLE_FOO
    &foo_bootstrap,
#endif

#ifdef ENABLE_X11
    &x11_bootstrap,
#endif

#ifdef ENABLE_UNIXSIGNAL
    &unixsignal_bootstrap,
#endif

#ifdef ENABLE_LINUXJOY
    &linuxjoy_bootstrap,
#endif

#ifdef ENABLE_WIN32
    &win32_bootstrap,
#endif

#ifdef ENABLE_DX9
    &dx9_bootstrap,
#endif

    // Terminate list
    NULL
};
#endif

/* ******************************************************************** */

#endif
