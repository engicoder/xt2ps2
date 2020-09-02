/* =======================================================================
 * ps2d_kbd_config.h
 *
 * Purpose:
 *  Defines application specific values used by Ps2dKbd to configure
 *  runtime behavior.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */


#ifndef PS2D_KBD_CONFIG_H_
#define PS2D_KBD_CONFIG_H_

#include "config.h"

//#define PS2D_KBD_POR_DURATION  150UL
//#define PS2D_KBD_BAT_DURATION  300UL

#ifndef PS2D_KBD_INTER_BYTE_DELAY
    #define PS2D_KBD_INTER_BYTE_DELAY 500UL  /* Model M uses about 1ms */  
#endif

#ifndef PS2D_KBD_DEVICE_ID 
    #define PS2D_KBD_DEVICE_ID {0xAB, 0x83}
#endif

#endif /* PS2D_KBD_CONFIG_H_ */