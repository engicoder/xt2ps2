/* =======================================================================
 * ps2d_xcvr_hal_avr.h
 *
 * Purpose:
 *  Implementation of the HAL for the PS/2 device subsystem for AVR 
 *  devices. Uses Timer0
 *  Currently supports:
 *    ATTiny85
 *    ATMega88
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef PS2D_XCVR_HAL_AVR_H_
#define PS2D_XCVR_HAL_AVR_H_

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "ps2.h"
#include "ps2d_xcvr_config.h"

/* Validate that all require configuration data has been defined */
#ifndef PS2D_XCVR_CLOCK_PORT
    #error PS2D_XCVR_CLOCK_PORT not defined
#endif

#ifndef PS2D_XCVR_CLOCK_PINS
    #error PS2D_XCVR_CLOCK_PINS not defined
#endif

#ifndef PS2D_XCVR_CLOCK_DDR
    #error PS2D_XCVR_CLOCK_DDR not defined
#endif

#ifndef PS2D_XCVR_CLOCK_BIT
    #error PS2D_XCVR_CLOCK_BIT not defined
#endif

#ifndef PS2D_XCVR_DATA_PORT
    #error PS2D_XCVR_DATA_PORT not defined
#endif

#ifndef PS2D_XCVR_DATA_PINS
    #error PS2D_XCVR_DATA_PINS not defined
#endif

#ifndef PS2D_XCVR_DATA_DDR
    #error PS2D_XCVR_DATA_DDR not defined
#endif

#ifndef PS2D_XCVR_DATA_BIT
    #error PS2D_XCVR_DATA_BIT not defined
#endif

#ifndef PS2D_XCVR_CLOCK_INTERRUPT_VECTOR
    #error PS2D_XCVR_CLOCK_INTERRUPT_VECTOR not defined
#endif

#ifndef PS2D_XCVR_DATA_INTERRUPT_VECTOR
    #error PS2D_XCVR_DATA_INTERRUPT_VECTOR not defined
#endif

#ifdef DEBUG_TIMING

    #ifndef PS2D_XCVR_DBG_TIMING_PORT
        #error PS2D_XCVR_DBG_TIMING_PORT not defined
    #endif

    #ifndef PS2D_XCVR_DBG_TIMING_DDR
        #error PS2D_XCVR_DBG_TIMING_DDR not defined
    #endif

    #ifndef PS2D_XCVR_DBG_TIMING_BIT
        #error PS2D_XCVR_DBG_TIMING_BIT not defined
    #endif
#endif



/* This transceiver ISR is called 4 times per clock period */
#define PS2D_XCVR_PULSE_WIDTH (F_CPU/1000000U/CLOCK_PRESCALER * (PS2_CLOCK_PERIOD/4))


static inline void Ps2dXcvrHal_BusTimerInit(void)
{
    /* Configure Timer0 to generate PS/2 clock */
	
	/* Enable CTC mode and stop timer by selecting no clock source*/
    TCCR0A = (1 << WGM01); 
    TCCR0B = 0x00; 

    /* Set timer Compare Match values for CTC mode*/
    OCR0A = PS2D_XCVR_PULSE_WIDTH;
    
	/* Rese counter */
    TCNT0 = 0;


#ifdef DEBUG_TIMING
    Ps2dXcvrHal_DbgTimingInit();
#endif
    
}


static inline void Ps2dXcvrHal_BusTimerStart(void)
{
    /* Reset counter */
    TCNT0 = 0;

    /* Clear the Compare Match flags*/
    /* Enable interrupt on Compare Match */
#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega32U4__)
    TIFR0 = (1<<OCF0A);
    TIMSK0 |= 1 << OCIE0A;
#elif defined (__AVR_ATtiny85__)
    TIFR = (1<<OCF0A);
    TIMSK |= 1 << OCIE0A;
#endif

    /* Start timer by selecting CLk/8 source */
    TCCR0B = (1 << CS01); 
}

static inline void Ps2dXcvrHal_BusTimerStop(void)
{

    /* Stop timer by selecting no clock source */
    TCCR0B = 0;

    /* Disable Compare Match interrupts */
#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega32U4__)
    TIMSK0 &= ~(1 << OCIE0A);
#elif defined (__AVR_ATtiny85__)
    TIMSK &= ~(1 << OCIE0A); 
#endif
}

