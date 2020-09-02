/* =======================================================================
 * console_source.h
 * 
 * Purpose:
 *  Declare console source enum values used to specify the source when
 *  sending a console message.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef CONSOLE_SOURCE_H_
#define CONSOLE_SOURCE_H_

typedef enum _ConsoleSource
{
    CON_SRC_CONSOLE, /* Do not remove */

    /* Add custom sources here 
     * Maximum number of simultaneous sources is 16 */
    CON_SRC_XTH_XCVR,
    CON_SRC_XTH_KBD,
    CON_SRC_PS2D_XCVR,
    CON_SRC_PS2D_KBD,
    CON_SRC_XT2PS2,
    CON_SRC_KEYMAP,
    CON_SRC_TEST,

    CON_SRC_COUNT, /* Must be last */

} ConsoleSource;


#endif /* CONSOLE_SOURCE_H_ */