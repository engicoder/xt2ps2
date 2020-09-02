/* =======================================================================
 * device_ps2.h
 * 
 * Purpose:
 *  Defines the interface for the PS2 device subsystem. 
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef DEVICE_H_
#define DEVICE_H_

#include "circular_buffer.h"
#include "keyevent.h"
#include "led_status.h"

/* Definition of Status LED update event handler */
typedef void (*StatusLedUpdateReceived)(LedStatus status);

/* -----------------------------------------------------------------------
 * Description:
 *  Initializes the Device subsystem and enables sending of key events
 *  to the host. 
 *
 * Parameters:
 *  statusLedUpdateHandler - function pointer which will be invoked 
 *                           when a status led update is received from 
 *                           the remote host.
 *
 * Returns:
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Device_Init(StatusLedUpdateReceived statusLedUpdateHandler);

/* -----------------------------------------------------------------------
 * Description:
 *  Provides the Device subsystem with an opportunity to perform any 
 *  startup operations that require interrupts/timers.
 *
 * Returns:
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Device_Start(void);

/* -----------------------------------------------------------------------
 * Description:
 *  Provides the Device subsystem with an opportunity to carry out any 
 *  required periodic processing.
 *
 * Parameters:
 *  n/a
 *
 * Returns:
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Device_Update(void);


/* -----------------------------------------------------------------------
 * Description:
 *  Send the specified KeyEvent to the remote host.
 *
 * Parameters:
 *  keyEvent - Pointer to a KeyEvent object to be sent.
 * 
 * Returns:
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Device_SendKeyEvent(KeyEvent* keyEvent);

#endif /* DEVICE_H_ */