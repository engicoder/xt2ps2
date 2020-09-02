/* =======================================================================
 * console_expansion.h
 *
 * Purpose:
 *  Declare console source enum values
 *
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE.txt for license details.
 * ----------------------------------------------------------------------- */


#ifndef CONSOLE_EXPANSION_H_
#define CONSOLE_EXPANSION_H_

#include "console.h"

typedef void (*ConsoleMessageHandler)(char* out, ConsoleMessage* message);

bool ConsoleExpansion_Init(char* binaryFilename, char* textFilename, char* startupText);
void ConsoleExpansion_RegisterExpander(ConsoleSource source, ConsoleMessageHandler handler, char* sourceText);
void ConsoleExpansion_Exit(void);
void ConsoleExpansion_ProcessData(uint8_t data);


#endif /* CONSOLE_EXPANSION_H_ */
