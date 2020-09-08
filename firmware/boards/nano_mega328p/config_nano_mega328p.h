/* =======================================================================
* config_console_nano.h
*
* Purpose:
*  Configuration values for console board 
*
* License:
*  Copyright (c) 2015, Engicoder
*  All rights reserved.
*  See LICENSE.txt for license details.
* ----------------------------------------------------------------------- */

#ifndef CONFIG_CONSOLE_NANO_H_
#define CONFIG_CONSOLE_NANO_H_

#include "xth_xcvr.h"
#include "xth_kbd.h"

#define F_CPU 16000000UL 
#define CLOCK_PRESCALER 8

// XT Clock on PD2 (INT0)
#define XTH_XCVR_CLOCK_INTERRUPT 0
#define XTH_XCVR_CLOCK_PORT PORTD
#define XTH_XCVR_CLOCK_PINS PIND
#define XTH_XCVR_CLOCK_DDR  DDRD
#define XTH_XCVR_CLOCK_BIT  2

// XT Data on PC1
#define XTH_XCVR_DATA_PORT PORTC
#define XTH_XCVR_DATA_PINS PINC
#define XTH_XCVR_DATA_DDR  DDRC
#define XTH_XCVR_DATA_BIT  1


// XT Reset on PC2 
#define XTH_XCVR_RESET_PORT PORTC
#define XTH_XCVR_RESET_DDR  DDRC
#define XTH_XCVR_RESET_BIT  2

// PS2 Clock on PC3 
#define PS2D_XCVR_CLOCK_PORT PORTC
#define PS2D_XCVR_CLOCK_PINS PINC
#define PS2D_XCVR_CLOCK_DDR  DDRC
#define PS2D_XCVR_CLOCK_BIT  3

// PS2 Data on PC4
#define PS2D_XCVR_DATA_PORT PORTC
#define PS2D_XCVR_DATA_PINS PINC
#define PS2D_XCVR_DATA_DDR  DDRC
#define PS2D_XCVR_DATA_BIT  4


#define PS2D_XCVR_CLOCK_INTERRUPT_VECTOR TIMER0_COMPA_vect
#define PS2D_XCVR_DATA_INTERRUPT_VECTOR TIMER0_COMPB_vect

#define PS2D_KBD_POR_DURATION 150UL
#define PS2D_KBD_BAT_DURATION 300UL


#define XTH_RECV_BUFFER_SIZE 16
#define PS2D_RECV_STORAGE_SIZE 16
#define PS2D_SEND_STORAGE_SIZE 64
#define KEYEVENT_QUEUE_SIZE 10

#ifdef USE_CONSOLE
    #define CONSOLE_BAUD_RATE 38400UL
    #define CONSOLE_BAUD_PRESCALE (((F_CPU / (CONSOLE_BAUD_RATE * 16UL))) - 1)
    #define CONSOLE_SEND_BUFFER_SIZE 128

    /* Bus power detect pin */
    #define CONSOLE_POWER_DETECT_PORT PORTC
    #define CONSOLE_POWER_DETECT_PINS PINC
    #define CONSOLE_POWER_DETECT_DDR  DDRC
    #define CONSOLE_POWER_DETECT_BIT  0

#endif


/* Interrupt interval */
#define SCHEDULER_INTERVAL 100 /* microseconds */
#define SCHEDULER_MAX_TASKS 4

#ifdef DEBUG_TIMING
    #define PS2D_XCVR_DBG_TIMING_PORT PORTC
    #define PS2D_XCVR_DBG_TIMING_DDR  DDRC
    #define PS2D_XCVR_DBG_TIMING_BIT  5
#endif


#endif /* CONFIG_CONSOLE_H_ */