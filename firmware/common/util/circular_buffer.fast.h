/* =======================================================================
* circular_buffer.h
*
* Purpose:
*  A simple, lightweight circular buffer with FIFO behavior.
*
* License:
*  Copyright (c) 2015, Engicoder
*  All rights reserved.
*  See LICENSE.txt for license details.
* ----------------------------------------------------------------------- */


#ifndef CIRCULAR_BUFFER_H_
    #define CIRCULAR_BUFFER_H_

/* Includes: */
#include <stdint.h>
#include <stdbool.h>

/* Defines: */
typedef struct _CircularBuffer
{
    uint8_t* storage;
    uint8_t  capacity;
    uint8_t  front;
    uint8_t  rear;
} CircularBuffer;

/* Inline Functions: */
static uint8_t mask(CircularBuffer* const buffer, uint8_t val)
{
    return val & (buffer->capacity - 1);
}


/* -----------------------------------------------------------------------
 * Description:
 *  Initializes a CircularBuffer to its base state and configures it to
 *  use the specified memory for storage of its data.
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *  storage     - pointer to memory used to store data in the buffer
 *  storageSize - size of the storage memory (must be power of 2)
 *
 * Returns:
 *   n/a
 *------------------------------------------------------------------------*/
static inline void CircularBuffer_Init(CircularBuffer* const buffer, uint8_t* storage, uint8_t storageSize)
{
    buffer->storage = storage;
    buffer->capacity = storageSize;
    buffer->front = 0;
    buffer->rear = 0;
}

/* -----------------------------------------------------------------------
 * Description:
 *  Clears all data stored in the CircularBuffer and marks it as empty.
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *
 * Returns:
 *   n/a
 *------------------------------------------------------------------------*/
static inline void CircularBuffer_Clear(CircularBuffer* const buffer)
{
     buffer->front = 0;
     buffer->rear = 0;
}

/* -----------------------------------------------------------------------
 * Description:
 *  Returns the number of bytes of data in the buffer.
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *
 * Returns: uint8_t
 *   Number of bytes stored in the CircularBuffer.
 *------------------------------------------------------------------------*/
static inline uint8_t CircularBuffer_Count(CircularBuffer* const buffer)
{
    return buffer->rear - buffer->front;
}

/* -----------------------------------------------------------------------
 * Description:
 *  Checks to see if the CircularBuffer is full
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *
 * Returns: bool
 *   true  - if the CircularBuffer is full.
 *   false - it there is space available in the CircularBuffer.
 *------------------------------------------------------------------------*/
static inline bool CircularBuffer_IsFull(CircularBuffer* const buffer)
{
    return (CircularBuffer_Count(buffer) == buffer->capacity);
}

/* -----------------------------------------------------------------------
 * Description:
 *  Checks to see if the CircularBuffer is empty of data.
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *
 * Returns: bool
 *   true  - if the CircularBuffer is empty.
 *   false - it the CircularBuffer contains data
 *------------------------------------------------------------------------*/
static inline bool CircularBuffer_IsEmpty(CircularBuffer* const buffer)
{
    return (buffer->front == buffer->rear);
}

/* -----------------------------------------------------------------------
 * Description:
 *  Inserts the specified data into the CircularBuffer.
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *  data        - a byte of data to be inserted into the CircularBuffer
 *
 * Returns:
 *      n/a
 *------------------------------------------------------------------------*/
 static inline void CircularBuffer_Insert(CircularBuffer* const buffer, const uint8_t data)
 {
    if (!CircularBuffer_IsFull(buffer))
        buffer->storage[mask(buffer, buffer->rear++)] = data;
 }


/* -----------------------------------------------------------------------
 * Description:
 *  Inserts the specified data into the CircularBuffer.
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *  data        - a byte of data to be inserted into the CircularBuffer
 *
 * Returns:
 *      n/a
 *------------------------------------------------------------------------*/
 static inline void CircularBuffer_InsertFront(CircularBuffer* const buffer, const uint8_t data)
 {
    if (!CircularBuffer_IsFull(buffer)) {
        buffer->front -= 1;
        buffer->storage[mask(buffer, buffer->front)] = data;
    }
 }

