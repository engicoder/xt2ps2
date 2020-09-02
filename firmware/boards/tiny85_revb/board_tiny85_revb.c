#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "board.h"
#include "config.h"

void Board_Init(void)
{
}


void Board_KeyPressed(void) {}
void Board_KeyReleased(void) {}
void Board_UpdateLedStatus(LedStatus status) {}

bool Board_PowerDetected(void)
{
	return false;
}