/* =======================================================================
 * xth_xcvr_xcvr.h
 * 
 * Purpose:
 *  Implements platform specific functionality of the Xth subsystem.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef XT_AVR_H_
#define XT_AVR_H_

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#if defined( MCUCR ) && ! defined( EICRA )
#define EICRA MCUCR
#endif

#if defined( GIMSK ) && ! defined( EIMSK )
#define EIMSK GIMSK
#endif

#include "xth_xcvr_config.h"

/* Validate that all require configuration data has been defined */
#ifndef XTH_XCVR_CLOCK_PORT
    #error XTH_XCVR_CLOCK_PORT not defined
#endif

#ifndef XTH_XCVR_CLOCK_PINS
    #error XTH_XCVR_CLOCK_PINS not defined
#endif

#ifndef XTH_XCVR_CLOCK_DDR
    #error XTH_XCVR_CLOCK_DDR not defined
#endif

#ifndef XTH_XCVR_CLOCK_BIT
    #error XTH_XCVR_CLOCK_BIT not defined
#endif

#ifndef XTH_XCVR_DATA_PORT
    #error XTH_XCVR_DATA_PORT not defined
#endif

#ifndef XTH_XCVR_DATA_PINS
    #error XTH_XCVR_DATA_PINS not defined
#endif

#ifndef XTH_XCVR_DATA_DDR
    #error XTH_XCVR_DATA_DDR not defined
#endif

#ifndef XTH_XCVR_DATA_BIT
    #error XTH_XCVR_DATA_BIT not defined
#endif

#ifndef XTH_XCVR_RESET_PORT
    #error XTH_XCVR_RESET_PORT not defined
#endif

#ifndef XTH_XCVR_RESET_DDR
    #error XTH_XCVR_RESET_DDR not defined
#endif

#ifndef XTH_XCVR_RESET_BIT
    #error XTH_XCVR_RESET_BIT not defined
#endif

#define XSTR(x) #x
#define STR(x) XSTR(x)

#ifndef XTH_XCVR_CLOCK_INTERRUPT
    #error XTH_XCVR_CLOCK_INTERRUPT not defined
#endif

#if XTH_XCVR_CLOCK_INTERRUPT > 3 
    #error XTH_XCVR_CLOCK_INTERRUPT must be INT0, INT1, INT2 or INT3
#endif

#define MAKE_VECTOR_CAT(a) INT ## a ##_vect
#define MAKE_VECTOR(a) MAKE_VECTOR_CAT(a)  
#define XTH_XCVR_CLOCK_INTERRUPT_VECTOR MAKE_VECTOR(XTH_XCVR_CLOCK_INTERRUPT)

/* Reset timeout fires once per millisecond  */
#define RESET_TIMEOUT_PRESCALER 64
#define XTH_XCVR_RESET_TIMEOUT_MATCH ((F_CPU / RESET_TIMEOUT_PRESCALER / 1000U))

#define XTH_XCVR_SOF_PRESCALER 8
#define XTH_XCVR_SOF_MULTIPLIER (F_CPU/XTH_XCVR_SOF_PRESCALER/1000000)

static inline void XthXcvrHal_ConfigureClockInterruptTrigger(TriggerMode mode)
{
    switch(mode)
    {
        case TRIG_MODE_LOGIC_CHANGE:
            EICRA &= ~(3 << (XTH_XCVR_CLOCK_INTERRUPT * 2));
            EICRA |= (1 << (XTH_XCVR_CLOCK_INTERRUPT * 2)); 
            break;

        case TRIG_MODE_FALLING:
            EICRA &= ~(3 << (XTH_XCVR_CLOCK_INTERRUPT * 2));
            EICRA |= (2 << (XTH_XCVR_CLOCK_INTERRUPT * 2)); 
            break;

        case TRIG_MODE_RISING:
            EICRA &= ~(3 << (XTH_XCVR_CLOCK_INTERRUPT * 2));
            EICRA |= (3 << (XTH_XCVR_CLOCK_INTERRUPT * 2)); 
            break;
        default:
            break;
    }
}

