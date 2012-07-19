/*
 * win32-config.h : Toplevel "config.h" for Windows platforms
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

/* **** NOTE / HOWTO / WHAT IS THIS ****
 *
 * If you do not use a UNIX-based build system on Windows, for example
 * if you use the MSVC compiler, you MIGHT need to copy THIS file
 * into the top-level OpenInput directory.
 *
 * This file contains some very simply build-instructions for the
 * OpenInput library targetted for the Windows platform.
 *
 * If you think this is strange, the answer is yes. But since
 * Windows does not support GNU Automake, nor is able to run
 * POSIX shell scripts, this is the only feasible way to make
 * OpenInput somewhat "portable" without making some very nasty
 * header-file hacks.
 *
 * You could also just install Cygwin or Mingw, after which you
 * have a basic POSIX-compliant system on Windows so you can
 * run the standard "configure; make; make install" build process.
 */

/* ******************************************************************** */

/* Microsoft Windows input driver */
#define ENABLE_WIN32 1

/* ANSI signal handler */
/* #undef ENABLE_UNIXSIGNAL 1 */

/* Name of package */
#define PACKAGE "openinput"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "makob@makob.dk"

/* Define to the full name of this package. */
#define PACKAGE_NAME "openinput"

/* Internal debugging */
/* #undef DEBUG */

/* Debug input system */
/* #undef ENABLE_FOO */
