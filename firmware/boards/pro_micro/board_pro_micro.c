#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "board.h"
#include "config.h"


#define KEY_PRESS_PORT PORTB
#define KEY_PRESS_DDR  DDRB
#define KEY_PRESS_PIN  PB0

void Board_Init(void)
{
    /* Configure power detect pin as and input */
    BOARD_POWER_DETECT_DDR  &= ~(1 << BOARD_POWER_DETECT_BIT);

    /* Turn off internal pullup */
    BOARD_POWER_DETECT_PORT &= ~(1 << BOARD_POWER_DETECT_BIT); 	    

    KEY_PRESS_PORT |= (1 << KEY_PRESS_PIN);
    KEY_PRESS_DDR |= (1 << KEY_PRESS_PIN);

}

void Board_KeyPressed(void) 
{
    KEY_PRESS_PORT &= ~(1 << KEY_PRESS_PIN);
}

void Board_KeyReleased(void)
{
    KEY_PRESS_PORT |= (1 << KEY_PRESS_PIN);
}

void Board_UpdateLedStatus(LedStatus status) {}

bool Board_PowerDetected(void)
{
	return ((BOARD_POWER_DETECT_PINS & (1 << BOARD_POWER_DETECT_BIT)) != 0);
}