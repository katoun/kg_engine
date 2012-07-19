/*
 * debug.c : Debugging helpers
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
#include <stdio.h>
#include <stdarg.h>
#include "openinput.h"
#include "internal.h"

/* ******************************************************************** */

#ifdef DEBUG

/**
 * @ingroup IMain
 * @brief Debug printf-helper
 *
 * @param format string pointer with printf-formatting
 * @param ... more arguments (varargs)
 *
 * Print debug information to standard-error. You should use this
 * function instead of regular printf's, as this function is only
 * used on debug-enabled builds.
 */
void debug(char* format, ...) {
    va_list args;

    fprintf(stderr, "openinput: ");

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, "\n");
}
#endif

/* ******************************************************************** */

#if !defined(DEBUG) && !defined(__GNUC__)

/**
 * @ingroup IMain
 * @brief Debug printf-helper, dummy version
 *
 * @param format string pointer with printf-formatting
 * @param ... more arguments (varargs)
 *
 * Empty function. We need this since MSVC does not allow for
 * vararg-style preprocessor macros. MSVC is broken!
 */
void debug(char* format, ...) {
}

#endif

/* ******************************************************************** */
