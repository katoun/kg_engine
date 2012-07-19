/*
 * device.c : The generic device functions
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
#include "openinput.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "internal.h"
#include "private.h"

// Globals
static oi_device *devices[OI_MAX_DEVICES];
static oi_private *private[OI_MAX_DEVICES];
static char devices_run[OI_MAX_DEVICES];
static unsigned int num_devices;
static char more_avail;

// Include the bootstrap table
#define _DEVICE_FILLER_
#include "bootstrap.h"
#undef _DEVICE_FILLER_

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Initialize device manager
 *
 * @returns errorcode, see @ref PErrors
 *
 * Called on library initialization. This functions prepares the
 * device manager before devices are booted and initialized.
 */
int device_init() {
    unsigned char i;

    for(i=0; i<OI_MAX_DEVICES; i++) {
        devices[i] = NULL;
        private[i] = NULL;
        devices_run[i] = FALSE;
    }
    num_devices = 0;
    more_avail = FALSE;

    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Bootstrap all devices
 *
 * @param window_id window hook parameters, see @ref PWindow
 * @param flags library initization flags
 *
 * This function parses the bootstrap table and
 * registers (bootstraps and initializes) all devices.
 */
void device_bootstrap(char *window_id, unsigned int flags) {
    unsigned int i;
    unsigned int j;
    int ok;

    debug("device_bootstrap");

    // Critical error - bootstrap is empty
    if(!bootstrap[0]) {
        return;
    }
    if(num_devices != 0) {
        debug("device_bootstrap: devices already initialized");
        return;
    }

    // Fill structure array with available devices
    j = 0;
    for(i=0; bootstrap[i]; i++) {
        debug("device_bootstrap: checking bootstrap entry %u", i, bootstrap[i]->name);

        // Some drivers may control more devices, allow them using the more_avail callback
        more_avail = FALSE;
        do {
            // Be pessimistic
            ok = FALSE;

            // Check bootstrap entry
            if(bootstrap[i] &&
               bootstrap[i]->avail && bootstrap[i]->create &&
               bootstrap[i]->name && bootstrap[i]->desc) {

                // Check that the device is available
                if(bootstrap[i]->avail(flags)) {

                    // Register device, but continue even if that fails
                    ok = TRUE;
                    if(device_register(bootstrap[i], window_id, flags) == OI_ERR_OK) {
                        j++;
                    }
                }
            }

            // Continue till register fails or driver exhausted
        } while(ok && more_avail);
    }

    debug("device_bootstrap: %u drivers compiled in, %u devices analyzed" \
          " and %u available", i, j, num_devices);
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Register new device via bootstrap
 *
 * @param boot pointer to bootstrap structure
 * @param window_id window hook parameters, see @ref PWindow
 * @param flags library initialization flags, see @ref PFlags
 * @returns errorcode, see @ref PErrors
 *
 * @pre bootstrap structure pointed to by "boot" must be valid
 *
 * Helper function the initialize a device driver. This
 * is automatically called for all "available" devices on
 * library initilization
 */
int device_register(oi_bootstrap *boot, char *window_id, unsigned int flags) {
    oi_event ev;

    // Create the device
    devices[num_devices] = boot->create();
    if(devices[num_devices] == NULL) {
        return OI_ERR_NO_DEVICE;
    }

    // Check required functions
    if(!devices[num_devices]->init ||
       !devices[num_devices]->destroy ||
       !devices[num_devices]->process) {

        // Device creation failed, abort
        return OI_ERR_NOT_IMPLEM;
    }

    devices[num_devices]->index = num_devices+1;

    // Some drivers are lazy, others are advanced
    if(!devices[num_devices]->name) {
        devices[num_devices]->name = boot->name;
    }
    if(!devices[num_devices]->desc) {
        devices[num_devices]->desc = boot->desc;
    }
    if(!devices[num_devices]->provides) {
        devices[num_devices]->provides = boot->provides;
    }

    // Allocate managment data placeholder
    private[num_devices] = (oi_private*)malloc(sizeof(oi_private));
    memset(private[num_devices], 0, sizeof(oi_private));

    // Initialize managment data
    keyboard_manage(&(private[num_devices]->key), devices[num_devices]->provides);
    mouse_manage(&(private[num_devices]->mouse), devices[num_devices]->provides);
    joystick_manage(&(private[num_devices]->joy), devices[num_devices]->provides);

    // Ok, initialize the device
    if(devices[num_devices]->init(devices[num_devices], window_id, flags) != OI_ERR_OK) {

        // Init failed, free the device structure and abort
        devices[num_devices]->destroy(devices[num_devices]);
        return OI_ERR_NO_DEVICE;
    }

    // Send the discovery event
    ev.type = OI_DISCOVERY;
    ev.discover.device = num_devices+1;
    ev.discover.name = devices[num_devices]->name;
    ev.discover.description = devices[num_devices]->desc;
    ev.discover.provides = devices[num_devices]->provides;
    queue_add(&ev);

    // Enable device for event pumping
    devices_run[num_devices] = TRUE;
    debug("device_bootstrap: device '%s' (%s) added as index %i",
          devices[num_devices]->name, devices[num_devices]->desc, num_devices+1);

    // Ok, we're done
    num_devices++;
    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Destroy a device
 *
 * @param index device index
 * @returns errorcode, see @ref PErrors
 *
 * Shutdown a device.
 */
int device_destroy(unsigned char index) {
    oi_device *dev;

    debug("device_destroy");

    // Dummy check
    dev = device_get(index);
    if(!dev) {
        return OI_ERR_INDEX;
    }

    // Free private manager data
    if(private[index-1]) {
        if(private[index-1]->joy) {
            free(private[index-1]->joy);
        }
        if(private[index-1]->key) {
            free(private[index-1]->key);
        }
        if(private[index-1]->mouse) {
            free(private[index-1]->mouse);
        }
        free(private[index-1]);
    }

    // Kill device
    return dev->destroy(dev);
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Driver can initialize more devices
 *
 * @param more true (1) for more available, false (0) otherwise
 *
 * Tell the device initialization framework, that the device driver
 * who's "bootstrap->create" or "bootstrap->avail" function is
 * currently run can (possibly) initialize more drivers. This could be
 * the case if a system has two or more joysticks connected.
 *
 * You do not have to use this control - it is assumed that drivers
 * control only a single device. If however, your driver can control
 * more devices, call this function with the "more" param set to true.
 * The device driver framework will then do another pass at the
 * avail/create invokation. This can be repeated as long as the
 * device table is not full.
 */
void device_moreavail(char more) {
    if(more) {
        more_avail = TRUE;
    }
    else {
        more_avail = FALSE;
    }
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Get device structure
 *
 * @param index device index
 * @returns pointer to device structure
 *
 * Fetch a device structure given device index.
 */
oi_device *device_get(unsigned char index) {
    // Dummy check
    if((index < 1) || (index > num_devices)) {
        // debug("device_get: no device, index %i", index);
        return NULL;
    }

    return devices[index-1];
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Get private managment data
 *
 * @param index device index
 * @param manager provide-code (see @ref PProvide) of
 * manager structure to return
 * @returns pointer to management structure, or NULL if not found
 *
 * Get pointer to one of the private managment structures in
 * a safe way. You'll have to throw in a typecast since we
 * return a void-pointer to make the function versatile.
 */
void *device_priv(unsigned char index, unsigned int manager) {

    // Dummy check
    if(!private[index-1]) {
        // debug("device_priv: no private struct, index %i", index);
        return NULL;
    }

    // Return the manager data
    switch(manager) {
    case OI_PRO_KEYBOARD:
        return private[index-1]->key;

    case OI_PRO_MOUSE:
        return private[index-1]->mouse;

    case OI_PRO_JOYSTICK:
        return private[index-1]->joy;

    default:
        return NULL;
    }
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Pump events from all devices
 *
 * Run through all devices and process them, ie.
 * make them pump events into the queue.
 */
void device_pumpall() {
    unsigned int i;

    for(i=0; i<num_devices; i++) {
        // Only pump devices if it's there and enabled!
        if(devices[i] && (devices_run[i] == TRUE)) {
            // debug("device_pumpall: now pumping device index %i", i);
            devices[i]->process(devices[i]);
        }
    }
}

/* ******************************************************************** */

/**
 * @ingroup IDevice
 * @brief Parse window_id init string
 *
 * @param str pointer to window_id string, see @ref PWindow
 * @param tok parameter to parse, see @ref PWindow
 * @returns converted parameter (as an unsigned int)
 *
 * Parse and convert string parameter to an unsigned long. Basically
 * this is just an advanced atoi-function.
 */

unsigned int device_windowid(char *str, char tok) {
    char match[5];
    char *sub;
    int e;
    unsigned int val;

    memset(match, 0, 5);

    // Set token to find, and scan
    match[0] = tok;
    strcat(match, ":%u");

    // Find sub-string index (index of tok in str)
    sub = str;
    while(sub < str+strlen(str)) {
        if(*sub == tok) {
            break;
        }
        sub++;
    }

    e = sscanf(sub, match, &val);
    if(e != 1) {
        debug("device_windowid: parameter not found");
        return 0;
    }

    return val;
}

/* ******************************************************************** */

/**
 * @ingroup PDevice
 * @brief Enable/disable a device
 *
 * @param index index of device to enable/disable
 * @param q new state of device or query, see @ref PBool
 * @returns state of device, OI_QUERY on error
 *
 * Use this function to turn a device on or off. Devices that are
 * off do not generate events and do not update the state managers.
 * All devices defaults to "on".
 *
 * Enabling/disabling a device will force a full driver resynchronization.
 */
oi_bool oi_device_enable(unsigned char index, oi_bool q) {
    char enable;

    // Check that the devices exists
    if(device_get(index) == NULL) {
        return OI_QUERY;
    }

    switch(q) {
    case OI_ENABLE:
        enable = TRUE;
        break;

    case OI_DISABLE:
        enable = FALSE;
        break;

    case OI_QUERY:
        if(devices_run[index-1]) {
            return OI_ENABLE;
        }
        else {
            return OI_DISABLE;
        }
    }

    // Reset driver
    if(devices[index-1] && devices[index-1]->reset) {
        devices[index-1]->reset(devices[index-1]);
    }

    // Set new device state
    devices_run[index-1] = enable;
    return q;
}

/* ******************************************************************** */

/**
 * @ingroup PDevice
 * @brief Get information about a device
 *
 * @param index index of device to get info about
 * @param name pointer to string with device name
 * @param desc pointer to string with device description
 * @param provides pointer to provide flags, see @ref PProvide
 * @returns errorcode, see @ref PErrors
 *
 * Use this function to obtain some basic information about
 * an initialized device driver. The device index (parameter "dev")
 * can be obtained from most event structures, and in particular
 * via the discovery events.
 *
 * @note
 * The pointers you receive may NOT be freed, as
 * they are internal OpenInput library data!
 */
int oi_device_info(unsigned char index, char **name, char **desc, unsigned int *provides) {
    oi_device *dev;

    // Get device
    dev = device_get(index);
    if(dev == NULL) {
        return OI_ERR_NO_DEVICE;
    }

    // Ok, fill
    if(name) {
        *name = dev->name;
    }
    if(desc) {
        *desc = dev->desc;
    }
    if(provides) {
        *provides = dev->provides;
    }
    return OI_ERR_OK;
}

/* ******************************************************************** */