static inline void XthXcvrHal_EnableClockInterrupt(void)
{
    EIMSK |= (1 << XTH_XCVR_CLOCK_INTERRUPT); 
}

static inline void XthXcvrHal_DisableClockInterrupt(void)
{
    EIMSK &= ~(1 << XTH_XCVR_CLOCK_INTERRUPT);
}

static inline void XthXcvrHal_ClockInit(void)
{
    /* Configure clock pin as an input */
    XTH_XCVR_CLOCK_DDR  &= ~(1 << XTH_XCVR_CLOCK_BIT);

    /* Enable internal pullup for clock pin */
    XTH_XCVR_CLOCK_PORT |= (1 << XTH_XCVR_CLOCK_BIT);

    /* Note: since we are the host, the keyboard will be driving the 
       clock line. Enable the pullup to give a default state of high.
       The keyboard will pull this low to signal beginning of data. */

    /* Configure generation of interrupt on falling edge with
     * interrupt disabled */
	XthXcvrHal_DisableClockInterrupt();
    XthXcvrHal_ConfigureClockInterruptTrigger(TRIG_MODE_FALLING);
}

static inline void XthXcvrHal_DataInit(void)
{
    /* Configure data pin as and input */
    XTH_XCVR_DATA_DDR  &= ~(1<<XTH_XCVR_DATA_BIT);

    /* Enable internal pullup for data pin */
    XTH_XCVR_DATA_PORT |=  (1<<XTH_XCVR_DATA_BIT);

   
    /* Note: since we are the host, the keyboard will be driving the 
       data line. Set to input and enable internal pullup.
       TBD: How does this affect other XT compatible keyboards */
}

static inline void XthXcvrHal_ResetInit(void)
{
    /* Release reset line */
    XTH_XCVR_RESET_DDR  &= ~(1<<XTH_XCVR_RESET_BIT);
    XTH_XCVR_RESET_PORT &= ~(1<<XTH_XCVR_RESET_BIT);  
}

static inline bool XthXcvrHal_ClockIsHigh(void)
{
    return (XTH_XCVR_CLOCK_PINS & (1 << XTH_XCVR_CLOCK_BIT));
}


static inline void XthXcvrHal_ClockHoldLow(void)
{
    XTH_XCVR_CLOCK_DDR |= (1 << XTH_XCVR_CLOCK_BIT); /* Set as output */
    XTH_XCVR_CLOCK_PORT &= (0 << XTH_XCVR_CLOCK_BIT); /* Pull low */
}

static inline void XthXcvrHal_ClockRelease(void)
{
    XTH_XCVR_CLOCK_DDR &= ~(1 << XTH_XCVR_CLOCK_BIT); /* Set as input */
    XTH_XCVR_CLOCK_PORT &= (0 << XTH_XCVR_CLOCK_BIT); /* Ensure pullup is off */
}


static inline bool XthXcvrHal_DataIsHigh(void)
{
    return (XTH_XCVR_DATA_PINS & (1 << XTH_XCVR_DATA_BIT));
}


static inline void XthXcvrHal_ResetRelease(bool externalPullup)
{
    if (!externalPullup)
    {
        XTH_XCVR_RESET_DDR  |= (1<<XTH_XCVR_RESET_BIT);
        XTH_XCVR_RESET_PORT |= (1<<XTH_XCVR_RESET_BIT);
    }
    else
    {
        XTH_XCVR_RESET_DDR  &= ~(1<<XTH_XCVR_RESET_BIT);
        XTH_XCVR_RESET_PORT &= ~(1<<XTH_XCVR_RESET_BIT);

    }
}

static inline void XthXcvrHal_ResetHoldLow(void)
{
    XTH_XCVR_RESET_DDR  |= (1<<XTH_XCVR_RESET_BIT);
    XTH_XCVR_RESET_PORT &= ~(1<<XTH_XCVR_RESET_BIT);
}



