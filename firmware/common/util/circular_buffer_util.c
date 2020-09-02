/* =======================================================================
 * circular_buffer_util.h
 *
 * Purpose:
 *  Utility functions to add and remove complex structures from a 
 *  CircularBuffer.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */

#include "circular_buffer_util.h"

bool CircularBuffer_ReadByteSequence(CircularBuffer* queue, ByteSequence* sequence)
{
    uint8_t length;
 
    if (CircularBuffer_IsEmpty(queue))
        return false;

    length = CircularBuffer_Peek(queue);
    
    if (CircularBuffer_Count(queue) < length + 1)
        return false;

    ByteSequence_Clear(sequence);
    
    for(uint8_t n = 0; n < length; n++)
    {
        ByteSequence_Add(sequence, CircularBuffer_PeekIndex(queue, n+1));
    }
    
    return true;
}

void CircularBuffer_InsertByteSequence(CircularBuffer* queue, const ByteSequence* sequence)
{
    uint8_t length = ByteSequence_Length(sequence);
    
    CircularBuffer_Insert(queue, length);
    for(uint8_t n = 0; n < length; n++)
    {
        CircularBuffer_Insert(queue, ByteSequence_DataAt(sequence, n));
    }
    
    return;
}

bool CircularBuffer_ReadKeyEvent(CircularBuffer* queue, KeyEvent* keyEvent)
{
    if (CircularBuffer_IsEmpty(queue))
    return false;
    
    if (CircularBuffer_Count(queue) < KEY_EVENT_SIZE)
    return false;
    
    KeyEvent_Init(keyEvent, 
                  (KeyAction)CircularBuffer_Peek(queue), 
                  (KeyCode)CircularBuffer_PeekIndex(queue,1));
    
    return true;
}

void CircularBuffer_InsertKeyEvent(CircularBuffer* queue, const KeyEvent* keyEvent)
{
    CircularBuffer_Insert(queue, (uint8_t) KeyEvent_Action(keyEvent));
    CircularBuffer_Insert(queue, (uint8_t) KeyEvent_Code(keyEvent));
    
    return;
}