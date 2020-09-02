/* =======================================================================
 * host_xt.c
 * 
 * Purpose:
 *  Implements the XT host subsystem interface
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 

#include "config.h"
#include "circular_buffer.h"
#include "circular_buffer_util.h"
#include "host.h"
#include "keymap.h"
#include "xth_xcvr.h"
#include "xth_kbd.h"
#include "keyevent.h"
#include "keycode.h"

#include "console.h"


static uint8_t _keyEventQueueStorage[KEYEVENT_QUEUE_SIZE * sizeof(KeyEvent)];
static CircularBuffer _keyEventQueue;

static bool ToKeyEvent(uint8_t scanCode, KeyEvent* keyEvent);

/* -----------------------------------------------------------------------
 *  Calls XT host Xth_Init() function
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
 void Host_Init(void)
 {
    /* TODO read dip switch for options 
     * maybe this should be in board.c */

    Keymap_Init();
    XthKbd_Init();
    CircularBuffer_Init(&_keyEventQueue, _keyEventQueueStorage, sizeof(_keyEventQueueStorage));
 }


/* -----------------------------------------------------------------------
 *  Calls XT host Xth_Start() function
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Host_Start(void)
{
    XthKbd_Enable();
}

/* -----------------------------------------------------------------------
 *  Checks to see if data has been received from the XT device. If so, 
 *  invokes Keymap to map the scan code to one or more KeyEvent objects
 *  which are added to the key event queue.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Host_Update(void)
{
    KeyEvent keyEvent;

    XthKbd_Task();

    if (XthKbd_IsScanCodeAvailable())
    {
        uint8_t scanCode = XthKbd_GetScanCode();

        if (ToKeyEvent(scanCode, &keyEvent))
        {
            CircularBuffer_InsertKeyEvent(&_keyEventQueue, &keyEvent);
        }
    }
    return;
}


/* -----------------------------------------------------------------------
 *  Checks to see if the KeyEvent queue holds any events. If so, the 
 *  first event is removed and returned. If no event is available, the
 *  KeyEvent code is set to KC_NO_EVENT.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
bool Host_GetKeyEvent(KeyEvent* keyEvent)
{
    /* If there is a KeyEvent available */
    if (!CircularBuffer_IsEmpty(&_keyEventQueue))
    {
        /* Read and remove the KeyEvent from the queue */
        CircularBuffer_ReadKeyEvent(&_keyEventQueue, keyEvent);
        CircularBuffer_RemoveKeyEvent(&_keyEventQueue);
        return true;
    }

    return false;
}


/* -----------------------------------------------------------------------
 *  Converts XT scancode to KeyEvent. Determines if it represents
 *  a key press or key release event.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
bool ToKeyEvent(uint8_t scanCode, KeyEvent* keyEvent)
{
    KeyCode code;
    KeyAction action;

    /* Extract the base scan code and make/break status from the
     * the received data */
    XtScanCode baseCode = (XtScanCode)(scanCode & ~(1 << 7));

    if (baseCode > XT_SC_MAX_CODE)
    {
        return false;
    }

    code = baseCode;

    bool isBreakCode = scanCode & (1<<7);

    /* Set the KeyEvent Type appropriately */
    if (isBreakCode)
        action = KEY_ACTION_RELEASE;
    else
    {
        action = KEY_ACTION_PRESS;
    }    

    KeyEvent_Init(keyEvent, action, code);

    return true;
}