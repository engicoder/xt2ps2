#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "board.h"
#include "config.h"

void Board_Init(void)
{
    /* Configure power detect pin as and input */
    BOARD_POWER_DETECT_DDR  &= ~(1 << BOARD_POWER_DETECT_BIT);

    /* Turn off internal pullup */
    BOARD_POWER_DETECT_PORT &= ~(1 << BOARD_POWER_DETECT_BIT); 	    
}


void Board_KeyPressed(void) {}
void Board_KeyReleased(void) {}
void Board_UpdateLedStatus(LedStatus status) {}

bool Board_PowerDetected(void)
{
	return ((BOARD_POWER_DETECT_PINS & (1 << BOARD_POWER_DETECT_BIT)) != 0);
}