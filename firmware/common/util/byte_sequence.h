/* =======================================================================
* byte_sequence.h
*
* Purpose:
*  Simplifies handling of a sequence of bytes.
*
* License:
*  Copyright (c) 2015, Engicoder
*  All rights reserved.
*  See LICENSE.txt for license details.
* ----------------------------------------------------------------------- */


#ifndef BYTE_SEQUENCE_H_
#define BYTE_SEQUENCE_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct _ByteSequence
{
    uint8_t length;
    uint8_t data[];
} ByteSequence;


/* -----------------------------------------------------------------------
 * Description:
 *  Creates a ByteSequence from the specified block of memory. 
 *  Note: The capacity of the resulting sequence is two less than the 
 *        size of the memory block provided.
 *
 * Parameters:
 *  storage     - pointer to memory used to store data in the sequence
 *  storageSize - size of the storage memory 
 *
 * Returns: ByteSequence*
 *   Pointer to an initialize ByteSequence object
 *------------------------------------------------------------------------*/
static inline ByteSequence* ByteSequence_Create(uint8_t* storage, uint8_t storageLength)
{
    ByteSequence* sequence = (ByteSequence*)storage;
    sequence->length = 0;
    return sequence;    
}

/* -----------------------------------------------------------------------
 * Description:
 *  Adds a byte of data to the ByteSequence. The byte will be added at the
 *  end of the sequence.
 *
 * Parameters:
 *  sequence     - pointer to a ByteSequence object
 *  data         - a byte of data to be added
 *
 * Returns: 
 *   n/a
 *------------------------------------------------------------------------*/
static inline void ByteSequence_Add(ByteSequence* sequence, uint8_t data)
{
    sequence->data[sequence->length] = data;
    sequence->length++;
}

/* -----------------------------------------------------------------------
 * Description:
 *  Removes all data from the sequence and resets the count to 0.
 *
 * Parameters:
 *  sequence     - pointer to a ByteSequence object
 *
 * Returns: 
 *   n/a
 *------------------------------------------------------------------------*/
static inline void ByteSequence_Clear(ByteSequence* sequence)
{
    sequence->length = 0;
}

/* -----------------------------------------------------------------------
 * Description:
 *  Returns the length of the ByteSequence in bytes.
 *
 * Parameters:
 *  sequence     - pointer to a ByteSequence object
 *
 * Returns: uint8_t
 *  length of the ByteSequence in bytes.
 *------------------------------------------------------------------------*/
static inline uint8_t ByteSequence_Length(const ByteSequence* sequence)
{
    return sequence->length;
}

/* -----------------------------------------------------------------------
 * Description:
 *  Returns the value of the byte of data at the specified index of the 
 *  ByteSequence. 
 *
 * Parameters:
 *  sequence     - pointer to a ByteSequence object
 *  index        - index of the ByteSequence
 *
 * Returns: uint8_T
 *  The value of the byte of data at the specified index of the
 *  ByteSequence.
 *------------------------------------------------------------------------*/
static inline uint8_t ByteSequence_DataAt(const ByteSequence* sequence, uint8_t index)
{
    return sequence->data[index];
}

/* -----------------------------------------------------------------------
 * Description:
 *  Copies the contents of one ByteSequence to another.
 *
 * Parameters:
 *  source     - pointer to a ByteSequence object to copy from
 *  target     - pointer to a ByteSequence object to copy to
 *
 * Returns: 
 *  n/a
 *------------------------------------------------------------------------*/
static inline void ByteSequence_Duplicate(ByteSequence* target, const ByteSequence* source )
{
    target->length = source->length;

    for (uint8_t n = 0; n < source->length; n++)
    {
        target->data[n] = source->data[n];
    }

    return;
}

/* -----------------------------------------------------------------------
 * Description:
 *  Copies the contents of one ByteSequence to another.
 *
 * Parameters:
 *  source     - pointer to a ByteSequence object to copy from
 *  target     - pointer to a ByteSequence object to copy to
 *
 * Returns: 
 *  n/a
 *------------------------------------------------------------------------*/
static inline void ByteSequence_AppendTo(ByteSequence* target, const ByteSequence* source)
{
    for (uint8_t n = 0; n < source->length; n++)
    {
        ByteSequence_Add(target, source->data[n]);
    }

    return;
}




#endif /* BYTE_SEQUENCE_H_ */