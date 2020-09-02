/* =======================================================================
 * xth_kbd.h
 *
 * Purpose:
 *  Declares the interface to the XT keyboard as a host.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef XTH_KBD_H_
#define XTH_KBD_H_

#include <stdint.h>
#include <stdbool.h>

#include "xth_xcvr.h"


typedef void (*OnScanCode)(uint8_t scanCode);


/* -----------------------------------------------------------------------
 * Description:
 *  Initializes the XT host subsystem.
 *
 * Parameters:
 *  xcvrOptions - optons for the transceiver
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthKbd_Init(void);

/* -----------------------------------------------------------------------
 * Description:
 *  Enables the XT host subsystem. 
 *
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthKbd_Enable(void);


/* -----------------------------------------------------------------------
 * Description:
 *  Disables the XT host subsystem. 
 *
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthKbd_Disable(void);


/* -----------------------------------------------------------------------
 * Description:
 *  Periodic update to allow the XT host subsystem to perform any
 *  required processing. 
 *
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthKbd_Task(void);

/* -----------------------------------------------------------------------
 * Description:
 *  Issues a reset to the XT keyboard.
 * 
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthKbd_Reset(void);


/* -----------------------------------------------------------------------
 * Description:
 *  Determine if a scan code has been received from the keyboard. 
 * 
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  true is a scan code is available.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
bool XthKbd_IsScanCodeAvailable(void);


/* -----------------------------------------------------------------------
 * Description:
 *  Retrieve a scan code received from the keyboard.
 * 
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  Scan code received from the keyboard.
 *  0x00 if there was no scanCode available
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
uint8_t XthKbd_GetScanCode(void);


/* -----------------------------------------------------------------------
 * Description:
 *  Register a handler to be called when a scancode is available. This 
 *  handler will be called during the execution of the XthKbd_Task()
 *  function.
 * 
 * Parameters:
 *  handler - pointer to scan code handler function.
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthKbd_RegisterScanCodeHandler(OnScanCode handler);



#endif /* XTH_KBD_H_ */