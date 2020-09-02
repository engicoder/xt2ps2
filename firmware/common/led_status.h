/* =======================================================================
 * led_status.h
 * 
 * Purpose:
 *  Defines the flags passed to the LED Status notification 
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 

#ifndef LEDSTATUS_H_
#define LEDSTATUS_H_

typedef enum
{
    LedStatusScrollLock = (1 << 0),
    LedStatusNumLock = (1 << 1),
    LedStatusCapsLock = (1 << 2),
} LedStatus;

#endif /* LEDSTATUS_H_ */