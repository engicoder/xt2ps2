/* ========================================================================
 * eeprom_util.h
 * 
 * Purpose:
 *  Declares utility routines to aid in working with on-board eeprom.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE file for license details.
 * ------------------------------------------------------------------------ */ 


#ifndef EEPROM_UTIL_H_
#define EEPROM_UTIL_H_

#if ARCH == AVR8
    #include <avr/eeprom.h>
#else
    #error No EEPROM support implemented for defined ARCH
#endif

/* The byte value written to indicate an empty slot */
#define EEPROM_WLD_EMPTY_BYTE_VALUE 0xFF

/* -----------------------------------------------------------------------
 * Description:
 *  Initializes the EEPROM Wear Leveling Data component.
 *
 * Parameters:
 *  blockAddress - The address in EEPROM of the block of memory to be used.
 *  blockSizie - The size of the block of memory.
 *
 * Returns:
 *  n/a
 *------------------------------------------------------------------------*/
void EepromWld_Init(uint8_t* blockAddress, uint16_t blockSize);

/* -----------------------------------------------------------------------
 * Description:
 *  Reads a block of data from the EEPROM Wear Leveling memory area.
 *
 * Parameters:
 *  data - A buffer into which the data from the EEPROM will be read.
 *  dataSize - the number of bytes to read.
 *
 * Returns:
 *  n/a
 *------------------------------------------------------------------------*/
void EepromWld_ReadData(uint8_t* data, uint8_t dataSize);

/* -----------------------------------------------------------------------
 * Description:
 *  Writes a block of data to the EEPROM Wear Leveling memory area.
 *
 * Parameters:
 *  data - The data to be written.
 *  dataSize - The number of bytes to write.
 *
 * Returns:
 *  n/a
 *------------------------------------------------------------------------*/
void EepromWld_WriteData(uint8_t* data, uint8_t dataSize);

/* -----------------------------------------------------------------------
 * Description:
 *  Reads a byte of data from the EEPROM Wear Leveling memory area.
 *
 * Parameters:
 *  n/a
 *
 * Returns: uint8_t
 *  The byte of data read
 *------------------------------------------------------------------------*/
uint8_t EepromWld_ReadByte(void);


/* -----------------------------------------------------------------------
 * Description:
 *  Writes a byte of data to the EEPROM Wear Leveling memory area.
 *
 * Parameters:
 *  data - The data to be written.
 *
 * Returns:
 *  n/a
 *------------------------------------------------------------------------*/
void EepromWld_WriteByte(uint8_t data);




#endif /* EEPROM_UTIL_H_ */