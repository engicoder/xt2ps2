/* =======================================================================
 * ps2d_kbd.h
 *
 * Purpose:
 *  Declares the interface to the PS/2 device subsystem.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef PS2D_KBD_H_
#define PS2D_KBD_H_

#include "config.h"
#include "common.h"

#include "byte_sequence.h"

#include "ps2.h"
#include "ps2_command.h"
#include "keyevent.h"

typedef bool (*Ps2dKbd_BatHandler)(void);
typedef void (*Ps2dKbd_LedStatusUpdate)(Ps2LedStatus status);
typedef void (*Ps2dKbd_ResetReceived)(void);

/* -----------------------------------------------------------------------
 * Description:
 *  Initializes the PS/2 device subsystem.
 *
 * Parameters:
 *  statusLedUpdateReceived - a callback to be invoked when a status LED
 *                            update command is received from the host.
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dKbd_Init(Ps2dKbd_BatHandler batHandler,
                  Ps2dKbd_LedStatusUpdate ledStatusUpdateHandler,
                  Ps2dKbd_ResetReceived resetHandler);


/* -----------------------------------------------------------------------
 * Description:
 *  Stores the id returned to the host in response to the ID command.
 *  The default id is { 0xAB, 0x83 }. The id bytes are sent in order
 *  from low array index to high.
 *
 * Parameters:
 *  id         - array contain the id bytes
 *  idLength   - number of bytes in the id
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dKbd_SetId(uint8_t* id, uint8_t idLength);

/* -----------------------------------------------------------------------
 * Description:
 *  Sets the scan code set used by the keyboard
 *
 * Parameters:
 *  set         - scan code set to use
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2Kbd_SetScanCodeSet(Ps2ScanCodeSet set);


/* -----------------------------------------------------------------------
 * Description:
 *  Initiate execution of the PS/2 device subsystem. After this function 
 *  is called the PS/2 device subsystem will begin listening for commands
 *  from and sending scan code sequences to the host.
 *
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dKbd_Start(void);

/* -----------------------------------------------------------------------
 * Description:
 *  Periodic update to allow the PS/2 device subsystem to perform any
 *  required processing. 
 *
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dKbd_Task(void);

/* -----------------------------------------------------------------------
 * Description:
 *  Sends the specified sequence to the host.
 * 
 * Notes:
 *  This will not track key state and therefore typematic behavior
 *  will not be affected.
 *
 * Parameters:
 *  sequence - a PS/2 scan code sequence to be sent to the host.
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dKbd_SendSequence(const ByteSequence* sequence);


/* -----------------------------------------------------------------------
 * Description:
 *  Update key state and send appropriate sequence (as specified by 
 *  current scan code setting) to the host if active.
 *
 * Parameters:
 *  keyEvent - key code and action
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void Ps2dKbd_OnKeyEvent(KeyEvent* keyEvent);

#endif /* PS2D_KBD_H_ */