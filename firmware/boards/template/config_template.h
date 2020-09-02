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

#ifndef CONFIG_TEMPLATE_H_
#define CONFIG_TEMPLATE_H_

#define F_CPU 16000000UL 
#define CLOCK_PRESCALER 8

/* XT Host transceiver configuration values 
 * ---------------------------------------------------------------------- */

/* XT Clock on PD0 (INT0) */
#define XTH_XCVR_CLOCK_PORT PORTD
#define XTH_XCVR_CLOCK_PINS PIND
#define XTH_XCVR_CLOCK_DDR  DDRD
#define XTH_XCVR_CLOCK_BIT  0

/* XT Clock interrupt vector */
#define XTH_XCVR_CLOCK_INTERRUPT_VECTOR INT0_vect

/* XT Data on PD1 */
#define XTH_XCVR_DATA_PORT PORTD
#define XTH_XCVR_DATA_PINS PIND
#define XTH_XCVR_DATA_DDR  DDRD
#define XTH_XCVR_DATA_BIT  1

/* XT Reset on PD6 */
#define XTH_XCVR_RESET_PORT PORTD
#define XTH_XCVR_RESET_DDR  DDRD
#define XTH_XCVR_RESET_BIT  6

/* Receive buffer size */
#define XTH_RECV_BUFFER_SIZE 16

/* Start of frame threshold */
#define XTH_XCVR_SOF_THRESHOLD 200U /* Microseconds */

/* Use 1 start bit */
#define XTH_XCVR_1_START_BIT 0

/* Enable use of Reset line */
#define XTH_XCVR_RESET_LINE_ENABLE 0

/* Reset line has an external pullup */
#define XTH_XCVR_RESET_LINE_HAS_PULLUP 0

/* Enable use of soft reset (hold clock low) */
#define XTH_XCVR_SOFT_RESET_ENABLE 1

/* Duration of soft reset (clock held low) */
#define XTH_XCVR_SOFT_RESET_DURATION 20 /* milliseconds */

/* Duration of hard reset (reset held low) */
#define XTH_XCVR_HARD_RESET_DURATION 50 /* milliseconds */

/* Perform Power-On-Reset when enabled */
#define XTH_XCVR_POR_ON_ENABLE 1


/* PS/2 device transceiver configuration values
 * ---------------------------------------------------------------------- */
 
/* PS/2 Clock on PF4 */
#define PS2D_XCVR_CLOCK_PORT PORTF
#define PS2D_XCVR_CLOCK_PINS PINF
#define PS2D_XCVR_CLOCK_DDR  DDRF
#define PS2D_XCVR_CLOCK_BIT  4

/* PS/2 Data on PF5 */
#define PS2D_XCVR_DATA_PORT PORTF
#define PS2D_XCVR_DATA_PINS PINF
#define PS2D_XCVR_DATA_DDR  DDRF
#define PS2D_XCVR_DATA_BIT  5

/* PS/2 Clock timer interrupt */
#define PS2D_XCVR_CLOCK_INTERRUPT_VECTOR TIMER0_COMPA_vect

/* PS/2 Data timer interrupt */
#define PS2D_XCVR_DATA_INTERRUPT_VECTOR TIMER0_COMPB_vect

/* PS/2 Power-On-Reset duration */
#define PS2D_KBD_POR_DURATION 150UL

/* PS/2 Basic-Assurance-Test duration */
#define PS2D_KBD_BAT_DURATION 300UL

/* PS/2 receive queue size */
#define PS2D_RECV_STORAGE_SIZE 16

/* PS/2 transmit queue size */
#define PS2D_SEND_STORAGE_SIZE 64

/* General configuration values
 * ---------------------------------------------------------------------- */

/* Key event queue size */
#define KEYEVENT_QUEUE_SIZE 10

/* Bus power detect pin */
#define BOARD_POWER_DETECT_PORT PORTC
#define BOARD_POWER_DETECT_PINS PINC
#define BOARD_POWER_DETECT_DDR  DDRC
#define BOARD_POWER_DETECT_BIT  0

#ifdef USE_CONSOLE
    #define CONSOLE_BAUD_RATE 38400UL
    #define CONSOLE_BAUD_PRESCALE (((F_CPU / (CONSOLE_BAUD_RATE * 16UL))) - 1)
    #define CONSOLE_SEND_BUFFER_SIZE 128

#endif

/* Task scheduler update interval */
#define SCHEDULER_INTERVAL 100 /* microseconds */

/* Maximum number of tasks */
#define SCHEDULER_MAX_TASKS 4


#endif /* CONFIG_TEMPLATE_H_ */