static inline bool Ps2dXcvrHal_ClockIsHigh(void)
{
    return (PS2D_XCVR_CLOCK_PINS & (1 << PS2D_XCVR_CLOCK_BIT));
}


static inline void Ps2dXcvrHal_ClockHigh(void)
{
    /* If current pin direction is output change to input */
    if (PS2D_XCVR_CLOCK_DDR & (1 << PS2D_XCVR_CLOCK_BIT))
        PS2D_XCVR_CLOCK_DDR  &= ~(1<<PS2D_XCVR_CLOCK_BIT);

    /* Turn on internal pullup to go to high-Z state */
    PS2D_XCVR_CLOCK_PORT |=  (1<<PS2D_XCVR_CLOCK_BIT); 
      
}

static inline void Ps2dXcvrHal_ClockLow(void)
{
    PS2D_XCVR_CLOCK_PORT &= ~(1<<PS2D_XCVR_CLOCK_BIT);
    PS2D_XCVR_CLOCK_DDR  |=  (1<<PS2D_XCVR_CLOCK_BIT);   
}

static inline void Ps2dXcvrHal_ClockInit(void)
{
    /* Turn on internal pullup to go to high-Z state */
    PS2D_XCVR_CLOCK_PORT |=  (1<<PS2D_XCVR_CLOCK_BIT); 
    /* Set pin direction to input */    
    PS2D_XCVR_CLOCK_DDR  &= ~(1<<PS2D_XCVR_CLOCK_BIT);    
}

static inline bool Ps2dXcvrHal_DataIsHigh(void)
{
    return (PS2D_XCVR_DATA_PINS & (1 << PS2D_XCVR_DATA_BIT));
}

static inline void Ps2dXcvrHal_DataHigh(void)
{
    /* If current pin direction is output change to input */
    if (PS2D_XCVR_DATA_DDR & (1 << PS2D_XCVR_DATA_BIT))
        PS2D_XCVR_DATA_DDR  &= ~(1<<PS2D_XCVR_DATA_BIT);

    /* Turn on internal pullup to go to high-Z state */
    PS2D_XCVR_DATA_PORT |=  (1<<PS2D_XCVR_DATA_BIT); 
}

static inline void Ps2dXcvrHal_DataLow(void)
{
    PS2D_XCVR_DATA_PORT &= ~(1<<PS2D_XCVR_DATA_BIT);
    PS2D_XCVR_DATA_DDR  |=  (1<<PS2D_XCVR_DATA_BIT);   
}

static inline void Ps2dXcvrHal_DataInit(void)
{
    /* Turn on internal pullup to go to high-Z state */
    PS2D_XCVR_DATA_PORT |=  (1<<PS2D_XCVR_DATA_BIT); 
    /* Set pin direction to input */    
    PS2D_XCVR_DATA_DDR  &= ~(1<<PS2D_XCVR_DATA_BIT);   
}


static inline Ps2BusState Ps2dXcvrHal_BusState(void)
{
    uint8_t clockState = ((PS2D_XCVR_CLOCK_PINS & (1 << PS2D_XCVR_CLOCK_BIT)) != 0) ? 1 : 0;
    uint8_t dataState =  ((PS2D_XCVR_DATA_PINS & (1 << PS2D_XCVR_DATA_BIT)) != 0) ? 1 : 0;

    return  (Ps2BusState)( (clockState << 1) | dataState ); 
}

#define PS2D_XCVR_CLOCK_ISR() ISR(PS2D_XCVR_CLOCK_INTERRUPT_VECTOR)
#define PS2D_XCVR_DATA_ISR() ISR(PS2D_XCVR_DATA_INTERRUPT_VECTOR)

#ifdef DEBUG_TIMING

static inline void Ps2dXcvrHal_DbgTimingInit(void)
{
    PS2D_XCVR_DBG_TIMING_DDR  |= (1<<PS2D_XCVR_DBG_TIMING_BIT);
}

static inline void Ps2dXcvrHal_DbgTimingHigh(void)
{
    PS2D_XCVR_DBG_TIMING_PORT  |= (1<<PS2D_XCVR_DBG_TIMING_BIT);
}

static inline void Ps2dXcvrHal_DbgTimingLow(void)
{
    PS2D_XCVR_DBG_TIMING_PORT  &= ~(1<<PS2D_XCVR_DBG_TIMING_BIT);
}

#endif

#endif /* PS2D_XCVR_HAL_AVR_H_ */