/* =======================================================================
* board.h
*
* Purpose:
*  Defines types and interface for the hardware. Includes the header file
*  for the specific board implementation based on compilation symbols.
*
* License:
*  Copyright (c) 2015, Engicoder
*  All rights reserved.
*  See LICENSE.txt for license details.
* ----------------------------------------------------------------------- */

#ifndef BOARD_H_
#define BOARD_H_

#include <stdbool.h>

#include "led_status.h"

/* -----------------------------------------------------------------------
* Description:
*  Initializes the board.
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Board_Init(void);


/* -----------------------------------------------------------------------
* Description:
*  Notifies the board that the a key has been pressed.
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Board_KeyPressed(void);


/* -----------------------------------------------------------------------
* Description:
*  Notifies the board that the a key has been released.
*bas
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Board_KeyReleased(void);


/* -----------------------------------------------------------------------
* Description:
*  Notifies the board that the status LEDs have been updated.
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Board_UpdateLedStatus(LedStatus status);


/* -----------------------------------------------------------------------
* Description:
*  Determines if the board has detected power on the bus
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
bool Board_PowerDetected(void);


#endif /* BOARD_H_ */