/* =======================================================================
 * xth_xcvr_xcvr.h
 * 
 * Purpose:
 *  Provide basic interface to the XT host subsystem which is responsible
 *  for detecting and receiving PX/XT scancodes from the keyboard.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef XT_HOST_XCVR_H
#define XT_HOST_XCVR_H

#include <stdint.h>
#include <stdbool.h>

#include "xt_scancode.h"


typedef enum _XthXcvrOptions
{
    XTH_XCVR_OPT_1_START_BIT = (1 << 0),
    XTH_XCVR_OPT_HARD_RESET_ENABLE = (1 << 1),
    XTH_XCVR_OPT_EXT_RESET_PULLUP = (1 << 2),
    XTH_XCVR_OPT_SOFT_RESET_ENABLE = (1 << 3),
    XTH_XCVR_OPT_DEFAULT = ( XTH_XCVR_OPT_HARD_RESET_ENABLE | 
                             XTH_XCVR_OPT_SOFT_RESET_ENABLE )
} XthXcvrOptions;

typedef enum 
{
    XTH_XCVR_STATUS_KBD_DETECTED           = (1 << 0),
    XTH_XCVR_STATUS_READY                  = (1 << 1),
    XTH_XCVR_STATUS_RECV_BUFFER_FULL       = (1 << 3),
    XTH_XCVR_STATUS_RECV_OVERFLOW          = (1 << 4),
}XthXcvrStatus;

extern volatile XthXcvrStatus _xthXcvrStatus;

static inline XthXcvrStatus XthXcvr_Status(void)
{
    return _xthXcvrStatus;
}

static inline bool XthXcvr_StatusIsSet(XthXcvrStatus status)
{
    return (_xthXcvrStatus & status);
}

static inline bool XthXcvr_StatusDataReceived(void)
{
    return XthXcvr_StatusIsSet(XTH_XCVR_STATUS_RECV_BUFFER_FULL);
}

static inline bool XthXcvr_StatusReady(void)
{
    return XthXcvr_StatusIsSet(XTH_XCVR_STATUS_READY);
}

static inline bool XthXcvr_StatusKeyboardDetected(void)
{
    return XthXcvr_StatusIsSet(XTH_XCVR_STATUS_KBD_DETECTED);
}

static inline bool XthXcvr_StatusIsOverflow(void)
{
	return XthXcvr_StatusIsSet(XTH_XCVR_STATUS_RECV_OVERFLOW);  
}

/* -----------------------------------------------------------------------
 * Description:
 *  Initialize the XT host subsystem.
 *
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthXcvr_Init(void);

/* -----------------------------------------------------------------------
 * Description:
 *  Initiate execution of the XT host subsystem. After this function is
 *  called the XT host subsystem will begin detecting scan codes from the
 *  keyboard.
 *
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthXcvr_Enable(void);

/* -----------------------------------------------------------------------
 * Description:
 *  Halt execution of the XT transciever subsystem. The XT host subsystem will 
 *  no longer detect scan codes from the keyboard.
 *
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthXcvr_Disable(void);

/* -----------------------------------------------------------------------
 * Description:
 *  Initialize the XT host subsystem.
 *
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
uint8_t XthXcvr_ReadReceivedData(void);


/* -----------------------------------------------------------------------
 * Description:
 *  Issue a soft reset to the keyboard. The KBD_DETECTED status bit will 
 *  be cleared. 
 *
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthXcvr_SoftReset(void);


/* -----------------------------------------------------------------------
 * Description:
 *  Handles a power on reset (POR) of the keyboard. A soft reset will also
 *  be issued and the KBD_DETECTED status bit will be cleared. 
 *
 * Parameters:
 *  n/a
 * 
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void XthXcvr_HardReset(void);


#endif /* XT_HOST_H_ */