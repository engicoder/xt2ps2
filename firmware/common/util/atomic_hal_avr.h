/*
 * atomic_avr.h
 *
 * Created: 10/29/2015 9:47:59 AM
 *  Author: Ben
 */ 


#ifndef ATOMIC_HAL_AVR_H_
#define ATOMIC_HAL_AVR_H_

#include <util/atomic.h>

#define ATOMIC() ATOMIC_BLOCK(ATOMIC_FORCEON)


#endif /* ATOMIC_HAL_AVR_H_ */