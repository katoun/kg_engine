/*
 * queue.c : The event queue functions
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
#include <string.h>
#include "openinput.h"
#include "internal.h"

// Globals
static struct {
    oi_event events[OI_MAX_EVENTS];
    unsigned int head;
    unsigned int tail;
} queue;

/* ******************************************************************** */

/**
 * @ingroup IQueue
 * @brief Initialize queue
 *
 * @returns errorcode, see @ref PErrors
 *
 * Called on library initialization. This function initializes
 * the queue system, which is one of the first subsystems to
 * be initialized, as device driver bootstrapping may depend
 * of the queue to exist (to generate discovery events).
 */
int queue_init() {
    debug("queue_init");

    // Clear event queue
    queue.head = 0;
    queue.tail = 0;
    memset(queue.events, 0, sizeof(queue.events));

    //TODO: Mutexes and threads should gracefully be started here

    // All done
    return OI_ERR_OK;
}

/* ******************************************************************** */

/**
 * @ingroup IQueue
 * @brief Lock queue
 *
 * @returns errorcode, see @ref PErrors
 *
 * Mutual exclusion lock of event queue.
 */
int queue_lock() {
    //TODO: Implement this
    return OI_ERR_NOT_IMPLEM;
}

/* ******************************************************************** */

/**
 * @ingroup IQueue
 * @brief Unlock queue
 *
 * @returns errorcode, see @ref PErrors
 *
 * Mutual exclusion unlock of event queue.
 */
int queue_unlock() {
    //TODO: Implement this
    return OI_ERR_NOT_IMPLEM;
}

/* ******************************************************************** */

/**
 * @ingroup IQueue
 * @brief Add event to queue
 *
 * @param evt pointer to event
 * @returns true (1) if event added, false (0) otherwise
 *
 * Add a single event to the event queue. This is the function
 * you want to use if you want to inject events into the
 * event queue yourself. Please note that you should use
 * the state managers if possible, as these will take
 * care of a lot of other nice stuff for you.
 */
int queue_add(oi_event *evt) {
    unsigned int tail;
	int add;

    //FIXME Generate action events on keyboard/mouse
    if((evt->type == OI_KEYUP) ||
       (evt->type == OI_KEYDOWN) ||
       (evt->type == OI_MOUSEMOVE) ||
       (evt->type == OI_MOUSEBUTTONUP) ||
       (evt->type == OI_MOUSEBUTTONDOWN)) {
        action_process(evt);
    }

    //FIXME: Check mask before we add the event

    // Find position for insertion
    tail = (queue.tail+1)%OI_MAX_EVENTS;

    // Overflow, drop it
    if(tail == queue.head) {
        add = 0;
    }

    // Insert it by COPYING!
    else {
        queue.events[queue.tail] = *evt;
        add = 1;
        // SDL does some special windowmanager event handling here

        debug("queue_add: type %i added at position %i",
              evt->type, queue.tail);

        queue.tail = tail;
    }

    return add;
}

/* ******************************************************************** */

/**
 * @ingroup IQueue
 * @brief Delete event in queue
 *
 * @param where index of event to delete
 * @returns index of deleted event
 *
 * Cut (delete) an event from the queue.
 * Handles both deletion of head, tail and somewhere
 * in between (though the latter is slow).
 */
int queue_cut(unsigned int where) {

    // Cut head
    if(where == queue.head) {
        // Simply increase head
        queue.head = (queue.head+1)%OI_MAX_EVENTS;
        return queue.head;
    }

    // Cut tail
    if(((where+1)%OI_MAX_EVENTS) == queue.tail) {
        queue.tail = where;
        return queue.tail;
    }

    // Cut somewhere in between
    else {
        unsigned int next;
        unsigned int here;

        // Wrap around negative tail
        --queue.tail;
        if(queue.tail < 0) {
            queue.tail = OI_MAX_EVENTS-1;
        }

        // Shift everything backwards
        for(here=where; here!=queue.tail; here=next) {
            next = (here+1)%OI_MAX_EVENTS;

            // We use COPYING here
            queue.events[here] = queue.events[next];
        }

        // Done
        return where;
    }
}

/* ******************************************************************** */

/**
 * @ingroup IQueue
 * @brief Look at events in the queue
 *
 * @param evts pointer to where to store events
 * @param num number of events to copy
 * @param mask event filter mask
 * @param remove true (1) if events should be cut, false (0) otherwise
 * @returns number of copied events
 *
 * Take a peep at the event queue, ie. copy events to pointer.
 * Events are ignored if they match the filter mask. If the
 * "remove" paramter is set, the events are cut from the queue
 * using queue_cut.
 */
int queue_peep(oi_event *evts, unsigned int num, unsigned int mask, char remove) {
    oi_event tmpevt;
    unsigned int here;
    unsigned int copy;

    // User wants to know if events for the mask are pending
    if((evts == NULL) || (num <= 0)) {
        evts = &tmpevt;
        num = 1;
        remove = 0;
    }

    // Start from head, continue till tail or num reached
    here = queue.head;
    copy = 0;
    while((copy < num) && (here != queue.tail)) {
        // Check mask
        if(mask & OI_EVENT_MASK(queue.events[here].type)) {

            // Transfer to user by COPYING
            evts[copy] = queue.events[here];
            copy++;

            // With or without removal
            if(remove) {
                here = queue_cut(here);
            }
            else {
                here = (here+1)%OI_MAX_EVENTS;
            }
        }

        // Mask does not match, fetch next event in queue
        else {
            here = (here+1)%OI_MAX_EVENTS;
        }
    }

    return copy;
}

/* ******************************************************************** */
