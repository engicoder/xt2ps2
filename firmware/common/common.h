/* =======================================================================
 * common.h
 * 
 * Purpose:
 *  Declarations commonly used by multiple subsystems.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef COMMON_H_
#define COMMON_H_

static inline void CommonHal_EnableGlobalInterrupts(void);
static inline void CommonHal_DisableGlobalInterrupts(void);

/* Include the appropriate HAL implementation */
#if ARCH == AVR8
    #include "common_avr.h"
#else
    #error No device specific implementation for Common defined.
#endif

typedef enum {
    LOW = 0x00,
    HIGH = 0x01,
} LineState;

typedef enum {
    INPUT,
    OUTPUT,
} PinState;

typedef enum
{
    TRIG_MODE_FALLING,
    TRIG_MODE_RISING,
    TRIG_MODE_LOGIC_CHANGE,
} TriggerMode;

static inline void Watchdog_Disable(void)
{
	CommonHal_DisableWatchdog();
}

static inline void Watchdog_Enable(void)
{
	CommonHal_EnableWatchdog();
}

static inline void Watchdog_Reset(void)
{
	CommonHal_ResetWatchdog();
}

static inline void System_Reset(void)
{
    CommonHal_SystemReset();
}


static inline void EnableGlobalInterrupts(void)
{
    CommonHal_EnableGlobalInterrupts();
}

static inline void DisableGlobalInterrupts(void)
{
    CommonHal_DisableGlobalInterrupts();
}



#endif /* COMMON_H_ */