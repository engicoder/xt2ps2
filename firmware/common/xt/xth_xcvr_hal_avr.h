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

#ifndef XTH_XCVR_CLOCK_INTERRUPT_VECTOR
    #error XTH_XCVR_CLOCK_INTERRUPT_VECTOR not defined
#endif

#define FRAME_TIMER_PRESCALER 64

#define XTH_XCVR_FRAME_TIMER_THRESHOLD ((F_CPU / FRAME_TIMER_PRESCALER * XTH_XCVR_FRAME_START_THRESHOLD) /1000000U)


#define RESET_TIMEOUT_PRESCALER 64

#define XTH_XCVR_RESET_TIMEOUT_MATCH ((F_CPU / RESET_TIMEOUT_PRESCALER / 10000U))


static inline void XthXcvrHal_ClockInit(void)
{
    /* Configure clock pin as and input */
    XTH_XCVR_CLOCK_DDR  &= ~(1<<XTH_XCVR_CLOCK_BIT);

    /* Enable internal pullup for clock pin */
    XTH_XCVR_CLOCK_PORT |=  (1<<XTH_XCVR_CLOCK_BIT);

    /* Note: since we are the host, the keyboard will be driving the 
       clock line. Enable the pullup to give a default state of high.
       The keyboard will pull this low to signal beginning of data. */

    /* Configure generation of interrupt on falling edge with
     * interrupt disabled */
    EICRA |= (1<<ISC01);
    EICRA &= ~(1<<ISC00);    
	EIMSK &= ~(1 << INT0);    
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
    // Since we set the data pin as input, we can just read pin state
    return (XTH_XCVR_DATA_PINS & (1 << XTH_XCVR_DATA_BIT));
}

static inline void XthXcvrHal_ConfigureClockInterruptTrigger(TriggerMode mode)
{
    switch(mode)
    {
        case TRIG_MODE_LOGIC_CHANGE:
        {
            EICRA &= ~(1<<ISC01);
            EICRA |= (1<<ISC00); 
        }
        break;
        case TRIG_MODE_RISING:
        {
            EICRA |= (1<<ISC01);
            EICRA |= (1<<ISC00);
        }
        break;
        case TRIG_MODE_FALLING:
        {
            EICRA |= (1<<ISC01);
            EICRA &= ~(1<<ISC00);
        }
        
        break;
        default:
        break;
    }
    
}

static inline void XthXcvrHal_EnableClockInterrupt(void)
{
    EIMSK |= (1 << INT0); 
}

static inline void XthXcvrHal_DisableClockInterrupt(void)
{
	EIMSK &= ~(1 << INT0);
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




static inline void XthXcvrHal_TimerInit(void)
{
    /* Configure Timer in CTC mode */
#if defined (__AVR_ATmega328P__)
    TCCR2A &= ~(1 << WGM22 | 1 << WGM21 | 1 << WGM20); // normal mode
	TCNT2 = 0; // Clear counter
#elif defined (__AVR_ATmega32U4__)
    TCCR1A &= ~(1 << WGM11 | 1 << WGM10); // normal mode
    TCCR1B &= ~(1 << WGM13 | 1 << WGM12); // normal mode
	TCNT1 = 0; // Clear counter
#elif defined (__AVR_ATtiny85__)
    TCCR1 = 0; // Normal mode, timer stopped.
    TCNT1 = 0; // Clear counter
    GTCCR =  (1 << PSR1); // Reset prescaler
#endif

}

static inline void XthXcvrHal_TimerStartTimeout(void)
{
    /* Enable timer by setting prescaler 
	 * Enable interrupt of TOP match */
    /* Enable timer by setting prescaler */
#if defined (__AVR_ATmega328P__)
    TCCR2B |= (1 << CS22);
    TIMSK2 |= (1 << OCIE2A);
    OCR2A = XTH_XCVR_RESET_TIMEOUT_MATCH; // Set CTC TOP
#elif defined (__AVR_ATmega32U4__)
    TCCR1B |= ( 1 << CS11 | 1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    OCR1A = XTH_XCVR_RESET_TIMEOUT_MATCH;
#elif defined (USE_TIM1_8)
    TCCR1 |= (1 << CS12 | 1 << CS11 | 1 << CS10);
    TIMSK |=(1 << OCIE1A);
    OCR1A = XTH_XCVR_RESET_TIMEOUT_MATCH; // Set CTC TOP
#endif
}

static inline void XthXcvrHal_TimerStartDetectSof(void)
{
    /* Enable timer by setting prescaler */
#if defined (__AVR_ATmega328P__)
    TCCR2B |= (1 << CS22);
#elif defined (__AVR_ATmega32U4__)
    TCCR1B |= ( 1 << CS11 | 1 << CS10);
#elif defined (__AVR_ATtiny85__)
    TCCR1 |= (1 << CS12 | 1 << CS11 | 1 << CS10);
#endif
}

static inline void XthXcvrHal_TimerStop(void)
{
    /* Disable timer */
#if defined (__AVR_ATmega328P__)
	TCCR2B &= ~(1 << CS22 | 1 << CS21 | 1 << CS20);
    TIMSK2 &= ~(1 << OCIE2A);
#elif defined (__AVR_ATmega32U4__)
    TCCR1B &= ~( 1 << CS11 | 1 << CS10);
    TIMSK1 &= ~(1 << OCIE1A);
#elif defined (__AVR_ATtiny85__)
    TCCR1 &= ~(1 << CS13 | 1 << CS12 | 1 << CS11 | 1 << CS10);
    TIMSK &= ~(1 << OCIE1A);
#endif

}

static inline uint8_t XthXcvrHal_TimerCount(void)
{
#if defined (__AVR_ATmega328P__)
	return TCNT2;
#elif defined (__AVR_ATmega32U4__)
    return TCNT1;
#elif defined (__AVR_ATtiny85__)
    return TCNT1;
#endif    

}

static inline void XthXcvrHal_TimerResetCount(void)
{
#if defined (__AVR_ATmega328P__)
	TCNT2 = 0;
    TIFR2 |= (1 << TOV2); /* Clear overflow flag */
#elif defined (__AVR_ATmega32U4__)
    TCNT1 = 0;
    TIFR1 |= (1 << TOV1);
#elif defined (__AVR_ATtiny85__)
    TCNT1 = 0;
	TIFR |= (1 << TOV1); /* Clear overflow flag */
#endif    
}

static inline bool XthXcvrHal_TimerOverflow(void)
{
#if defined (__AVR_ATmega328P__)
	return ((TIFR2 & (1 << TOV2)) != 0);
#elif defined (__AVR_ATmega32U4__)
	return ((TIFR1 & (1 << TOV1)) != 0);
#elif defined (__AVR_ATtiny85__)
	return ((TIFR & (1 << TOV1)) != 0);
#endif    
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