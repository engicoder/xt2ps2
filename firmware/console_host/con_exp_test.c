#include <stdio.h>

#include "console_expansion.h"
#include "con_exp_test.h"
#include "con_msg_test.h"

static void Handler(char* out, ConsoleMessage* message);

char testSourceText[11] = "TEST MAIN";

ConsoleMessageHandler testConsoleHandler = &Handler;

void Handler(char* out, ConsoleMessage* message)
{
    switch (message->messageId)
    {
        case CON_MSG_TEST_WAITING:
            sprintf(out, "Waiting for bus power...");
            break;

        case CON_MSG_TEST_POWER_ON:
            sprintf(out, "Bus power detected.");
            break;

        case CON_MSG_TEST_POWER_OFF:
            sprintf(out, "Bus power not detected.");
            break;

        case CON_MSG_TEST_KBD_DETECTED:
            sprintf(out, "XT Keyboard detected.");
            break;

        default:
            out[0] = 0;
            break;
    }
    return;
}
