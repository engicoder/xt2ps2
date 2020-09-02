/* =======================================================================
 * xth_kbd.c
 *
 * Purpose:
 *  Provides function to interface the XT keyboard as a host.
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */ 
#include "xth_kbd.h"


#include "config.h"
#include "common.h"
#include "circular_buffer.h"
#include "xth_xcvr.h"
#include "console.h"
#include "con_msg_xth_kbd.h"
#include "bit_array.h"

#define XTH_KBD_ERROR_THRESHOLD  10

#ifndef XTH_KBD_FWD_TYPEMATIC
    #define XTH_KBD_FWD_TYPEMATIC 0
#endif

CircularBuffer _scanCodeBuffer;
uint8_t _scanCodeBufferStorage[XTH_RECV_BUFFER_SIZE];
bool _enabled = false;
uint8_t _errorCount = 0;
bool _detected = false;
BitArray _keyState;
uint8_t _keyStateStorage[BIT_ARRAY_STORAGE_SIZE(XT_SC_MAX_CODE)];

OnScanCode _scanCodeHandler = (OnScanCode)0;

void XthKbd_Init(void)
{
    CircularBuffer_Init(&_scanCodeBuffer, _scanCodeBufferStorage, XTH_RECV_BUFFER_SIZE);
    BitArray_Init(&_keyState, _keyStateStorage, sizeof (_keyStateStorage));
    XthXcvr_Init();
}

void XthKbd_Enable(void)
{
    XthXcvr_Enable();
    CircularBuffer_Clear(&_scanCodeBuffer);
    _enabled = true;
    _detected = false;
    BitArray_ClearAll(&_keyState);
}


void XthKbd_Disable(void)
{
    _enabled = false;
    XthXcvr_Disable();
}

void XthKbd_Task(void)
{
    if (!_enabled || !XthXcvr_StatusReady())
        return;
    
    if (!_detected)
    {
        if (XthXcvr_StatusKeyboardDetected())
        {
            _detected = true;
            CONSOLE_SEND0(CON_SRC_XTH_KBD, CON_SEV_TRACE_EVENT, CON_MSG_XTH_KBD_DETECTED);
        }
    }

    if (XthXcvr_StatusIsError())
    {
        CONSOLE_SEND0(CON_SRC_XTH_KBD, CON_SEV_TRACE_EVENT, CON_MSG_XTH_KBD_BAD_START_BIT);
        _errorCount++;
        if (_errorCount > XTH_KBD_ERROR_THRESHOLD)
        {
            XthXcvr_SoftReset();
            _errorCount = 0;
        }
    }
    else if (XthXcvr_StatusDataReceived())
    {
        uint8_t scanCode = XthXcvr_ReadReceivedData();
        if (scanCode != XT_SC_NONE)
        {
            uint8_t baseCode = scanCode & 0x7F;
            if (scanCode & (1 << 7))
            {
                BitArray_ClearBit(&_keyState, baseCode);
                CircularBuffer_Insert(&_scanCodeBuffer, scanCode);
            }
            /* Ignore key press if it has already been set */
            else if (XTH_KBD_FWD_TYPEMATIC || !BitArray_IsSet(&_keyState, baseCode) )
            {
                BitArray_SetBit(&_keyState, baseCode);
                CircularBuffer_Insert(&_scanCodeBuffer, scanCode);
            }
        }
    }

    if (_scanCodeHandler != (OnScanCode)0)
    {
        uint8_t scanCode = XthKbd_GetScanCode();
        _scanCodeHandler(scanCode);
    }
}

void XthKbd_Reset(void)
{
    if (!_enabled)
        return;

    CircularBuffer_Clear(&_scanCodeBuffer);
    XthXcvr_SoftReset();
    _detected = false;
}


bool XthKbd_IsScanCodeAvailable(void)
{
    return !CircularBuffer_IsEmpty(&_scanCodeBuffer);
}

uint8_t XthKbd_GetScanCode(void)
{
    uint8_t scanCode = 0x00;

    if (_enabled && !CircularBuffer_IsEmpty(&_scanCodeBuffer))
    {
        scanCode = CircularBuffer_Remove(&_scanCodeBuffer);
    }

    return scanCode;
}

void XthKbd_RegisterScanCodeHandler(OnScanCode handler)
{
    _scanCodeHandler = handler;
}