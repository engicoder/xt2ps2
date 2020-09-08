/* ========================================================================
 * main.c
 * 
 * Purpose:
 *  Serves as entry point and main loop for the executable.
 *
 * Operational Summary:
 *  
 * 
 * License:
 *  Copyright (c) 2015, Engicoder
 *  All rights reserved.
 *  See LICENSE file for license details.
 * ------------------------------------------------------------------------ */ 

#include "board.h"
#include "common.h"
#include "config.h"
#include "host.h"
#include "device.h"
#include "keymap.h"
#include "led_status.h"
#include "keyevent.h"


#include <avr/io.h>
#include <avr/interrupt.h>

#include "con_msg_test.h"
#include "con_msg_xt2ps2.h"


#include "console.h"

void StatusLedUpdateReceivedHandler(LedStatus status)
{
    Board_UpdateLedStatus(status);
}

int main(void) 
{
    /* Initialize the watchdog timer */
	Watchdog_Disable();

    Board_Init();

#ifdef USE_CONSOLE    
	Console_Init(CON_SEV_ERROR | CON_SEV_TRACE_EVENT | CON_SEV_TRACE_INFO);
    CONSOLE_SEND0(CON_SRC_TEST, CON_SEV_TRACE_EVENT, CON_MSG_TEST_WAITING);

    Console_Flush();

    while(!Console_PowerDetected());
    CONSOLE_SEND0(CON_SRC_TEST, CON_SEV_TRACE_EVENT, CON_MSG_TEST_POWER_ON);
#endif

    Host_Init();

    Device_Init(&StatusLedUpdateReceivedHandler);

    EnableGlobalInterrupts();

    Device_Start();

    Host_Start();

    KeyEvent hostEvent;
    KeyEvent mappedEvent;

    Watchdog_Enable();

    while(1) 
    {
        Watchdog_Reset();

        Host_Update();

        if (Host_GetKeyEvent(&hostEvent))
        {
            if(Keymap_MapToKeyEvent(&hostEvent, &mappedEvent))
            {
                if (KeyEvent_IsPress(&mappedEvent))
                    Board_KeyPressed();
                else 
                    Board_KeyReleased();

                CONSOLE_SEND8(CON_SRC_XT2PS2, CON_SEV_TRACE_INFO, CON_MSG_XT2PS2_KEYMAPPED, KeyEvent_Code(&mappedEvent));

                Device_SendKeyEvent(&mappedEvent);
                
            }
        }

        Device_Update();

#ifdef USE_CONSOLE
		Console_Update();
        if (!Console_PowerDetected())
        {
            CONSOLE_SEND0(CON_SRC_TEST, CON_SEV_TRACE_EVENT, CON_MSG_TEST_POWER_OFF);
            Console_Flush();
            System_Reset();
        }
#endif


    }

    return 1;
}
