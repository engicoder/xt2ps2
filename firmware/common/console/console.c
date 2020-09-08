/* =======================================================================
 * console.c
 * 
 * Purpose:
 *  Implements console
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */

#include "config.h"

#include "console.h"

#ifdef USE_CONSOLE

#include "console_hal.h"

static uint8_t _sendQueueStorage[CONSOLE_SEND_BUFFER_SIZE]; //NOTE: Buffer must stay in RAM
static CircularBuffer _sendQueue; /* Holds data received from the host */
CircularBuffer* consoleSendQueue;
uint8_t severityFlags = 0;

void Console_Init(ConsoleSeverity severity)
{
    /* Initialize the internal send and receive queues */
    CircularBuffer_Init(&_sendQueue, _sendQueueStorage, CONSOLE_SEND_BUFFER_SIZE);
    consoleSendQueue = &_sendQueue;

    ConsoleHal_Init();

    severityFlags = severity;

    CONSOLE_SEND0(CON_SRC_CONSOLE, CON_SEV_TRACE_EVENT, CON_MSG_CONSOLE_START);
}

void Console_Update(void)
{
    if (CircularBuffer_IsEmpty(&_sendQueue))
        return;

    uint8_t data = CircularBuffer_Peek(&_sendQueue);

    if (ConsoleHal_TrySend(data))
        CircularBuffer_Remove(&_sendQueue);

}

void Console_Flush(void)
{
    while (!CircularBuffer_IsEmpty(&_sendQueue))
    {
        uint8_t data = CircularBuffer_Remove(&_sendQueue);
        while(!ConsoleHal_TrySend(data));
    }

}

bool Console_PowerDetected(void)
{
    return ConsoleHal_PowerDetected();
}


void Console_Send0(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId)
{
    ConsoleMessage message;
    message.sourceType = (uint8_t)(source << 4) | CON_MSG_DATA0;
    message.messageId = messageId;
    CircularBuffer_InsertBlock(consoleSendQueue, message.messageBytes, CON_MSG_LEN_DATA0);
}

void Console_Send8(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint8_t data)
{
    ConsoleMessage message;
    message.sourceType = (uint8_t)(source << 4) | CON_MSG_DATA8;
    message.messageId = messageId;
    message.data.type8.data1 = data;
    CircularBuffer_InsertBlock(consoleSendQueue, message.messageBytes, CON_MSG_LEN_DATA8);
}

void Console_Send88(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint8_t data1, uint8_t data2)
{
    ConsoleMessage message;
    message.sourceType = (uint8_t)(source << 4) | CON_MSG_DATA88;
    message.messageId = messageId;
    message.data.type8.data1 = data1;
    message.data.type88.data2 = data2;
    CircularBuffer_InsertBlock(consoleSendQueue, message.messageBytes, CON_MSG_LEN_DATA88);
}

void Console_Send888(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint8_t data1, uint8_t data2, uint8_t data3)
{
    ConsoleMessage message;
    message.sourceType = (uint8_t)(source << 4) | CON_MSG_DATA888;
    message.messageId = messageId;
    message.data.type888.data1 = data1;
    message.data.type888.data2 = data2;
    message.data.type888.data3 = data3;
    CircularBuffer_InsertBlock(consoleSendQueue, message.messageBytes, CON_MSG_LEN_DATA888);
}

void Console_Send8888(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4)
{
    ConsoleMessage message;
    message.sourceType = (uint8_t)(source << 4) | CON_MSG_DATA8888;
    message.messageId = messageId;
    message.data.type8888.data1 = data1;
    message.data.type8888.data2 = data2;
    message.data.type8888.data3 = data3;
    message.data.type8888.data4 = data4;
    CircularBuffer_InsertBlock(consoleSendQueue, message.messageBytes, CON_MSG_LEN_DATA8888);
}

void Console_Send16(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId,  uint16_t data)
{
    ConsoleMessage message;
    message.sourceType = (uint8_t)(source << 4) | CON_MSG_DATA16;
    message.messageId = messageId;
    message.data.type16.data1 = data;
    CircularBuffer_InsertBlock(consoleSendQueue, message.messageBytes, CON_MSG_LEN_DATA16);
}

void Console_Send1616(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint16_t data1, uint16_t data2)
{
    ConsoleMessage message;
    message.sourceType = (uint8_t)(source << 4) | CON_MSG_DATA16;
    message.messageId = messageId;
    message.data.type1616.data1 = data1;
    message.data.type1616.data2 = data2;
    CircularBuffer_InsertBlock(consoleSendQueue, message.messageBytes, CON_MSG_LEN_DATA1616);
}

void Console_Send32(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint32_t data)
{
    ConsoleMessage message;
    message.sourceType = (uint8_t)(source << 4) | CON_MSG_DATA32;
    message.messageId = messageId;
    message.data.type32.data1 = data;
    CircularBuffer_InsertBlock(consoleSendQueue, message.messageBytes, CON_MSG_LEN_DATA32);
}

void Console_Send816(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint8_t data1, uint16_t data2)
{
    ConsoleMessage message;
    message.sourceType = (uint8_t)(source << 4) | CON_MSG_DATA816;
    message.messageId = messageId;
    message.data.type816.data1 = data1;
    message.data.type816.data2 = data2;
    CircularBuffer_InsertBlock(consoleSendQueue, message.messageBytes, CON_MSG_LEN_DATA816);
}

void Console_Send8816(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint8_t data1, uint8_t data2, uint16_t data3)
{
    ConsoleMessage message;
    message.sourceType = (uint8_t)(source << 4) | CON_MSG_DATA8816;
    message.messageId = messageId;
    message.data.type8816.data1 = data1;
    message.data.type8816.data2 = data2;
    message.data.type8816.data3 = data3;
    CircularBuffer_InsertBlock(consoleSendQueue, message.messageBytes, CON_MSG_LEN_DATA8816);
}

#endif