static inline void XthXcvrHal_TimerResetStart(void)
{
    /* Enable timer by setting prescaler 
	 * Enable interrupt of TOP match */
    /* Enable timer by setting prescaler */
    /* Enable CTC mode */
#if defined (__AVR_ATmega328P__)
    TCCR2B |= (1 << CS22 | 1 << WGM21); /* clk/64 */
    TIMSK2 |= (1 << OCIE2A);
    OCR2A = XTH_XCVR_RESET_TIMEOUT_MATCH;
#elif defined (__AVR_ATmega32U4__)
    TCCR1B = ( 1 << CS11 | 1 << CS10 | 1 << WGM12);  /* clk/64 */
    TIMSK1 |= (1 << OCIE1A);
    OCR1A = XTH_XCVR_RESET_TIMEOUT_MATCH;
#elif defined (USE_TIM1_8)
    TCCR1 |= (1 << CS12 | 1 << CS11 | 1 << CS10 | 1 << CTC1);  /* clk/64 */
    TIMSK |=(1 << OCIE1A);
    OCR1A = XTH_XCVR_RESET_TIMEOUT_MATCH;
    OCR1C = XTH_XCVR_RESET_TIMEOUT_MATCH;
#endif
}

static inline void XthXcvrHal_TimerSofStart(void)
{
    XthXcvrHal_TimerSofCountReset();
#if defined (__AVR_ATmega328P__)
    TCCR2B |= (1 << CS21); /* clk\8 */
#elif defined (__AVR_ATmega32U4__)
    TCCR1B = (1 << CS11); /* clk\8 */
#elif defined (__AVR_ATtiny85__)
    TCCR1 |= (1 << CS12); /* clk\8 */
#endif
}

static inline void XthXcvrHal_TimerStop(void)
{
#if defined (__AVR_ATmega328P__)
	TCCR2B = 0;
    TIMSK2 = 0;
#elif defined (__AVR_ATmega32U4__)
    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 = 0;
#elif defined (__AVR_ATtiny85__)
    TCCR1 = 0;
    TIMSK = 0;
#endif

}

static inline uint16_t XthXcvrHal_TimerSofCount(void)
{
#if defined (__AVR_ATmega328P__)
	return TCNT2;
#elif defined (__AVR_ATmega32U4__)
    return TCNT1;
#elif defined (__AVR_ATtiny85__)
    return TCNT1;
#endif    

}

/* Reset counter 
 * Clear overflow flag */
static inline void XthXcvrHal_TimerSofCountReset(void)
{
#if defined (__AVR_ATmega328P__)
	TCNT2 = 0;
    TIFR2 = (1 << TOV2);
#elif defined (__AVR_ATmega32U4__)
    TCNT1 = 0;
    TIFR1 = (1 << TOV1);
#elif defined (__AVR_ATtiny85__)
    TCNT1 = 0;
	TIFR = (1 << TOV1);
#endif
}

static inline bool XthXcvrHal_TimerSofOverflow(void)
{
    bool overflow = 
#if defined (__AVR_ATmega328P__)
	    ((TIFR2 & (1 << TOV2)) != 0);
#elif defined (__AVR_ATmega32U4__)
	    ((TIFR1 & (1 << TOV1)) != 0);
#elif defined (__AVR_ATtiny85__)
        ((TIFR & (1 << TOV1)) != 0);
#endif
    return overflow;
}


/* Definition of clock line interrupt vector */
#define XTH_XCVR_CLOCK_ISR() ISR(XTH_XCVR_CLOCK_INTERRUPT_VECTOR)

#if defined (__AVR_ATmega328P__)
	#define XTH_XCVR_RESET_TIMEOUT_ISR() ISR(TIMER2_COMPA_vect)
#elif defined (__AVR_ATmega32U4__)
	#define XTH_XCVR_RESET_TIMEOUT_ISR() ISR(TIMER1_COMPA_vect)
#elif defined (__AVR_ATtiny85__)
	#define XTH_XCVR_RESET_TIMEOUT_ISR() ISR(TIMER1_COMPA_vect)
#endif 


#endif /* XT_AVR_H_ */