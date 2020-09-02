/* =======================================================================
 * ps2_sc_set2.h
 * 
 * Purpose:
 *  Converts KeyEvents to PS/2 scan code sequences
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef PS2_SC_SET2_H_
#define PS2_SC_SET2_H_

#include "keyevent.h"
#include "byte_sequence.h"


ByteSequence* Ps2ScanCodeSet2_KeyEventToSequence(KeyEvent* keyEvent);


#endif /* PS2_SC_SET2_H_ */