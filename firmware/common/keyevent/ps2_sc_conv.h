/* =======================================================================
 * keyevt_conv.h
 * 
 * Purpose:
 *  Converts KeyEvents to the host protocol
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 


#ifndef KEYEVENT_CONVERTER_H_
#define KEYEVENT_CONVERTER_H_

#include "byte_sequence.h"
#include "keyevent.h"
#include "ps2.h"


ByteSequence* Ps2ScanCodeConvert(KeyEvent* keyEvent, Ps2ScanCodeSet scanCodeset);


#endif /* KEYEVENT_CONVERTER_H_ */