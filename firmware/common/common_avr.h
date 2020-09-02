/* =======================================================================
 * common_avr.c
 * 
 * Purpose:
 *  Implementation of the HAL for the common functions previously declared.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */  


#ifndef COMMON_AVR_H_
#define COMMON_AVR_H_

#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#if defined(WDTCR) && !defined(WDTCSR)
	#define WDTCSR WDTCR
#endif


static inline void CommonHal_DisableWatchdog(void)
{
    MCUSR = 0;    
    wdt_disable();
}

static inline void CommonHal_EnableWatchdog(void)
{
    wdt_enable(WDTO_1S);
}

static inline void CommonHal_ResetWatchdog(void)
{
	wdt_reset();
}


static inline void CommonHal_SystemReset(void)
{
    wdt_disable();    
    wdt_enable(WDTO_120MS);    
    while(1); /* Wait until watchdog triggers reset */
}

static inline void CommonHal_EnableGlobalInterrupts(void)
{
    sei();     
}

static inline void CommonHal_DisableGlobalInterrupts(void)
{
    cli();    
}


#endif /* COMMON_AVR_H_ */