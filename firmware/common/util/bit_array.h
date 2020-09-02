/* =======================================================================
* bit_array.h
*
* Purpose:
*  Manages an array backed bit field of arbitrary length.
*
* License:
*  Copyright (c) 2015, Engicoder
*  All rights reserved.
*  See LICENSE.txt for license details.
* ----------------------------------------------------------------------- */
#ifndef BITARRAY_H_
#define BITARRAY_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct _BitArray
{
    uint8_t* Storage;
    uint8_t  StorageSize;
} BitArray;


#define BIT_ARRAY_STORAGE_SIZE(bitLength) (bitLength / 8U + 1U)

static inline void BitArray_ClearAll(BitArray* bitArray);


/* -----------------------------------------------------------------------
 * Description:
 *  Initializes BitArray object to used the specified storage. All bits
 *  are initialized to 0 (cleared).
 *
 * Parameters:
 *  bitArray - pointer to BitArray object/structure to be initialized
 *  storage  - pointer to block of memory to be used to store the bit field
 *  storageSize - size of the storage block in bytes
 *
 * Returns: 
 *   n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void BitArray_Init(BitArray* bitArray, uint8_t* storage, uint8_t storageSize)
{
    bitArray->Storage = storage;
    bitArray->StorageSize = storageSize;
    
    BitArray_ClearAll(bitArray);
}

/* -----------------------------------------------------------------------
 * Description:
 *  Clears all bits in the array
 *
 * Parameters:
 *  bitArray - pointer to BitArray object/structure to be initialized
 *
 * Returns: 
 *   n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void BitArray_ClearAll(BitArray* bitArray)
{
    /* Clear all bits */
    for (int n = 0; n < bitArray->StorageSize; n++)
        bitArray->Storage[n] = 0;
}

/* -----------------------------------------------------------------------
 * Description:
 *  Sets all bits in the array
 *
 * Parameters:
 *  bitArray - pointer to BitArray object/structure to be initialized
 *
 * Returns: 
 *   n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void BitArray_SetAll(BitArray* bitArray)
{
    /* Clear all bits */
    for (int n = 0; n < bitArray->StorageSize; n++)
        bitArray->Storage[n] = 0xFF;
}


/* -----------------------------------------------------------------------
 * Description:
 *  Determines if the specified bit in the array is set.
 *
 * Parameters:
 *  bitArray - pointer to BitArray object/structure to be initialized
 *  index    - the index of the bit in the array
 *
 * Returns: bool
 *   true - if the bit is set 
 *   false - if the bit is clear
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline bool BitArray_IsSet(BitArray* bitArray, uint8_t index)
{
    return ((bitArray->Storage[index/8] & (1 << index % 8)) != 0);
}

/* -----------------------------------------------------------------------
 * Description:
 *  Sets the specified bit in the array.
 *
 * Parameters:
 *  bitArray - pointer to BitArray object/structure to be initialized
 *  index    - the index of the bit in the array
 *
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void BitArray_SetBit(BitArray* bitArray, uint8_t index)
{
    bitArray->Storage[index/8] |= (1 << index % 8);
}


/* -----------------------------------------------------------------------
 * Description:
 *  Clears the specified bit in the array.
 *
 * Parameters:
 *  bitArray - pointer to BitArray object/structure to be initialized
 *  index    - the index of the bit in the array
 *
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void BitArray_ClearBit(BitArray* bitArray, uint8_t index)
{
    bitArray->Storage[index/8] &= ~(1 << index % 8);
}

/* -----------------------------------------------------------------------
 * Description:
 *  Toggles the specified bit in the array.
 *
 * Parameters:
 *  bitArray - pointer to BitArray object/structure to be initialized
 *  index    - the index of the bit in the array
 *
 * Returns: 
 *  n/a
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static inline void BitArray_ToggleBit(BitArray* bitArray, uint8_t index)
{
    bitArray->Storage[index/8] ^= (1 << index % 8);
}





#endif /* BITARRAY_H_ */