/* -----------------------------------------------------------------------
 * Description:
 *  Inserts a block data into the CircularBuffer.
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *  data        - point to buffer containing the block of data to be
 *                inserted into the CircularBuffer
 *
 * Returns:
 *      n/a
 *------------------------------------------------------------------------*/
 static inline
 void CircularBuffer_InsertBlock(CircularBuffer* const buffer,
                                 const uint8_t* block, uint8_t blockLen)
 {
    if (buffer->capacity - CircularBuffer_Count(buffer) >= blockLen) {
        for (int i = 0; i < blockLen; i++) {
            CircularBuffer_Insert(buffer, block[i]);
        }
    }
 }

 /* -----------------------------------------------------------------------
  * Description:
  *  Returns the value of the first byte in the CircularBuffer without
  *  removing it.
  *
  * Parameters:
  *  buffer      - pointer the CircularBuffer object/structure
  *
  * Returns: uint8_t
  *      Returns the value of the first byte in the CircularBuffer.
  *------------------------------------------------------------------------*/
 static inline uint8_t CircularBuffer_Peek(CircularBuffer* const buffer)
 {
     return buffer->storage[mask(buffer, buffer->front)];
 }

 /* -----------------------------------------------------------------------
  * Description:
  *  Returns the value of the byte at the specified index in the
  *  CircularBuffer without removing it.
  *
  * Parameters:
  *  buffer      - pointer the CircularBuffer object/structure
  *  index       - the index of byte to return
  *
  * Returns: uint8_t
  *      Returns the value of the byte at the specified index in the
  *      CircularBuffer.
  *------------------------------------------------------------------------*/
 static inline uint8_t CircularBuffer_PeekIndex(CircularBuffer* const buffer, uint8_t index)
 {
     uint8_t trueIndex = mask(buffer, index);

     return buffer->storage[trueIndex];
 }

 /* -----------------------------------------------------------------------
  * Description:
  *  Removes the first byte from the CircularBuffer and returns its value.
  *
  * Parameters:
  *  buffer      - pointer the CircularBuffer object/structure
  *
  * Returns: uint8_t
  *      Returns the value of the first byte in the CircularBuffer.
  *------------------------------------------------------------------------*/
 static inline uint8_t CircularBuffer_Remove(CircularBuffer* const buffer)
 {
    uint8_t val = 0;
    if (!CircularBuffer_IsEmpty(buffer))
        val = buffer->storage[mask(buffer, buffer->front++)];

    return val;
 }

 /* -----------------------------------------------------------------------
  * Description:
  *  Removes the last byte from the CircularBuffer and returns its value.
  *
  * Parameters:
  *  buffer      - pointer the CircularBuffer object/structure
  *
  * Returns: uint8_t
  *      Returns the value of the first byte in the CircularBuffer.
  *------------------------------------------------------------------------*/
static inline uint8_t CircularBuffer_RemoveRear(CircularBuffer* const buffer)
{
    uint8_t val = 0;
    if (!CircularBuffer_IsEmpty(buffer))
        val = buffer->storage[mask(buffer, buffer->rear--)];

    return val;
}


 /* -----------------------------------------------------------------------
 * Description:
 *  Removes the N bytes from the front of the CircularBuffer
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *  n           - number of bytes to remove
 *
 * Returns: uint8_t
 *      Returns the true if bytes were removed.
 *------------------------------------------------------------------------*/
static inline bool CircularBuffer_RemoveN(CircularBuffer* const buffer, uint8_t n)
{
    if (n > CircularBuffer_Count(buffer))
       return false;
    
    buffer->front = mask(buffer, buffer->front + n);

    return true;
}

 /* -----------------------------------------------------------------------
 * Description:
 *  Replaces the byte at the specified index in the CircularBuffer with the
 *  specified value.
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *  index       - the index of byte to replace
 *
 * Returns: uint8_t
 *      Returns true if index valid and item replaced.
 *------------------------------------------------------------------------*/
static inline
bool CircularBuffer_ReplaceIndex(CircularBuffer* const buffer, uint8_t index, uint8_t data)
{
    if (index > CircularBuffer_Count(buffer))
       return false;
    uint8_t trueIndex = mask(buffer, index);
    buffer->storage[trueIndex] = data;
    return true;
}

 /* -----------------------------------------------------------------------
 * Description:
 *  Returns the maximum number of element the buffer can hold.
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *
 * Returns: uint8_t
 *      Returns size of buffer.
 *------------------------------------------------------------------------*/
static inline uint8_t CircularBuffer_Size(CircularBuffer* const buffer)
{
    return buffer->capacity;
}


#endif
