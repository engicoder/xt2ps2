#include <stdio.h>

#include "console_expansion.h"
#include "con_exp_xt2ps2.h"
#include "con_msg_xt2ps2.h"


static void Handler(char* out, ConsoleMessage* message);

char xt2Ps2SourceText[11] = "XT2PS2";

ConsoleMessageHandler xt2Ps2ConsoleHandler = &Handler;



void Handler(char* out, ConsoleMessage* message)
{
    out[0] = 0;
    switch (message->messageId)
    {
        case CON_MSG_XT2PS2_KEYMAPPED:
            {
                uint8_t data = message->data.type8.data1;
                sprintf(out, "Keymapped to %02X", data);
            }
            break;

        default:
            break;
    }
    return;
}
