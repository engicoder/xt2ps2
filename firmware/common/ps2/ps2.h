/* =======================================================================
* ps2.h
*
* Purpose:
*  Declarations common to PS2 subsystems
*
* License:
*  Copyright (c) 2015, Engicoder
*  All rights reserved.
*  See LICENSE.txt for license details.
* ----------------------------------------------------------------------- */


#ifndef PS2_H_
#define PS2_H_

#include <stdbool.h>

#define PS2_MAX_SEQUENCE_LENGTH 8

#define PS2_BUS_CLK_HIGH (1 << 1)
#define PS2_BUS_DATA_HIGH (1 << 0)

/* PS/2 clock frequency is defined by the IBM specs in terms of 
 * the duration of the clock line states. The clock high and low
 * states have a duration defined as between 30 and 50 us.
 * The Model M keyboard uses a duration of 40 us. 
 * 40us per transition gives a clock period of 80us */
 #define PS2_CLOCK_PERIOD 80UL

typedef enum _Ps2ScanCodeSet
{
    PS2_SCAN_CODE_SET1 = 0x1,
    PS2_SCAN_CODE_SET2,
    PS2_SCAN_CODE_SET3,
} Ps2ScanCodeSet;


/* The state of the PS/2 bus is determined by the combined state
 * of the clock and data lines using the following table:
 *  State        Clock    Data
 *  ------------ -------- --------
 *   IDLE         HIGH     HIGH
 *   INHIBIT      LOW      HIGH
 *   INHIBIT_RTS  LOW      LOW
 *   RTS          HIGH     LOW
 * This enum defines a 2 bit value encoding the bus state. */
typedef enum _Ps2BusState
{
    PS2_BUS_STATE_IDLE = (PS2_BUS_CLK_HIGH | PS2_BUS_DATA_HIGH),
    PS2_BUS_STATE_INHIBIT = PS2_BUS_DATA_HIGH,
    PS2_BUS_STATE_INHIBIT_RTS = 0, 
    PS2_BUS_STATE_RTS = PS2_BUS_CLK_HIGH,
} Ps2BusState;


static inline bool Ps2BusState_ClockIsHigh(Ps2BusState busState)
{
    return ( (busState & PS2_BUS_CLK_HIGH) != 0 );
}

static inline bool Ps2BusState_DataIsHigh(Ps2BusState busState)
{
    return ( (busState & PS2_BUS_DATA_HIGH) != 0 );
}


#endif /* PS2_H_ */