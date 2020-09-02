/* =======================================================================
 * console.h
 *
 * Purpose:
 *  Defines interface for serial debug console
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */


#ifndef CONSOLE_MSG_H_
#define CONSOLE_MSG_H_

#include <stdint.h>

typedef enum _ConsoleMessageType
{
	CON_MSG_DATA0,
	CON_MSG_DATA8,
	CON_MSG_DATA88,
	CON_MSG_DATA888,
	CON_MSG_DATA8888,
	CON_MSG_DATA16,
	CON_MSG_DATA1616,
	CON_MSG_DATA32,
	CON_MSG_DATA816,
	CON_MSG_DATA8816,
    CON_MSG_COUNT,
} ConsoleMessageType;


typedef enum _ConsoleMessageLength
{
	CON_MSG_LEN_DATA0 = 2,
	CON_MSG_LEN_DATA8 = 3,
	CON_MSG_LEN_DATA88 = 4,
	CON_MSG_LEN_DATA888 = 5,
	CON_MSG_LEN_DATA8888 = 6,
	CON_MSG_LEN_DATA16 = 4,
	CON_MSG_LEN_DATA1616 = 6,
	CON_MSG_LEN_DATA32 = 6,
	CON_MSG_LEN_DATA816 = 5,
	CON_MSG_LEN_DATA8816 = 6,
}_ConsoleMessageLength;

typedef struct _ConsoleMessageData
{
 	union
	{
		struct { uint8_t data1; } __attribute__((packed)) type8;
		struct { uint8_t data1; uint8_t data2; } __attribute__((packed)) type88;
		struct { uint8_t data1; uint8_t data2; uint8_t data3; } __attribute__((packed)) type888;
		struct { uint8_t data1; uint8_t data2; uint8_t data3; uint8_t data4; } __attribute__((packed)) type8888;
		struct { uint16_t data1; } __attribute__((packed)) type16;
		struct { uint16_t data1; uint16_t data2; } __attribute__((packed)) type1616;
		struct { uint32_t data1; } __attribute__((packed)) type32;
		struct { uint16_t data2; uint8_t data1; } __attribute__((packed)) type816;
		struct { uint8_t data1; uint8_t data2; uint16_t data3; } __attribute__((packed)) type8816;
		uint8_t dataBytes[4];
	};

} __attribute__((packed)) ConsoleMessageData;

typedef struct _ConsoleMessage
{
    union
	{
		struct
		{
			uint8_t sourceType;
			uint8_t messageId;
			ConsoleMessageData data;
		} __attribute__((packed));
		uint8_t messageBytes[6];
	};
} __attribute__((packed)) ConsoleMessage;

static inline uint8_t ConsoleMessage_Source(ConsoleMessage* message)
{
    return (uint8_t)(message->sourceType >> 4);
}

static inline uint8_t ConsoleMessage_Type(ConsoleMessage* message)
{
    return (uint8_t)(message->sourceType & 0x0F);
}

#endif /* CONSOLE_MSG_H_ */
