/* =======================================================================
 * device_ps3.c
 * 
 * Purpose:
 *  Implements the PS/2 device subsystem interface
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 
 
#include "device.h"
#include "config.h"
#include "ps2d_kbd.h"
#include "circular_buffer_util.h"
#include "keycode.h"

StatusLedUpdateReceived _statusLedUpdateReceived;

bool BatHandler(void)
{
    return true;
}

void LedStatusUpdate(Ps2LedStatus status)
{
    _statusLedUpdateReceived((LedStatus)status);
}

void ResetReceived(void)
{

}

 /* -----------------------------------------------------------------------
  *  Initialize the device subsystem
  * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Device_Init(StatusLedUpdateReceived statusLedUpdateHandler)
{
    _statusLedUpdateReceived = statusLedUpdateHandler;
    Ps2dKbd_Init(&BatHandler, &LedStatusUpdate, &ResetReceived);
}

/* -----------------------------------------------------------------------
 * Start the device subsystem
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Device_Start(void)
{
    Ps2dKbd_Start();
}



/* -----------------------------------------------------------------------
 *  Called periodically to allow the device subsystem to perform needed
 *  processing.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Device_Update(void)
{
    /* Run the Ps2dKbd task */
    Ps2dKbd_Task();
}

/* -----------------------------------------------------------------------
 *  Send the specified KeyEvent to the remote host. The KeyEvent is first
 *  mapped to the appropriate PS/2 code sequence. The sequence is then 
 *  sent via the PS/2 Keyboard module.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Device_SendKeyEvent(KeyEvent* keyEvent)
{
    if (KeyEvent_Code(keyEvent) != KC_NONE)
    {
        Ps2dKbd_OnKeyEvent(keyEvent);
    }
}

