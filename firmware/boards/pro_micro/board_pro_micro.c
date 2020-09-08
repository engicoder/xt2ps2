#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "board.h"
#include "config.h"

#define TX_LED_PORT PORTD
#define TX_LED_DDR DDRD
#define TX_LED_PIN PD5

#define RX_LED_PORT PORTB
#define RX_LED_DDR  DDRB
#define RX_LED_PIN  PB0

void Board_Init(void)
{
    /* Set RX_LED as output for used with key press */
    RX_LED_PORT |= (1 << RX_LED_PIN);
    RX_LED_DDR |= (1 << RX_LED_PIN);

    /* Set TX_LED as output for use with caps lock */
    TX_LED_PORT |= (1 << TX_LED_PIN);
    TX_LED_DDR |= (1 << TX_LED_PIN);
}

void Board_KeyPressed(void) 
{
    RX_LED_PORT &= ~(1 << RX_LED_PIN);
}

void Board_KeyReleased(void)
{
    RX_LED_PORT |= (1 << RX_LED_PIN);
}

void Board_UpdateLedStatus(LedStatus status) 
{
    if (status & LedStatusCapsLock)
        TX_LED_DDR &= ~(1 << TX_LED_PIN);
    else
        TX_LED_DDR |= (1 << TX_LED_PIN);
}

