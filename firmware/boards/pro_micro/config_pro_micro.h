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

// XT Clock on PD1 (INT1)
#define XTH_XCVR_CLOCK_INTERRUPT 1
#define XTH_XCVR_CLOCK_PORT PORTD
#define XTH_XCVR_CLOCK_PINS PIND
#define XTH_XCVR_CLOCK_DDR  DDRD
#define XTH_XCVR_CLOCK_BIT  1

// XT Data on PD0
#define XTH_XCVR_DATA_PORT PORTD
#define XTH_XCVR_DATA_PINS PIND
#define XTH_XCVR_DATA_DDR  DDRD
#define XTH_XCVR_DATA_BIT  0

// XT Reset on PB5
#define XTH_XCVR_RESET_PORT PORTB
#define XTH_XCVR_RESET_DDR  DDRB
#define XTH_XCVR_RESET_BIT  5

// PS2 Clock on PC6
#define PS2D_XCVR_CLOCK_PORT PORTC
#define PS2D_XCVR_CLOCK_PINS PINC
#define PS2D_XCVR_CLOCK_DDR  DDRC
#define PS2D_XCVR_CLOCK_BIT  6

// PS2 Data on PD7
#define PS2D_XCVR_DATA_PORT PORTD
#define PS2D_XCVR_DATA_PINS PIND
#define PS2D_XCVR_DATA_DDR  DDRD
#define PS2D_XCVR_DATA_BIT  7


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
    #define CONSOLE_POWER_DETECT_PORT PORTB
    #define CONSOLE_POWER_DETECT_PINS PINB
    #define CONSOLE_POWER_DETECT_DDR  DDRB
    #define CONSOLE_POWER_DETECT_BIT  5
#endif

/* Interrupt interval */
#define SCHEDULER_INTERVAL 100 /* microseconds */
#define SCHEDULER_MAX_TASKS 4

#define XTH_XCVR_RESET_LINE_ENABLE 1

#endif /* CONFIG_PRO_MICRO_H_ */