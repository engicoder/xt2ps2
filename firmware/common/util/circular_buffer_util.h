/* =======================================================================
 * circular_buffer_util.h
 *
 * Purpose:
 *  Declaration of utility functions to add and remove complex structures 
 *  from a CircularBuffer.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */


#ifndef CIRCULAR_BUFFER_UTIL_H_
#define CIRCULAR_BUFFER_UTIL_H_

#include "circular_buffer.h"
#include "byte_sequence.h"
#include "keyevent.h"

/* -----------------------------------------------------------------------
 * Description:
 *  Read a ByteSequence from the specified CircularBuffer without removing
 *  it. The storage for the returned sequence must be passed in as a 
 *  parameter. 
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *  sequence    - pointer to a storage backed sequence
 *
 * Returns: bool
 *   true  - if a complete sequence was read from the buffer.
 *   false - if there was not enough data in the buffer to return a 
 *           complete sequence.
 *   Note: behavior is undefined if sequence pointer in NULL.
 *------------------------------------------------------------------------*/
bool CircularBuffer_ReadByteSequence(CircularBuffer* queue, ByteSequence* sequence);

/* -----------------------------------------------------------------------
 * Description:
 *  Insert a ByteSequence into the specified CircularBuffer.
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *  sequence    - pointer to the sequence to be inserted
 *
 * Returns: 
 *   n/a
 *   Note: behavior is undefined if sequence pointer in NULL.
 *------------------------------------------------------------------------*/
void CircularBuffer_InsertByteSequence(CircularBuffer* queue, const ByteSequence* sequence);

/* -----------------------------------------------------------------------
 * Description:
 *  Remove a ByteSequence from the specified CircularBuffer.
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *  sequence    - pointer to the sequence to be inserted
 *
 * Returns: 
 *   n/a
 *   Note: behavior is undefined if sequence pointer in NULL.
 *------------------------------------------------------------------------*/
static inline bool CircularBuffer_RemoveByteSequence(CircularBuffer* queue)
{
    uint8_t length = CircularBuffer_Remove(queue);
    return CircularBuffer_RemoveN(queue, length);
}


bool CircularBuffer_ReadKeyEvent(CircularBuffer* queue, KeyEvent* keyEvent);
void CircularBuffer_InsertKeyEvent(CircularBuffer* queue, const KeyEvent* keyEvent);

static inline bool CircularBuffer_RemoveKeyEvent(CircularBuffer* queue)
{
    return CircularBuffer_RemoveN(queue, KEY_EVENT_SIZE);
}

#endif /* CIRCULAR_BUFFER_UTIL_H_ */