/*
 * delayms_avr.h
 *
 * Created: 6/7/2018 5:14:45 PM
 *  Author: Ben
 */ 


#ifndef DELAYMS_AVR_H_
#define DELAYMS_AVR_H_



#include <avr/io.h>
#include <util/delay.h>

static inline void Delay10ms(uint8_t loops)
{
	while(loops--)
		_delay_ms(10);
}

static inline void Delay10us(uint8_t loops)
{
	while(loops--)
		_delay_us(10);

}

#endif /* DELAYMS_AVR_H_ */