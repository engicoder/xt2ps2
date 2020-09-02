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
    uint8_t* Storage;
    uint8_t  StorageSize;
    uint8_t  In;
    uint8_t  Out;
    uint8_t  Count;
} CircularBuffer;

/* Inline Functions: */

/* -----------------------------------------------------------------------
 * Description:
 *  Initializes a CircularBuffer to its base state and configures it to
 *  use the specified memory for storage of its data.
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *  storage     - pointer to memory used to store data in the buffer
 *  storageSize - size of the storage memory
 *
 * Returns:
 *   n/a
 *------------------------------------------------------------------------*/
static inline void CircularBuffer_Init(CircularBuffer* const buffer, uint8_t* storage, uint8_t storageSize)
{
    buffer->Storage = storage;
    buffer->StorageSize = storageSize;
    buffer->In = 0;
    buffer->Out = 0;
    buffer->Count = 0;

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
     buffer->In = 0;
     buffer->Out = 0;
     buffer->Count = 0;
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
    return buffer->Count;
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
    return (CircularBuffer_Count(buffer) == buffer->StorageSize);
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
    return (CircularBuffer_Count(buffer) == 0);
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
    buffer->Storage[buffer->In] = data;
    if (++buffer->In == buffer->StorageSize)
        buffer->In = 0;

    buffer->Count++;
 }

/* -----------------------------------------------------------------------
 * Description:
 *  Pushed the specified data to the front the CircularBuffer.
 *
 * Parameters:
 *  buffer      - pointer the CircularBuffer object/structure
 *  data        - a byte of data to be inserted into the CircularBuffer
 *
 * Returns:
 *      n/a
 *------------------------------------------------------------------------*/
static inline void CircularBuffer_Push(CircularBuffer* const buffer, const uint8_t data)
{
    if (CircularBuffer_IsFull(buffer))
        return;

    if (buffer->Out == 0)
        buffer->Out = buffer->StorageSize - 1;
    else
        buffer->Out--;

    buffer->Storage[buffer->Out] = data;

    buffer->Count++;
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
 static inline void CircularBuffer_Insert16(CircularBuffer* const buffer, const uint16_t data)
 {
    buffer->Storage[buffer->In] = ((data >> 8) & 0xFF);
    if (++buffer->In == buffer->StorageSize)
        buffer->In = 0;

    buffer->Count++;

    buffer->Storage[buffer->In] = (data & 0xFF);
    if (++buffer->In == buffer->StorageSize)
        buffer->In = 0;

    buffer->Count++;
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
    for (int i = 0; i < blockLen; i++)
    {
        buffer->Storage[buffer->In] = block[i];
        if (++buffer->In == buffer->StorageSize)
            buffer->In = 0;
    }
    buffer->Count += blockLen;
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
     return buffer->Storage[buffer->Out];
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
     uint8_t trueIndex = (buffer->Out + index) % (buffer->StorageSize);

     return buffer->Storage[trueIndex];
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
     uint8_t Data = buffer->Storage[buffer->Out];

     if (++buffer->Out == buffer->StorageSize)
        buffer->Out = 0;

     buffer->Count--;

     return Data;
 }

  /* -----------------------------------------------------------------------
  * Description:
  *  Removes the byte at the specified index in the CircularBuffer
  *
  * Parameters:
  *  buffer      - pointer the CircularBuffer object/structure
  *  index       - the index of byte to remove
  *
  * Returns: uint8_t
  *      Returns the true if index valid and item removed.
  *------------------------------------------------------------------------*/
 static inline bool CircularBuffer_RemoveN(CircularBuffer* const buffer, uint8_t index)
 {
     if (index > buffer->Count)
        return false;

     buffer->Out = (buffer->Out + index) % buffer->StorageSize;

     buffer->Count -= index;

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
 static inline bool CircularBuffer_ReplaceIndex(CircularBuffer* const buffer, uint8_t index, uint8_t data)
 {
     if (index > buffer->Count)
        return false;

    uint8_t trueIndex = (buffer->Out + index) % (buffer->StorageSize);

    buffer->Storage[trueIndex] = data;

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
     return buffer->StorageSize;
 }

 #endif
