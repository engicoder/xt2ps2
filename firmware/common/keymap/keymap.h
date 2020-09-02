/* =======================================================================
 * keymap.h
 * 
 * Purpose:
 *  Converts keyevents from one format to another.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 

#ifndef KEYMAP_H_
#define KEYMAP_H_

#include <stdint.h>
#include "keyevent.h"


/* -----------------------------------------------------------------------
 * Description:
 *  Initializes the XT host keymap used to map XT scan codes to KeyEvents.
 *
 * Parameters:
 *  n/a
 *
 * Returns:
 *  n/a
 *------------------------------------------------------------------------*/
void Keymap_Init(void);

/* -----------------------------------------------------------------------
 * Description:
 *  Initializes the XT host keymap used to map XT scan codes to KeyEvents.
 *
 * Parameters:
 *  xtEvent - the XT key event to be mapped.
 *  mappedEvent - the mapped KeyEvent.
 *
 * Returns: 
 *   n/a
 *------------------------------------------------------------------------*/
 bool Keymap_MapToKeyEvent(KeyEvent* xtEvent, KeyEvent* mappedEvent);

#endif /* KEYMAP_H_ */