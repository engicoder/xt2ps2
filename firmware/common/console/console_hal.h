/* =======================================================================
 * console_hal.h
 *
 * Purpose:
 *  Declares the interface to the underlying hardware resources used by the 
 *  debug console subsystem.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef CONSOLE_HAL_H_
#define CONSOLE_HAL_H_


static inline void ConsoleHal_Init(void);

static inline bool ConsoleHal_TrySend(uint8_t data);
static inline bool ConsoleHal_Send(uint8_t data);

static inline bool ConsoleHal_PowerDetected(void);

/* Include the appropriate HAL implementation */
#if ARCH == AVR8
#include "console_hal_avr.h"
#else
#error No HAL implementation for Console defined.
#endif



#endif /* CONSOLE_HAL_H_ */