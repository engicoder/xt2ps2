/* =======================================================================
* config.h
*
* Purpose:
*  Defines configuration information. 
*  Includes appropriate configuration header as specifid in CONFIG_H
*
* License:
*  Copyright (c) 2015, Engicoder
*  All rights reserved.
*  See LICENSE.txt for license details.
* ----------------------------------------------------------------------- */
#define TOSTRING(s) XSTRING(s)
#define XSTRING(s) #s

#define CONFIG_FILENAME TOSTRING(CONFIG_H)


/* Include appropriate configuration header based one board definition */
#include TOSTRING(CONFIG_H)

