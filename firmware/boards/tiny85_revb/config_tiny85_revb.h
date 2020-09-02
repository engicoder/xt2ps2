/* =======================================================================
* config_internal_tiny85_revb.h
*
* Purpose:
*  Configuration values for internal converter using ATtiny85 
*
* License:
*  Copyright (c) 2015, Engicoder
*  All rights reserved.
*  See LICENSE.txt for license details.
* ----------------------------------------------------------------------- */

#ifndef CONFIG_TINY85_REVB_H_
#define CONFIG_TINY85_REVB_H_

#define F_CPU 8000000UL // 8 MHz
#define CLOCK_PRESCALER 4

// XT Clock on PB2 (INT0)
#define XTH_XCVR_CLOCK_PORT PORTB
#define XTH_XCVR_CLOCK_PINS PINB
#define XTH_XCVR_CLOCK_DDR  DDRB
#define XTH_XCVR_CLOCK_BIT  2

// XT Clock interrupt vector
#define XTH_XCVR_CLOCK_INTERRUPT_VECTOR INT0_vect

// XT Data on PB1
#define XTH_XCVR_DATA_PORT PORTB
#define XTH_XCVR_DATA_PINS PINB
#define XTH_XCVR_DATA_DDR  DDRB
#define XTH_XCVR_DATA_BIT  1


// PS2 Clock on PB3 (INT0)
#define PS2D_XCVR_CLOCK_PORT PORTB
#define PS2D_XCVR_CLOCK_PINS PINB
#define PS2D_XCVR_CLOCK_DDR  DDRB
#define PS2D_XCVR_CLOCK_BIT  3

// PS2 Data on PB4
#define PS2D_XCVR_DATA_PORT PORTB
#define PS2D_XCVR_DATA_PINS PINB
#define PS2D_XCVR_DATA_DDR  DDRB
#define PS2D_XCVR_DATA_BIT  4


#define PS2D_XCVR_CLOCK_INTERRUPT_VECTOR TIMER0_COMPA_vect
#define PS2D_XCVR_DATA_INTERRUPT_VECTOR TIMER0_COMPB_vect
#define PS2D_XCVR_IDLE_INTERRUPT_VECTOR TIMER0_OVF_vect
#define PS2D_XCVR_RTS_INTERRUPT_VECTOR PCINT0_vect

// XT Reset on PB0
#define XTH_XCVR_RESET_PORT PORTB
#define XTH_XCVR_RESET_PINS PINB
#define XTH_XCVR_RESET_DDR  DDRB
#define XTH_XCVR_RESET_BIT  1

#define XTH_RECV_BUFFER_SIZE 16
#define PS2D_RECV_STORAGE_SIZE 16
#define PS2D_SEND_STORAGE_SIZE 64
#define KEYEVENT_QUEUE_SIZE 10

/* Interrupt interval */
#define SCHEDULER_INTERVAL 100 /* microseconds */
#define SCHEDULER_MAX_TASKS 4

#endif /* CONFIG_TINY85_REVB_H_ */