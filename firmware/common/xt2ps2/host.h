/* =======================================================================
 * host_xt.h
 * 
 * Purpose:
 *  Defines the interface for the xt_host subsystem. 
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 

#ifndef HOST_H_
#define HOST_H_

#include <stdbool.h>
#include "keyevent.h"

/* -----------------------------------------------------------------------
 * Description:
 *  Initializes the XT host subsystem and enables reception of key events from 
 *  the device. 
 *
 * Returns:
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Host_Init(void);


/* -----------------------------------------------------------------------
 * Description:
 *  Provides the Xt host subsystem with an opportunity to perform any 
 *  startup operations that require interrupts to be enabled.
 *
 * Returns:
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Host_Start(void);

/* -----------------------------------------------------------------------
 * Description:
 *  Provides the XT host subsystem with an opportunity to carry out any 
 *  required periodic processing.
 *
 * Parameters:
 *  n/a
 *
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Host_Update(void);


/* -----------------------------------------------------------------------
 * Description:
 *  Provides the XT host subsystem with an opportunity to carry out any 
 *  required update processing.
 *
 * Parameters:
 *  keyEvent - A KeyEvent object represent a key press received from the 
 *             remote device. 
 *             Note: If no keyEvent was received, the KeyEvent Code will
 *             be set to KC_NO_EVENT.
 *
 * Returns: bool
 *  true  - if a valid KeyEvent was available and returned.
 *  false - otherwise
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
bool Host_GetKeyEvent(KeyEvent* keyEvent);

#endif /* HOST_H_ */