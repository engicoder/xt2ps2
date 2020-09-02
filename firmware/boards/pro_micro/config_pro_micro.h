/* =======================================================================
* config_pro_micro.h
*
* Purpose:
*  Configuration values for console board 
*
* License:
*  Copyright (c) 2020, Engicoder
*  All rights reserved.
*  See LICENSE.txt for license details.
* ----------------------------------------------------------------------- */

#ifndef CONFIG_PRO_MICRO_H_
#define CONFIG_PRO_MICRO_H_

#define F_CPU 16000000UL 
#define CLOCK_PRESCALER 8

// XT Clock on PD0 (INT0)
#define XTH_XCVR_CLOCK_PORT PORTD
#define XTH_XCVR_CLOCK_PINS PIND
#define XTH_XCVR_CLOCK_DDR  DDRD
#define XTH_XCVR_CLOCK_BIT  0

// XT Clock interrupt vector
#define XTH_XCVR_CLOCK_INTERRUPT_VECTOR INT0_vect

// XT Data on PD1
#define XTH_XCVR_DATA_PORT PORTD
#define XTH_XCVR_DATA_PINS PIND
#define XTH_XCVR_DATA_DDR  DDRD
#define XTH_XCVR_DATA_BIT  1

// XT Reset on PD6
#define XTH_XCVR_RESET_PORT PORTD
#define XTH_XCVR_RESET_DDR  DDRD
#define XTH_XCVR_RESET_BIT  6

// PS2 Clock on PF4
#define PS2D_XCVR_CLOCK_PORT PORTF
#define PS2D_XCVR_CLOCK_PINS PINF
#define PS2D_XCVR_CLOCK_DDR  DDRF
#define PS2D_XCVR_CLOCK_BIT  4

// PS2 Data on PF5
#define PS2D_XCVR_DATA_PORT PORTF
#define PS2D_XCVR_DATA_PINS PINF
#define PS2D_XCVR_DATA_DDR  DDRF
#define PS2D_XCVR_DATA_BIT  5


#define PS2D_XCVR_CLOCK_INTERRUPT_VECTOR TIMER0_COMPA_vect
#define PS2D_XCVR_DATA_INTERRUPT_VECTOR TIMER0_COMPB_vect

#define PS2D_KBD_POR_DURATION 150UL
#define PS2D_KBD_BAT_DURATION 300UL

#define XTH_RECV_BUFFER_SIZE 16
#define PS2D_RECV_STORAGE_SIZE 16
#define PS2D_SEND_STORAGE_SIZE 64
#define KEYEVENT_QUEUE_SIZE 10

/* Bus power detect pin */
#define BOARD_POWER_DETECT_PORT PORTF
#define BOARD_POWER_DETECT_PINS PINF
#define BOARD_POWER_DETECT_DDR  DDRF
#define BOARD_POWER_DETECT_BIT  6

#ifdef USE_CONSOLE
    #define CONSOLE_BAUD_RATE 38400UL
    #define CONSOLE_BAUD_PRESCALE (((F_CPU / (CONSOLE_BAUD_RATE * 16UL))) - 1)
    #define CONSOLE_SEND_BUFFER_SIZE 128

#endif

/* Interrupt interval */
#define SCHEDULER_INTERVAL 100 /* microseconds */
#define SCHEDULER_MAX_TASKS 4

#ifdef DEBUG_TIMING
    #define PS2D_XCVR_DBG_TIMING_PORT PORTB
    #define PS2D_XCVR_DBG_TIMING_DDR  DDRB
    #define PS2D_XCVR_DBG_TIMING_BIT  1
#endif


#endif /* CONFIG_PRO_MICRO_H_ */