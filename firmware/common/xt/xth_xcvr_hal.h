/* =======================================================================
* xth_xcvr_xcvr.h
*
* Purpose:
*  Defines the interface for the platform specific functionality of the 
*  XT host subsystem. The implementation resides in a separate module.
*
* License:
*  Copyright (c) 2015, Engicoder
*  All rights reserved.
*  See LICENSE.txt for license details.
* ----------------------------------------------------------------------- */



#ifndef XT_HOST_HAL_H_
#define XT_HOST_HAL_H_

#include <stdbool.h>
#include "common.h"


/* Forward declaration of functions that must be implemented by the HAL */


/* -----------------------------------------------------------------------
* Description:
*  Initializes the XT clock line.
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void XthXcvrHal_ClockInit(void);

/* -----------------------------------------------------------------------
* Description:
*  Initializes the XT data line.
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void XthXcvrHal_DataInit(void);

/* -----------------------------------------------------------------------
* Description:
*  Initializes the XT reset line.
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void XthXcvrHal_ResetInit(void);

/* -----------------------------------------------------------------------
* Description:
*  Read the state of the XT data line determine if it high
*
* Returns: bool
*  true if DATA line is HIGH
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline bool XthXcvrHal_DataIsHigh(void);

/* -----------------------------------------------------------------------
* Description:
*  Enables the interrupt for the clock line
*
* Notes:
*  Global interrupts must be enabled after this function is called in
*  order for the interrupt to fire.
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

static inline void XthXcvrHal_EnableClockInterrupt(void);

/* -----------------------------------------------------------------------
* Description:
*  Disables the interrupt for the clock line
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void XthXcvrHal_DisableClockInterrupt(void);

/* -----------------------------------------------------------------------
* Description:
*  Set clock line as output and pull low.
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void XthXcvrHal_ClockHoldLow(void);

/* -----------------------------------------------------------------------
* Description:
*  Set clock as input and release.
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void XthXcvrHal_ClockRelease(void);


/* -----------------------------------------------------------------------
* Description:
*  Set reset line high
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void XthXcvrHal_ResetRelease(bool externalPullup);

/* -----------------------------------------------------------------------
* Description:
*  Set reset line low
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void XthXcvrHal_ResetHoldLow(void);

/* -----------------------------------------------------------------------
* Description:
*  Start the timer with configuration to detect start of data frame
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void XthXcvrHal_TimerSofStart(void);


/* -----------------------------------------------------------------------
* Description:
*  Start the timer with configuration to support a timeout 
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void XthXcvrHal_TimerResetStart(void);

/* -----------------------------------------------------------------------
* Description:
*  Stop the timer
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void XthXcvrHal_TimerStop(void);

/* -----------------------------------------------------------------------
* Description:
*  Reset the timer counter to 0
*
* Returns:
*  n/a
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void XthXcvrHal_TimerSofCountReset(void);

/* -----------------------------------------------------------------------
* Description:
*  Get the current timer count
*
* Returns: uint8_t
*  The current timer count
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline uint16_t XthXcvrHal_TimerSofCount(void);


/* -----------------------------------------------------------------------
* Description:
*  Determine if the timer has overflowed.
*
* Returns: bool
*  true if the timer overflowed
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline bool XthXcvrHal_TimerSofOverflow(void);

/* Include the appropriate HAL implementation */
#if ARCH == AVR8
    #include "xth_xcvr_hal_avr.h"
#else
    #error No HAL implementation for Xt Host defined.
#endif
 
/* Ensure that the HAL implementation has defined the clock falling edge ISR */
#if !defined (XTH_XCVR_CLOCK_ISR)
    #error XTH_XCVR_CLOCK_ISR not defined
#endif

/* Ensure that the HAL implementation has defined the reset timeout ISR */
#if !defined (XTH_XCVR_RESET_TIMEOUT_ISR)
    #error XTH_XCVR_RESET_TIMEOUT_ISR not defined
#endif



#endif /* XT_HOST_HAL_H_ */