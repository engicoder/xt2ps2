/*
 * atomic.h
 *
 * Created: 10/29/2015 9:47:07 AM
 *  Author: Ben
 */ 


#ifndef ATOMIC_HAL_H_
#define ATOMIC_HAL_H_

/* Include the appropriate HAL implementation */
#if ARCH == AVR8
    #include "atomic_hal_avr.h"
#else
#error No device specifc implementation for ATOMIC defined.
#endif

#if !defined (ATOMIC) 
    #error ATOMIC not defined.
#endif

#endif /* ATOMIC_HAL_H_ */