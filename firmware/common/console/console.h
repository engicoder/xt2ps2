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


#ifndef CONSOLE_H_
#define CONSOLE_H_


#include <stdbool.h>
#include <stdint.h>

#include "circular_buffer.h"
#include "console_source.h"
#include "console_msg.h"

typedef enum _ConsoleSeverity
{
	CON_SEV_ERROR   	 = (1 << 0),
	CON_SEV_TRACE_EVENT  = (1 << 1),
	CON_SEV_TRACE_INFO   = (1 << 2),
	CON_SEV_TRACE_FLOW   = (1 << 3),
} ConsoleSeverity;


typedef enum _ConsoleMessageId
{
	CON_MSG_CONSOLE_START,
} ConsoleMessageId;


#ifdef USE_CONSOLE

extern CircularBuffer* consoleSendQueue;
extern uint8_t severityFlags;

void Console_Init(ConsoleSeverity severity);
void Console_Update(void);
void Console_Flush(void);

static inline void Console_SeveritySet(ConsoleSeverity severity)
{
	severityFlags = severity;
}

void Console_Send0(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId);
void Console_Send8(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint8_t data);
void Console_Send88(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint8_t data1, uint8_t data2);
void Console_Send888(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint8_t data1, uint8_t data2, uint8_t data3);
void Console_Send8888(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4);
void Console_Send16(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId,  uint16_t data);
void Console_Send1616(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint16_t data1, uint16_t data2);
void Console_Send32(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint32_t data);
void Console_Send816(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint8_t data1, uint16_t data2);
void Console_Send8816(ConsoleSource source, ConsoleSeverity severity, uint8_t messageId, uint8_t data1, uint8_t data2, uint16_t data3);



#define CONSOLE_SEND0(source, severity, messageId) do { \
	if (severityFlags & severity) Console_Send0(source, severity, messageId); } while(0);
#define CONSOLE_SEND8(source, severity, messageId, data) do { \
	if (severityFlags & severity) Console_Send8(source, severity, messageId, data); } while(0);
#define CONSOLE_SEND88(source, severity, messageId, data1, data2) do { \
	if (severityFlags & severity) Console_Send88(source, severity, messageId, data1, data2); } while(0);
#define CONSOLE_SEND888(source, severity, messageId, data1, data2, data3) do { \
	if (severityFlags & severity) Console_Send888(source, severity, messageId, data1, data2, data3); } while(0);
#define CONSOLE_SEND16(source, severity, messageId, data) do { \
	if (severityFlags & severity) Console_Send16(source, severity, messageId, data); } while(0);
#define CONSOLE_SEND1616(source, severity, messageId, data1, data2) do { \
	if (severityFlags & severity) Console_Send1616(source, severity, messageId, data1, data2); } while(0);

#else

static inline void Console_Init(ConsoleSeverity severity) { (void)severity; }
static inline void Console_Update(void) {}
static inline void Console_Flush(void) {}

#define CONSOLE_SEND0(source, severity, messageId)
#define CONSOLE_SEND8(source, severity, messageId, data)
#define CONSOLE_SEND88(source, severity, messageId, data1, data2)
#define CONSOLE_SEND888(source, severity, messageId, data1, data2, data3)
#define CONSOLE_SEND16(source, severity, messageId, data)
#define CONSOLE_SEND1616(source, severity, messageId, data1, data2)

#endif



#endif /* CONSOLE_H_ */
