/*
 * delay.h
 *
 * Created: 6/7/2018 5:12:40 PM
 *  Author: Ben
 */ 


#ifndef DELAYMS_H_
#define DELAYMS_H_

#include <stdint.h>

static inline void Delay10ms(uint8_t tenMilleseconds);
static inline void Delay10us(uint8_t tenMicroseconds);


/* Include the appropriate HAL implementation */
#if ARCH == AVR8
	#include "delayms_avr.h"
#else
	#error No device specifc implementation for Delay10ms defined.
#endif

#endif /* DELAYMS_H_ */