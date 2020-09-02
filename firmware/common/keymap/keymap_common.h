/* =======================================================================
 * xt_keymap_common.h
 * 
 * Purpose:
 *  Common declarations used by keymaps
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef KEYMAP_COMMON_H_
#define KEYMAP_COMMON_H_

#include <stdint.h>
#include "modifier_status.h"
#include "keycode.h"

/* Information to map a combination of modifiers and
 * one non-modifier key to a different key. */
typedef struct 
{
    Modifiers       RequiredModifiers;
    KeyCode         Original;
    KeyCode         MappedTo;
} KeyCombination;

typedef struct  
{
    const KeyCode(*             ScanCodeMap)[2];
    const KeyCombination* const KeyCombinations; 
    const uint8_t               NumKeyCombinations;
} Keymap;


#endif /* KEYMAP_COMMON_H_ */