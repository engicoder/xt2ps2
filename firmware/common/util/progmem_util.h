/* =======================================================================
 * progmem_util.h
 *
 * Purpose:
 *  Declaration of utility functions to read data from program memory.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */
#ifndef PROGMEM_UTIL_H_
#define PROGMEM_UTIL_H_

#include <stdint.h>

#include "byte_sequence.h"

 /* -----------------------------------------------------------------------
 * Description:
 *  Reads a ByteSequence from flash program memory (PROGMEM) in to 
 *  previously allocated ByteSequence object in memory (SRAM).
 *
 * Parameters:
 *  address     - address of sequence in program memory.
 *  sequence    - previously allocated ByteSequence object
 *
 * Returns: 
 *  n/a
 *------------------------------------------------------------------------*/
void ProgMem_ReadByteSequence(uint16_t address, ByteSequence* sequence);


#endif /* PROGMEM_UTIL_H_ */