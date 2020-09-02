/* =======================================================================
 * ps2d_xcvr_hal.h
 *
 * Purpose:
 *  Declares the interface to the underlying hardware resources used by the 
 *  PS/2 device subsystem.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef PS2D_XCVR_HAL_H_
#define PS2D_XCVR_HAL_H_

#include <stdbool.h>
#include "ps2.h"
#include "ps2d_xcvr_config.h"

/* Forward declaration of functions that must be implemented by the HAL */


/* -----------------------------------------------------------------------
 * Description:
 *  Initializes the PS/2 bus timer used to monitor the state of the PS/2 
 *  bus, and manage data transmissoin by generating PS/2 CLOCK signals and
 *  writing and reading data using the DATA line.
 *  Transmssion is interrupt driven.
 *
 * Returns:
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void Ps2dXcvrHal_BusTimerInit(void);

/* -----------------------------------------------------------------------
 * Description:
 *  Starts the PS/2 bus timer.
 *
 * Returns:
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void Ps2dXcvrHal_BusTimerStart(void);

/* -----------------------------------------------------------------------
 * Description:
 *  Stops the PS/2 bus timer. 
 *
 * Returns:
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void Ps2dXcvrHal_BusTimerStop(void);


/* -----------------------------------------------------------------------
 * Description:
 *   Initializes the CLOCK line.
 *
 * Returns: 
 *   n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void Ps2dXcvrHal_ClockInit(void);

/* -----------------------------------------------------------------------
 * Description:
 *   Determines if PS/2 CLOCK line is HIGH. 
 *
 * Returns: 
 *   n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline bool Ps2dXcvrHal_ClockIsHigh(void) __attribute__((always_inline));

/* -----------------------------------------------------------------------
 * Description:
 *   Sets the CLOCK line to HIGH by setting the pin direction to INPUT and
 *   enabling the internal pull-up resistor.

 *
 * Returns: 
 *   n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void Ps2dXcvrHal_ClockHigh(void) __attribute__((always_inline));

/* -----------------------------------------------------------------------
 * Description:
 *   Sets the CLOCK line to LOW by setting the pin direction to OUTPUT and
 *   setting the pin to LOW.
 *
 * Returns: 
 *   n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void Ps2dXcvrHal_ClockLow(void) __attribute__((always_inline));


/* -----------------------------------------------------------------------
 * Description:
 *   Initializes the DATA line.
 *
 * Returns: 
 *   n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void Ps2dXcvrHal_DataInit(void);

/* -----------------------------------------------------------------------
 * Description:
 *   Determines if PS/2 DATA line is HIGH. 
 *
 * Returns: 
 *   true if DATA line is HIGH, false otherwise;
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline bool Ps2dXcvrHal_DataIsHigh(void) __attribute__((always_inline));

/* -----------------------------------------------------------------------
 * Description:
 *   Sets the DATA line to HIGH by setting the pin direction to INPUT and
 *   enabling the internal pull-up resistor.
 *
 * Returns: 
 *   n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void Ps2dXcvrHal_DataHigh(void) __attribute__((always_inline));

/* -----------------------------------------------------------------------
 * Description:
 *   Sets the DATA line to LOW by setting the pin direction to OUTPUT and
 *   setting the pin to LOW.
 *
 * Returns: 
 *   n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void Ps2dXcvrHal_DataLow(void) __attribute__((always_inline));


/* -----------------------------------------------------------------------
 * Description:
 *   Returns the current state of the PS/2 buss
 *
 * Returns: 
 *   
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline Ps2BusState Ps2dXcvrHal_BusState(void) __attribute__((always_inline));



/* Include the appropriate HAL implementation */
#ifdef AVR
    #include "ps2d_xcvr_hal_avr.h"
#else
    #error No HAL implementation for Ps2 Device defined.
#endif

/* Ensure that the HAL implementation has defined the ISRs */
#if !defined(PS2D_XCVR_CLOCK_ISR)
    #error PS2D_XCVR_CLOCK_ISR not defined.
#endif



#endif /* PS2D_XCVR_HAL_H_ */