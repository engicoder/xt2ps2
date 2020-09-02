/* =======================================================================
 * progmem_util.c
 *
 * Purpose:
 *  Declaration of utility functions to read data from program memory.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */
#include <avr/pgmspace.h>
#include "byte_sequence.h"

void ProgMem_ReadByteSequence(uint16_t address, ByteSequence* sequence)
{
    ByteSequence_Clear(sequence);
    
    uint8_t length = pgm_read_byte(address + 0);

    for (int n = 0; n < length; n++ )
    {
        ByteSequence_Add(sequence, pgm_read_byte(address + n + 1));
    }
}