#include <stdio.h>

#include "console_expansion.h"
#include "con_exp_xth_kbd.h"
#include "con_msg_xth_kbd.h"

static void Handler(char* out, ConsoleMessage* message);

char xthKbdSourceText[11] = "XT KBD";

ConsoleMessageHandler xthKbdConsoleHandler = &Handler;

void Handler(char* out, ConsoleMessage* message)
{
    switch (message->messageId)
    {
        case CON_MSG_XTH_KBD_BAD_START_BIT:
            sprintf(out, "Start bit error; frame ignored.");
            break;

        case CON_MSG_XTH_KBD_DETECTED:
            sprintf(out, "XT Keyboard detected.");
            break;

        default:
            out[0] = 0;
            break;
    }
    return;
}
