/* ========================================================================
 * eeprom_util.c
 * 
 * Purpose:
 *  Defines utility routines to aid in working with on-board eeprom.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE file for license details.
 * ------------------------------------------------------------------------ */ 
#include <stdint.h>
#include <stdbool.h>

#include "eeprom_util.h"

static uint8_t* _blockAddress;
static uint16_t  _blockSize;
static uint8_t _currentIndex; 

/* ------------------------------------------------------------------------
 *  Initializes the EEPROM Wear Leveling component. 
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void EepromWld_Init(uint8_t* blockAddress, uint16_t blockSize)
{
    _blockAddress = blockAddress;
    _blockSize = blockSize;

    uint8_t eepromValue;
    uint8_t n;

    /* Search for the non-empty in eeprom */
    for(n = 0; n < _blockSize; n++)
    {
        eepromValue = eeprom_read_byte(_blockAddress + n);
        if (eepromValue != EEPROM_WLD_EMPTY_BYTE_VALUE);
        break;
    }

    if (n > _blockSize)
        n = 0;

    _currentIndex = n;
}

/* ------------------------------------------------------------------------
 *  Reads a block of data from the EEPROM Wear Leveling memory area.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void EepromWld_ReadData(uint8_t* data, uint8_t dataSize)
{
    for (uint8_t n = 0; n < dataSize; n++)
    {
        data[n] = eeprom_read_byte(_blockAddress + ((uint16_t)(_currentIndex + n) % _blockSize));
    }

    return;
}

/* ------------------------------------------------------------------------
 *  Writes a block of data to the EEPROM Wear Leveling memory area.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void EepromWld_WriteData(uint8_t* data, uint8_t dataSize)
{
    uint8_t n;

    for (n = 0; n < dataSize; n++)
    {
        /* Overwrite existing data */
        eeprom_write_byte(_blockAddress + ((uint16_t)(_currentIndex + n) % _blockSize), EEPROM_WLD_EMPTY_BYTE_VALUE);
    }    

    /* Increment index, and wrap if greater than size of eeprom buffer */
    _currentIndex = (uint16_t)(_currentIndex + dataSize) % _blockSize;

    for (n = 0; n < dataSize; n++)
    {
        /* Write new data */
        eeprom_write_byte(_blockAddress + ((uint16_t)(_currentIndex + n) % _blockSize), data[n]);
    }   

    return;
}

/* ------------------------------------------------------------------------
 *  Reads a byte of data from the EEPROM Wear Leveling memory area.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
uint8_t EepromWld_ReadByte(void)
{
    return eeprom_read_byte(_blockAddress + _currentIndex);
}

/* ------------------------------------------------------------------------
 *  Writes a byte of data to the EEPROM Wear Leveling memory area.
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void EepromWld_WriteByte(uint8_t data)
{
    eeprom_write_byte(_blockAddress + _currentIndex, EEPROM_WLD_EMPTY_BYTE_VALUE);

    /* Increment index, and wrap if greater than size of eeprom buffer */
    _currentIndex = (uint16_t)(_currentIndex + 1) % _blockSize;

    /* Write new data */
    eeprom_write_byte(_blockAddress + _currentIndex , data);

}
