/*
 * xthxcvr_config.h
 *
 * Created: 9/18/2015 7:39:02 PM
 *  Author: Ben
 */ 


#ifndef XTHXCVR_CONFIG_H_
#define XTHXCVR_CONFIG_H_

#include "config.h"

#ifndef XTH_XCVR_SOF_THRESHOLD
    #define XTH_XCVR_SOF_THRESHOLD 200U /* Microseconds */
#endif

#ifndef XTH_XCVR_1_START_BIT
    #define XTH_XCVR_1_START_BIT 0
#endif

#ifndef XTH_XCVR_RESET_LINE_ENABLE
    #define XTH_XCVR_RESET_LINE_ENABLE 0
#endif

#ifndef XTH_XCVR_RESET_LINE_HAS_PULLUP
    #define XTH_XCVR_RESET_LINE_HAS_PULLUP 0
#endif

#ifndef XTH_XCVR_SOFT_RESET_ENABLE
    #define XTH_XCVR_SOFT_RESET_ENABLE 1
#endif

#ifndef XTH_XCVR_SOFT_RESET_DURATION
    #define XTH_XCVR_SOFT_RESET_DURATION 20 /* milliseconds */
#endif

#ifndef XTH_XCVR_HARD_RESET_DURATION
    #define XTH_XCVR_HARD_RESET_DURATION 50 /* milliseconds */
#endif 

#ifndef XTH_XCVR_POR_ON_ENABLE
    #define XTH_XCVR_POR_ON_ENABLE 1
#endif

#define XTH_XCVR_POR_DURATION (XTH_XCVR_HARD_RESET_DURATION + 25U)

#endif /* XTHXCVR_CONFIG_H_ */