/* =======================================================================
 * keyevt_conv.c
 * 
 * Purpose:
 *  Converts KeyEvents to the host protocol
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 

#include <stddef.h>
#include "ps2_sc_conv.h"
#include "ps2_sc_set2.h"

ByteSequence* Ps2ScanCodeConvert(KeyEvent* keyEvent, Ps2ScanCodeSet scanCodeset)
{
    switch(scanCodeset)
    {
        case PS2_SCAN_CODE_SET2:
            return Ps2ScanCodeSet2_KeyEventToSequence(keyEvent);
        default:
            return NULL;
    }

}