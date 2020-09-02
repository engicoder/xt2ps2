/* =======================================================================
 * console.h
 *
 * Purpose:
 *  Implementation of the HAL for the console subsystem for AVR 
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


#ifndef CONSOLE_HAL_AVR_H_
#define CONSOLE_HAL_AVR_H_


#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"

#define CONSOLE_SEND_ATTEMPTS 100

static inline void ConsoleHal_Init(void)
{
#if defined (__AVR_ATmega328P__)
    /* Set baud rate */
    UBRR0H = (unsigned char)(CONSOLE_BAUD_PRESCALE>>8);
    UBRR0L = (unsigned char)CONSOLE_BAUD_PRESCALE;
    /* Enable transmitter */
    UCSR0B = (1 << TXEN0);
    
    /* Set data format to 8 data bits, 1 stop bit */
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
#elif defined (__AVR_ATmega32U4__)
    /* Set baud rate */
    UBRR1H = (unsigned char)(CONSOLE_BAUD_PRESCALE>>8);
    UBRR1L = (unsigned char)CONSOLE_BAUD_PRESCALE;
    /* Enable transmitter */
    UCSR1B = (1 << TXEN1);
    
    /* Set data format to 8 data bits, 1 stop bit */
    UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);

#else
    #error UART console not supported for this mcu.
#endif
}

static inline bool ConsoleHal_TrySend(uint8_t data)
{
#if defined (__AVR_ATmega328P__)
    if (UCSR0A & (1 << UDRE0)) 
    {
        UDR0 = data;        
#elif defined (__AVR_ATmega32U4__)
    if (UCSR1A & (1 << UDRE1)) 
    {
        UDR1 = data;        
#endif
        return true;
    }
    return false;
}

static inline bool ConsoleHal_Send(uint8_t data)
{
    uint8_t timeout = CONSOLE_SEND_ATTEMPTS;
#if defined (__AVR_ATmega328P__)
    while(((UCSR0A & (1 << UDRE0)) == 0)
#elif defined (__AVR_ATmega32U4__)
    while(((UCSR1A & (1 << UDRE1)) == 0)
#endif
          && (timeout > 0)) {
        timeout--;
    } 

    if (timeout > 0) {
#if defined (__AVR_ATmega328P__)
        UDR0 = data;        
#elif defined (__AVR_ATmega32U4__)
        UDR1 = data;        
#endif
        return true;
    }
    return false;
    
}

#endif /* CONSOLE_HAL_AVR_H_ */